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
    RSC_input.proc_folder= tmp.toStdString();
    RSC_input.display="display";
    RSC_input.crop=true;
    RSC_input.recording_time=ui->spinBox_rectime->value() * 60;

    updateBarrage();

    bool VisualStimulation_on=ui->radioButton->isChecked();

    Rec_onDisk_conditional((void*)&RSC_input, VisualStimulation_on, &StimulationBarrage);
}

void MainWindow::updateBarrage()
{
    int userIndex = ui->comboBox->currentIndex();

    if(userIndex==0) {
        StimulationBarrage.optstimfile="../opt/StimList_Tom.txt";
        StimulationBarrage.Background_ON=true;
    }

    if(userIndex==1) {
        StimulationBarrage.optstimfile="../opt/StimList_Rachel.txt";
        StimulationBarrage.Background_ON=false;
    }

    StimulationBarrage.repeats=ui->repeats->value();
    StimulationBarrage.inter_epoch_time=ui->spinBox_inter_epoch->value();
}

void MainWindow::on_pushButton_2_clicked()
{
    close();
    record();
}

void MainWindow::on_pushButton_clicked()
{

    PrintBuildInfo();

    BusManager busMgr;

    PGRGuid guid;
    busMgr.GetCameraFromIndex(0, &guid);
    Run_SingleCamera(guid);

}

void MainWindow::on_pushButton_3_clicked()
{

    updateBarrage();
    int i;
    QString tmp=ui->FolderEdit->text();
    cv::namedWindow("display - track",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
    cv::resizeWindow("display - track", 800,800);

// Read number of lines in the output file
    string line;

    int LN=0;
    ifstream logfile(tmp.toStdString()+"/time.log");
    while(getline(logfile,line)) LN++;
    logfile.close();

    // Get Stim file    
    int userIndex = ui->comboBox->currentIndex();
    if(userIndex==0) StimulationBarrage.optstimfile="opt/StimList_Tom.txt";
    if(userIndex==1) StimulationBarrage.optstimfile="opt/StimList_Rachel.txt";

    bool VisualStimulation_on=ui->radioButton->isChecked();



    if(VisualStimulation_on)
        ReadImageSeq_vs(tmp.toStdString(),"display - track",0,".tiff",&StimulationBarrage,LN);
    else
        ReadImageSeq_and_track(tmp.toStdString(),"display - track",0,".tiff",LN);

}

void MainWindow::on_repeats_valueChanged(int arg1)
{
    updateBarrage();
    StimulationBarrage.repeats=arg1;
    ui->Duration->setText(QString::number(StimulationBarrage.barrage_duration())+" s");
}

void MainWindow::on_spinBox_inter_epoch_valueChanged(int arg1)
{
    StimulationBarrage.inter_epoch_time=arg1;
}
