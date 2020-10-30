//Autore: Marco Martini 1189321
#ifndef LINECHART_H
#define LINECHART_H
#include "chart.h"
#include <QtCharts>
#include <QTimer>

class LineChart : public Chart
{

public:
    LineChart();
    LineChart(int); //Construct by channels
    LineChart(const LineChart&);
    LineChart& operator=(const LineChart&);
    LineChart(LineChart&&);
    LineChart& operator=(LineChart&&);
    QChart*  getQChart() const;
    void setTimeWindow(int);
    void setAmpScale(double) override;
    void refreshChart();
    void hideChannel(int) override;
    void showChannel(int) override;
    void setOpenGL(bool);
//    ~LineChart() override;

private slots:
    void handleSignal(); //Through signals we update the charts in sequence

private:
    void setupChannels();
    QVector<QVector<QPointF>> points;
    int time_window;
    ulong values_displayed; //Values displayed at the moment
    std::vector<QXYSeries*> ch_series;
    QValueAxis *axisX; //axis X
    QValueAxis *axisY; //axis Y
    QCategoryAxis *channelAxis; //list channels axis + phantom channel
    QChart *qchart; //pointer to qchart
};

#endif // LINECHART_H
