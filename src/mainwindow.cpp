#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::record(){
    PrintBuildInfo();

    BusManager busMgr;
    PGRGuid guid;
    busMgr.GetCameraFromIndex(0, &guid);
    F7 f7;

    Mode mode;
    mode=MODE_1;

    Camera cam; cam.Connect(&guid); SetCam(&cam,f7,mode,PIXEL_FORMAT_RAW8);

    struct thread_data2 RSC_input;
    QString tmp=ui->FolderEdit->text();
    RSC_input.cam = &cam;
    RSC_input.proc_folder= tmp.toLocal8Bit().data();
    RSC_input.display="display";
    RSC_input.crop=true;
    RSC_input.repeats=ui->repeats->value();

    int userIndex = ui->comboBox->currentIndex();

    if(userIndex==0) RSC_input.optstimfile="opt/StimList_Tom.txt";
    if(userIndex==1) RSC_input.optstimfile="opt/StimList_Rachel.txt";

    bool VisualStimulation_on=ui->radioButton->isChecked();



    size_t counter;

    Rec_onDisk_conditional((void*)&RSC_input, VisualStimulation_on);
}

void MainWindow::on_pushButton_2_clicked()
{
    close();
    record();
}
