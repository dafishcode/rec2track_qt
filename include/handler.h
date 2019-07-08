#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include<fstream>

using namespace std;

class mouse_GetVector_param {
	public:
	cv::Point2i pt1, pt2;
	bool status;
};

std::string fixedLengthString(int value, int digits = 10); 
void mouse_GetVector(int event, int x, int y, int flags, void* p);
void get_interp(cv::Mat &src, cv::Point2i start, cv::Point2d tgt,vector<cv::Point2i>& anchor_pts,cv::Mat mask);
void get_interp2(cv::Mat &src, cv::Point2i start, cv::Point2d tgt,double, vector<cv::Point2i>& anchor_pts);
void get_interp3(cv::Mat &src, cv::Point2i start, cv::Point2d tgt,double, vector<cv::Point2i>& anchor_pts);
void get_interp4(cv::Mat &src, cv::Point2i start, cv::Point2d tgt,int, vector<cv::Point2i>& anchor_pts,const size_t AP, int,double,int);
