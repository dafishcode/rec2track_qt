#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> //Qt Changed Dir structure and need to add QtWidgets prefix
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <FlyCapture2.h>
#include <CameraBase.h>
#include <iostream>
#include <sstream>
#include<fstream>
#include<sys/stat.h>
#include<signal.h>
#include<cstdio>
#include"../include/camera_functions.h"
#include"../include/barrage.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void record(float fFrameRate,float fShutterDuration,bool btrigger);
    int TickProgress();
    void ResetProgress();
    barrage StimulationBarrage;

private slots:

    void updateBarrage();



    void on_repeats_valueChanged(int arg1);

    void on_spinBox_inter_epoch_valueChanged(int arg1);

    void on_btn_genBarrageFiles_clicked();

    void on_btn_testVizStimOnProjector_clicked();

    void on_btn_CalcDuration_clicked();

    void on_spinBox_framerate_valueChanged(int arg1);

    void on_spinBox_waiting_time_valueChanged(int arg1);

    void on_btn_startVideoRecording_clicked();

    void on_btnStartLiveTracking_clicked();

    void on_btn_camLiveView_clicked();




    void on_btn_selectStimSetFile_clicked();

    void on_btn_selectfolder_clicked();

    void on_radioVizStimOn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
