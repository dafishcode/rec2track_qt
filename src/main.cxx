
#include<QApplication>
#include"include/mainwindow.h"

using namespace std;
using namespace FlyCapture2;

int main(int argc, char** argv)
{

    QApplication a(argc,argv);
    MainWindow m;
    m.show();


    return a.exec();
}
