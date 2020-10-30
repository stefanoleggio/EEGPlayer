//Autore: Stefano Leggio 1195922
#include "Player.h"
#include <vector>
#include<iostream>
using namespace std;

Player::Player(Chart *ch, Reader *rd):reader{rd},chart{ch}{
    matrix = reader->readAndSend();
    row = 0;
    counter = 0;
    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(FREQUENCY);
    QObject::connect(&timer,&QTimer::timeout,this,&Player::loop);
};

/*
    Costruttore di copia
*/

Player::Player(const Player& player){
    chart = player.chart;
    reader = player.reader;
    row = player.row;
    buffer = player.buffer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(FREQUENCY);
    QObject::connect(&timer,&QTimer::timeout,this,&Player::loop);
}

/*
    Assegnamento di copia
*/

Player& Player::operator=(const Player& player){
    chart = player.chart;
    reader = player.reader;
    row = player.row;
    buffer = player.buffer;
    QObject::disconnect(&timer);
    QObject::connect(&timer,&QTimer::timeout,this,&Player::loop);
    return *this;
}

/*
    Costruttore di spostamento
*/

Player::Player(Player&& player){
    chart = player.chart;
    reader = player.reader;
    row = player.row;
    player.row = 0;
    player.buffer.clear();
    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(FREQUENCY);
    QObject::disconnect(&player.timer);
    QObject::connect(&timer,&QTimer::timeout,this,&Player::loop);
}

/*
    Assegnamento di spostamento
*/

Player& Player::operator=(Player&& player){
    chart = player.chart;
    reader = player.reader;
    row = player.row;
    player.row = 0;
    player.buffer.clear();
    player.chart = nullptr;
    player.reader = nullptr;
    return *this;
    QObject::disconnect(&player.timer);
    QObject::disconnect(&timer);
    QObject::connect(&timer,&QTimer::timeout,this,&Player::loop);
}

/*
    Loop per invio dati all'interfaccia grafica
*/

void Player::loop(){
    if(counter > 123){ //Dato che si è troncato da 31.25ms a 31 ogni 125 cicli ne abbiamo fatto uno di troppo. quindi non lo so fa
        counter = 0;
    }else {
        for(int i = 0; matrix.size() && i < BLOCKS_PER_PERIOD % matrix.size(); ++i){
            if(row >= matrix.size()){
                row = 0;
                request_data();
            }
            if(matrix.size()){
                buffer.push_back(vector<double>());
                for(int j = 0;j < matrix[row].size(); ++j){
                   buffer[i].push_back(double(matrix[row][j]));
                }
                row+=1;
            }
        }
    }
    if(matrix.size() || buffer.size()){ //Se effettivamente c'è qualcosa da aggiungere lo aggiungo
        chart->addValue(buffer);
        counter++;
    }
    else{
        request_data(); //Continua a richiedere dati anche se *sembrava* non ce ne fossero più
    }
    buffer.clear();
}

/*
    Funzione per aggiornare la matrice
*/

void Player::request_data(){
    matrix = reader->readAndSend();
};

void Player::pause(){
    timer.stop();
};

void Player::start(){
    timer.start();
};

/*
    Distruttore
*/

Player::~Player(){
    buffer.clear();
    matrix.clear();
}
