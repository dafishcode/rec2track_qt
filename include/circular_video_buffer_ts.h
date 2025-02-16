#ifndef CIRCULAR_VIDEO_BUFFER_TS_H
#define CIRCULAR_VIDEO_BUFFER_TS_H

#include <iostream>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <signal.h>

#include "camera_functions.h"
// Thread safe circular buffer


enum outputType {zCam_SEQIMAGES = 0,
                   zCam_RAWVID = 1,
                   zCam_MJPGVID = 2,
                   zCam_XVID = 3};




using namespace std;
/// \brief Handles writing of video frames to disk along with logging time of each frame. Uses a rolling Buffer of images so as to allow to antecedently save images following an event trigger.
///
//To prevent copying a class, you can very easily declare a private copy constructor / assignment operators. But you can also inherit boost::noncopyable.
//: private boost::noncopyable
class circular_video_buffer_ts: private boost::noncopyable
{
public:

    typedef boost::mutex::scoped_lock slock;
    cv::Size mszFrame; //Resolution Of vid in pixels

    circular_video_buffer_ts() {}

    circular_video_buffer_ts(int n, string pf,ofstream* lf, outputType filetype=outputType::zCam_RAWVID, double vidfps = 0.0) {
        circ_buff_img.set_capacity(n);
        frame_index.set_capacity(n);
        time_index.set_capacity(n);
        logstring.set_capacity(n);

        moutputType = filetype;
        mvidfps = vidfps;
        idx_last_recorded=0;
        mproc_folder=pf;
        mstreamlogfile=lf;
        mbrecording_state=false;
        mbwriting_buffer=false;
    }

    // Add new Image to Buffer - Cannot not be done while Buffer is being dumped to disk
    void update_buffer(const cv::Mat &imdata, int64 f,uint t, string logentrystring) {
        slock lk(monitor);
        if(!mbwriting_buffer){
            //cv::Mat im;
            //imdata.copyTo(im);
            if(mbverbose) std::cout<<"update buffer: "<< f <<' '<< t <<endl;
            circ_buff_img.push_back(imdata.clone());
            frame_index.push_back(f);
            time_index.push_back(t);
            logstring.push_back(logentrystring);

            buffer_not_empty.notify_one();
        }
    }

    void retrieve_last(cv::Mat &image, long int &lastframeIdx) {
        slock lk(monitor);

        if (frame_index.size() == 0)
            return; //No Images Exist

        if(mbverbose)
            cout<<"retrieve frame: "<<frame_index.back()<<' '<<time_index.back()<<endl;

        circ_buff_img.back().copyTo(image);
        lastframeIdx=frame_index.back();
    }


    std::string filenameImgSequence(int value, int digits = 10) {
        unsigned int uvalue = value;
        if (value < 0) {
            uvalue = -uvalue;
        }
        std::string result;
        while (digits-- > 0) {
            result += ('0' + uvalue % 10);
            uvalue /= 10;
        }
        if (value < 0) {
            result += '-';
        }
        std::reverse(result.begin(), result.end());

        return (result.append(".pgm") );
    }

    void clear() {
        slock lk(monitor);
        circ_buff_img.clear();
        frame_index.clear();
        time_index.clear();
        logstring.clear();
        idx_last_recorded = 0;
    }

    int size() {
        slock lk(monitor);
        return circ_buff_img.size();
    }

    void set_capacity(int capacity) {
        slock lk(monitor);
        circ_buff_img.set_capacity(capacity);
    }

    void set_recorder_state(bool rs){
        slock lk(monitor);
        mbrecording_state=rs;
    }

    void set_last_recorded_index(long int f){
        slock lk(monitor);
        idx_last_recorded=f;
    }

    void set_writing_buffer(bool br){
        slock lk(monitor);
        mbwriting_buffer = br;
    }

    //Changing Output folder Resets The Video Stream - As this indicates a new event is being recorded the Buffer Is also Flushed
    void set_outputfolder(string sdir)
    {   slock lk(monitor);
        if (sdir != mproc_folder)
        {
            mproc_folder = sdir;
            moVideowriter.release(); //Close Current vid to open new one
            mbrecording_state = false;
        }

    }

    bool get_recorder_state(){
        slock lk(monitor);
        return mbrecording_state;
    }

    bool get_writing_buffer(){
        slock lk(monitor);
        return mbwriting_buffer;
    }


    // Opens New Video file on the current output folder File
    bool openNewVideoStream()
    {

        if (circ_buff_img.size() == 0)
            mszFrame = cv::Size(720,720); //Default Size - Set To default camera ROI
        else
            mszFrame = cv::Size(circ_buff_img[0].cols,circ_buff_img[0].rows);

        string stroutputfile;
        if (moutputType == outputType::zCam_SEQIMAGES){
           stroutputfile = mproc_folder + string("/img_%09d.bmp");
           moVideowriter.open(stroutputfile, 0, 0, mszFrame, false); //initialize the VideoWriter object
         }
        if (moutputType == outputType::zCam_RAWVID){
             stroutputfile = mproc_folder + string("/exp_video_y800.avi"); //cv::VideoWriter::fourcc('Y','8','0','0')
             moVideowriter.open(stroutputfile, 0 , mvidfps, mszFrame, false); //initialize the VideoWriter object //('Y','8','0','0') cv::VideoWriter::fourcc('M','J','P','G') cv::VideoWriter::fourcc('X','V','I','D')
        }
         if (moutputType == outputType::zCam_MJPGVID){
              stroutputfile = mproc_folder + string("/exp_video_mpeg.avi");
              moVideowriter.open(stroutputfile, cv::VideoWriter::fourcc('M','J','P','G') , mvidfps, mszFrame, false); //initialize the VideoWriter object cv::VideoWriter::fourcc('Y','8','0','0')
         }
         if (moutputType == outputType::zCam_XVID){
              stroutputfile = mproc_folder + string("/exp_video_xvid.avi");
              moVideowriter.open(stroutputfile, cv::VideoWriter::fourcc('X','V','I','D') , mvidfps, mszFrame, false); //initialize the VideoWriter object cv::VideoWriter::fourcc('Y','8','0','0')
         }

         cout << "New output file:" <<  stroutputfile <<std::endl;

         return (moVideowriter.isOpened());
    }

    // Dumps unexported part of Buffer To Disk - while it lock adding any new contents to it
    // This action can only be done by the processor thread so it does not need to be thread safe.
    void writeNewFramesToImageSequence(){


        unsigned int i=0;
        unsigned int lri;

        // set writing_buffer, get last recorded index
        {
            slock lk(monitor);
            mbwriting_buffer=true;
            lri=idx_last_recorded;
            //cout<<"writing whole buffer"<<endl;
        }

        for(i=0;i<circ_buff_img.size();i++){
            if(frame_index[i]>lri){
                // Writing to file
                stringstream filename;

                filename << mproc_folder<<"/" << filenameImgSequence((int)frame_index[i]);
                if (mbrecording_state)
                {
                    cv::imwrite(filename.str().c_str(),circ_buff_img[i]);
                    *mstreamlogfile << logstring[i];// << std::endl;// frame_index[i] << '\t' <<time_index[i]<<'\t'<<cv::getTickFrequency() << endl;
                }
                if(mbverbose)
                    cout<<"write buffer: "<<'\t'<<frame_index[i]<<'\t'<<time_index[i]<<endl;
            }
        }

        // update last recorded index and turn off writing_buffer
        {
            slock lk(monitor);
            idx_last_recorded=frame_index[i-1];
            mbwriting_buffer=false;
        }
    }

    // Dumps unexported part of Buffer Contents To Disk - while it lock adding any new contents to it
    // This action can only be done by the processor thread so it does not need to be thread safe.
    // Only exports frames that have not been saved yet
    void writeNewFramesToVideostream(){

        unsigned int i=0;
        unsigned int lri; //Last frame idx exported to disk

        if (circ_buff_img.size() == 0)
        {
             cerr << "Cannot write to stream, video buffer is empty." <<std::endl;
             return;
        }

        if ( !moVideowriter.isOpened() ) //if not initialize the VideoWriter successfully, exit
        {
             //Open New Stream given image Sizes
             if (!openNewVideoStream())
                cerr << "ERROR: Failed to write circular buffer to video, videoWriter not opened" << endl;
              //pVideoWriterB->open()
               return;
        }


        // set writing_buffer, get last recorded index
        {
            slock lk(monitor);
            mbwriting_buffer=true;
            lri=idx_last_recorded; //save idx of most recently exported/saved img to file
            if(mbverbose)
                cout<<"writeNewFramesToVideostream: fidx>"<< lri <<endl;
        }

        for(i=0;i < circ_buff_img.size();i++){
            if(frame_index[i] > lri){
                // Writing to file
                //cv::imwrite(filename.str().c_str(),);
                if (mbrecording_state)
                {
                    moVideowriter.write(circ_buff_img[i]);
                    *mstreamlogfile << logstring[i];//frame_index[i] << '\t' <<time_index[i]<<'\t'<<cv::getTickFrequency() << endl;
                }
                if(mbverbose)
                    cout<<"write "<<'\t'<<frame_index[i]<<'\t'<<time_index[i]<<endl;
            }
        }

        // update last recorded index and turn off writing_buffer
        {
            slock lk(monitor);
            idx_last_recorded=frame_index[i-1];
            mbwriting_buffer=false;
        }
    }



private:

    boost::condition buffer_not_empty;
    boost::condition buffer_ready;
    boost::mutex monitor;
    boost::circular_buffer<cv::Mat> circ_buff_img;
    boost::circular_buffer<int64> frame_index;
    boost::circular_buffer<int64> time_index; //Camera Microseconds
    boost::circular_buffer<string> logstring; //Camera Microseconds

    bool mbrecording_state;
    bool mbwriting_buffer =false;
    long int idx_last_recorded;
    outputType moutputType = zCam_RAWVID;
    string mproc_folder;
    double mvidfps = 0.0;
    ofstream* mstreamlogfile;
    cv::VideoWriter moVideowriter;

    bool mbverbose=false;
};

#endif // CIRCULAR_VIDEO_BUFFER_TS_H
