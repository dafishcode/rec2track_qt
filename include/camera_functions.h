#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <FlyCapture2.h>
#include <CameraBase.h>
#include <iostream>
#include <string>
#include"../include/barrage.h"
#include "../include/circular_video_buffer_ts.h"

using namespace std;
using namespace FlyCapture2;

class ioparam {
	public:
	cv::Point2i center;
	cv::Point2i pt1, pt2;
        int WinSize;
        cv::Size OrigSize;
	bool status;
};

class F7 {
	public:
	FC2Config config;
	Format7Info fmt7Info;
	Format7ImageSettings fmt7ImageSettings;
	Format7PacketInfo fmt7PacketInfo;
	bool valid;
	bool supported;
};

struct thread_data{
	PGRGuid *guid;
	char* proc_folder;
	char* display;
	size_t seq_size;
	bool crop;
};

struct thread_data2{
    BusManager *bus;
    PGRGuid *guid;
	Camera *cam;
    string proc_folder;
    char* display;    
    size_t seq_size;
    int recording_time;
    uint eventCount;
    bool crop;   
};

void my_handler(int);

void SetCam(Camera *,F7&,const Mode, const PixelFormat, bool triggerON, float pfFrameRate, float pfShutter);
inline void PrintError(Error error) { error.PrintErrorTrace(); }
std::string fixedLengthString(int value, int digits);
//int ChangeTrigger(thread_data2*);
void CheckOutputFolder(string folder);
void PrintBuildInfo();
void PrintFormat7Capabilities(Format7Info fmt7Info);
void PrintCameraInfo(CameraInfo *pCamInfo);
int Rec_SingleCamera(void*);
void *Rec_onDisk_SingleCamera2(void *tdata, size_t &);
void *Rec_onDisk_conditional(void *tdata,bool VisualStimulation_ON, barrage *Barrage,float fFrameRate,outputType ioutputType);
void ReadImageSeq(string prefix, char* display, int mode=0, char* format=".pgm", char* prefix0="", int maxind=1);
void ReadImageSeq_vs(string prefix, char* display, int mode=0, char* format=".pgm", barrage *B = nullptr, int maxind=1);
void ReadImageSeq_and_track(string prefix,char* display,int mode=0,char* format=".pgm",int maxind=1);

int Run_SingleCamera(PGRGuid*,float pFrameRate, float pfShutter);

#endif
