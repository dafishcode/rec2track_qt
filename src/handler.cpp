#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include<fstream>
#include"../include/handler.h"

using namespace std;
using namespace cv;

int get_angle_deg(const Point2i& tgt){
    int angle;
    if(tgt.x>0 ){
        angle=floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
    } else if(tgt.x<0) {
        angle=180+floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
    } else if(tgt.x==0){
        if(tgt.y>0) angle=90;
        else angle=-90;
    }
    return(angle);
}

void mouse_GetVector(int event, int x, int y, int flags, void* p){
    mouse_GetVector_param* param=(mouse_GetVector_param*)p;
	if  ( event == cv::EVENT_LBUTTONDOWN ){
	    param->pt1.x=x;
		param->pt1.y=y;
		param->status=true;
    } 
    
	if (param->status==true){
		param->pt2.x=x;
		param->pt2.y=y;
	}

    if (event==cv::EVENT_LBUTTONUP ){
            param->status=false;
    }
	   
}

void get_interp(Mat &src, Point2i start, Point2d tgt, vector<Point2i>& anchor_pts,Mat mask){
	MatIterator_<uchar> it_mask;
	MatIterator_<float> it=src.begin<float>();
	int counter=0;
	int x,y;
	int cols=src.cols;
	double av_x=0, av_y=0, weight=1;
	tgt.x/=tgt.dot(tgt);
	tgt.y/=tgt.dot(tgt);
	Point2i pos=start;
	int n_anch=0;

	while(n_anch<20){
		Mat mask2 = Mat::zeros(mask.rows,mask.cols,CV_8U);
        
		circle(mask2,pos,6,255,1);
		src.copyTo(mask,mask2);
		it=src.begin<float>();
		counter=0;
		av_x=0; av_y=0; weight=1;
		for(it_mask=mask2.begin<uchar>();it_mask!=mask2.end<uchar>();++it_mask){
			y=counter/cols;
			x=counter%cols;
			if(*it_mask>0) {
				if((x-pos.x)*tgt.x+(y-pos.y)*tgt.y>0){
				//if((x-pos.x)*tgt.x+(y-pos.y)*tgt.y>sqrt(pow(x-pos.x,2)+pow(y-pos.y,2))*sqrt(tgt.dot(tgt))*0.6){
					if(*it<weight) {
						av_x=x;
						av_y=y;
						weight=*it;
					}
				}
			}
			++it;
			counter++;
		}
		anchor_pts.push_back(Point2i(floor(av_x),floor(av_y)));
		tgt=anchor_pts.back()-pos;
		tgt.x/=tgt.dot(tgt);
		tgt.y/=tgt.dot(tgt);
		
		pos=anchor_pts.back();
		n_anch++;
	}

}

void get_interp2(Mat &src, Point2i start, Point2d tgt_start, double step_size, vector<Point2i>& anchor_pts){
	const size_t AP_N=18;
	vector<Point2i> tmp_pts(AP_N);
	Point2i tgt;
	anchor_pts.resize(AP_N);
	anchor_pts[0]=start;
	tmp_pts[0]=start;
	unsigned int k=0;
	double loc,loc_add, val=0;
	int angle;
	srand(2);
	ofstream out("file.log");
	
	for(unsigned int counter=0;counter<7000;counter++){
		loc=0;
		for(k=1;k<AP_N;k++){			
			vector<Point2i> ellipse_pts;
			if(k==1) tgt=tgt_start;
			else tgt=tmp_pts[k-1]-tmp_pts[k-2];
			
			if(tgt.x>0 ){
				angle=floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
			} else if(tgt.x<0) {
				angle=180+floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
			} else if(tgt.x==0){
				if(tgt.y>0) angle=90;
				else angle=-90;
			}

			ellipse2Poly(tmp_pts[k-1], Size(4,4), 0, angle-20, angle+20, 1, ellipse_pts);
					    
			int index;
			loc_add=0;
			while(loc_add==0){
				index=rand() % ellipse_pts.size();
				loc_add=src.at<float>(ellipse_pts[index].y,ellipse_pts[index].x);
			}
			tmp_pts[k]=ellipse_pts[index];
			out<<tgt<<' '<<angle<<' '<<tmp_pts[k]<<' '<<loc<<' '<<index<<' '<<ellipse_pts.size()<<endl;
			loc+=loc_add;

			//if(loc>val) break;
		}
	    
		if(loc > val){
			val=loc;
			for(k=1;k<AP_N;++k) anchor_pts[k]=tmp_pts[k];
		}
		out<<endl;
	}
}


void get_interp3(Mat &src, Point2i start, Point2d tgt_start, double step_size, vector<Point2i>& anchor_pts){
	const size_t AP_N=20;
	vector<Point2i> tmp_pts(AP_N);
	Point2i tgt;
	anchor_pts.resize(AP_N);
	anchor_pts[0]=start;
	tmp_pts[0]=start;
	unsigned int k=0;
	double loc,loc_add, val=0;
	int angle;
	ofstream out("file.log");
	
	for(k=1;k<AP_N;k++){			
		vector<Point2i> ellipse_pts;
		if(k==1) tgt=tgt_start;
		else tgt=tmp_pts[k-1]-tmp_pts[k-2];
		
		if(tgt.x>0 ){
			angle=floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
		} else if(tgt.x<0) {
			angle=180+floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
		} else if(tgt.x==0){
			if(tgt.y>0) angle=90;
			else angle=-90;
		}
		
		ellipse2Poly(tmp_pts[k-1], Size(4,4), 0, angle-20, angle+20, 1, ellipse_pts);
		
		int index;
		loc_add=0;
		index=0;
		for(index=0;index<ellipse_pts.size();++index){
			loc=src.at<float>(ellipse_pts[index].y,ellipse_pts[index].x);
			if(loc>loc_add){
				tmp_pts[k]=ellipse_pts[index];
				loc_add=loc;
			}
		}
		//out<<tgt<<' '<<angle<<' '<<tmp_pts[k]<<' '<<loc<<' '<<index<<' '<<ellipse_pts.size()<<endl;
	    
		anchor_pts[k]=tmp_pts[k];
	}
	out<<endl;
}

void get_interp4(Mat &src0, Point2i start, Point2d tgt_start, 
                 int step, vector<Point2i>& anchor_pts,
				 const size_t AP_N,int max_angle,double threshold,int blur){
	
    Mat kernel = (Mat_<float>(3,3) << 1,  1, 1,
	                                  1, -8, 1,
									  1,  1, 1);

	Mat src,src1;
	Mat ones(src0.rows,src0.cols,CV_32F,Scalar(1));
	src0.convertTo(src,CV_32F,1./255);
	src1 = Mat::zeros(src.size(),CV_32F);
	Mat draw, imgLaplacian,mask, draw_inv;
	GaussianBlur(src,draw,Size(blur,blur),2*blur,2*blur);
	//bilateralFilter(src, draw, blur, 1, 1, BORDER_DEFAULT );
	filter2D(draw,imgLaplacian,CV_32F,kernel);
	cv::threshold(-imgLaplacian, mask,threshold,1,THRESH_BINARY);
	mask.convertTo(mask,CV_8U);
	//draw_inv=ones-draw;
	draw_inv=draw;
	draw_inv.copyTo(src1,mask);

	vector<Point2i> tmp_pts(AP_N);
	Point2i tgt;
	anchor_pts.resize(AP_N);
	anchor_pts[0]=start;
	tmp_pts[0]=start;
	unsigned int k=0;
	double loc,loc_add, val=0;
	int angle;
	ofstream out("file.log");
	
	for(k=1;k<AP_N;k++){			
		vector<Point2i> ellipse_pts;
		if(k==1) tgt=tgt_start;
		else tgt=tmp_pts[k-1]-tmp_pts[k-2];
		
		if(tgt.x>0 ){
			angle=floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
		} else if(tgt.x<0) {
			angle=180+floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
		} else if(tgt.x==0){
			if(tgt.y>0) angle=90;
			else angle=-90;
		}
		
		ellipse2Poly(tmp_pts[k-1], Size(step,step), 0, angle-max_angle, angle+max_angle, 1, ellipse_pts);
		
		int index;
		loc_add=0;
		index=0;
		for(index=0;index<ellipse_pts.size();++index){
			loc=src1.at<float>(ellipse_pts[index].y,ellipse_pts[index].x);
			if(loc>loc_add){
				tmp_pts[k]=ellipse_pts[index];
				loc_add=loc;
			}
		}

		//out<<tgt<<' '<<angle<<' '<<tmp_pts[k]<<' '<<loc<<' '<<index<<' '<<ellipse_pts.size()<<endl;
	    
		anchor_pts[k]=tmp_pts[k];
	}
	out<<endl;
}

// Uses fillPoly to use the cones instead of single pixel values.
void get_interp5(Mat &src0, Point2i start, Point2d tgt_start,
                 int step, vector<Point2i>& anchor_pts,
                 const size_t AP_N,int max_angle,double threshold,int blur){

    Mat kernel = (Mat_<float>(3,3) << 1,  1, 1,
                                      1, -8, 1,
                                      1,  1, 1);

    Mat src,src1;
    Mat ones(src0.rows,src0.cols,CV_32F,Scalar(1));

    src0.convertTo(src,CV_32F,1./255);
    src1 = Mat::zeros(src.size(),CV_32F);
    Mat draw, imgLaplacian,mask, draw_inv;
    GaussianBlur(src,draw,Size(blur,blur),2*blur,2*blur);

    filter2D(draw,imgLaplacian,CV_32F,kernel);
    cv::threshold(imgLaplacian, mask,threshold,1,THRESH_BINARY);
    mask.convertTo(mask,CV_8U);
    //draw_inv=ones-draw;
    draw_inv=draw;
    draw_inv.copyTo(src1,mask);

    vector<Point2i> tmp_pts(AP_N);
    Point2i tgt;
    Point2i mid;
    anchor_pts.resize(AP_N);
    anchor_pts[0]=start;
    tmp_pts[0]=start;
    unsigned int k=0;
    double loc,loc_add, val=0;
    int angle;
    ofstream out("file.log");

    for(k=1;k<AP_N;k++){
        vector<Point2i> ellipse_pts;
        if(k==1) tgt=tgt_start;
        else tgt=tmp_pts[k-1]-tmp_pts[k-2];

        if(tgt.x>0 ){
            angle=floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
        } else if(tgt.x<0) {
            angle=180+floor(atan((double)tgt.y/tgt.x)/CV_PI*180);
        } else if(tgt.x==0){
            if(tgt.y>0) angle=90;
            else angle=-90;
        }

        ellipse2Poly(tmp_pts[k-1], Size(step,step), 0, angle-max_angle, angle+max_angle, 1, ellipse_pts);

        int index;
        loc_add=0;
        index=0;
        for(index=0;index<ellipse_pts.size()-1;++index){
            Point2i triangle[1][3];
            triangle[0][0]=tmp_pts[k-1];
            triangle[0][1]=ellipse_pts[index];
            triangle[0][2]=ellipse_pts[index+1];

            const Point* ppt[1] = { triangle[0] };
            int npt[]={3};

            Mat mask(src0.rows,src0.cols,CV_8U,Scalar(0));

            fillPoly(mask,ppt,npt,1,Scalar(1));


            loc=cv::mean(src1,mask)[0];
            if(loc>loc_add){
                tmp_pts[k]=ellipse_pts[index];
                loc_add=loc;
            }
        }

        //out<<tgt<<' '<<angle<<' '<<tmp_pts[k]<<' '<<loc<<' '<<index<<' '<<ellipse_pts.size()<<endl;

        anchor_pts[k]=tmp_pts[k];
    }
    out<<endl;
}

// use 2-points search
void get_interp_quadsearch(Mat &src0, Point2i start, Point2d tgt_start,
                 int step, vector<Point2i>& anchor_pts,
                 const size_t AP_N,int max_angle,double threshold,int blur,int circle_size, const Mat& mask){


    Mat src,src1,sub;
    Mat draw;



    cv::subtract(src0,mask,sub);

    src=0.5*src0+sub*0.5;
    medianBlur(src,src1,blur);

    vector<Point2i> tmp_pts(AP_N);
    Point2i tgt1,tgt2;
    anchor_pts.resize(AP_N);
    anchor_pts[0]=start;
    tmp_pts[0]=start;
    unsigned int k=0;
    double loc1,loc2,loc_add, val=0;
    int angle1, angle2;
    ofstream out("file.log");

    for(k=1;k<AP_N;k++){
        vector<Point2i> ellipse_pts1, ellipse_pts2;
        if(k==1) tgt1=tgt_start;
        else tgt1=tmp_pts[k-1]-tmp_pts[k-2];

        angle1=get_angle_deg(tgt1);

        ellipse2Poly(tmp_pts[k-1], Size(step,step), 0, angle1-max_angle, angle1+max_angle, 1, ellipse_pts1);

        int index1,index2;
        loc_add=0;

        for(index1=0;index1<ellipse_pts1.size();++index1){
            Mat circle1(src.size(),CV_8U,Scalar(0));
            circle(circle1,ellipse_pts1[index1],circle_size,Scalar(1),-1);
            loc1=mean(src1,circle1)[0];

            tgt2=ellipse_pts1[index1]-tmp_pts[k-1];
            angle2=get_angle_deg(tgt2);

            ellipse_pts2.resize(0);
            ellipse2Poly(ellipse_pts1[index1], Size(step,step), 0, angle2-max_angle, angle2+max_angle, 1, ellipse_pts2);

            for(index2=0;index2<ellipse_pts2.size();++index2){
                Mat circle2(src.size(),CV_8U,Scalar(0));
                circle(circle2,ellipse_pts2[index2],circle_size,Scalar(1),-1);
                loc2=loc1+mean(src1,circle2)[0];

                if(loc2>loc_add){
                    tmp_pts[k]=ellipse_pts1[index1];
                    loc_add=loc2;
                }
            }


        }

        anchor_pts[k]=tmp_pts[k];
    }

    out<<endl;
}

// Use k-means exploration strategy
void get_interp_kmeans(Mat &src0, Point2i start, Point2d tgt_start,
                 int step, vector<Point2i>& anchor_pts,
                 const size_t AP_N,int max_angle,double threshold,int blur,int circle_size){

    Mat src;
    src0.convertTo(src,CV_32F,1./255);
    medianBlur(src0,src,blur);

    Point2d tgt;
    anchor_pts.resize(AP_N);
    anchor_pts[0]=start;
    unsigned int k=0;
    ofstream out("file.log");
    Point pt;

    for(k=1;k<AP_N;k++){
        if(k==1) tgt=tgt_start;
        else tgt=anchor_pts[k-1]-anchor_pts[k-2];

        Mat draw;
        Mat circle1(src0.size(),CV_8U,Scalar(0));


        circle(circle1,newp,circle_size,Scalar(1),-1);
        src.copyTo(draw,circle1);

        Moments M = moments(draw);

        anchor_pts[k] = Point(anchor_pts[k-1].x+M.mu01/sqrt(tgt.dot(tgt))*step,
                     anchor_pts[k-1].y+tgt.y/sqrt(tgt.dot(tgt))*step);

        //anchor_pts[k]=Point2i(M.m10/M.m00,M.m01/M.m00);
    }
    out<<endl;
}





