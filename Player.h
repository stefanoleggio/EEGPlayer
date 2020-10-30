//Autore: Stefano Leggio 1195922
#ifndef PLAYER_H
#define PLAYER_H
#include "chart.h"
#include "Reader.h"
#include "QObject"
#include <QTimer>

#define BLOCKS_PER_PERIOD 16
#define FREQUENCY 31 //Tronchiamo il 31.25

class Player : public QObject{ //Estende QObject per usare il timer
    public:
        ~Player();
        Player(Chart *ch, Reader *rd);
        Player(const Player& player);
        Player& operator=(const Player& player);
        Player(Player&& player);
        Player& operator=(Player&& player);
        void pause();
        void start();

    private slots:
        void loop();
        void request_data();


    private:
        QTimer timer;
        Chart *chart;
        Reader *reader;
        int row; // counter della riga corrente
        int counter;
        std::vector <std::vector <double>> buffer; // array di float contenente i valori della riga row di ogni canale
        std::vector <std::vector <float>> matrix;
};

#endif
