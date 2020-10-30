//Autore: Marco Martini 1189321
#ifndef EEGPLAYER_H
#define EEGPLAYER_H

#include <QMainWindow>
#include <chart.h>
#include <QtCharts>
#include <QCheckBox>
#include "linechart.h"
#include "Player.h"
#include "Reader.h"

namespace Ui{
    class EEGPlayer;
}

class EEGPlayer : public QMainWindow
{
    Q_OBJECT

public:
    EEGPlayer(QWidget *parent = nullptr);
    EEGPlayer(const EEGPlayer& p);
    EEGPlayer(EEGPlayer&& p);
    EEGPlayer& operator=(const EEGPlayer& p);
    EEGPlayer& operator=(EEGPlayer&& p);
    ~EEGPlayer();


private slots:
    void hideCheckBox();
    void on_cmb_time_currentIndexChanged(int index);
    void on_cmb_ampl_currentIndexChanged(int index);
    void on_btn_startpause_pressed();

private:
    void load();
    Player *player;
    Reader *reader;
    LineChart *l_chart;
    Ui::EEGPlayer *ui;
    QChartView* chartView; //ChartView
    std::vector<QCheckBox*> chk_v; //Checkboxes
    bool readyforcopy = false; //Determines wether or not we can copy the object
};

#endif // EEGPLAYER_H
