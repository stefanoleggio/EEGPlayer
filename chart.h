//Autore: Stefano Leggio 1195922
#ifndef CHART_H
#define CHART_H

#include <vector>
#include <QObject>

#define TIME_WINDOW_MAX 10
#define POLLING_RATE 512


class Chart : public QObject
{
    Q_OBJECT
public slots:
    //Updates every ~30ms giving it a refresh rate of 30fps, which is enough for every type of view
    void addValue(std::vector<std::vector<double>>); //Adds a value for each channel (determined by the cell in the vector)

signals:
    void valueChanged();

public:
    Chart(); //Default Constructor
    Chart(int); //By channels
    Chart(const Chart& chart);
    Chart& operator=(const Chart& chart);
    Chart(Chart&& chart);
    Chart& operator=(Chart&& chart);
    virtual void setAmpScale(double);
    virtual void hideChannel(int) = 0;
    virtual void showChannel(int) = 0;
    double getAmpScale();

protected:
    std::vector<std::vector<double>> current_data; //Contains a 10seconds long buffer
    int num_channels;
    ulong values_added = 0;
    double amplitude_scale;
};

#endif // CHART_H
