#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/features2d.hpp"
#include <FlyCapture2.h>
#include <CameraBase.h>
#include <iostream>
#include <sstream>
#include<fstream>
#include<signal.h>
#include<sys/stat.h>
#include"../include/functions2.h"
#include"../include/handler.h"
#include"../include/circular_buffer_ts.h"
#include<boost/thread.hpp>
#include<boost/chrono.hpp>
#include"../include/barrage.h"
#include<QMainWindow>
#include<QMessageBox>

using namespace std;
using namespace FlyCapture2;

bool run=true;
boost::mutex mtx;

cv::Ptr<cv::BackgroundSubtractor> pMOG;
cv::Mat fgMaskMOG;

cv::SimpleBlobDetector::Params params;
cv::Ptr<cv::SimpleBlobDetector> detector;// = cv::SimpleBlobDetector::create(params);
cv::Mat im_with_keypoints;

boost::condition wait_condition;
boost::condition MOG_done;
double threshold=0;
double threshold_sd=0;
double threshold_var=0;
long int frame_counter=0;
long int threshold_frame_count=0;
const double learning_rate=0.001;

bool verbose=false;

int buffer_ID=0;

void my_handler(int sig){
    cout<<endl<<"Recording stopped."<<endl;
    boost::mutex::scoped_lock lk(mtx);
    run=false;
}

void setup_blob_detector(cv::Ptr<cv::SimpleBlobDetector>  &d){

    cv::SimpleBlobDetector::Params params;
    
    // Change thresholds
    params.minThreshold = 1;
    params.maxThreshold = 2000;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 5;

    // Filter by Circularity
    params.filterByCircularity = false;
    params.minCircularity = 0;

    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = false;
    params.minInertiaRatio = 0.01;

    // Detect blobs.
    d = cv::SimpleBlobDetector::create(params);
}


std::string fixedLengthString(int value, int digits) {
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
    return result;
}


void on_mouse(int event, int x, int y, int flags, void* p){    
    ioparam* param=(ioparam*)p;
    int ws=param->WinSize;
    param->center.x=x;
    param->center.y=y;
    
    param->pt1.x=max(0,(int)(x-ws)); 
    param->pt1.y=max(0,(int)(y-ws));

    param->pt2.x=min(param->OrigSize.width,(int)(x+ws)); 
    param->pt2.y=min(param->OrigSize.height,(int)(y+ws));

    if  ( event == cv::EVENT_LBUTTONDOWN ){
        param->status=true;
    }
    
    if (event==cv::EVENT_LBUTTONUP ){
        param->status=false;
    }

}

void on_mouse2(int event, int x, int y, int flags, void* p){    
    ioparam* param=(ioparam*)p;
    int ws=param->WinSize;
    param->center.x=x;
    param->center.y=y;
    
    param->pt1.x=max(0,(int)(x-ws)); 
    param->pt1.y=max(0,(int)(y-ws));

    param->pt2.x=min(param->OrigSize.width,(int)(x+ws)); 
    param->pt2.y=min(param->OrigSize.height,(int)(y+ws));

    if  ( event == cv::EVENT_LBUTTONDOWN ){
        param->status=true;
    }
    
    if (event==cv::EVENT_LBUTTONUP ){
        param->status=false;
    }

}

void PrintBuildInfo()
{
    FC2Version fc2Version;
    Utilities::GetLibraryVersion(&fc2Version);

    ostringstream version;
    version << "FlyCapture2 library version: " << fc2Version.major << "."
            << fc2Version.minor << "." << fc2Version.type << "."
            << fc2Version.build;
    cout << version.str() << endl;

    ostringstream timeStamp;
    timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
    cout << timeStamp.str() << endl << endl;
}

void PrintFormat7Capabilities(Format7Info fmt7Info)
{
    cout << "Max image pixels: (" << fmt7Info.maxWidth << ", "
         << fmt7Info.maxHeight << ")" << endl;
    cout << "Image Unit size: (" << fmt7Info.imageHStepSize << ", "
         << fmt7Info.imageVStepSize << ")" << endl;
    cout << "Offset Unit size: (" << fmt7Info.offsetHStepSize << ", "
         << fmt7Info.offsetVStepSize << ")" << endl;
    cout << "Pixel format bitfield: 0x" << fmt7Info.pixelFormatBitField << endl;
}

void PrintCameraInfo(CameraInfo *pCamInfo)
{
    cout << endl;
    cout << "*** CAMERA INFORMATION ***" << endl;
    cout << "Serial number - " << pCamInfo->serialNumber << endl;
    cout << "Camera model - " << pCamInfo->modelName << endl;
    cout << "Color - "<< pCamInfo->isColorCamera <<endl;
    cout << "Camera vendor - " << pCamInfo->vendorName << endl;
    cout << "Sensor - " << pCamInfo->sensorInfo << endl;
    cout << "Resolution - " << pCamInfo->sensorResolution << endl;
    cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
    cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl
         << endl;
}

void SetCam(Camera *cam, F7 &f7, const Mode k_fmt7Mode, const PixelFormat k_fmt7PixFmt, bool triggerON){

    FlyCapture2::Error error;

    CameraInfo cInfo;
    cam->GetCameraInfo(&cInfo);
    PrintCameraInfo(&cInfo);

    error = cam->GetConfiguration(&(f7.config));

    std::cerr << "GetConfiguration returned Error:" << error.GetDescription() << std::endl;

    // Set the number of driver buffers used to 10.
    //f7.config.numBuffers = 10; THIS IS CORRECT
    f7.config.numBuffers = 10;

    // Set the camera configuration
    cam->SetConfiguration(&(f7.config));

    f7.fmt7Info.mode=k_fmt7Mode;
    cam->GetFormat7Info(&(f7.fmt7Info), &(f7.supported));
    PrintFormat7Capabilities(f7.fmt7Info);
    f7.fmt7ImageSettings.mode = k_fmt7Mode;
    f7.fmt7ImageSettings.offsetX = 0;
    f7.fmt7ImageSettings.offsetY = 0;
    f7.fmt7ImageSettings.width = f7.fmt7Info.maxWidth;
    f7.fmt7ImageSettings.height = f7.fmt7Info.maxHeight;
    f7.fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;

    cam->ValidateFormat7Settings(&(f7.fmt7ImageSettings), &(f7.valid), &(f7.fmt7PacketInfo));
    cam->SetFormat7Configuration(&(f7.fmt7ImageSettings), f7.fmt7PacketInfo.recommendedBytesPerPacket);

    // Set camera timestamp ON /////////////////////////////////////////////////////
    EmbeddedImageInfo imageInfo;
    imageInfo.timestamp.onOff = true;
    error = cam->SetEmbeddedImageInfo(&imageInfo);
    if (error != PGRERROR_OK)
    {
        cerr << "Error setting camera imageInfo." << std::endl;
        if (cam)
            delete cam;
        cout << "Press Enter to exit." << endl;
        cin.ignore();
    }
    // #############################################################################

    // Set SHUTTER SPEED ///////////////////////////////////////////////////////////
    //Declare a Property struct.
    Property prop;
    //Define the property to adjust.
    prop.type = SHUTTER;
    //Ensure the property is on.
    prop.onOff = true;
    //Ensure auto-adjust mode is off.
    prop.autoManualMode = false;
    //Ensure the property is set up to use absolute value control.
    prop.absControl = true;
    //Set the absolute value of shutter to 1 ms.
    prop.absValue = 1;
    //Set the property.
    error = cam->SetProperty( &prop );
    // #############################################################################

    // Set TRIGGER /////////////////////////////////////////////////////////////////
    TriggerMode mTrigger;
    mTrigger.mode = 0;
    mTrigger.source = 3;
    mTrigger.parameter = 0;
    mTrigger.onOff = triggerON;
    mTrigger.polarity = 0;
    cam->SetTriggerMode(&mTrigger);
      

    // Start capturing images
    cam->StartCapture();
}

void CreateOutputFolder(string folder){
    struct stat sb;
    if (stat(folder.c_str(), &sb) != 0){
        const int dir_err = mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (-1 == dir_err){
            printf("Error creating directory!");
            exit(1);
        }
    } else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Folder exists !");
        messageBox.setFixedSize(500,200);
        exit(1);
    }

}

void Select_ROI(Camera *cam, ioparam &center, int &recording, int ROISize=100){

    int ind_brightness=0, ind_brightness_max=100;
    double brightness;
    Image rawImage;
    cv::Mat tmp_image;
    cv::namedWindow("ROI selection",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
    cv::resizeWindow("ROI selection", 800,800);
    cv::createTrackbar( "brightness", "ROI selection", &ind_brightness, ind_brightness_max);

    // Retrieve a single image
    cam->FireSoftwareTrigger(false);
    cam->RetrieveBuffer(&rawImage);

    // convert Image to Mat
    unsigned char* data = rawImage.GetData();
    cv::Mat cvm(rawImage.GetRows(),rawImage.GetCols(),CV_8U,(void*)data);
    
    // Setup ROI
    ioparam tmp_center;
    tmp_center.status=0;
    tmp_center.WinSize=ROISize;
    tmp_center.OrigSize=cvm.size();
    cv::setMouseCallback("ROI selection",on_mouse,&tmp_center);

    cout<< "Press r to proceed" << std::endl;
    cv::Mat drawing;
    cvm.copyTo(drawing);
    
    char key=' ';
    while(key!='q'){
        brightness=0.2+(2-0.2)*(ind_brightness-1)/ind_brightness_max;
        if(tmp_center.status){
            cvm.copyTo(drawing);
            cv::rectangle(drawing,tmp_center.pt1,tmp_center.pt2,255,4,8,0);
            ostringstream info;
            info<<"center = ("<<tmp_center.center.x<<','<<tmp_center.center.y<<")";
            cv::imshow("ROI selection",brightness*drawing);
            cv::displayStatusBar("ROI selection",info.str(),0);
            center=tmp_center;
        } else cv::imshow("ROI selection",brightness*drawing);

        key=cv::waitKey(10);
        if(key=='r'){
            recording=1;
            break;
        }

    }

    cv::destroyWindow("ROI selection");
}

void Select_ROI(cv::Mat &cvm, ioparam &center, int &recording, int ROISize=100){

    int ind_brightness=0, ind_brightness_max=100;
    double brightness;
    cv::namedWindow("ROI selection",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
    cv::resizeWindow("ROI selection", 800,800);

    // Setup ROI
    ioparam tmp_center;
    tmp_center.status=0;
    tmp_center.WinSize=ROISize;
    tmp_center.OrigSize=cvm.size();
    cv::setMouseCallback("ROI selection",on_mouse,&tmp_center);
    cv::createTrackbar( "brightness", "ROI selection", &ind_brightness, ind_brightness_max);

    cv::Mat drawing;
    cvm.copyTo(drawing);

    char key=' ';
    while(key!='q'){
        brightness=0.2+(2-0.2)*(ind_brightness-1)/ind_brightness_max;
        if(tmp_center.status){
            cvm.copyTo(drawing);
            cv::rectangle(drawing,tmp_center.pt1,tmp_center.pt2,255,4,8,0);
            ostringstream info;
            info<<"center = ("<<tmp_center.center.x<<','<<tmp_center.center.y<<")";
            cv::imshow("ROI selection",brightness*drawing);
            cv::displayStatusBar("ROI selection",info.str(),0);
            center=tmp_center;
        } else cv::imshow("ROI selection",brightness*drawing);

        key=cv::waitKey(10);
        if(key=='r'){
            recording=1;
            break;
        }

    }

    cv::destroyWindow("ROI selection");
}

int Rec_SingleCamera(void* tdata)
{
    FlyCapture2::Error error;

    struct thread_data2 * RSC_input;
    RSC_input = (struct thread_data2*) tdata;
    size_t seq_size = RSC_input->seq_size;
    ioparam center;
    int recording=0;
    
    if(RSC_input->crop){
        Select_ROI(RSC_input->cam, center, recording);
        if(!recording){
            cout<<"Quit."<<endl;
            exit(0);
        }
    } else recording=1;

    cout<<"RECORDING..."<<endl;
    
    vector<cv::Mat> imvec(seq_size);
    cout<<seq_size<<endl;
    Image rawImage;
    unsigned char* data;
    cv::Mat tmp_image;
    ofstream logfile("log.txt");
    
    if(recording){
        long int ms0 = cv::getTickCount();
        for(unsigned int i=0;i<imvec.size();i++){

            RSC_input->cam->RetrieveBuffer(&rawImage);
            long int ms1 = cv::getTickCount();
            double delta = (ms1-ms0)/cv::getTickFrequency();
            logfile<<i<<' '<<delta<<' '<<endl;
            data = rawImage.GetData();
            cv::Mat cvm(rawImage.GetRows(),rawImage.GetCols(),CV_8U,(void*)data);
            if(RSC_input->crop)
                tmp_image=cvm(cv::Range(center.pt1.y,center.pt2.y),cv::Range(center.pt1.x,center.pt2.x));
            else
                tmp_image=cvm;
            tmp_image.copyTo(imvec[i]);
        }
    }
    // Stop capturing images
    error = RSC_input->cam->StopCapture();

    // Disconnect the camera
    error = RSC_input->cam->Disconnect();
    
    if(recording){
        CreateOutputFolder(RSC_input->proc_folder);

        for(unsigned int i=0;i<imvec.size();i++){
            stringstream filename;
            filename<<RSC_input->proc_folder<<"/"<<i<<".pgm";
            imwrite(filename.str().c_str(),imvec[i]);
        }
    }

}


void *Rec_onDisk_SingleCamera2(void *tdata,size_t &frame_counter)
{
    signal(SIGINT,my_handler);
    struct thread_data2 * RSC_input;
    RSC_input = (struct thread_data2*) tdata;

    FlyCapture2::Error error;
    CreateOutputFolder(RSC_input->proc_folder);

    Image rawImage;
    cv::Mat tmp_image;

    unsigned char* data;
    
    int recording=0;
    ioparam center;
    ioparam tmp_center;
    
    stringstream logfilename;
    logfilename	<< RSC_input->proc_folder<<"/time.log";
    ofstream logfile(logfilename.str().c_str());

    if(RSC_input->crop){
        Select_ROI(RSC_input->cam, center , recording);
        if(!recording){
            cout<<"Quit."<<endl;
            exit(0);
        }
    } else {
        recording=1;
    }

    cout<<"RECORDING..."<<endl;

    unsigned int i=0;
    //long int ms0 = cv::getTickCount();
    
    frame_counter=0;
    while(recording && run){
        RSC_input->cam->RetrieveBuffer(&rawImage);
        TimeStamp tsmp = rawImage.GetTimeStamp();
        frame_counter++;
        int64 ms1 = cv::getTickCount();
        //double delta = (ms1-ms0)/cv::getTickFrequency();
        logfile<<ms1<<' '<<cv::getTickFrequency()<<' '<<tsmp.microSeconds<<' '<<tsmp.cycleCount<<endl;


        data = rawImage.GetData();
        cv::Mat cvm(rawImage.GetRows(),rawImage.GetCols(),CV_8U,(void*)data);
        if(RSC_input->crop){
            tmp_image=cvm(cv::Range(center.pt1.y,center.pt2.y),cv::Range(center.pt1.x,center.pt2.x));
        } else {
            tmp_image=cvm;
        }

        stringstream filename;

        filename<<RSC_input->proc_folder<<"/"<<fixedLengthString(i)<<".pgm";
        if(tmp_image.empty()) cout<<center.center.x<<' '<<center.center.y<<endl;
        imwrite(filename.str().c_str(),tmp_image); //CV_IMWRITE_PXM_BINARY
        i++;

    }

    // Stop capturing images
    error = RSC_input->cam->StopCapture();
    
    // Disconnect the camera
    error = RSC_input->cam->Disconnect();

    return 0;
}

void ReadImageSeq(string prefix,char* display, int mode, char* format,char* prefix0,int maxind){
    int ind=0;
    int ind_brightness=1;
    int ind_brightness_max=100;
    double brightness;
    cv::Mat image;
    cv::namedWindow(display,cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO );
    cv::resizeWindow(display, 800,800);
    cv::createTrackbar( "level", display, &ind, maxind);
    cv::createTrackbar( "brightness", display, &ind_brightness, ind_brightness_max);

    cout << "Reading image sequence. Press q to exit." << endl;
    char c='1';
    while(c!='q'){
        if(c=='f') ind++;
        if(c=='b') ind=max(0,ind-1);
        stringstream filename;
        if(mode==0){
            filename<<prefix<<'/'<<fixedLengthString(ind)<<".pgm";
        } else {
            filename<<prefix<<'/'<<ind<<format;
        }
        image=imread(filename.str().c_str(),cv::IMREAD_UNCHANGED);
        if(!image.empty()){
            brightness=0.2+(2-0.2)*(ind_brightness-1)/ind_brightness_max;
            imshow(display,brightness*image);
        }
        //else
        //break;
        c=cv::waitKey(10);
    }
}

void ReadImageSeq_and_track(string prefix,char* display, int mode, char* format,int maxind){

    unsigned int i;

    bool hide_trace=false;

    int64 tmptime,tmpframe;
    double tmpfreq;
    int tmprec;
    string tmplab;
    ifstream tcamera_file((prefix+"/time.log").c_str());
    vector<int64> tcam;
    vector<int> stimseq, frameseq,framerec;

    while(tcamera_file>>tmptime>>tmpfreq>>tmprec) {
        tcam.push_back(tmptime);
        framerec.push_back(tmprec);
    }

    int ind=0;
    int speed=1;
    int max_angle=20;
    int blur=2;
    int circle_size=3;
    int AP_N=10;
    double th;
    int th_int=0;
    int step=17;
    int brightness_int=0;
    double brightness;

    cv::Mat image;

    // Get index of the first recorded image ///////////////////////////////
    while(image.empty()){
        stringstream ss;
        ss<<prefix<<'/'<<fixedLengthString(ind)<<".pgm";
        image=cv::imread(ss.str().c_str(),cv::IMREAD_UNCHANGED);
        //cout<<ss.str()<<endl;
        ind++;
    }
    // #####################################################################


    cv::namedWindow(display,cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO );
    cv::namedWindow("fg mask",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO );
    cv::resizeWindow(display, 800,800);
    cv::createTrackbar( "level", display, &ind, maxind);
    cv::createTrackbar( "speed", display, &speed, 50);
    cv::createTrackbar( "max_angle", display, &max_angle, 50);
    cv::createTrackbar( "anchors", display, &AP_N, 50);
    cv::createTrackbar( "th", display, &th_int, 500);
    cv::createTrackbar( "step", display, &step, 20);
    cv::createTrackbar( "blur", display, &blur, 8);
    cv::createTrackbar( "circle size", display, &circle_size, 8);
    cv::createTrackbar( "brightness", display, &brightness_int, 100);

    cout << "Reading image sequence. Press q to exit." << endl;
    char c='1';

    mouse_GetVector_param p; p.status=false;
    cv::setMouseCallback(display,mouse_GetVector,&p);

    while(c!='x'){
        stringstream ss;
        ss<<prefix<<'/'<<fixedLengthString(ind)<<".pgm";
        image=cv::imread(ss.str().c_str(),cv::IMREAD_UNCHANGED);

        if(p.status) {
            cv::arrowedLine(image,p.pt1,p.pt2,255);
            cv::circle(image,p.pt2,10,255);
        }
        if(!image.empty()) {
            brightness=0.5+(brightness_int-1)/100.*(10-0.5);
            cv::imshow(display,brightness*image);            
        }
        c=cv::waitKey(10);
        if(c=='q'){break; cv::destroyAllWindows(); exit(0);}
    }

    cv::Mat background(image.size(),CV_32F,cv::Scalar(0));

    int counter=0;
    i=ind;
    while(i<1000){
        stringstream filename;
        filename<<prefix<<'/'<<fixedLengthString(i)<<".pgm";
        image=cv::imread(filename.str().c_str(),cv::IMREAD_UNCHANGED);
        if(!image.empty()){
            image.convertTo(image,CV_32F);
            background+=image;
            counter++;
        }
        i++;
        cout<<i<<"       \r";
    }

    cout<<endl;
    background/=counter;
    background.convertTo(background,CV_8U);


    while(c!='q'){
        if(c=='f') ind+=speed;
        if(c=='t') ind+=40;
        if(c=='b') ind=max(0,ind-speed);
        if(c=='h') hide_trace=!hide_trace;
        stringstream filename;

        filename<<prefix<<'/'<<fixedLengthString(ind)<<".pgm";

        image=cv::imread(filename.str().c_str(),cv::IMREAD_UNCHANGED);

        if(!image.empty()){
            vector<cv::Point2i> a_pts;
            cv::Point2d tangent;
            tangent=p.pt2-p.pt1;
            th=-20+40./500*th_int;

            get_interp_quadsearch(image,p.pt1,tangent,step,a_pts,AP_N,max_angle,th,2*blur+1,circle_size,background);
            if(!hide_trace) for(unsigned int j=0;j<a_pts.size()-1;++j) cv::line(image,a_pts[j],a_pts[j+1],255,1);

            brightness=0.5+(brightness_int-1)/100.*(10-0.5);
            cv::imshow(display,brightness*image);

            cv::Mat sub;
            cv::subtract(image,background,sub);


            cv::imshow("fg mask",brightness*(0.1*image+sub*0.9));
        }

        cv::setTrackbarPos("level",display,ind);

        c=cv::waitKey(10);
    }

    cv::destroyWindow(display);
}


void ReadImageSeq_vs(string prefix,char* display, int mode, char* format, barrage *Barrage,int maxind){

    unsigned int i;  

    bool hide_trace=false;

    // Build barrage

    map<int,int> StimMap;
    ifstream StimList_file(prefix+"/epoch_order.log");

    vector<stim> StimList;

    int str;
    int counter=0;
    bool found=false;

    while(StimList_file>>str){
        for(unsigned int k=0;k<StimList.size();++k){
            if(str==(int)StimList[k]) found=true;
        }
        if(!found){
            StimList.push_back((stim)str);
            StimMap[str]=counter;
            counter++;
        } else break;
    }

    cv::Mat mask_mat(Barrage->H,Barrage->W,CV_8U);
    Barrage->get_vs_mask(mask_mat);
    vector<unsigned char*> stimdata(StimList.size());

    for(i=0;i<StimList.size();i++){
        stimdata[i] = new unsigned char[Barrage->H*Barrage->W*(Barrage->nframes_vec[(int)StimList[i]]+1)];
    }


    Barrage->FillPoints(stimdata,StimList);


    int64 tmptime,tmpframe;
    double tmpfreq;
    int tmprec;
    string tmplab;
    ifstream tcamera_file((prefix+"/time.log").c_str());
    ifstream tvs_file((prefix+"/ticks.log").c_str());
    vector<int64> tcam;
    vector<int64> tvs;
    vector<int> stimseq, frameseq,framerec;

    while(tcamera_file>>tmptime>>tmpfreq>>tmprec) {
        tcam.push_back(tmptime);
        framerec.push_back(tmprec);
    }

    while(tvs_file>>tmptime>>tmplab>>tmpframe){
        tvs.push_back(tmptime);
        if(strcmp(tmplab.c_str(),"-1")==0) stimseq.push_back(-1);
        else stimseq.push_back(StimMap[(int)Barrage->string_to_stim(tmplab.c_str())]);
        frameseq.push_back(tmpframe);
    }


    int ind=0,ind_vs=0;
    int speed=1;
    int max_angle=20;
    int blur=4;
    int AP_N=20;
    double th;
    int th_int=0;
    int step=4;
    int brightness_int=0;
    double brightness;

    cv::Mat image,image_vs;

    // Get index of the first recorded image ///////////////////////////////
    while(image.empty()){
        stringstream ss;
        ss<<prefix<<'/'<<fixedLengthString(ind)<<".pgm";
        image=cv::imread(ss.str().c_str(),cv::IMREAD_UNCHANGED);
        ind++;
    }
    // #####################################################################

    cv::namedWindow(display,cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO );
    cv::namedWindow("vs",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO );
    cv::resizeWindow(display, 800,800);
    cv::createTrackbar( "level", display, &ind, maxind);
    cv::createTrackbar( "speed", display, &speed, 50);
    cv::createTrackbar( "max_angle", display, &max_angle, 50);
    cv::createTrackbar( "anchors", display, &AP_N, 50);
    cv::createTrackbar( "th", display, &th_int, 50);
    cv::createTrackbar( "step", display, &step, 20);
    cv::createTrackbar( "blur", display, &blur, 8);
    cv::createTrackbar( "brightness", display, &brightness_int, 100);

    cout << "Reading image sequence. Press q to exit." << endl;
    char c='1';

    mouse_GetVector_param p; p.status=false;
    cv::setMouseCallback(display,mouse_GetVector,&p);

    while(c!='x'){
        stringstream ss;
        ss<<prefix<<'/'<<fixedLengthString(ind)<<".pgm";
        image=cv::imread(ss.str().c_str(),cv::IMREAD_UNCHANGED);

        if(p.status) {
            cv::arrowedLine(image,p.pt1,p.pt2,255);
            cv::circle(image,p.pt2,10,255);
        }
        if(!image.empty()) {
            brightness=0.5+(brightness_int-1)/100.*(4-0.5);
            cv::imshow(display,brightness*image);
        }
        c=cv::waitKey(10);
    }

    while(c!='q'){
        if(c=='f') ind+=speed;
        if(c=='t') ind+=40;
        if(c=='b') ind=max(0,ind-speed);
        if(c=='h') hide_trace=!hide_trace;
        stringstream filename;

        filename<<prefix<<'/'<<fixedLengthString(ind)<<".pgm";

        image=cv::imread(filename.str().c_str(),cv::IMREAD_UNCHANGED);
        /// ???
        ind_vs=Barrage->matchtime(tcam[ind],tvs);

        if(stimseq[ind_vs]<0){
            image_vs=mask_mat;
        } else {
            image_vs.data=stimdata[stimseq[ind_vs]]+Barrage->H*Barrage->W*frameseq[ind_vs];
        }

        if(!image.empty()){
            vector<cv::Point2i> a_pts;
            cv::Point2d tangent;
            tangent=p.pt2-p.pt1;
            th=-2+2./50*th_int;

            get_interp4(image,p.pt1,tangent,step,a_pts,AP_N,max_angle,th,2*blur+1);
            if(!hide_trace) for(unsigned int j=0;j<a_pts.size()-1;++j) cv::line(image,a_pts[j],a_pts[j+1],255,1);

            brightness=0.5+(brightness_int-1)/100.*(4-0.5);
            cv::imshow(display,brightness*image);
        }

        cv::setTrackbarPos("level",display,ind);

        if(!image_vs.empty())
            imshow("vs",image_vs);
        //else
        //break;
        c=cv::waitKey(10);
    }

    cv::destroyAllWindows();
}


// DEPRECATED FUNCTION
/*
int ChangeTrigger(thread_data2 *input){


    input->cam->StopCapture();
    input->cam->Disconnect();

    input->bus->GetCameraFromIndex(0, input->guid);
    F7 f7;

    Mode mode;
    mode=MODE_1;

    input->cam->Connect(input->guid);
    SetCam(input->cam,f7,mode,PIXEL_FORMAT_RAW8,true);
    input->cam->StartCapture();

    Image rawImage;
    input->cam->RetrieveBuffer(&rawImage);

}
*/

int Run_SingleCamera(PGRGuid *guid)
{

//    FlyCapture2::Error error;

//    // Connect to a camera
//    Camera cam;
//    cam.Connect(&guid);

//    // Get the camera configuration
//    FC2Config config;
//    error = cam.GetConfiguration(&config);

//    // Set the number of driver buffers used to 20.
//    config.numBuffers = 10;

//    // Set the camera configuration
//    cam.SetConfiguration(&config);

//    CameraInfo cInfo;
//    cam.GetCameraInfo(&cInfo);
//    PrintCameraInfo(&cInfo);

//    // Set format7 custom mode
//    const Mode k_fmt7Mode = MODE_1;
//    const PixelFormat k_fmt7PixFmt = PIXEL_FORMAT_RAW8;

//    Format7Info fmt7Info;
//    bool supported;
//    fmt7Info.mode=k_fmt7Mode;
//    cam.GetFormat7Info(&fmt7Info, &supported);
//    PrintFormat7Capabilities(fmt7Info);

//    Format7ImageSettings fmt7ImageSettings;
//    fmt7ImageSettings.mode = k_fmt7Mode;
//    fmt7ImageSettings.offsetX = 0;
//    fmt7ImageSettings.offsetY = 0;
//    fmt7ImageSettings.width = fmt7Info.maxWidth;
//    fmt7ImageSettings.height = fmt7Info.maxHeight;
//    fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;
//    bool valid;
//    Format7PacketInfo fmt7PacketInfo;

//    cam.ValidateFormat7Settings(&fmt7ImageSettings, &valid, &fmt7PacketInfo);
//    cam.SetFormat7Configuration(&fmt7ImageSettings, fmt7PacketInfo.recommendedBytesPerPacket);


// ALTERNATIVE /////////////////////////////////
    FlyCapture2::Error error;
    BusManager busMgr;
    busMgr.GetCameraFromIndex(0, guid);
    F7 f7;

    Mode mode;
    mode=MODE_1;

    Camera cam;
    cam.Connect(guid);
    SetCam(&cam,f7,mode,PIXEL_FORMAT_RAW8,false);
// /////////////////////////////////////////////

    Image rawImage;
    namedWindow("display",cv::WINDOW_NORMAL);
    int ind=0;
    int maxind=100;
    cv::createTrackbar( "Brightness", "display", &ind, maxind);

    while (cv::waitKey(30)!='q')
    {
        // Retrieve an image        
        cam.RetrieveBuffer(&rawImage);

        // Create a converted image
        Image convertedImage=rawImage;

        // Convert the raw image
        //rawImage.Convert(PIXEL_FORMAT_MONO8, &convertedImage);
        
        unsigned char* data = convertedImage.GetData();
        cv::Mat cvm(convertedImage.GetRows(),convertedImage.GetCols(),CV_8U,(void*)data);
        cv::transpose(cvm,cvm);
        cv::imshow("display",(1+ind)*5/maxind*cvm);
        //cv::imshow("display",cvm);

    }

    // Stop capturing images
    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        cout<<"ERROR!";
        return -1;
    }

    // Disconnect the camera
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
        cout<<"ERROR!";
        return -1;
    }

    return 0;
}


void display_blobs(circular_buffer_ts &circ_buffer){

    mtx.lock();
    cout<<"run display_image on thread "<<boost::this_thread::get_id()<<endl;
    mtx.unlock();

    cv::namedWindow("display",cv::WINDOW_NORMAL);
    cv::resizeWindow("display",400,400);

    cv::namedWindow("camera",cv::WINDOW_NORMAL);
    cv::resizeWindow("camera",400,400);

    cv::Mat image;
    cv::Mat rawImage;

    long int cfc;
    char c=' ';
    while(c!='q' && run){
        ostringstream info;

        {
            boost::mutex::scoped_lock lk(mtx);
            if(!im_with_keypoints.empty()) im_with_keypoints.copyTo(image);
            circ_buffer.retrieve_last(rawImage, cfc);
        }

        if(!image.empty()){
            imshow("display",image);
            imshow("camera",rawImage);
            cv::displayStatusBar("display",info.str(),0);
        }

        c=cv::waitKey(10);
    }

    cv::destroyWindow("display");
    cv::destroyWindow("camera");
    boost::mutex::scoped_lock lk(mtx);
    run=false;

}



void blob_detector_thread(circular_buffer_ts &circ_buffer,const ioparam &center_head){

    long int last_processed=0;
    long int current_frame_counter;
    int MIN_BLOB_SIZE=5;
    int timer_init=490*1;
    int timer=timer_init;
    bool large_blobs=false;
    setup_blob_detector(detector);


    mtx.lock();
    cout<<"Created blob detector on thread "<<boost::this_thread::get_id()<<endl;
    mtx.unlock();

    while(run){
        cv::Mat image_from_buffer;
        circ_buffer.retrieve_last(image_from_buffer, current_frame_counter);

        // set the headROI to zero
        cv::Mat headROI = image_from_buffer(cv::Range(center_head.pt1.y,center_head.pt2.y),cv::Range(center_head.pt1.x,center_head.pt2.x));
        headROI.setTo(0);


        if(last_processed<current_frame_counter){
            timer-=(current_frame_counter-last_processed);
            {
                // -----------------------------------
                // This line was slowing down the acquisition by obviously locking the mutex
                // when not (??) necessary.
                // boost::mutex::scoped_lock lk(mtx);
                // -----------------------------------

                pMOG->apply(image_from_buffer, fgMaskMOG,learning_rate);

                // Detect blobs.

                std::vector<cv::KeyPoint> keypoints;
                detector->detect( fgMaskMOG, keypoints);

                // Draw detected blobs as red circles.
                // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob

                cv::drawKeypoints( fgMaskMOG, keypoints, im_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

                // DETECTIOn OF LARGE BLOB Triggers Recording - SHORTCUT
                large_blobs=true;
//OVERIDE
//                for(unsigned int l=0;l<keypoints.size();l++){
//                    if(keypoints[l].size>MIN_BLOB_SIZE ){
//                        large_blobs=true;
//                        break;
//                    }
//                }

            } //IF


            if(!circ_buffer.get_recorder_state()){
                if(large_blobs){
                    timer=timer_init;
                    circ_buffer.set_recorder_state(true);

                    mtx.lock();
                    cout<<"------------------------"<<endl;
                    cout<<"recording at time frame "<<current_frame_counter<<endl;
                    mtx.unlock();
                    circ_buffer.write_buffer();
                }
            } else {
                if(!large_blobs && timer<=0){
                    circ_buffer.set_recorder_state(false);
                    mtx.lock();
                    cout<<"Stop recording at time frame "<<current_frame_counter<<endl;
                    cout<<"-----------------------------"<<endl<<endl;
                    mtx.unlock();
                } else if(large_blobs){
                    timer=timer_init;
                }
            }

        }

        last_processed=current_frame_counter;
    }

    mtx.lock();
    cout<<"out of while loop (blob detector)"<<endl;
    mtx.unlock();
}

void recorder_thread(circular_buffer_ts &circ_buffer, thread_data2* const RSC_input, const ioparam &center){

    mtx.lock();
    cout<<"Created main on thread "<<boost::this_thread::get_id()<<endl;
    mtx.unlock();
    int64 initial_time=cv::getTickCount();    

    FlyCapture2::Error error;


    unsigned char* data;

    stringstream logfilename;
    logfilename	<< RSC_input->proc_folder<<"/time.log";
    ofstream logfile(logfilename.str().c_str());

    Image rawImage;

    while(run){
        //RSC_input->cam->FireSoftwareTrigger(false);
        RSC_input->cam->RetrieveBuffer(&rawImage);
        TimeStamp TimeStamp_fromCamera = rawImage.GetTimeStamp(); // use time stamp
        int64 TimeStamp_microseconds = TimeStamp_fromCamera.seconds*1e6+TimeStamp_fromCamera.microSeconds;
        int64 ms1 = cv::getTickCount();  // use clock ticks (less good)

        data = rawImage.GetData();
        cv::Mat cvm(rawImage.GetRows(),rawImage.GetCols(),CV_8U,(void*)data);

        cv::Mat image;

        image=cvm(cv::Range(center.pt1.y,center.pt2.y),cv::Range(center.pt1.x,center.pt2.x));

        //circ_buffer.update_buffer(image,frame_counter,ms1);
        circ_buffer.update_buffer(image,frame_counter,TimeStamp_microseconds);

        if(circ_buffer.get_recorder_state()){

            stringstream filename;
            filename<<RSC_input->proc_folder<<"/"<<fixedLengthString(frame_counter)<<".pgm";
            cv::imwrite(filename.str().c_str(),image);

            mtx.lock();
            if(verbose) cout<<"writing: "<<frame_counter<<' '<<ms1<<endl;
            mtx.unlock();

            circ_buffer.set_last_recorded_index(frame_counter);
        }

        logfile<<ms1<<' '<<TimeStamp_microseconds<<' '<<circ_buffer.get_recorder_state()<<endl;

        mtx.lock();
        frame_counter++;
        mtx.unlock();

        if((cv::getTickCount()-initial_time)/1e9>RSC_input->recording_time){
            mtx.lock();
            run=false;
            mtx.unlock();
        }

    }

    // Stop capturing images
    error = RSC_input->cam->StopCapture();

    // Disconnect the camera
    error = RSC_input->cam->Disconnect();

    mtx.lock();
    cout<<"Camera disconnected."<<endl;
    mtx.unlock();
}

void *Rec_onDisk_conditional(void *tdata,bool VisualStimulation_ON, barrage *Barrage)
{

    // Set SIGINT
    //signal(SIGINT,my_handler);

    // Retrieve RSC input from main
    struct thread_data2 * RSC_input;
    RSC_input = (struct thread_data2*) tdata;

    int BUFFER_SIZE=200;
    int ROI_acquired=0;
    ioparam center, center_head;


    // Image format from the camera and corresponding mat object
    Image rawImage;
    cv::Mat tmp_image;
    unsigned char* data;

    // Frames recorded when writing the buffer are specified in bufferfile
    stringstream bufferfilename;
    bufferfilename<<RSC_input->proc_folder<<"/buffer.log";;
    ofstream bufferfile(bufferfilename.str().c_str());

    // Creation of the thread-safe curcular buffer
    circular_buffer_ts circ_buffer(BUFFER_SIZE,RSC_input->proc_folder,&bufferfile);

    // Setting the background model
    pMOG = cv::createBackgroundSubtractorMOG2(2000,16,true);

    // Select tail ROI //////////////////////////////////////////////////////////////
    Select_ROI(RSC_input->cam, center , ROI_acquired, 100);
    if(!ROI_acquired){
        cout<<"Error with ROI detection"<<endl;
        exit(0);
    }
    // ##############################################################################

    // Retrieve a single image to get the relative head ROI relative to the fish ROI
    // NO LONGER USED
//    RSC_input->cam->RetrieveBuffer(&rawImage);
//    data = rawImage.GetData();
//    cv::Mat fishROI(rawImage.GetRows(),rawImage.GetCols(),CV_8U,(void*)data);
//    tmp_image = fishROI(cv::Range(center.pt1.y,center.pt2.y),cv::Range(center.pt1.x,center.pt2.x));

    // ##############################################################################

    // Select head ROI //////////////////////////////////////////////////////////////
    // note: center_head is now relative to the fish ROI.
    // NO LONGER USED
//    Select_ROI(tmp_image, center_head , ROI_acquired, 40);
//    if(!ROI_acquired){
//        cout<<"Error with head ROI detection"<<endl;
//        exit(0);
//    }
    // ##############################################################################

    // Snippet for detecting laser scanning on the head ROI /////////////
//    double rat=1;
//    double old_sum=-1, new_sum;
//    while(rat<1.5 && run){
//        RSC_input->cam->RetrieveBuffer(&rawImage);
//        data = rawImage.GetData();
//        cv::Mat cvm(rawImage.GetRows(),rawImage.GetCols(),CV_8U,(void*)data);
//       cv::Mat image,headROI;
//        image=cvm(cv::Range(center.pt1.y,center.pt2.y),cv::Range(center.pt1.x,center.pt2.x));
//        headROI=image(cv::Range(center_head.pt1.y,center_head.pt2.y),cv::Range(center_head.pt1.x,center_head.pt2.x));
//        if(old_sum<0){
//            old_sum=cv::sum(headROI)[0];
//            new_sum=old_sum;
//        } else {
//            old_sum=new_sum;
//            new_sum=cv::sum(headROI)[0];
//            if(new_sum>0) rat=new_sum/old_sum;
//        }
//    }
    // ####################################################################


    
    RSC_input->cam->RetrieveBuffer(&rawImage);
    TimeStamp TS=rawImage.GetTimeStamp();
    cout<<"Start!"<<endl;
    
    F7 f7;
    SetCam(RSC_input->cam,f7,MODE_1,PIXEL_FORMAT_RAW8,false);

    stringstream logfilename;
    logfilename	<< RSC_input->proc_folder<<"/time.log";
    ofstream logfile(logfilename.str().c_str());
    logfile<<cv::getTickCount()<<' '<<TS.seconds*1e6+TS.microSeconds<<' '<<0<<endl;
    logfile.close();

    // Fill buffer ////////////////////////////////////////////////////////
    for(unsigned int k=0;k<BUFFER_SIZE;k++){

        FlyCapture2::Error error=RSC_input->cam->RetrieveBuffer(&rawImage);
        int64 ms1 = cv::getTickCount();
        TS=rawImage.GetTimeStamp();

        data = rawImage.GetData();
        cv::Mat cvm(rawImage.GetRows(),rawImage.GetCols(),CV_8U,(void*)data);

        cv::Mat image;
        image=cvm(cv::Range(center.pt1.y,center.pt2.y),cv::Range(center.pt1.x,center.pt2.x));

        if (image.empty())
        {

            cout << "Press Enter to exit." << endl;
            cin.ignore();
        }

        image.copyTo(tmp_image);
        circ_buffer.update_buffer(tmp_image,k,TS.seconds*1e6+TS.microSeconds);
    }
    // ####################################################################

    // Start rec and proc threads /////////////////////////////////////////////////////////////////////
    boost::thread T_REC(recorder_thread, std::ref(circ_buffer), std::ref(RSC_input),std::ref(center));
    boost::thread T_PROC(blob_detector_thread,std::ref(circ_buffer),std::ref(center_head));
    // ################################################################################################

    if(VisualStimulation_ON && run){
        if(!Barrage->Background_ON) Barrage->VisualStimulation(RSC_input->proc_folder,run);
        else Barrage->VisualStimulation_BG(RSC_input->proc_folder,run);
    } else display_blobs(circ_buffer);

    if(T_REC.joinable()){
        T_REC.join();
    }
    if(T_PROC.joinable()){
        T_PROC.join();
    }

    cv::destroyAllWindows();
    return 0;
}


