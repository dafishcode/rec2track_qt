#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "../include/barrage.h"
#include "../include/Point.h"
#include "../include/mainwindow.h"

#include "string.h"
#include <sstream>
#include <fstream>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include<algorithm>
#include <iomanip>
#include<QCoreApplication>


extern MainWindow* gpMainwindow;

using namespace std;

barrage::barrage(){
    pi=acos(-1);

    H=480;
    W=640;

    // X and Y sizes of the projected surface in the tangent plane
    Y0=6.3;
    X0=4./3*Y0;

    // Geometry of the screen/projector
    hs=2.9;                  // height
    d=23.23;                  // distance from the projector
    //double r=42;                   // radius of the screen
    r=3.5/2.;                   // radius of the screen
    th_max=acos(r/(r+d));   // maximal longitudinal angle
    xmax=r*sqrt(d/(2*r+d)); // maximum x position on the tangent plane
    ymax=hs/2.0;            // maximum y position on the tangent plane
    frac=ymax/xmax;

    Hp=hs/Y0*H;
    Wp=Hp/frac;

    // Time settings
    dt=40;
    timeEP=10;
    inter_epoch_time=20;
    //double rp_max=sqrt(pow(d+(r-r*cos(th_max)),2)+pow(r*sin(th_max),2)+pow(get_z((double)th_max,ymax),2));
    nframes=floor(timeEP/dt*1000);
    time_bar_horizontal=8.50115;
    time_bar_vertical=4.870141;
    fading=true;
    flat_background_color=127;

    nframes_vec[0]=floor(4./dt*1000); // LOOM

    for(unsigned int k=1;k<25;k++) nframes_vec[k]=nframes; // WIGGLY+GRATINGS

    nframes_vec[25]=floor(time_bar_horizontal/dt*1000);  // BAR_L
    nframes_vec[26]=floor(time_bar_horizontal/dt*1000);  // BAR_R
    nframes_vec[27]=floor(time_bar_vertical/dt*1000);  // BAR_D
    nframes_vec[28]=floor(time_bar_vertical/dt*1000);   // BAR_U
    nframes_vec[29]=floor(time_bar_horizontal/dt*1000);  // BAR LIGHT_L
    nframes_vec[30]=floor(time_bar_horizontal/dt*1000);  // BAR LIGHT_R
    nframes_vec[31]=floor(time_bar_vertical/dt*1000);  // BAR LIGHT D
    nframes_vec[32]=floor(time_bar_vertical/dt*1000);   // BAR LIGHT U

    for(unsigned int k=33;k<39;++k) nframes_vec[k]=floor(time_bar_horizontal/dt*1000); // DOTS
    for(unsigned int k=39;k<46;++k) nframes_vec[k]=nframes; // WDOTS

    nframes_vec[46]=nframes; // CONCENTRIC

    for(unsigned int k=47;k<50;++k) nframes_vec[k]=floor(5.0/dt*1000); // WDOTS TOM

    for(unsigned int k=50;k<56;++k) nframes_vec[k]=floor(time_bar_horizontal/dt*1000); // LIGHTDOTS

    nframes_vec[56]=floor(4.0/dt*1000); // LIGHT
    nframes_vec[57]=floor(4.0/dt*1000); // DARK

    for(unsigned int k=58;k<66;++k) nframes_vec[k]=floor(5.0/dt*1000); // WDOTS_Tb TOM
    for(unsigned int k=66;k<76;++k) nframes_vec[k]=floor(5.0/dt*1000); // WDOTS_CONTRAST TOM

//    mask = new Point[H*W];

//    for(unsigned int i=0;i<W*H;i++){

//        double x = i%W;
//        double y = i/W+1;
//        double u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2*xmax);
//        double v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2*ymax,u);

//        if(fabs(u)<th_max && fabs(v)<ymax){
//            mask[i].r=mask[i].b=mask[i].g=127;
//        } else {
//            mask[i].r=mask[i].b=mask[i].g=0;
//        }

//    }

}

//covert stim enum to string
string barrage::code_stim(stim s){
    string r;
    switch (s) {
    case GRAT_90_SF05 :
        r = "GRAT_90_SF05";
        break;
    case GRAT_270_SF05 :
        r = "GRAT_270_SF05";
        break;
    case GRAT_0_SF05 :
        r = "GRAT_0_SF05";
        break;
    case GRAT_180_SF05 :
        r = "GRAT_180_SF05";
        break;
    case GRAT_90_SF15 :
        r = "GRAT_90_SF15";
        break;
    case GRAT_270_SF15 :
        r = "GRAT_270_SF15";
        break;
    case GRAT_0_SF15 :
        r = "GRAT_0_SF15";
        break;
    case GRAT_180_SF15 :
        r = "GRAT_180_SF15";
        break;
    case GRAT_90_SF26 :
        r = "GRAT_90_SF26";
        break;
    case GRAT_270_SF26 :
        r = "GRAT_270_SF26";
        break;
    case GRAT_0_SF26 :
        r = "GRAT_0_SF26";
        break;
    case GRAT_180_SF26 :
        r = "GRAT_180_SF26";
        break;
    case BAR_0 :
        r = "BAR_0";
        break;
    case BAR_90 :
        r = "BAR_90";
        break;
    case BAR_180 :
        r = "BAR_180";
        break;
    case BAR_270 :
        r = "BAR_270";
        break;
    case LIGHTBAR_0 :
        r = "LIGHTBAR_0";
        break;
    case LIGHTBAR_90 :
        r = "LIGHTBAR_90";
        break;
    case LIGHTBAR_180 :
        r = "LIGHTBAR_180";
        break;
    case LIGHTBAR_270 :
        r = "LIGHTBAR_270";
        break;
    case LOOM :
        r = "LOOM";
        break;
    case WIGGLY5_UL :
        r = "WIGGLY5_UL";
        break;
    case WIGGLY5_UR :
        r = "WIGGLY5_UR";
        break;
    case WIGGLY5_DL :
        r = "WIGGLY5_DL";
        break;
    case WIGGLY5_DR :
        r = "WIGGLY5_DR";
        break;
    case WIGGLY15_UL :
        r = "WIGGLY15_UL";
        break;
    case WIGGLY15_UR :
        r = "WIGGLY15_UR";
        break;
    case WIGGLY15_DL :
        r = "WIGGLY15_DL";
        break;
    case WIGGLY15_DR :
        r = "WIGGLY15_DR";
        break;
    case WIGGLY26_UL :
        r = "WIGGLY26_UL";
        break;
    case WIGGLY26_UR :
        r = "WIGGLY26_UR";
        break;
    case WIGGLY26_DL :
        r = "WIGGLY26_DL";
        break;
    case WIGGLY26_DR :
        r = "WIGGLY26_DR";
        break;
    case DOT_90_SF05:
        r="DOT_90_SF05";
        break;
    case DOT_90_SF15:
        r="DOT_90_SF15";
        break;
    case DOT_90_SF26:
        r="DOT_90_SF26";
        break;
    case DOT_270_SF05:
        r="DOT_270_SF05";
        break;
    case DOT_270_SF15:
        r="DOT_270_SF15";
        break;
    case DOT_270_SF26:
        r="DOT_270_SF26";
        break;
    case LIGHTDOT_90_SF05:
        r="LIGHTDOT_90_SF05";
        break;
    case LIGHTDOT_90_SF15:
        r="LIGHTDOT_90_SF15";
        break;
    case LIGHTDOT_90_SF26:
        r="LIGHTDOT_90_SF26";
        break;
    case LIGHTDOT_270_SF05:
        r="LIGHTDOT_270_SF05";
        break;
    case LIGHTDOT_270_SF15:
        r="LIGHTDOT_270_SF15";
        break;
    case LIGHTDOT_270_SF26:
        r="LIGHTDOT_270_SF26";
        break;
    case WDOT_1:
        r="WDOT_1";
        break;
    case WDOT_2:
        r="WDOT_2";
        break;
    case WDOT_3:
        r="WDOT_3";
        break;
    case WDOT_4:
        r="WDOT_4";
        break;
    case WDOT_5:
        r="WDOT_5";
        break;
    case WDOT_6:
        r="WDOT_6";
        break;
    case WDOT_7:
        r="WDOT_7";
        break;
    case WDOT_T1:
        r="WDOT_T1";
        break;
    case WDOT_T2:
        r="WDOT_T2";
        break;
    case WDOT_T3:
        r="WDOT_T3";
        break;
    case CONCENTRIC :
        r = "CONCENTRIC";
        break;
    case DARK:
        r = "DARK";
        break;
    case LIGHT:
        r = "LIGHT";
        break;
    case WDOT_Tb1:
        r="WDOT_Tb1";
        break;
    case WDOT_Tb2:
        r="WDOT_Tb2";
        break;
    case WDOT_Tb3:
        r="WDOT_Tb3";
        break;
    case WDOT_Tb4:
        r="WDOT_Tb4";
        break;
    case WDOT_Tb5:
        r="WDOT_Tb5";
        break;
    case WDOT_Tb6:
        r="WDOT_Tb6";
        break;
    case WDOT_Tb7:
        r="WDOT_Tb7";
        break;
    case WDOT_Tb8:
        r="WDOT_Tb8";
        break;
    case WDOT_CONTRAST_1:
        r="WDOT_CONTRAST_1";
        break;
    case WDOT_CONTRAST_2:
        r="WDOT_CONTRAST_2";
        break;
    case WDOT_CONTRAST_3:
        r="WDOT_CONTRAST_3";
        break;
    case WDOT_CONTRAST_4:
        r="WDOT_CONTRAST_4";
        break;
    case WDOT_CONTRAST_5:
        r="WDOT_CONTRAST_5";
        break;
    case WDOT_CONTRAST_6:
        r="WDOT_CONTRAST_6";
        break;
    case WDOT_CONTRAST_7:
        r="WDOT_CONTRAST_7";
        break;
    case WDOT_CONTRAST_8:
        r="WDOT_CONTRAST_8";
        break;
    case WDOT_CONTRAST_9:
        r="WDOT_CONTRAST_9";
        break;
    case WDOT_CONTRAST_10:
        r="WDOT_CONTRAST_10";
        break;
    default:
        break;
    }
    return(r);

}

void barrage::get_vs_mask(cv::Mat& mask_mat){

    int x,y;
    double u,v;
    int i;

    for(i=0;i<W*H;i++){
        x = i%W;
        y = i/W+1;
        u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2*xmax);
        v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2*ymax,u);

        if(fabs(u)<th_max && fabs(v)<ymax){
            mask_mat.data[i]=127;
        } else {
            mask_mat.data[i]=0;
        }
    }
}

int barrage::matchtime(int64 timecamera, vector<int64>& trace2){
    int64 timevs=trace2[0];
    int counter=0;
    while(timevs<timecamera && counter+1<trace2.size()){
        counter++;
        timevs=trace2[counter];
    }
    if(counter==0) return(counter);
    else return(counter-1);
}

stim barrage::string_to_stim(const char* s){
    stim r;
    if(strcmp(s,"GRAT_90_SF05")==0) r=GRAT_90_SF05;
    else if(strcmp(s,"GRAT_270_SF05")==0) r=GRAT_270_SF05;
    else if(strcmp(s,"GRAT_0_SF05")==0) r= GRAT_0_SF05;
    else if(strcmp(s,"GRAT_180_SF05")==0) r=GRAT_180_SF05;
    else if(strcmp(s,"GRAT_90_SF15")==0) r=GRAT_90_SF15;
    else if(strcmp(s,"GRAT_270_SF15")==0) r=GRAT_270_SF15;
    else if(strcmp(s,"GRAT_0_SF15")==0) r=GRAT_0_SF15;
    else if(strcmp(s,"GRAT_180_SF15")==0) r=GRAT_180_SF15;
    else if(strcmp(s,"GRAT_90_SF26")==0) r=GRAT_90_SF26;
    else if(strcmp(s,"GRAT_270_SF26")==0) r=GRAT_270_SF26;
    else if(strcmp(s,"GRAT_0_SF26")==0) r=GRAT_0_SF26;
    else if(strcmp(s,"GRAT_180_SF26")==0) r=GRAT_180_SF26;
    else if(strcmp(s,"BAR_0")==0) r=BAR_0;
    else if(strcmp(s,"BAR_90")==0) r=BAR_90;
    else if(strcmp(s,"BAR_180")==0) r=BAR_180;
    else if(strcmp(s,"BAR_270")==0) r=BAR_270;
    else if(strcmp(s,"LIGHTBAR_0")==0) r=LIGHTBAR_0;
    else if(strcmp(s,"LIGHTBAR_90")==0) r=LIGHTBAR_90;
    else if(strcmp(s,"LIGHTBAR_180")==0) r=LIGHTBAR_180;
    else if(strcmp(s,"LIGHTBAR_270")==0) r=LIGHTBAR_270;
    else if(strcmp(s,"LOOM")==0) r=LOOM;
    else if(strcmp(s,"WIGGLY5_UL")==0) r=WIGGLY5_UL;
    else if(strcmp(s,"WIGGLY5_UR")==0) r=WIGGLY5_UR;
    else if(strcmp(s,"WIGGLY5_DL")==0) r=WIGGLY5_DL;
    else if(strcmp(s,"WIGGLY5_DR")==0) r=WIGGLY5_DR;
    else if(strcmp(s,"WIGGLY15_UL")==0) r=WIGGLY15_UL;
    else if(strcmp(s,"WIGGLY15_UR")==0) r=WIGGLY15_UR;
    else if(strcmp(s,"WIGGLY15_DL")==0) r=WIGGLY15_DL;
    else if(strcmp(s,"WIGGLY15_DR")==0) r=WIGGLY15_DR;
    else if(strcmp(s,"WIGGLY26_UL")==0) r=WIGGLY26_UL;
    else if(strcmp(s,"WIGGLY26_UR")==0) r=WIGGLY26_UR;
    else if(strcmp(s,"WIGGLY26_DL")==0) r=WIGGLY26_DL;
    else if(strcmp(s,"WIGGLY26_DR")==0) r=WIGGLY26_DR;
    else if(strcmp(s,"DOT_90_SF05")==0) r=DOT_90_SF05;
    else if(strcmp(s,"DOT_90_SF15")==0) r=DOT_90_SF15;
    else if(strcmp(s,"DOT_90_SF26")==0) r=DOT_90_SF26;
    else if(strcmp(s,"DOT_270_SF05")==0) r=DOT_270_SF05;
    else if(strcmp(s,"DOT_270_SF15")==0) r=DOT_270_SF15;
    else if(strcmp(s,"DOT_270_SF26")==0) r=DOT_270_SF26;

    else if(strcmp(s,"LIGHTDOT_90_SF05")==0) r=LIGHTDOT_90_SF05;
    else if(strcmp(s,"LIGHTDOT_90_SF15")==0) r=LIGHTDOT_90_SF15;
    else if(strcmp(s,"LIGHTDOT_90_SF26")==0) r=LIGHTDOT_90_SF26;
    else if(strcmp(s,"LIGHTDOT_270_SF05")==0) r=LIGHTDOT_270_SF05;
    else if(strcmp(s,"LIGHTDOT_270_SF15")==0) r=LIGHTDOT_270_SF15;
    else if(strcmp(s,"LIGHTDOT_270_SF26")==0) r=LIGHTDOT_270_SF26;

    else if(strcmp(s,"WDOT_1")==0) r=WDOT_1;
    else if(strcmp(s,"WDOT_2")==0) r=WDOT_2;
    else if(strcmp(s,"WDOT_3")==0) r=WDOT_3;
    else if(strcmp(s,"WDOT_4")==0) r=WDOT_4;
    else if(strcmp(s,"WDOT_5")==0) r=WDOT_5;
    else if(strcmp(s,"WDOT_6")==0) r=WDOT_6;
    else if(strcmp(s,"WDOT_7")==0) r=WDOT_7;
    else if(strcmp(s,"WDOT_T1")==0) r=WDOT_T1;
    else if(strcmp(s,"WDOT_T2")==0) r=WDOT_T2;
    else if(strcmp(s,"WDOT_T3")==0) r=WDOT_T3;
    else if(strcmp(s,"CONCENTRIC")==0) r=CONCENTRIC;

    else if(strcmp(s,"DARK")==0) r=DARK;
    else if(strcmp(s,"LIGHT")==0) r=LIGHT;

    else if(strcmp(s,"WDOT_Tb1")==0) r=WDOT_Tb1;
    else if(strcmp(s,"WDOT_Tb2")==0) r=WDOT_Tb2;
    else if(strcmp(s,"WDOT_Tb3")==0) r=WDOT_Tb3;
    else if(strcmp(s,"WDOT_Tb4")==0) r=WDOT_Tb4;
    else if(strcmp(s,"WDOT_Tb5")==0) r=WDOT_Tb5;
    else if(strcmp(s,"WDOT_Tb6")==0) r=WDOT_Tb6;
    else if(strcmp(s,"WDOT_Tb7")==0) r=WDOT_Tb7;
    else if(strcmp(s,"WDOT_Tb8")==0) r=WDOT_Tb8;

    else if(strcmp(s,"WDOT_CONTRAST_1")==0) r=WDOT_CONTRAST_1;
    else if(strcmp(s,"WDOT_CONTRAST_2")==0) r=WDOT_CONTRAST_2;
    else if(strcmp(s,"WDOT_CONTRAST_3")==0) r=WDOT_CONTRAST_3;
    else if(strcmp(s,"WDOT_CONTRAST_4")==0) r=WDOT_CONTRAST_4;
    else if(strcmp(s,"WDOT_CONTRAST_5")==0) r=WDOT_CONTRAST_5;
    else if(strcmp(s,"WDOT_CONTRAST_6")==0) r=WDOT_CONTRAST_6;
    else if(strcmp(s,"WDOT_CONTRAST_7")==0) r=WDOT_CONTRAST_7;
    else if(strcmp(s,"WDOT_CONTRAST_8")==0) r=WDOT_CONTRAST_8;
    else if(strcmp(s,"WDOT_CONTRAST_9")==0) r=WDOT_CONTRAST_9;
    else if(strcmp(s,"WDOT_CONTRAST_10")==0) r=WDOT_CONTRAST_10;

    else {
        std::cout<<"Stimulus "<<s<<" not recognized"<<std::endl;
        abort();
    }

    return(r);
}

double barrage::radDot(double angle){
    return(r*pi/180*angle/2.0); // where angle is the visual angle corresponding to the diameter of the dot
}

double barrage::spatFreq(double angle){
    return(1./(2*angle*pi/180*r)); // angle is half wavelength.
}

void barrage::setStimLib(){
    QString prefix=QCoreApplication::applicationDirPath();
    string optfilename = prefix.toStdString()+"/../opt/StimLibFolder.txt";

    ifstream StimLib_optfile(optfilename.c_str());
    // Make sure the file is open
    if(!StimLib_optfile.is_open())
    {
        stringstream ssErr;
        ssErr << "could not open "  << optfilename << "StimLibFolder.txt settings file to set Stimulus .bin directory.";
        throw std::runtime_error(ssErr.str());
    }

    StimLib_optfile>>stimlibloc;
    StimLib_optfile.close();

}

void barrage::transform_image(string imfile){
    double x,y,u,v;

    cv::Mat im;

    im=cv::imread(imfile);
    if(im.empty()){
        cerr<<"Background image not found in: " << imfile <<endl;
        exit(0);
    }
//    if (im.rows == 0)
//        throw("Failed to load background image");
    cvtColor(im,im, cv::COLOR_BGR2GRAY);
    im.convertTo(im,CV_8U);


    background.resize(W*H);

    for(unsigned int i=0;i<W*H;++i){
        x=(double) (i%W);
        y=(double) i/W+1;
        u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
        v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);
        if(fabs(u)<th_max && fabs(v)<ymax){
            background[i] = im.at<uchar>((int)((ymax+v)/(2*ymax)*im.rows),(int)((th_max+u)/(2.2*th_max)*im.cols));
        } else background[i]=0;
    }

    ostringstream ss;
    ss<<stimlibloc<<"/background.bin";
    ofstream output(ss.str().c_str(),ios::out | ios::binary);

    cout<<"writing on "<<ss.str()<<endl;
    output.write((char*)&background[0],W*H*sizeof(background[0]));
    output.close();

}

void barrage::getBackground(){
    ostringstream ss;
    ss<<stimlibloc<<"/background.bin";
    ifstream in(ss.str().c_str(),ios::in|ios::binary);
    background.resize(H*W);
    in.read((char*)&background[0], H*W*sizeof(unsigned char));
    in.close();
}

void barrage::setBackgroundColor(int color){
    flat_background_color=color;
}



void barrage::GenFrames(vector<Point*> &points, stim s, unsigned int NF){

    // STIMULUS PROPERTIES
    double sf; // SPATIAL FREQ
    double tf=1;
    double phi;
    double phi_right = pi;
    double phi_left = 0;
    double phi_up = pi/2;
    double phi_down = -pi/2;
    double sf_low=spatFreq(5.0);
    double sf_med=spatFreq(15);
    double sf_hi=spatFreq(26);

    double rad_loom;
    double dist_vec;
    gsl_rng * rg = gsl_rng_alloc (gsl_rng_mt19937);
    gsl_rng_set(rg,(int)s); // seed
    double cen_dots_x,cen_dots_y;
    double wdotx;
    double th_ran;
    double step_dots_x,step_dots_y;
    double rad_dot;
    double dot_diff=r*pi/180*0.6;
    double fading_factor;
    double width_bar,min_bar, v_bar;
    double cen_bar;
    double u,v;
    double w;
    int bar_fg_int;
    double time_bar;
    double rdelta=r*10./180*pi;

    //double rp;
    unsigned int i,ti;
    double x,y;
    int P0=0,P1=0;
    //ProgressBar(0);

    switch (s) {
    // ---------- LOOMing stimulus --------------
    case LOOM :

        for(ti=0;ti<NF;ti++){

            //rad_loom=rad_low+ti*1.0/NF*rad_hi;
            rad_loom=r*(2*atan(.2/(-2.448*ti*dt/1000+10)));

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);
                if(fabs(u)<th_max && fabs(v)<ymax){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    if(pow(r*u,2)+pow(v,2)<pow(rad_loom,2)){
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=0;//*pow(rp/rp_max,2));
                    } else {
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

        // ---------- GRATINGS -------------
    case GRAT_270_SF05 : case GRAT_90_SF05 : case GRAT_0_SF05 : case GRAT_180_SF05 :
    case GRAT_270_SF15 : case GRAT_90_SF15 : case GRAT_0_SF15 : case GRAT_180_SF15 :
    case GRAT_270_SF26 : case GRAT_90_SF26 : case GRAT_0_SF26 : case GRAT_180_SF26 :

        switch (s) {
        case GRAT_270_SF05 : phi=phi_right; sf=sf_low; break;
        case GRAT_90_SF05 : phi=phi_left; sf=sf_low; break;
        case GRAT_0_SF05 : phi=phi_down; sf=sf_low; break;
        case GRAT_180_SF05 : phi=phi_up; sf=sf_low; break;

        case GRAT_270_SF15 : phi=phi_right; sf=sf_med; break;
        case GRAT_90_SF15 : phi=phi_left; sf=sf_med; break;
        case GRAT_0_SF15 : phi=phi_down; sf=sf_med; break;
        case GRAT_180_SF15 : phi=phi_up; sf=sf_med; break;

        case GRAT_270_SF26 : phi=phi_right; sf=sf_hi; break;
        case GRAT_90_SF26 : phi=phi_left; sf=sf_hi; break;
        case GRAT_0_SF26 : phi=phi_down; sf=sf_hi; break;
        case GRAT_180_SF26 : phi=phi_up; sf=sf_hi; break;
        default: break;
        }

        for(ti=0;ti<NF;ti++){
            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);
                if(fabs(u)<th_max && fabs(v)<ymax){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    if(fading){
                        if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                        if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=fading_factor*255*((sin(2*pi*sf*(r*u*cos(phi)-v*sin(phi))+2*pi*tf*ti*dt/1000)+1)/2.)+127*(1-fading_factor);
                    } else {
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=255*((sin(2*pi*sf*(r*u*cos(phi)-v*sin(phi))+2*pi*tf*ti*dt/1000)+1)/2.);//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

        // -------- BARS ---------------------------
    case BAR_0 : case BAR_90 : case BAR_180 : case BAR_270 :
    case LIGHTBAR_0 : case LIGHTBAR_90 : case LIGHTBAR_180 : case LIGHTBAR_270 :

        switch (s) {
        case BAR_270 : case LIGHTBAR_270:
            min_bar=-r*th_max; v_bar=1;
            time_bar=time_bar_horizontal;
            break;
        case BAR_90 : case LIGHTBAR_90:
            min_bar=r*th_max; v_bar=-1;;
            time_bar=time_bar_horizontal;
            break;
        case BAR_180 : case LIGHTBAR_180:
            min_bar=-ymax; v_bar=1;
            time_bar=time_bar_vertical;
            break;
        case BAR_0 : case LIGHTBAR_0:
            min_bar=ymax; v_bar=-1;
            time_bar=time_bar_vertical;
            break;
        default: break;
        }
        
        width_bar=1./sf_med/2.;

        for(ti=0;ti<NF;ti++){
            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);
                cen_bar=min_bar+v_bar*ti*dt*tf/1000./sf_med;

                if(s==BAR_270 || s==BAR_90 || s==LIGHTBAR_270 || s==LIGHTBAR_90) {
                    w=r*u;
                } else {
                    w=v;
                }

                if(s==BAR_90 || s==BAR_270 || s==BAR_0 || s== BAR_180) bar_fg_int=0;
                if(s==LIGHTBAR_90 || s==LIGHTBAR_270 || s==LIGHTBAR_0 || s== LIGHTBAR_180) bar_fg_int=255;

                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=127;//*pow(rp/rp_max,2));
                    if (fabs(w-cen_bar)<width_bar/2.){
                        if(fading){
                            if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                            if(ti*dt/1000>time_bar-2) fading_factor=(time_bar-ti*dt/1000.)/2;
                        } else {fading_factor=1;}
                        
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=fading_factor*bar_fg_int+(1-fading_factor)*127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

        // --------- WIGGLY DOTS -----------------

    case WIGGLY5_UL : case WIGGLY15_UL : case WIGGLY26_UL :
    case WIGGLY5_UR : case WIGGLY15_UR : case WIGGLY26_UR :
    case WIGGLY5_DL : case WIGGLY15_DL : case WIGGLY26_DL :
    case WIGGLY5_DR : case WIGGLY15_DR : case WIGGLY26_DR :

        if(s==WIGGLY5_UL || s==WIGGLY5_UR || s==WIGGLY5_DL || s==WIGGLY5_DR) rad_dot=radDot(5.0);
        if(s==WIGGLY15_UL || s==WIGGLY15_UR || s==WIGGLY15_DL || s==WIGGLY15_DR) rad_dot=radDot(15.0);
        if(s==WIGGLY26_UL || s==WIGGLY26_UR || s==WIGGLY26_DL || s==WIGGLY26_DR) rad_dot=radDot(26.0);

        // Set random generator
        if(s==WIGGLY5_UL || s==WIGGLY15_UL || s==WIGGLY26_UL){
            cen_dots_x=-r*th_max/2;
            cen_dots_y=-ymax/2;
        }
        if(s==WIGGLY5_UR || s==WIGGLY15_UR || s==WIGGLY26_UR){
            cen_dots_x=r*th_max/2;
            cen_dots_y=-ymax/2;
        }
        if(s==WIGGLY5_DL || s==WIGGLY15_DL || s==WIGGLY26_DL){
            cen_dots_x=-r*th_max/2;
            cen_dots_y=ymax/2;
        }
        if(s==WIGGLY5_DR || s==WIGGLY15_DR || s==WIGGLY26_DR){
            cen_dots_x=r*th_max/2;
            cen_dots_y=ymax/2;
        }

        th_ran=gsl_ran_flat(rg,0,2*pi);
        for(ti=0;ti<NF;ti++){
            //step_dots_x=gsl_ran_flat(rg,-dot_diff,dot_diff);
            //step_dots_y=gsl_ran_flat(rg,-dot_diff,dot_diff);
            
            step_dots_x=dot_diff*cos(th_ran);
            step_dots_y=dot_diff*sin(th_ran);
            th_ran+=gsl_ran_gaussian(rg,pi/12);

            //cout<<"CHECK GenFrames: \n"<<
            //      "   rad_dot    = "<< rad_dot<<endl<<
            //      "   step_dot_x = "<<step_dots_x<<endl<<
            //      "   step_dot_y = "<<step_dots_y<<endl<<
            //      "   rthmax     = "<<r*th_max<<endl<<
            //      "   ymax       = "<<ymax<<endl;

            if(s==WIGGLY5_DL || s==WIGGLY15_DL || s==WIGGLY26_DL){
                while(cen_dots_x+step_dots_x+rad_dot>0 ||
                      cen_dots_x+step_dots_x-rad_dot<-r*th_max ||
                      cen_dots_y+step_dots_y+rad_dot>ymax ||
                      cen_dots_y+step_dots_y-rad_dot<0){
                    
                    th_ran+=gsl_ran_gaussian(rg,pi/12);
                    step_dots_x=dot_diff*cos(th_ran);
                    step_dots_y=dot_diff*sin(th_ran);
                    //cout<<cen_dots_y<<' '<<step_dots_y<<' '<<rad_dot<<' '<<ymax/2<<endl;
                }
            }

            if(s==WIGGLY5_DR || s==WIGGLY15_DR || s==WIGGLY26_DR){
                while(cen_dots_x+step_dots_x-rad_dot<0 ||
                      cen_dots_x+step_dots_x+rad_dot>r*th_max ||
                      cen_dots_y+step_dots_y+rad_dot>ymax ||
                      cen_dots_y+step_dots_y-rad_dot<0){

                    th_ran+=gsl_ran_gaussian(rg,pi/12);
                    step_dots_x=dot_diff*cos(th_ran);
                    step_dots_y=dot_diff*sin(th_ran);
                    //cout<<cen_dots_y<<' '<<step_dots_y<<' '<<rad_dot<<' '<<ymax/2<<endl;
                }
            }

            if(s==WIGGLY5_UL || s==WIGGLY15_UL || s==WIGGLY26_UL){
                while(cen_dots_x+step_dots_x+rad_dot>0 ||
                      cen_dots_x+step_dots_x-rad_dot<-r*th_max ||
                      cen_dots_y+step_dots_y-rad_dot<-ymax ||
                      cen_dots_y+step_dots_y+rad_dot>0){

                    th_ran+=gsl_ran_gaussian(rg,pi/12);
                    step_dots_x=dot_diff*cos(th_ran);
                    step_dots_y=dot_diff*sin(th_ran);
                }

            }

            if(s==WIGGLY5_UR || s==WIGGLY15_UR || s==WIGGLY26_UR){
                while(cen_dots_x+step_dots_x-rad_dot<0 ||
                      cen_dots_x+step_dots_x+rad_dot>r*th_max ||
                      cen_dots_y+step_dots_y-rad_dot<-ymax ||
                      cen_dots_y+step_dots_y+rad_dot>0){

                    th_ran+=gsl_ran_gaussian(rg,pi/12);
                    step_dots_x=dot_diff*cos(th_ran);
                    step_dots_y=dot_diff*sin(th_ran);
                }
            }
            
            cen_dots_x+=step_dots_x;
            cen_dots_y+=step_dots_y;

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);
                
                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=127;//*pow(rp/rp_max,2));
                    dist_vec=pow(cen_dots_x-u*r,2)+pow(cen_dots_y-v,2);

                    if (dist_vec<=pow(rad_dot,2)){
                        if(fading){
                            if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                            if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        } else fading_factor=1;
                        
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=(1-fading_factor)*127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

    case DOT_270_SF05: case DOT_90_SF05:
    case DOT_270_SF15: case DOT_90_SF15:
    case DOT_270_SF26: case DOT_90_SF26:


        if(s==DOT_90_SF05 || s==DOT_270_SF05) rad_dot=radDot(5);
        if(s==DOT_90_SF15 || s==DOT_270_SF15) rad_dot=radDot(15);
        if(s==DOT_90_SF26 || s==DOT_270_SF26) rad_dot=radDot(26);

        if(s==DOT_270_SF05||s==DOT_270_SF15||s==DOT_270_SF26){
            cen_dots_x=-r*th_max;
            cen_dots_y=0;
            th_ran=0;
            v_bar=1;
        }
        if(s==DOT_90_SF05||s==DOT_90_SF15||s==DOT_90_SF26){
            cen_dots_x=r*th_max;
            cen_dots_y=0;
            th_ran=pi;
            v_bar=-1;
        }

        for(ti=0;ti<NF;ti++){

            step_dots_y=0;
            cen_dots_x+=v_bar*dt*tf/1000./sf_med;;

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);

                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=127;//*pow(rp/rp_max,2));
                    dist_vec=pow(cen_dots_x-u*r,2)+pow(cen_dots_y-v,2);

                    if (dist_vec<=pow(rad_dot,2)){
                        if(fading){
                            if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                            if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        } else fading_factor=1;

                        points[ti][i].r=points[ti][i].g=points[ti][i].b=(1-fading_factor)*127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;


    case LIGHTDOT_270_SF05: case LIGHTDOT_90_SF05:
    case LIGHTDOT_270_SF15: case LIGHTDOT_90_SF15:
    case LIGHTDOT_270_SF26: case LIGHTDOT_90_SF26:


        if(s==LIGHTDOT_90_SF05 || s==LIGHTDOT_270_SF05) rad_dot=radDot(5);
        if(s==LIGHTDOT_90_SF15 || s==LIGHTDOT_270_SF15) rad_dot=radDot(15);
        if(s==LIGHTDOT_90_SF26 || s==LIGHTDOT_270_SF26) rad_dot=radDot(26);

        if(s==LIGHTDOT_270_SF05||s==LIGHTDOT_270_SF15||s==LIGHTDOT_270_SF26){
            cen_dots_x=-r*th_max;
            cen_dots_y=0;
            th_ran=0;
            v_bar=1;
        }
        if(s==LIGHTDOT_90_SF05||s==LIGHTDOT_90_SF15||s==LIGHTDOT_90_SF26){
            cen_dots_x=r*th_max;
            cen_dots_y=0;
            th_ran=pi;
            v_bar=-1;
        }

        for(ti=0;ti<NF;ti++){

            step_dots_y=0;
            cen_dots_x+=v_bar*dt*tf/1000./sf_med;;

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);

                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=127;//*pow(rp/rp_max,2));
                    dist_vec=pow(cen_dots_x-u*r,2)+pow(cen_dots_y-v,2);

                    if (dist_vec<=pow(rad_dot,2)){
                        if(fading){
                            if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                            if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        } else fading_factor=1;

                        points[ti][i].r=points[ti][i].g=points[ti][i].b=fading_factor*255+(1-fading_factor)*127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

    case LIGHT:


        for(ti=0;ti<NF;ti++){

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);

                if(fabs(u)<th_max && fabs(v)<ymax ){
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=255;//*pow(rp/rp_max,2));
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

    case DARK:


        for(ti=0;ti<NF;ti++){

            for(i = 0; i < W*H; ++i ){

                points[ti][i].r=points[ti][i].g=points[ti][i].b=0;

            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

    case WDOT_1 : case WDOT_2 : case WDOT_3 : case WDOT_4 : case WDOT_5 : case WDOT_6 : case WDOT_7:
        rad_dot=radDot(5.0);

        switch (s) {
        case WDOT_1: wdotx=1; break;
        case WDOT_2: wdotx=2; break;
        case WDOT_3: wdotx=3; break;
        case WDOT_4: wdotx=4; break;
        case WDOT_5: wdotx=5; break;
        case WDOT_6: wdotx=6; break;
        case WDOT_7: wdotx=7; break;
        }

        // Set random generator
        cen_dots_x=2*r*th_max/7.0*(wdotx-0.5)-r*th_max ;
        cen_dots_y=-ymax/2.0;
        
        th_ran=gsl_ran_flat(rg,0,2*pi);
        for(ti=0;ti<NF;ti++){
            //step_dots_x=gsl_ran_flat(rg,-dot_diff,dot_diff);
            //step_dots_y=gsl_ran_flat(rg,-dot_diff,dot_diff);
            
            step_dots_x=dot_diff*cos(th_ran);
            step_dots_y=dot_diff*sin(th_ran);
            th_ran+=gsl_ran_gaussian(rg,pi/12);

            //cout<<"CHECK GenFrames: \n"<<
            //      "   rad_dot    = "<< rad_dot<<endl<<
            //      "   step_dot_x = "<<step_dots_x<<endl<<
            //      "   step_dot_y = "<<step_dots_y<<endl<<
            //      "   rthmax     = "<<r*th_max<<endl<<
            //      "   ymax       = "<<ymax<<endl;

            while(cen_dots_x+step_dots_x+2*rad_dot>2*r*th_max*wdotx/7.0 - r*th_max ||
                  cen_dots_x+step_dots_x-2*rad_dot<2*r*th_max*(wdotx-1)/7.0 - r*th_max ||
                  cen_dots_y+step_dots_y+2*rad_dot>0 ||
                  cen_dots_y+step_dots_y-2*rad_dot<-ymax){

                th_ran+=gsl_ran_gaussian(rg,pi/12);
                step_dots_x=dot_diff*cos(th_ran);
                step_dots_y=dot_diff*sin(th_ran);
                //cout<<cen_dots_y<<' '<<step_dots_y<<' '<<rad_dot<<' '<<ymax/2<<endl;
            }

            cen_dots_x+=step_dots_x;
            cen_dots_y+=step_dots_y;

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);
                
                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=127;//*pow(rp/rp_max,2));
                    dist_vec=pow(cen_dots_x-u*r,2)+pow(cen_dots_y-v,2);

                    if (dist_vec<=pow(rad_dot,2)){
                        if(fading){
                            if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                            if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        } else fading_factor=1;
                        
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=(1-fading_factor)*127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

    case WDOT_Tb1 : case WDOT_Tb2 : case WDOT_Tb3 : case WDOT_Tb4 : case WDOT_Tb5 : case WDOT_Tb6 : case WDOT_Tb7: case WDOT_Tb8:
        rad_dot=radDot(5.0);

        switch (s) {
        case WDOT_Tb1: wdotx=1; break;
        case WDOT_Tb2: wdotx=2; break;
        case WDOT_Tb3: wdotx=3; break;
        case WDOT_Tb4: wdotx=4; break;
        case WDOT_Tb5: wdotx=5; break;
        case WDOT_Tb6: wdotx=6; break;
        case WDOT_Tb7: wdotx=7; break;
        case WDOT_Tb8: wdotx=8; break;
        }

        // Set random generator
        cen_dots_x=r*(15.0+10.0*(wdotx-1))/180.0*pi; // We need to check this because it might be reversed.
        cen_dots_y=ymax/10.0;

        th_ran=gsl_ran_flat(rg,0,2*pi);
        for(ti=0;ti<NF;ti++){
            //step_dots_x=gsl_ran_flat(rg,-dot_diff,dot_diff);
            //step_dots_y=gsl_ran_flat(rg,-dot_diff,dot_diff);

            step_dots_x=dot_diff*cos(th_ran);
            step_dots_y=dot_diff*sin(th_ran);
            th_ran+=gsl_ran_gaussian(rg,pi/12);

            //cout<<"CHECK GenFrames: \n"<<
            //      "   rad_dot    = "<< rad_dot<<endl<<
            //      "   step_dot_x = "<<step_dots_x<<endl<<
            //      "   step_dot_y = "<<step_dots_y<<endl<<
            //      "   rthmax     = "<<r*th_max<<endl<<
            //      "   ymax       = "<<ymax<<endl;

            while(cen_dots_x+step_dots_x+1*rad_dot>r*(15.0+10.0*(wdotx-1))/180*pi + 0.5*rdelta||
                  cen_dots_x+step_dots_x-1*rad_dot<r*(15.0+10.0*(wdotx-1))/180*pi - 0.5*rdelta ||
                  cen_dots_y+step_dots_y+2*rad_dot>ymax/3. ||
                  cen_dots_y+step_dots_y-2*rad_dot<0){

                th_ran+=gsl_ran_gaussian(rg,pi/12);
                step_dots_x=dot_diff*cos(th_ran);
                step_dots_y=dot_diff*sin(th_ran);
                //cout<<cen_dots_y<<' '<<step_dots_y<<' '<<rad_dot<<' '<<ymax/2<<endl;
            }

            cen_dots_x+=step_dots_x;
            cen_dots_y+=step_dots_y;

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);

                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;//*pow(rp/rp_max,2));
                    dist_vec=pow(cen_dots_x-u*r,2)+pow(cen_dots_y-v,2);

                    if (dist_vec<=pow(rad_dot,2)){
                        if(fading){
                            if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                            if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        } else fading_factor=1;

                        points[ti][i].r=points[ti][i].g=points[ti][i].b=(fading_factor)*127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;


    case WDOT_T1 : case WDOT_T2 : case WDOT_T3 :
        rad_dot=radDot(5.0);

        switch (s) {
        case WDOT_T1: wdotx=1; break;
        case WDOT_T2: wdotx=2; break;
        case WDOT_T3: wdotx=3; break;
        }

        // Set random generator
        cen_dots_x=rdelta*(wdotx-2) ;
        cen_dots_y=ymax/10.;

        th_ran=gsl_ran_flat(rg,0,2*pi);
        for(ti=0;ti<NF;ti++){

            step_dots_x=dot_diff*cos(th_ran);
            step_dots_y=dot_diff*sin(th_ran);
            th_ran+=gsl_ran_gaussian(rg,pi/12);

            while(cen_dots_x+step_dots_x+1.5*rad_dot>rdelta*(wdotx-2)+0.5*rdelta ||
                  cen_dots_x+step_dots_x-1.5*rad_dot<rdelta*(wdotx-2)-0.5*rdelta ||
                  cen_dots_y+step_dots_y+2*rad_dot>ymax/3. ||
                  cen_dots_y+step_dots_y-2*rad_dot<0){

                th_ran+=gsl_ran_gaussian(rg,pi/12);
                step_dots_x=dot_diff*cos(th_ran);
                step_dots_y=dot_diff*sin(th_ran);
            }

            cen_dots_x+=step_dots_x;
            cen_dots_y+=step_dots_y;

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);

                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;//*pow(rp/rp_max,2));
                    dist_vec=pow(cen_dots_x-u*r,2)+pow(cen_dots_y-v,2);

                    if (dist_vec<=pow(rad_dot,2)){
                        if(fading){
                            if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                            if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        } else fading_factor=1;

                        points[ti][i].r=points[ti][i].g=points[ti][i].b=(fading_factor)*127;//*pow(rp/rp_max,2));
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

    case WDOT_CONTRAST_1 : 
    case WDOT_CONTRAST_2 : 
    case WDOT_CONTRAST_3 :
    case WDOT_CONTRAST_4 :
    case WDOT_CONTRAST_5 :
    case WDOT_CONTRAST_6 :
    case WDOT_CONTRAST_7 :
    case WDOT_CONTRAST_8 :
    case WDOT_CONTRAST_9 :
    case WDOT_CONTRAST_10:

        rad_dot=radDot(5.0);
        int dot_contrast;
        gsl_rng_set(rg,101); // seed

        switch (s) {
            case WDOT_CONTRAST_1: dot_contrast=125; break;
            case WDOT_CONTRAST_2: dot_contrast=124; break;
            case WDOT_CONTRAST_3: dot_contrast=122; break;
            case WDOT_CONTRAST_4: dot_contrast=120; break;
            case WDOT_CONTRAST_5: dot_contrast=114; break;
            case WDOT_CONTRAST_6: dot_contrast=107; break;
            case WDOT_CONTRAST_7: dot_contrast=95; break;
            case WDOT_CONTRAST_8: dot_contrast=76; break;
            case WDOT_CONTRAST_9: dot_contrast=45; break;
            case WDOT_CONTRAST_10: dot_contrast=0; break;
        }

        // Set random generator
        cen_dots_x=0 ;
        cen_dots_y=ymax/10.;

        th_ran=gsl_ran_flat(rg,0,2*pi);
        for(ti=0;ti<NF;ti++){

            step_dots_x=dot_diff*cos(th_ran);
            step_dots_y=dot_diff*sin(th_ran);
            th_ran+=gsl_ran_gaussian(rg,pi/12);

            while(cen_dots_x+step_dots_x+1.5*rad_dot>0.5*rdelta ||
                  cen_dots_x+step_dots_x-1.5*rad_dot<-0.5*rdelta ||
                  cen_dots_y+step_dots_y+2*rad_dot>ymax/3. ||
                  cen_dots_y+step_dots_y-2*rad_dot<0){

                th_ran+=gsl_ran_gaussian(rg,pi/12);
                step_dots_x=dot_diff*cos(th_ran);
                step_dots_y=dot_diff*sin(th_ran);
            }

            cen_dots_x+=step_dots_x;
            cen_dots_y+=step_dots_y;

            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);

                if(fabs(u)<th_max && fabs(v)<ymax ){
                    //rp=sqrt(pow(d+(r-r*cos(u)),2)+pow(r*sin(u),2)+pow(v,2));
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=127;//*pow(rp/rp_max,2));
                    dist_vec=pow(cen_dots_x-u*r,2)+pow(cen_dots_y-v,2);

                    if (dist_vec<=pow(rad_dot,2)){

                        points[ti][i].r=points[ti][i].g=points[ti][i].b=dot_contrast;
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

        // CONCENTRIC RINGS
    case CONCENTRIC :
        
        for(ti=0;ti<NF;ti++){
            for(i = 0; i < W*H; ++i ){
                x=(double) (i%W);
                y=(double) i/W+1;
                u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2.0*xmax);
                v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2.0*ymax,u);
                if(fabs(u)<th_max && fabs(v)<ymax){
                    if(fading){
                        if(ti*dt/1000<2) fading_factor=ti*dt/1000./2;
                        if(ti*dt/1000>timeEP-2) fading_factor=(timeEP-ti*dt/1000.)/2;
                        points[ti][i].r=points[ti][i].g=points[ti][i].b=fading_factor*255*((sin(2*pi*sf_med*sqrt(pow(r*u,2)+pow(v,2))+2*pi*tf*ti*dt/1000)+1)/2.)+127*(1-fading_factor);
                    }
                } else {
                    points[ti][i].r=points[ti][i].g=points[ti][i].b=0;
                }
            }

            P1 = floor(((double)ti) / NF *100);
            if(P1>=P0+1){
                //ProgressBar(((double)ti) / NF);
                P0=P1;
            }

        }

        cout<<endl;
        break;

        // -------- DEFAULT GRAY MASK --------------
    default :
        cout<<endl;
        break;


    } // END CASES

    points[NF]=mask;
}


void barrage::FillPoints(vector<unsigned char*> & stimdata,vector<stim> &StimList){

    if (StimList.empty())
    {
        cerr << "[ERROR] No stimuli list loaded " << std::endl;
        return;
    }

    for(unsigned int i=0;i<StimList.size();++i){
        ostringstream ss;
        ss<<stimlibloc<<"/"<<code_stim(StimList[i])<<".bin";
        cout<<"Reading from "<<ss.str()<<endl;
        ifstream in(ss.str().c_str(),ios::in|ios::binary);
        stimdata[i] = new unsigned char[H*W*(nframes_vec[StimList[i]]+1)];
        in.read((char*)stimdata[i], H*W*(nframes_vec[StimList[i]]+1)*sizeof(unsigned char));
        in.close();
    }



}


// latitude function
double barrage::get_th(double yb){
    double t = (d*(d+r)-sqrt(pow(d*r,2)-pow(yb,2)*(pow(d,2)+2*r*d)))/(pow(d,2)+pow(yb,2));
    return (double)asin(t*yb/r);
}

// Gives the height on the screen
double barrage::get_y(double ytan,double th){
    return ytan/d*(d+r-r*cos(th));
}


void barrage::ProgressBar(double d){

    char* env_columns=getenv("COLUMNS");
    if(env_columns==NULL) {
        cout<<"ERROR: COLUMNS environmental variable not defined!!"<<endl;
        exit(1);
    }

    int col = atoi(env_columns);
    int size=floor(d*(col-6));

    for(int i=0;i<col-6;i++){
        if(i<size){
            cout<<'=';
        } else {
            cout<<' ';
        }
    }

    cout<<"["<<setprecision(2)<<floor(d*100)<<"%]"<<"\r"<<flush;
    //cout.precision(6);
    cout.precision(3);

}

void barrage::WriteStim()
{

    vector<Point*> points;
    vec2d* grid = new vec2d[W*H];
    vector<stim> sti_ind;
    ifstream stimfile(optstimfile.c_str());
    string stim_string;


    int i,j,k;
    double u,v;
    double x,y;
    unsigned char r_tmp[H*W];

    // populate points
    for(i=0;i<W*H;i++){

        grid[i].x = x = i%W;
        grid[i].y = y = i/W+1;
        u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2*xmax);
        v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2*ymax,u);

    }

    int ngen;

    cout<< "Reading list of stimuli to generate from: " <<  optstimfile << endl;
    while(stimfile>>stim_string){
        sti_ind.push_back(string_to_stim(stim_string.c_str()));
    }

    ngen=sti_ind.size();

    for(i=0;i<ngen;i++){
        points.resize(nframes_vec[sti_ind[i]]+1);
        for(j=0;j<nframes_vec[sti_ind[i]]+1;j++) points[j] = new Point[H*W];

        cout<<"Generating epoch "<<code_stim(sti_ind[i])<<' '<<sti_ind[i]<<' '<<nframes_vec[sti_ind[i]]<<endl;
        cout<<"CHECK: "<<points.size()<<' '<<nframes_vec[sti_ind[i]]<<endl;
        GenFrames(points,sti_ind[i],nframes_vec[sti_ind[i]]);
        
 	ostringstream ss;
        ss<<stimlibloc<<"/"<<code_stim(sti_ind[i])<<".bin";
        ofstream output(ss.str().c_str(),ios::out | ios::binary);
        
    cout << "output to " << ss.str()<<endl;
        for(j=0;j<nframes_vec[sti_ind[i]];j++) {
            for(k=0;k<W*H;k++) r_tmp[k]=points[j][k].r;
            output.write((char*)r_tmp,sizeof r_tmp);
            //for(k=0;k<W*H;k++) {
            //	output<<points[j][k].r<<endl;
            //}
        }
        output.close();

    }

}

double barrage::barrage_duration()
{
    // Build barrage
    ifstream StimList_file(optstimfile.c_str());

    string str;
    double duration=waiting_time; // initial wait in seconds
    int nstim=0;

    while(StimList_file>>str){
        nstim++;
        stim tmpstim=string_to_stim(str.c_str());
        duration+=repeats*(nframes_vec[(int)tmpstim]*dt/1000.);
    }

    duration+=inter_epoch_time*(repeats*nstim-1);

    StimList_file.close();

    return(duration);
}


void barrage::displayXbackground(char* window,
                                 ofstream &ticksfile, ofstream &OUTFILE,
                                 const cv::Mat &mask_mat,
                                 int numEP_spec, double t0,
                                 const vector<unsigned char*>& stimdata,
                                 const vector<stim>& StimList,
                                 const vector<int>& random_order_all,
                                 int background_type){

    char c='a';
    int k=0;
    int epID=(CONCENTRIC_ON) ? 0 : 1;


    ticksfile<<cv::getTickCount()<<" -1 0"<<endl;

    cv::waitKey(1000);
    cv::Mat A(H,W,CV_8U,cv::Scalar(0));
    cv::Mat Asub(H,W,CV_8U,cv::Scalar(0));

    while(c!='q'){
        if(k==0 && epID==numEP_spec) break;
        A.data=(stimdata[random_order_all[epID]]+W*H*k);
        cv::subtract(mask_mat,A,Asub,mask_mat);

        if(k==0){
            OUTFILE<<((double)cv::getTickCount()-t0)/cv::getTickFrequency()
                  <<' '<<code_stim(StimList[epID])
                 <<' '<<background_type<<endl;
        }

        ticksfile<<cv::getTickCount()<<' '<<code_stim(StimList[epID])<<' '<<k<<endl;
        cv::imshow("vs",Asub);
        k++;
        cout<<code_stim(StimList[epID])<<"    \r"<<flush;

        if(k==nframes_vec[StimList[epID]] && epID<numEP_spec) {
            epID++;
            cout<<endl;
            k=0;
            cv::imshow("vs",mask_mat);
            OUTFILE<<((double)cv::getTickCount()-t0)/cv::getTickFrequency()
                  <<' '<<code_stim(StimList[epID-1])
                    <<' '<<background_type<<endl;
            ticksfile<<cv::getTickCount()<<' '<<"-1 0"<<endl;
            cv::waitKey(inter_epoch_time*1000);
        } else c=cv::waitKey(38);
    }
}


std::vector<string> barrage::loadStimListFromFile(string filename)
{
    ifstream StimList_file(filename.c_str());
    std::vector<string> StimList;
    string str;

    while(StimList_file>>str){
    //StimList.push_back(string_to_stim(str.c_str()));
        StimList.push_back(str);
        if(StimList.size()==1 && string_to_stim(str.c_str()) != CONCENTRIC){
            cout << "Please put CONCENTRIC in the first line of "<<
                    filename<<". "<<endl;
            exit(0);
        }

    }

return(StimList);

}

/// \brief Runs The visual stimulation routine based on user settings
///
void barrage::VisualStimulation(string prefix, bool &run){

    double t0 = (double)cv::getTickCount();
    size_t i;
    int counter;
    int number_of_stimuli=0;
    double u,v;
    double x,y;
    CONCENTRIC_ON=false;
    bool verbose=false;
    vector<int> random_order;



    // Build barrage - Load List from File
    ifstream StimList_file(optstimfile.c_str());
    vector<stim> StimList;
    vector<stim> StimList_tmp;
    string str;
    
    while(StimList_file>>str){
        StimList.push_back(string_to_stim(str.c_str()));
        if(StimList.size()>1 && StimList.back()==CONCENTRIC){
            cout << "Please put CONCENTRIC in the first line of "<<
                    optstimfile<<". Thanks."<<endl;
            exit(0);
        }

        StimList_tmp.push_back(StimList.back());
        if(str!="CONCENTRIC"){            
            random_order.push_back(number_of_stimuli+CONCENTRIC_ON);
            number_of_stimuli++;
        } else CONCENTRIC_ON=true;
    }
    
    // log files

    ofstream OUTFILE;
    ofstream epoch_order;
    ofstream ticksfile;
    ofstream setting_file;
    ostringstream epoch_order_name;
    ostringstream OUTFILE_name;
    ostringstream ticksfile_name;
    ostringstream settings_name;
    
    OUTFILE_name<<prefix<<"/time_epoches.log";
    epoch_order_name<<prefix<<"/epoch_order.log";
    ticksfile_name<<prefix<<"/ticks.log";
    settings_name<<prefix<<"/settings.log";
    
    OUTFILE.open(OUTFILE_name.str().c_str());
    epoch_order.open(epoch_order_name.str().c_str());
    ticksfile.open(ticksfile_name.str().c_str());

    setting_file.open(settings_name.str().c_str());

    setting_file << repeats<<' '<<inter_epoch_time<<' '<< waiting_time<< std::endl;
    setting_file << "repeats"<<' '<<"inter_epoch_time"<<' ' << "waiting_time" << std::endl; //Add field Names on Bottom So they do not interfere with file reading
    setting_file.close();

    cout<<"Recording epoches times in "<<OUTFILE_name.str()<<endl;
    cout<<"Recording ticks in "<<ticksfile_name.str()<<endl;
    
    cout<<endl;
    cout<<"Using "<<repeats<<" repeats"<<endl;
    cout<<"      "<<inter_epoch_time<<"s between stimuli"<<endl;
    cout<<"      "<<waiting_time<<"s before barrage start"<<endl;
    cout<<endl;
    
    int numEP_spec = CONCENTRIC_ON+number_of_stimuli*repeats;

    vector<unsigned char*> stimdata(number_of_stimuli+CONCENTRIC_ON);
    vector<int> random_order_all(numEP_spec);
    FillPoints(stimdata,StimList_tmp);
    vec2d* grid = new vec2d[W*H];
    unsigned char *mask = new unsigned char[H*W];

    // Setting up grid (x,y values of projected matrix) and the grey mask.
    for(i=0;i<W*H;i++){
        grid[i].x = x = i%W;
        grid[i].y = y = i/W+1;
        u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2*xmax);
        v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2*ymax,u);

        if(fabs(u)<th_max && fabs(v)<ymax){
            mask[i]=flat_background_color;
        } else {
            mask[i]=0;
        }
    }

    // Define Mat object using the std array mask.
    cv::Mat mask_mat(H,W,CV_8U,(void*)mask);
    std::srand(std::time(NULL));

    //for(i=0;i<StimList.size();++i) cout<<code_stim(StimList[i])<<endl;

    counter=0;
    if(CONCENTRIC_ON){
        StimList[0]=CONCENTRIC;
        random_order_all[0]=0;
        counter++;
    }

    for(unsigned int k=0;k<repeats;k++){
        random_shuffle (random_order.begin(),random_order.end());
        if(k==0) {
            for(i=0;i<number_of_stimuli;++i){
                StimList[counter]=StimList_tmp[random_order[i]];
                random_order_all[counter]=random_order[i];
                counter++;
            }
        } else {
            while(StimList_tmp[random_order[0]]==StimList[counter-1])
                random_shuffle (random_order.begin(),random_order.end());
            for(i=0;i<number_of_stimuli;++i) {
                StimList.push_back(StimList_tmp[random_order[i]]);
                random_order_all[counter]=random_order[i];
                counter++;
            }
        }
    }

    for(i=0;i<numEP_spec;i++) epoch_order<<StimList[i]<<endl;

    if(verbose){
        cout<<"X max  = "<<xmax<<endl;
        cout<<"Y max  = "<<ymax<<endl;
        cout<<"aspect = "<<frac<<endl;
        cout<<"FRAMES = "<<nframes<<endl;
        cout<<"MAX_TH = "<<th_max/pi*180<<endl;
    }

    for(i=0;i<numEP_spec;i++) {
        cout<<code_stim(StimList[i])<<' ';
        if(StimList[i]==CONCENTRIC || (i+1-CONCENTRIC_ON) % number_of_stimuli == 0)
            cout<<endl;
    }
    cout<<endl;

    char c='a';
    int k=0;
    int epID=0;

    // Important note on projection screen: //////////////////////////////////////////
    // The second screen needs to be positioned as
    //
    //       |^^^^^^^^^||^^^|
    //       |         || 2 |
    //       |    1    | ---
    //       |         |
    //       -----------
    //
    // The projection screen size must be 800x600 (4:3).
    // To avoid the appearance of the topbar use GNOME Classic as desktop environment
    //

    cv::namedWindow("vs", cv::WINDOW_GUI_NORMAL );
    // cv::moveWindow("vs",0,0);
    cv::moveWindow("vs",1680,0);
    //cv::resizeWindow("vs",1024,678);
    cv::setWindowProperty("vs", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    // ##############################################################################

    // Display the mask and wait
    cv::imshow("vs",mask_mat);

    // Show Camera Live View //
    cv::namedWindow("Camera",cv::WINDOW_NORMAL);
    cv::resizeWindow("Camera",400,400);


    ticksfile<<cv::getTickCount()<<' '<<"-1 0"<<endl;
    cout<<"Waiting for " << waiting_time << " sec. Press any key to override wait."<<endl;
    if(waiting_time>0)
        cv::waitKey(1000*waiting_time);
    cv::Mat A(H,W,CV_8U);

    while(c!='q' && run){
        if(k==0 && epID==numEP_spec) break;
        A.data=(stimdata[random_order_all[epID]]+W*H*k);
        if(k==0){
            OUTFILE<<((double)cv::getTickCount()-t0)/cv::getTickFrequency()<<' '<<code_stim(StimList[epID])<<endl;
        }
        ticksfile<<cv::getTickCount()<<' '<<code_stim(StimList[epID])<<' '<<k<<endl;
        cv::imshow("vs",A);
        k++;
        cout<<code_stim(StimList[epID])<<"    \r"<<flush;

        if(k==nframes_vec[StimList[epID]] && epID<numEP_spec) {
            epID++;


            k=0;
            cv::imshow("vs",mask_mat);
            double elapsedTsec = ((double)cv::getTickCount()-t0)/cv::getTickFrequency();
            OUTFILE << elapsedTsec  <<' '<<code_stim(StimList[epID-1])<<endl;
            ticksfile<<cv::getTickCount()<<' '<<"-1 0"<<endl;
            cout << " " << elapsedTsec << std::endl;



            if(inter_epoch_time>0)
                cv::waitKey(inter_epoch_time*1000);
        } else
            c=cv::waitKey(20);


        /// TODO  Show Live Cam To user
    } // Main VizStim Loop


    cv::destroywindow("Camera");
}

void barrage::VisualStimulation_BG(string prefix, bool &run)
{
    double t0 = (double)cv::getTickCount();
    size_t i;
    int counter;
    int number_of_stimuli=0;
    double u,v;
    double x,y;
    CONCENTRIC_ON=true;
    bool verbose=false;
    vector<int> random_order;
    cv::RNG rng(cv::getTickCount());


    // Build barrage
    ifstream StimList_file(optstimfile.c_str());
    vector<stim> StimList;
    vector<stim> StimList_tmp;
    string str;

    StimList.push_back(CONCENTRIC);
    StimList_tmp.push_back(CONCENTRIC);

    while(StimList_file>>str){
        StimList.push_back(string_to_stim(str.c_str()));
        if(StimList.back()==CONCENTRIC){
            cout << "Please remove CONCENTRIC from the StimList file "
                 << optstimfile <<". Thanks."<<endl;
            exit(0);
        }

        StimList_tmp.push_back(StimList.back());
        number_of_stimuli++;
        random_order.push_back(number_of_stimuli);
    }

    ofstream OUTFILE;
    ofstream epoch_order;
    ofstream ticksfile;
    ofstream setting_file;
    ostringstream epoch_order_name;
    ostringstream OUTFILE_name;
    ostringstream ticksfile_name;
    ostringstream settings_name;

    OUTFILE_name<<prefix<<"/time_epoches.log";
    epoch_order_name<<prefix<<"/epoch_order.log";
    ticksfile_name<<prefix<<"/ticks.log";
    settings_name<<prefix<<"/settings.log";

    OUTFILE.open(OUTFILE_name.str().c_str());
    epoch_order.open(epoch_order_name.str().c_str());
    ticksfile.open(ticksfile_name.str().c_str());    

    setting_file.open(settings_name.str().c_str());
    setting_file<<repeats<<' '<<inter_epoch_time<<' '<<waiting_time<<endl;
    setting_file.close();

    cout<<"Recording epoches times in "<<OUTFILE_name.str()<<endl;
    cout<<"Recording ticks in "<<ticksfile_name.str()<<endl;

    cout<<endl;
    cout<<"Using "<<repeats<<" repeats"<<endl;
    cout<<"      "<<inter_epoch_time<<"s between stimuli"<<endl;
    cout<<"      "<<waiting_time<<"s before barrage start"<<endl;
    cout<<endl;

    int numEP_spec = 1+number_of_stimuli*repeats;

    vector<unsigned char*> stimdata(number_of_stimuli+1);
    vector<int> random_order_all(numEP_spec);
    FillPoints(stimdata,StimList_tmp);
    vec2d* grid = new vec2d[W*H];
    unsigned char* mask[2];
    mask[0] = new unsigned char[H*W];
    mask[1] = new unsigned char[H*W];
    getBackground();

    // Setting up grid (x,y values of projected matrix) and the grey mask.
    for(i=0;i<W*H;i++){
        grid[i].x = x = i%W;
        grid[i].y = y = i/W+1;
        u=get_th(-xmax+(x-(W-Wp)/2)/Wp*2*xmax);
        v=get_y(-ymax+(y-(H-Hp)/2)/Hp*2*ymax,u);

        if(fabs(u)<th_max && fabs(v)<ymax){
            mask[1][i]=1+0.5*background[i];
            mask[0][i]=127;
        } else {
            mask[0][i]=0;
            mask[1][i]=0;
        }
    }

    // Define Mat object using the std array mask.
    cv::Mat mask_mat[2];
    mask_mat[0]=cv::Mat(H,W,CV_8U,(void*)mask[0]);
    mask_mat[1]=cv::Mat(H,W,CV_8U,(void*)mask[1]);
    std::srand(std::time(NULL));

    counter=0;
    StimList[0]=CONCENTRIC;
    random_order_all[0]=0;
    counter++;

    for(unsigned int k=0;k<repeats;k++){
        random_shuffle (random_order.begin(),random_order.end());
        if(k==0) {
            for(i=0;i<number_of_stimuli;++i){
                StimList[counter]=StimList_tmp[random_order[i]];
                random_order_all[counter]=random_order[i];
                counter++;
            }
        } else {
            while(StimList_tmp[random_order[0]]==StimList[counter-1])
                random_shuffle (random_order.begin(),random_order.end());
            for(i=0;i<number_of_stimuli;++i) {
                StimList.push_back(StimList_tmp[random_order[i]]);
                random_order_all[counter]=random_order[i];
                counter++;
            }
        }
    }

    for(i=0;i<numEP_spec;i++) epoch_order<<StimList[i]<<endl;

    if(verbose){
        cout<<"X max  = "<<xmax<<endl;
        cout<<"Y max  = "<<ymax<<endl;
        cout<<"aspect = "<<frac<<endl;
        cout<<"FRAMES = "<<nframes<<endl;
        cout<<"MAX_TH = "<<th_max/pi*180<<endl;
    }

    for(i=0;i<numEP_spec;i++) {
        cout<<code_stim(StimList[i])<<' ';
        if(StimList[i]==CONCENTRIC || i % number_of_stimuli == 0) cout<<endl;
    }
    cout<<endl;

    cv::namedWindow("vs", cv::WINDOW_GUI_NORMAL );
    //cv::moveWindow("vs",0,0);
    cv::moveWindow("vs",1680,0);
    //cv::resizeWindow("vs",1024,678);
    cv::setWindowProperty("vs", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    int background_type=(rng.uniform((double)0,(double)1)<.5) ? 0 : 1;

    // Display the mask and wait
    cv::imshow("vs",mask_mat[background_type]);
    cout<<"Wait "<< waiting_time<<" seconds..."<<endl;
    cv::waitKey(waiting_time*1000);

    displayXbackground("vs",ticksfile,OUTFILE,mask_mat[background_type],numEP_spec,t0,stimdata,StimList,random_order_all,background_type);

    CONCENTRIC_ON=false;
    background_type=(background_type==1) ? 0 : 1;

    cv::waitKey(inter_epoch_time*1000);

    displayXbackground("vs",ticksfile,OUTFILE,mask_mat[background_type],numEP_spec,t0,stimdata,StimList,random_order_all,background_type);

    cout<<"exiting."<<endl;
    cv::destroyAllWindows();
}

