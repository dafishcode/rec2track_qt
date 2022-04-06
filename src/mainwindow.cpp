#include "../include/mainwindow.h"
#include "../include/circular_video_buffer_ts.h"
#include "ui_mainwindow.h"
#include<QCoreApplication>
 #include <QFileDialog>
#include <QDebug>

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


void MainWindow::record(float fFrameRate,float fShutterDuration,bool btrigger = true){
    PrintBuildInfo();
    BusManager busMgr;
    PGRGuid guid;
    busMgr.GetCameraFromIndex(0, &guid);
    F7 f7;

    Mode mode;
    mode=MODE_0; //MODE_0 1280x1024 -  MODE_1 640x480 for high fps


    Camera cam; cam.Connect(&guid);
    // Send Options to Camera - Trigger On, FrameRate And SHutter Speed /
    SetCam(&cam,f7,mode,PIXEL_FORMAT_RAW8,btrigger,fFrameRate,fShutterDuration);
    cam.StartCapture();

    struct recorderthread_data RSC_input;
    QString tmp=ui->txt_outFolder->text();
    RSC_input.bus = &busMgr;
    RSC_input.guid = &guid;
    RSC_input.cam = &cam;
    RSC_input.proc_folder= tmp.toStdString();
    RSC_input.display="display";
    RSC_input.crop=true;
    RSC_input.recording_time = ui->spinBox_rectime->value() * 60; // Convert Duration to Seconds
    RSC_input.eventCount = 0; //FOr Triggered/ Conditional Recording

    RSC_input.frameRate = ui->spinBox_framerate_low->value();
    RSC_input.shutterSpeed = ui->spinBox_shutterms->value();

    updateBarrage();
    RSC_input.proc_folder = CheckOutputFolder(RSC_input.proc_folder);

    bool VisualStimulation_on=ui->radioVizStimOn->isChecked();
    if (VisualStimulation_on)
        cout << "Camera Recording simultaneously with barrage display " << std::endl;

    cout<<"inter epoch times = "<<StimulationBarrage.inter_epoch_time<<endl;

    ui->progressBar->setRange(0,RSC_input.recording_time);
    ui->progressBar->reset();
    Rec_onDisk_conditional((void*)&RSC_input, VisualStimulation_on, &StimulationBarrage,outputType::zCam_MJPGVID);

    //Rec_onDisk_SingleCamera2((void*)&RSC_input, VisualStimulation_on, &StimulationBarrage);
}

//Reads settings from GUI form
void MainWindow::updateBarrage()
{
    int userIndex = 1 ;//ui->comboBox->currentIndex();
    /// \TODO: Use Some Key Value - THis is really messy
//    if(userIndex==0) {
//        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Tom.txt";
//        //StimulationBarrage.Background_ON=true;
//        StimulationBarrage.Background_ON=false;
//    }

//    if(userIndex==1) {
//        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Rachel.txt";
//        StimulationBarrage.Background_ON=false;
//    }

//    if(userIndex==2) {
//        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Dominic.txt";
//        StimulationBarrage.Background_ON=false;
//        StimulationBarrage.setBackgroundColor(0);
//    }

//    if(userIndex==3) {
//        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_TomShallcross.txt";
//        StimulationBarrage.Background_ON=false;
//    }

//    if(userIndex==4) {
//        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_kostasl.txt";
//        StimulationBarrage.Background_ON=false;
//    }
//    if(userIndex==5) {
//        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_visitorA.txt";
//        StimulationBarrage.Background_ON=false;
//    }
//    if(userIndex==6) {
//        StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_visitorB.txt";
//        StimulationBarrage.Background_ON=false;
//    }


    StimulationBarrage.repeats           = ui->repeats->value();
    StimulationBarrage.inter_epoch_time  = ui->spinBox_inter_epoch->value();
    StimulationBarrage.waiting_time      = ui->spinBox_waiting_time->value();
    // Update Duration in UI
    ui->Duration->setText(QString::number(StimulationBarrage.barrage_duration())+" s");

}

// record button PUSHED - Start
void MainWindow::on_btn_startVideoRecording_clicked()
{
    close();
    float fFrameRate = ui->spinBox_framerate_low->value();
    bool waitForTrigger = ui->checkBoxWaitForTrigger->isChecked();
    record(fFrameRate,5.0f,waitForTrigger); //Set Framerate and shutter Speed
}

void MainWindow::on_btn_camLiveView_clicked()
{

    PrintBuildInfo();

    BusManager busMgr;

    PGRGuid guid;
    busMgr.GetCameraFromIndex(0, &guid);

    float fFrameRate = ui->spinBox_framerate_low->value();
    Run_SingleCamera(&guid,fFrameRate,5.0f);

}

/// (Track Button Pushed) "Analysis "
void MainWindow::on_btnStartLiveTracking_clicked()
{

    updateBarrage();
    int i;
    QString tmp=ui->txt_outFolder->text();
    cv::namedWindow("display - track",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
    cv::resizeWindow("display - track", 800,800);

// Read number of lines in the output file
    string line;

    int LN=0;
    ifstream logfile(tmp.toStdString()+"/time.log");

    while(getline(logfile,line)) LN++;
    logfile.close();

    /// TODO: this is a serious mess that needs to be moved
    // Get Stim file
    //int userIndex = 1; //ui->comboBox->currentIndex();
    //if(userIndex==0) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Tom.txt";
    //if(userIndex==1) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_Rachel.txt";
    //if(userIndex==3) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_TomShallcross.txt";
    //if(userIndex==4) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_kostasl.txt";
    //if(userIndex==5) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_visitorA.txt";
    //if(userIndex==6) StimulationBarrage.optstimfile=QApplication::applicationDirPath().toStdString()+"/../opt/StimList_visitorB.txt";


    ifstream setting_file;

    bool VisualStimulation_on=ui->radioVizStimOn->isChecked();

    if(VisualStimulation_on){
        // Get inter-epoch time and number of repeats
        setting_file.open(tmp.toStdString()+"/settings.log");

        setting_file>>StimulationBarrage.repeats>>StimulationBarrage.inter_epoch_time>>StimulationBarrage.waiting_time;
        setting_file.close();


        ReadImageSeq_vs(tmp.toStdString(),"display - track",0,".tiff",&StimulationBarrage,LN);
    } else
        ReadImageSeq_and_track(tmp.toStdString(),"display - track",0,".tiff",LN);

}

int MainWindow::TickProgress()
{
    ui->progressBar->setValue(ui->progressBar->value()+1);
    ui->progressBar->value();
}

void MainWindow::ResetProgress()
{
    ui->progressBar->reset();
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

void MainWindow::on_btn_genBarrageFiles_clicked()
{
    updateBarrage();    
    StimulationBarrage.transform_image(QApplication::applicationDirPath().toStdString()+"/../opt/background.jpg");
    cout<<"CIAO"<<endl;
    StimulationBarrage.WriteStim();
}

// Run barrage button - Tests projection of Viz stim  //
void MainWindow::on_btn_testVizStimOnProjector_clicked()
{
    updateBarrage();
    QString tmp=ui->txt_outFolder->text();
    tmp = tmp.fromStdString(CheckOutputFolder(tmp.toStdString()) ) ;

    cout<<"APPfolder "<<QCoreApplication::applicationDirPath().toStdString()<<endl;
    cout<<"opt <- "<<StimulationBarrage.optstimfile<<endl;
    cout<<"Stimulus folder <- "<<StimulationBarrage.stimlibloc<<endl;
    bool VisualStimulation_on = ui->radioVizStimOn->isChecked();
    cout<<"inter epoch times = "<<StimulationBarrage.inter_epoch_time<<endl;


    struct recorderthread_data RSC_input;

    RSC_input.bus = 0;
    RSC_input.guid = 0;
    RSC_input.cam = 0;
    RSC_input.proc_folder= tmp.toStdString();
    RSC_input.display="display";
    RSC_input.crop=true;
    RSC_input.recording_time = ui->spinBox_rectime->value() * 60; // Convert Duration to Seconds
    RSC_input.eventCount = 0; //FOr Triggered/ Conditional Recording

    bool run=true;

    if(!StimulationBarrage.Background_ON)
        StimulationBarrage.VisualStimulation(&RSC_input,run);
    else
        StimulationBarrage.VisualStimulation_BG(&RSC_input,run);

}

/// \brief Refresh Calculation of duration given user options in GUI
void MainWindow::on_btn_CalcDuration_clicked()
{
    updateBarrage();

}

void MainWindow::on_spinBox_framerate_valueChanged(int arg1)
{
    cout<< "Camera Frame rate is now " << arg1 << std::endl;
}

void MainWindow::on_spinBox_waiting_time_valueChanged(int arg1)
{

}




void MainWindow::on_btn_selectStimSetFile_clicked()
{
    QStringList stimSetFilename = QFileDialog::getOpenFileNames(nullptr, "Select stimulus-set config file to load", QApplication::applicationDirPath(),
                                                            "(*.txt *.stim)", nullptr, nullptr);
    this->ui->txt_stimSetFile->setText(stimSetFilename.first());
    StimulationBarrage.optstimfile = stimSetFilename.first().toStdString();

    QStringList stimList;
    std::vector<string> vStim = barrage::loadStimListFromFile(StimulationBarrage.optstimfile);
    this->ui->listStimSet->reset();
    for(int i=0;i<vStim.size();i++)
        this->ui->listStimSet->addItem(QString::fromStdString(vStim[i]));

}


void MainWindow::on_btn_selectfolder_clicked()
{
    QString stimSetFilename = QFileDialog::getExistingDirectory(nullptr, "Select output folder", QApplication::applicationDirPath(),
                                                            QFileDialog::ShowDirsOnly);

    stimSetFilename = stimSetFilename.fromStdString( CheckOutputFolder(stimSetFilename.toStdString()) );

    this->ui->txt_outFolder->setText(stimSetFilename);

}

