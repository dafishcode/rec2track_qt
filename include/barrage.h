#ifndef BARRAGE_H
#define BARRAGE_H

#include "../include/Point.h"
#include<vector>
#include<string>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

enum stim {LOOM,
           WIGGLY5_DL,WIGGLY5_DR,WIGGLY5_UL,WIGGLY5_UR,
           WIGGLY15_DL,WIGGLY15_DR,WIGGLY15_UL,WIGGLY15_UR,
           WIGGLY26_DL,WIGGLY26_DR,WIGGLY26_UL,WIGGLY26_UR,
           GRAT_90_SF05,GRAT_270_SF05,GRAT_180_SF05,GRAT_0_SF05,
           GRAT_90_SF15,GRAT_270_SF15,GRAT_180_SF15,GRAT_0_SF15,
           GRAT_90_SF26,GRAT_270_SF26,GRAT_180_SF26,GRAT_0_SF26,
           BAR_90,BAR_270,BAR_0,BAR_180,LIGHTBAR_90,LIGHTBAR_270,LIGHTBAR_0,LIGHTBAR_180,
           DOT_90_SF05,DOT_270_SF05,
           DOT_90_SF15,DOT_270_SF15,
           DOT_90_SF26,DOT_270_SF26,
           WDOT_1, WDOT_2,WDOT_3,WDOT_4,WDOT_5,WDOT_6,WDOT_7, //Number ref to position (note : 1,7 fall off screen)
           CONCENTRIC,
           WDOT_T1,WDOT_T2,WDOT_T3,
           LIGHTDOT_90_SF05,LIGHTDOT_270_SF05,
           LIGHTDOT_90_SF15,LIGHTDOT_270_SF15,
           LIGHTDOT_90_SF26,LIGHTDOT_270_SF26,
           LIGHT,DARK,
           WDOT_Tb1,WDOT_Tb2,WDOT_Tb3,WDOT_Tb4,WDOT_Tb5,WDOT_Tb6,WDOT_Tb7,WDOT_Tb8,
           WDOT_CONTRAST_1, // These Are for T. Sains Stimuli
           WDOT_CONTRAST_2,
           WDOT_CONTRAST_3,
           WDOT_CONTRAST_4,
           WDOT_CONTRAST_5,
           WDOT_CONTRAST_6,
           WDOT_CONTRAST_7,
           WDOT_CONTRAST_8,
           WDOT_CONTRAST_9,
           WDOT_CONTRAST_10,
          };

class barrage {
public:
    barrage();
    double get_th(double);
    double get_y(double,double);
    void setStimLib();
    void transform_image(string imfile);
    void getBackground();
    void setBackgroundColor(int);
    void GenFrames(vector<Point*> &, stim,unsigned int);


    void get_vs_mask(cv::Mat& mask_mat);
    int matchtime(int64 timecamera, vector<int64>& trace2);
    void ProgressBar(double);
    void FillPoints(vector<unsigned char*> &,vector<stim> &);
    void WriteStim();

    double barrage_duration();

    void displayXbackground(char* window,
                          ofstream &ticksfile, ofstream &OUTFILE,
                          const cv::Mat &mask_mat,
                          int numEP_spec, double t0,
                          const vector<unsigned char*>& stimdata,
                          const vector<stim>& StimList,
                          const vector<int>& random_order_all,
                          int background_type);

    void VisualStimulation(string prefix, bool &run);
    void VisualStimulation_BG(string prefix, bool &run);

    static string code_stim(stim);
    static stim string_to_stim(const char* s);
    static vector<string> loadStimListFromFile(string filename);

    size_t W;
    size_t H;
    double Hp;
    double X0,Y0;
    double pi;
    double d;
    double r;
    double dt;
    int timeEP;
    int inter_epoch_time;
    int waiting_time;
    int repeats;
    int numEP;
    double hs;
    double th_max;
    double xmax;
    double ymax;
    double frac;
    double Wp;
    double rp_max;
    int nframes;
    double time_bar_horizontal;
    double time_bar_vertical;
    //int nframes_vec[58]; before adding WDOT_Tb
    int nframes_vec[76];
    bool fading;
    Point *mask;
    vector<unsigned char> background;
    string optstimfile;
    string stimlibloc;
    bool Background_ON;

private:
    double radDot(double);
    double spatFreq(double);
    int flat_background_color;

    bool CONCENTRIC_ON;
};

#endif
