//Marco Martini 1189321
#include "linechart.h"
#include <thread>

//(shallow) copy constructor
LineChart::LineChart(const LineChart& lc):points{lc.points},  time_window{lc.time_window}, values_displayed{lc.values_displayed}, ch_series{lc.ch_series}, axisX{lc.axisX}, axisY{lc.axisY}, channelAxis{lc.channelAxis}, qchart{lc.qchart}{
    current_data = lc.current_data;
    num_channels = lc.num_channels;
    values_added = lc.values_added;
}

//move constructor
LineChart::LineChart(LineChart&& lc): points{lc.points}, time_window{lc.time_window}, values_displayed{lc.values_displayed},
ch_series{lc.ch_series}, axisX{lc.axisX}, axisY{lc.axisY},   channelAxis{lc.channelAxis}, qchart{lc.qchart}{
    current_data = lc.current_data;
    num_channels = lc.num_channels;
    values_added = lc.values_added;
    lc.time_window = 0;
    lc.values_displayed = 0;
    lc.ch_series.clear();
    lc.points.clear();
    lc.axisX = nullptr;
    lc.axisY = nullptr;
    lc.channelAxis = nullptr;
    lc.qchart = nullptr;
    lc.current_data.clear();
}


//constructor by channels
LineChart::LineChart(int nch){
    num_channels = nch;
    points.resize(num_channels+1); //Ghost series to append the vertical cursor
    time_window = 1;
    values_displayed = 0;
    amplitude_scale = 50;
    connect(this,&Chart::valueChanged,this,&LineChart::handleSignal);
    ch_series.resize(ulong(num_channels+1)); //Ghost series
    qchart = new QChart();
    axisX = new QValueAxis();
    axisY = new QValueAxis();
    channelAxis = new QCategoryAxis();


    setupChannels();
}

//default constructor
LineChart::LineChart(){
    num_channels = 16;
    points.resize(num_channels+1); //Ghost series
    time_window = 1;
    values_displayed = 0;
    amplitude_scale = 50;
    connect(this,&Chart::valueChanged,this,&LineChart::handleSignal);
    ch_series.resize(ulong(num_channels+1)); //Ghost series
    qchart = new QChart();
    axisX = new QValueAxis();
    axisY = new QValueAxis();
    channelAxis = new QCategoryAxis();

    setupChannels();
}



/* these gets deleted automatically by QGraphicsScene::clear() because it's a QOBJECT
LineChart::~LineChart(){
    for(ulong i=0; i<ch_series.size();i++){
        delete ch_series[i];
    }
    delete axisY;
    delete axisX;
    delete channelAxis;

    delete qchart;
}
*/

//(shallow) copy assignment
LineChart& LineChart::operator=(const LineChart& lc){
    //Shallow copy
    /* gets deleted automatically by being a QObject
    delete axisX;
    delete axisY;
    delete channelAxis;
    delete qchart;
    for(ulong i = 0; i < ch_series.size(); i++){
        delete ch_series[i];
    }
    */
    current_data = lc.current_data;
    num_channels = lc.num_channels;
    values_added = lc.values_added;
    axisX = lc.axisX;
    axisY = lc.axisY;
    channelAxis = lc.channelAxis;
    qchart = lc.qchart;
    time_window = lc.time_window;
    values_displayed = lc.values_displayed;
    ch_series = lc.ch_series;
    points = lc.points;
    return *this;
}

//move assignment
LineChart& LineChart::operator=(LineChart&& lc){
    /*Same as above
    delete axisX;
    delete axisY;
    delete channelAxis;
    delete qchart;
    for(ulong i = 0; i < ch_series.size(); i++){
        delete ch_series[i];
    }
    */
    current_data = lc.current_data;
    num_channels = lc.num_channels;
    values_added = lc.values_added;
    axisX = lc.axisX;
    lc.axisX = nullptr;
    axisY = lc.axisY;
    lc.axisY = nullptr;
    channelAxis = lc.channelAxis;
    lc.channelAxis = nullptr;
    qchart = lc.qchart;
    lc.qchart = nullptr;
    time_window = lc.time_window;
    lc.time_window = 0;
    values_displayed = lc.values_displayed;
    lc.values_displayed = 0;
    ch_series = lc.ch_series;
    lc.ch_series.clear();
    points = lc.points;
    lc.points.clear();
    current_data.clear();
    return *this;
}

void LineChart::setAmpScale(double scale){
    amplitude_scale = scale;
    axisY->setRange(0,2*amplitude_scale*num_channels);
    for(int j = 0; j < num_channels; j++){
        points[j].clear();
    }
    values_displayed = 0;
    refreshChart();
}

void LineChart::hideChannel(int ch){
   ch_series[ulong(ch)]->setVisible(false);
}

void LineChart::showChannel(int ch){
   ch_series[ulong(ch)]->setVisible(true);
}



void LineChart::handleSignal(){
    refreshChart();
}

void LineChart::setTimeWindow(int t){
    time_window = t;
    for(int i = 0; i < num_channels; i++){
        points[i].clear();
    }
    axisX->setRange(0,t);
    values_displayed = 0;
    refreshChart();
}


void LineChart::refreshChart(){
    ulong linestoRead = (values_added - values_displayed);
    double x = 0;
    if(!points[num_channels].isEmpty()){ //Remove last cursor
        points[num_channels].pop_back();
        points[num_channels].pop_back();
    }

    //Populate vector based on the series of points that will be used to redraw the chart all at once instead of once every item added (which is the default behaviour)
    for(ulong i = 0; i < linestoRead; i++){
        for(ulong j = 0; j < ulong(num_channels); j++){
            x = (1.0 / 512.0) * (values_displayed % (512*ulong(time_window)));
            double height = 2*j*amplitude_scale + amplitude_scale;
            double y = 0;
            if(!(j >= num_channels -1 && num_channels - current_data[j].size() > 0)) //If the buffer has less elements than the number of channels than set the problematic row to 0
                y = current_data[values_displayed % current_data.size()][j]; //Raw value
            (y <= (-1.0)*amplitude_scale ? y = (-1.0)*amplitude_scale : y);
            (y >= amplitude_scale ? y = amplitude_scale : y);
            y += height;


            QPointF point(x,y); //value read + channel offset
            if(values_displayed < 512*ulong(time_window)){
                points[j].push_back(point);
            }
            else{
                points[j].replace(values_displayed % (512*time_window),point);
            }
        }
        values_displayed++;
    }
    points[num_channels].push_back(QPointF(x,axisY->max()));//Update the cursor
    points[num_channels].push_back(QPointF(x,0));

    for(int i = 0; i < num_channels+1; i++){
        ch_series[i]->replace(points[i]); //This eats away my points vector
    }
}


void LineChart::setupChannels(){
    //Configure axis
    axisX->setRange(0,time_window);
    axisY->setRange(0,2.0*amplitude_scale*(num_channels));
    channelAxis->setRange(0,2.0*amplitude_scale*(num_channels));
    channelAxis->setLinePenColor(Qt::GlobalColor(Qt::black));
    channelAxis->setGridLinePen(QPen(Qt::GlobalColor(Qt::black)));

    //Configure qchart
    qchart->addAxis(channelAxis,Qt::AlignLeft);
    qchart->addAxis(axisX,Qt::AlignBottom);
    qchart->addAxis(axisY,Qt::AlignLeft);
    qchart->legend()->setVisible(false);
    qchart->setMargins(QMargins(0,0,0,0));
    qchart->setBackgroundRoundness(0);
    qchart->setAnimationOptions(QChart::NoAnimation);
    qchart->setAnimationEasingCurve(QEasingCurve::Linear);
    axisY->setVisible(false); //We do not need to show this

    for(ulong j = 0; j <= ulong(num_channels); j++){ //Do it num_channel+1 times because of the ghost channel
        //Allocation
        ch_series[j] = new QLineSeries();

        //Configuration
        Qt::GlobalColor colour(Qt::GlobalColor(j%10+5));
        QPen pen(colour);
        pen.setWidth(2);
        ch_series[j]->setPen(pen);
        qchart->addSeries(ch_series[j]);
        ch_series[j]->attachAxis(axisY);
        ch_series[j]->attachAxis(axisX);
        ch_series[j]->attachAxis(channelAxis);
        ch_series[j]->setUseOpenGL(true);

        //Configure lines for every channel
        channelAxis->append(QString::fromStdString("eeg:" + std::to_string(j) + "-"),2.0*double(j)*amplitude_scale+(amplitude_scale));
        channelAxis->append(QString::fromStdString("eeg:" + std::to_string(j) + "+"),2.0*double(j)*amplitude_scale+(2.0*amplitude_scale));
        ch_series[j]->detachAxis(channelAxis); //Detach the axis immediately so that it doesn't get updated when the Y axis gets updated
    }
    ch_series[ulong(num_channels)]->setPen(QPen(Qt::GlobalColor(Qt::black))); //Change colour to the ghost channel pen
}

void LineChart::setOpenGL(bool active){
    if(active){
        for(ulong j = 0; j <= ulong(num_channels); j++){
            ch_series[j]->setUseOpenGL(true);
        }
    }else{
        for(ulong j = 0; j <= ulong(num_channels); j++){
            ch_series[j]->setUseOpenGL(false);
        }

    }
}

QChart* LineChart::getQChart() const{
    return qchart;
}

