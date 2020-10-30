//Autore: Manuel Barusco 1189726

#include "eegplayer.h"

EEGPlayer::EEGPlayer(const EEGPlayer& p){
    readyforcopy = false;
    if(p.readyforcopy){
        player=new Player(*(p.player));
        reader=new Reader(*(p.reader));
        l_chart=new LineChart(*(p.l_chart));
        chartView=p.chartView;
        chk_v=p.chk_v;
        readyforcopy = true;
    }
    ui=p.ui;
}

EEGPlayer& EEGPlayer::operator=(const EEGPlayer& p){
    readyforcopy = false;
    if(p.readyforcopy){
        Player* pl=new Player(*(p.player));
        delete player;
        player=pl;
        Reader* r=new Reader(*(p.reader));
        delete reader;
        reader=r;
        LineChart* l=new LineChart(*(p.l_chart));
        delete l_chart;
        l_chart=l;
               QChartView* cv=p.chartView;
        delete chartView;
        chartView=cv;
        for(int i=0;i<chk_v.size();i++)
            delete chk_v[i];
        chk_v.clear();
        chk_v=p.chk_v;
        readyforcopy = true;
    }
    Ui::EEGPlayer* u=p.ui;
    delete ui;
    ui=u;

    return *this;
}

EEGPlayer::EEGPlayer(EEGPlayer&& p){
    readyforcopy = false;
    if(p.readyforcopy){
        player=p.player;
        reader=p.reader;
        l_chart=p.l_chart;
        chartView=p.chartView;
        chk_v=p.chk_v;
        p.player=nullptr;
        p.reader=nullptr;
        p.l_chart=nullptr;
        p.chartView=nullptr;
        p.chk_v.clear();
        readyforcopy = true;
    }
    ui=p.ui;
    p.ui=nullptr;
}

EEGPlayer& EEGPlayer::operator=(EEGPlayer&& p){
    readyforcopy = false;
    if(p.readyforcopy){
        delete player;
        delete reader;
        delete l_chart;
        delete chartView;
        for(int i=0;i<chk_v.size();i++)
            delete chk_v[i];
        chk_v.clear();
        player=p.player;
        reader=p.reader;
        l_chart=p.l_chart;
        chartView=p.chartView;
        chk_v=p.chk_v;
        p.player=nullptr;
        p.reader=nullptr;
        p.l_chart=nullptr;
        p.chartView=nullptr;
        p.chk_v.clear();
        readyforcopy = true;
    }
    delete ui;
    ui=p.ui;
    p.ui=nullptr;
    return *this;
}
