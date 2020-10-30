//Autore: Stefano Leggio 1195922
#include "chart.h"

Chart::Chart()
{
    num_channels = 16;
    amplitude_scale = 500;
    current_data.resize(TIME_WINDOW_MAX*POLLING_RATE);
}

Chart::Chart(int nch){
    num_channels = nch;
    amplitude_scale = 500;
    current_data.resize(TIME_WINDOW_MAX*POLLING_RATE);
}

/*
    Costruttore di copia
*/

Chart::Chart(const Chart& chart){
    num_channels = chart.num_channels;
    amplitude_scale = chart.amplitude_scale;
    current_data = chart.current_data;
}

/*
    Assegnamento di copia
*/

Chart& Chart::operator=(const Chart& chart){
    num_channels = chart.num_channels;
    amplitude_scale = chart.amplitude_scale;
    current_data = chart.current_data;
    return *this;
}

/*
    Costruttore di spostamento
*/

Chart::Chart(Chart&& chart){
    num_channels = chart.num_channels;
    amplitude_scale = chart.amplitude_scale;
    current_data = chart.current_data;
    chart.num_channels = 0;
    chart.amplitude_scale = 0;
    chart.current_data.clear();
}

/*
    Assegnamento di spostamento
*/

Chart& Chart::operator=(Chart&& chart){
    num_channels = chart.num_channels;
    amplitude_scale = chart.amplitude_scale;
    current_data = chart.current_data;
    chart.num_channels = 0;
    chart.amplitude_scale = 0;
    chart.current_data.clear();
    return *this;
}

void Chart::setAmpScale(double f){
    amplitude_scale = f;
}

double Chart::getAmpScale(){
    return amplitude_scale;
}

void Chart::addValue(std::vector<std::vector<double>> values){
    for(ulong i = 0; i < values.size(); i++){
        current_data[values_added % current_data.size()] = values[i];
        values_added+=1;
    }
    emit Chart::valueChanged();
}
