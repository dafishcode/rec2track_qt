
#include<QApplication>
#include"include/mainwindow.h"

using namespace std;

int main(int argc, char** argv)
{
    QApplication::setStyle("plastique");
    QApplication a(argc,argv);
    MainWindow m;
    m.show();


    return a.exec();
}
