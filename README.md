# rec2track_qt
C++ tool for stimuli presentation and behavioural recording
Used in conjunction with the Meyer lab 2p microscope to produce visual stimulation patterns synchronized to the 2p brain images.


```
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
```
 *Remember to Switch projector to HDMI input*, so it becomes the Desktop's 2nd screen. 

The opt directory needs be found one up from the folder where the binary is being executed, and needs to contain the following input files :

-   StimLibFolder.txt : the folder where the stimulus barrage binaries are to be saved and loaded from.

Output files:

  - time_epoches.txt : The main output file of interest is the
  - settings.txt:


## List of Available Stimulus codes

```
GRAT_90_SF05;
GRAT_270_SF05;
GRAT_0_SF05;
GRAT_180_SF05;
GRAT_90_SF15;
GRAT_270_SF15;
GRAT_0_SF15;
GRAT_180_SF15;
GRAT_90_SF26;
GRAT_270_SF26;
GRAT_0_SF26;
GRAT_180_SF26;
BAR_0;
BAR_90;
BAR_180;
BAR_270;
LIGHTBAR_0;
LIGHTBAR_90;
LIGHTBAR_180;
LIGHTBAR_270;
LOOM;
WIGGLY5_UL;
WIGGLY5_UR;
WIGGLY5_DL;
WIGGLY5_DR;
WIGGLY15_UL;
WIGGLY15_UR;
WIGGLY15_DL;
WIGGLY15_DR;
WIGGLY26_UL;
WIGGLY26_UR;
WIGGLY26_DL;
WIGGLY26_DR;
DOT_90_SF05;
DOT_90_SF15;
DOT_90_SF26;
DOT_270_SF05;
DOT_270_SF15;
DOT_270_SF26;
LIGHTDOT_90_SF05;
LIGHTDOT_90_SF15;
LIGHTDOT_90_SF26;
LIGHTDOT_270_SF05;
LIGHTDOT_270_SF15;
LIGHTDOT_270_SF26;
WDOT_1;
WDOT_2;
WDOT_3;
WDOT_4;
WDOT_5;
WDOT_6;
WDOT_7;
WDOT_T1;
WDOT_T2;
WDOT_T3;
CONCENTRIC;
DARK;
LIGHT;
WDOT_Tb1;
WDOT_Tb2;
WDOT_Tb3;
WDOT_Tb4;
WDOT_Tb5;
WDOT_Tb6;
WDOT_Tb7;
WDOT_Tb8;
WDOT_CONTRAST_1;
WDOT_CONTRAST_2;
WDOT_CONTRAST_3;
WDOT_CONTRAST_4;
WDOT_CONTRAST_5;
WDOT_CONTRAST_6;
WDOT_CONTRAST_7;
WDOT_CONTRAST_8;
WDOT_CONTRAST_9;
WDOT_CONTRAST_10;
 ```
## Behavioural recording and Tracking 

During behavioural recording a display of the realtime video is shown. 
Going through the code i Found the following keys give the user some control over options:
```
while(c!='q'){
       if(c=='f') ind+=speed;
        if(c=='t') ind+=40;
        if(c=='b') ind=max(0,ind-speed);
        if(c=='h') hide_trace=!hide_trace;
```
