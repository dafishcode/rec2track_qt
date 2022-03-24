
#include<QApplication>
#include"include/mainwindow.h"

using namespace std;

int main(int argc, char** argv)
{
    QApplication::setStyle("plastique");
    QApplication a(argc,argv);
    MainWindow m;
    m.show();


    cout << "Make video with: ffmpeg -framerate 90 -i %10d.pgm -c:v libx265 -strict experimental -x265-params lossless=1 <output_lossless.mp4>"

    return a.exec();
}
