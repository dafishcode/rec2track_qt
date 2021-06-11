# rec2track_qt
C++ tool for stimuli presentation and behavioural recording

Used in conjuction with the Meyer lab 2p microscope to produce visual stimulation patterns synchronized to the 2p brain images.
The opt directory needs be found one up from the folder where the binary is being executed, and needs to contain the following input files :

-   StimLibFolder.txt : the folder where the stimulus barrage binaries are to be saved and loaded from.

Output files:

  - time_epoches.txt : The main output file of interest is the
  - settings.txt:


## List of Available Stimulus codes

GRAT_90_SF05;
GRAT_270_SF05;
GRAT_0_SF05;
GRAT_180_SF05;
GRAT_90_SF15;
GRAT_270_SF15;
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
LIGHTBAR_0;
LIGHTBAR_90;
LIGHTBAR_180;
LIGHTBAR_270;
LOOM;
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
