//// \brief A software controller for visual stimuli presentation with simultaneous recording of behaviour
/// @authors Konstantinos Lagogiannis 2022, Giovanni Diana  ~ 2018
/// @title rec2track, a camera video recorder with simultaneous visual stimuli presentation.
///  It allows for camera triggers so as to syncronize with 2 photon imaging experiments - which is the original use it was designed for
///
/// Initially developed for Meyer Lab by Giovanni Diana (GD) ~ 2018 , with some contribution for how to do the threading by Konstantinos Lagogiannis (KL).
/// Updated and taken over by KL in 2022 - impoving functionality and GUI use while allowing to save behaviour as video directly

#include<QApplication>
#include<opencv4/opencv2/imgproc.hpp>
#include"include/mainwindow.h"

using namespace std;
MainWindow* gpMainwindow;

int main(int argc, char** argv)
{


    QApplication::setStyle("plastique");
    QApplication a(argc,argv);
    gpMainwindow = new MainWindow();
    gpMainwindow->show();

    printf("OpenCV: %s", cv::getBuildInformation().c_str());

    // "Make video with: ffmpeg -framerate 90 -i %10d.pgm -c:v libx265 -strict experimental -x265-params lossless=1 <output_lossless.mp4>";

    return a.exec();
}
