//Reader.h

//Autore: Manuel Barusco 1189726

#ifndef READER_H
#define READER_H
#include <fstream>
#include <vector>

class Reader{
    private:
        std::string fileName;
        int nChannels;
        std::ifstream inputFile;
        unsigned long position;
        std::vector<std::vector<float>> currentData;
        std::vector<std::vector<float>> nextData;


        //funzione che controlla se il file si apre, ha la giusta estensione e non è vuoto
        void checkFileAndOpen(std::string fName);
        //funzione che conta il numero di colonne (canali) presenti nel file (viene presa per buona la prima riga del file)
        void countChannels();
        //funzione che riempie il vettore di dati correnti
        void fillCurrentData();
        //funzione che riempie il vettore dei dati futuri
        void fillNextData();
    public:
        //classe per gestire la segnalazione di file invalidi (estensione sbagliata o non si aprono)
        class InvalidFile{
            public:
                InvalidFile();
        };

        //classe per gestire la segnalazione di file vuoti
        class EmptyFile{
            public:
                EmptyFile();
        };

        /* costruttore
        @param fileName nome del file da cui leggere i dati
        */
        Reader(std::string fileName);

        /* costruttore di copia
        @param r oggetto di classe Reader
        */
        Reader(const Reader& r);

        /* costruttore di spostamento
        @param r oggetto di classe Reader
        */
        Reader(Reader&& r);

        /*operatore assegnamento di copia
        @param r oggetto di classe Reader
        */
		Reader& operator=(const Reader& r);

		/*operatore assegnamento di spostamento
        @param r oggetto di classe Reader
        */
		Reader& operator=(Reader&& r);

        /*
        funzione che legge il valore dei canali a blocchi di 512 righe
        @return vettore di vettori di float che contiene gli elementi letti
        */
        std::vector<std::vector<float>> readAndSend();

        /*
        funzione che fa ricominciare la lettura del file
        */
        void rewind();

        /*
        @return nChannels numero di canali rilevati
        */
        int getNChannels();

        /* distruttore, chiude lo stream e distrugge il buffer*/
        ~Reader();
};   //Reader

#endif
