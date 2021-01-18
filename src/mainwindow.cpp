#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include<QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    StimulationBarrage.setStimLib();
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

    Camera cam; cam.Connect(&guid);
    SetCam(&cam,f7,mode,PIXEL_FORMAT_RAW8,true);
    cam.StartCapture();

    struct thread_data2 RSC_input;
    QString tmp=ui->FolderEdit->text();
    RSC_input.bus = &busMgr;
    RSC_input.guid = &guid;
    RSC_input.cam = &cam;
    RSC_input.proc_folder= tmp.toStdString();
    RSC_input.display="display";
    RSC_input.crop=true;
    RSC_input.recording_time=ui->spinBox_rectime->value() * 60;

    updateBarrage();
    CreateOutputFolder(RSC_input.proc_folder);

    bool VisualStimulation_on=ui->radioButton->isChecked();
    cout<<"inter epoch times = "<<StimulationBarrage.inter_epoch_time<<endl;

    Rec_onDisk_conditional((void*)&RSC_input, VisualStimulation_on, &StimulationBarrage);
}

void MainWindow::updateBarrage()
{
    int userIndex = ui->comboBox->currentIndex();

    if(userIndex==0) {
        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Tom.txt";
        StimulationBarrage.Background_ON=true;
    }

    if(userIndex==1) {
        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Rachel.txt";
        StimulationBarrage.Background_ON=false;
    }

    if(userIndex==2) {
        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Dominic.txt";
        StimulationBarrage.Background_ON=false;
        StimulationBarrage.setBackgroundColor(0);
    }

    if(userIndex==3) {
        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_TomShallcross.txt";
        StimulationBarrage.Background_ON=false;
    }

    StimulationBarrage.repeats=ui->repeats->value();
    StimulationBarrage.inter_epoch_time=ui->spinBox_inter_epoch->value();
    StimulationBarrage.waiting_time=ui->spinBox_waiting_time->value();

}

// record button
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
    Run_SingleCamera(&guid);

}

// Analysis
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
    if(userIndex==0) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Tom.txt";
    if(userIndex==1) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Rachel.txt";
    if(userIndex==3) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_TomShallcross.txt";

    ifstream setting_file;

    bool VisualStimulation_on=ui->radioButton->isChecked();

    if(VisualStimulation_on){
        // Get inter-epoch time and number of repeats
        setting_file.open(tmp.toStdString()+"/settings.log");
        setting_file>>StimulationBarrage.repeats>>StimulationBarrage.inter_epoch_time>>StimulationBarrage.waiting_time;
        setting_file.close();
        ReadImageSeq_vs(tmp.toStdString(),"display - track",0,".tiff",&StimulationBarrage,LN);
    } else
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

void MainWindow::on_pushButton_4_clicked()
{
    updateBarrage();    
    StimulationBarrage.transform_image(QApplication::applicationDirPath().toStdString()+"/../opt/background.jpg");
    cout<<"CIAO"<<endl;
    StimulationBarrage.WriteStim();
}

// Run barrage button
void MainWindow::on_pushButton_5_clicked()
{
    updateBarrage();
    QString tmp=ui->FolderEdit->text();
    CreateOutputFolder(tmp.toStdString());

    cout<<"APPfolder "<<QCoreApplication::applicationDirPath().toStdString()<<endl;
    cout<<"opt <- "<<StimulationBarrage.optstimfile<<endl;
    cout<<"Stimulus folder <- "<<StimulationBarrage.stimlibloc<<endl;
    bool VisualStimulation_on=ui->radioButton->isChecked();
    cout<<"inter epoch times = "<<StimulationBarrage.inter_epoch_time<<endl;


    bool run=true;

    if(!StimulationBarrage.Background_ON) StimulationBarrage.VisualStimulation(tmp.toStdString(),run);
    else StimulationBarrage.VisualStimulation_BG(tmp.toStdString(),run);

}
