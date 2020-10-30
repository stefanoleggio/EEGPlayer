//Autore: Marco Martini 1189321
#include "eegplayer.h"
#include <QtCharts/QChartView>
#include "./ui_eegplayer.h"
#include <QtCore/QRandomGenerator>
#include <thread>
#include <sstream>
#include <QObject>


EEGPlayer::EEGPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EEGPlayer)
{
    ui->setupUi(this);
    //Fine Creazione
}

void EEGPlayer::hideCheckBox(){
    for(ulong i = 0; i < chk_v.size(); i++){
        if(chk_v[i]->checkState() == Qt::CheckState::Unchecked){
            l_chart->hideChannel(i);
        }
        else{
            l_chart->showChannel(i);
        }
        chartView->repaint();
    }
}

EEGPlayer::~EEGPlayer()
{
    delete ui;
    delete player;
    delete reader;
    delete chartView;
    for(ulong i = 0; i < chk_v.size(); i++){
        delete chk_v[i];
    }
    delete l_chart;
}



void EEGPlayer::load()
{
    QString  fileName = QFileDialog::getOpenFileName(this, tr("Open EEG"), "$HOME", tr("EEG (*.txt)"));
    reader = new Reader(fileName.toStdString());
    int n_channels = int(reader->getNChannels());
    l_chart = new LineChart(n_channels);
    ui->cmb_ampl->setCurrentIndex(5); //Set amp and time scales
    ui->cmb_time->setCurrentIndex(3);
    player = new Player(l_chart,reader);
    //Create the chartview and channels checkboxes
    chartView = new QChartView();
    chk_v.resize(ulong(n_channels));


    //Create the charts
    for(ulong i = 0; i < ulong(n_channels); i++){
        //Allocate checkboxes
        chk_v[i] = new QCheckBox();
        chk_v[i]->setObjectName(QString::fromStdString("chk"+std::to_string(i)));
        chk_v[i]->setText(QString::fromStdString("channel:"+std::to_string(i)));
        chk_v[i]->setCheckState(Qt::CheckState::Checked);
        //Connect event of checkboxes to its function
        QObject::connect(chk_v[i],&QCheckBox::stateChanged,this,&EEGPlayer::hideCheckBox);
        //Show them on screen
        ui->channels_layout->addWidget(chk_v[i]);
        //Set chartviews properties
        std::string title = "Channel: " + std::to_string(i);
        chartView->setChart(l_chart->getQChart());
        chartView->setRenderHint(QPainter::NonCosmeticDefaultPen);
        chartView->setCacheMode(QChartView::CacheBackground);
        chartView->setMinimumHeight(100*reader->getNChannels());
        chartView->setMaximumHeight(100*reader->getNChannels());
        chartView->setRubberBand(QChartView::HorizontalRubberBand);
        ui->chart_layout->addWidget(chartView);
    }
}


void EEGPlayer::on_cmb_time_currentIndexChanged(int index)
{
    int time;
    std::string ctext = ui->cmb_time->itemText(index).toStdString();
    std::stringstream ss(ctext);
    ss >> time;
    l_chart->setTimeWindow(time);
}

void EEGPlayer::on_cmb_ampl_currentIndexChanged(int index)
{
    double scale;
    std::string ctext = ui->cmb_ampl->currentText().toStdString();
    std::stringstream ss(ctext);
    ss>>scale;
    ss>>ctext;
    if(ctext.size() == 2 && ctext[0] == 'm'){
        scale*=1000;
    }
    l_chart->setAmpScale(scale);
}

void EEGPlayer::on_btn_startpause_pressed()
{
    if(ui->btn_startpause->text() == "Start"){
        load();
        player->start();
        readyforcopy = true;
        ui->btn_startpause->setText("Pause");
    }else if(ui->btn_startpause->text() == "Pause"){
        player->pause();
        ui->btn_startpause->setText("Resume");
    }else{
        player->start();
        ui->btn_startpause->setText("Pause");
    }


}
