//Reader.cpp

//Autore: Manuel Barusco 1189726

#include "Reader.h"
#include <sstream>
#include <iostream>
using namespace std;

Reader::InvalidFile::InvalidFile(){
    cerr<< "Il file selezionato non è valido, controllarne l'estensione o l'integrita'";
} //InvalidFile

Reader::EmptyFile::EmptyFile(){
    cerr<< "Il file selezionato è vuoto";
} //EmptyFile

void Reader::checkFileAndOpen(string fName){
    //controllo l'estensione del file
    if(fName.find(".txt") == string::npos)
        throw InvalidFile();
    inputFile= ifstream(fName);
    //controllo se il file si apre
    if(!inputFile.is_open()){
        inputFile.close();
        throw InvalidFile();
    } //if
    //controllo se il file è vuoto
    string line;
    getline(inputFile,line);
    if(line.empty()){
        inputFile.close();
        throw EmptyFile();
    } //if
    fileName=fName;
} //checkFileAndOpen

void Reader::countChannels(){
    string line;
    getline(inputFile,line);
    stringstream ss(line);
    double x;
    while(ss>>x){
        nChannels++;
    } //while
} //countChannels

Reader::Reader(string fName){
    checkFileAndOpen(fName);
    nChannels=0;
    position=0;
    countChannels();
    //punto all'inizio del file
    inputFile.clear();
    inputFile.seekg(0);
    inputFile.close();
} //Reader

Reader::Reader(const Reader& r){
    fileName=r.fileName;
    nChannels=r.nChannels;
    inputFile= ifstream(fileName);
    position=r.position;
    currentData=r.currentData;
    nextData=r.nextData;
}//reader

Reader::Reader(Reader&& r){
    fileName=r.fileName;
    nChannels=r.nChannels;
    inputFile= ifstream(fileName);
    position=r.position;
    currentData=r.currentData;
    nextData=r.nextData;
    r.fileName="";
    r.nChannels=0;
    r.inputFile.close();
    r.position=0;
    r.currentData.clear();
    r.nextData.clear();
} //Reader

Reader& Reader::operator=(const Reader& r){
    fileName=r.fileName;
    nChannels=r.nChannels;
    inputFile.close();
    inputFile= ifstream(fileName);
    position=r.position;
    currentData=r.currentData;
    nextData=r.nextData;
    return *this;
}//operator=

Reader& Reader::operator=(Reader&& r){
    fileName=r.fileName;
    nChannels=r.nChannels;
    inputFile.close();
    inputFile= ifstream(fileName);
    position=r.position;
    currentData=r.currentData;
    nextData=r.nextData;
    r.fileName="";
    r.nChannels=0;
    r.inputFile.close();
    r.position=0;
    r.currentData.clear();
    r.nextData.clear();
    return *this;
}//operator=

void Reader::fillCurrentData(){
    string line;
    for(int i=0;(i<1024 && getline(inputFile, line));i++){
        stringstream ss(line);
        vector<float> v;
        float x;
        int j=0;
        while(ss>>x){
            v.push_back(x);
            j++;
        } //for
        /*controllo se la riga attuale ha un numero di canali minore o maggiore di nChannels
        in caso lancio InvalidFile perchè il file non è totalmente integro*/
        if(j>nChannels || j<nChannels)
            throw InvalidFile();
        currentData.push_back(v);
    } //for
} //fillCurrentData

void Reader::fillNextData(){
    string line;
    for(int i=0;(i<1024 && getline(inputFile, line));i++){
        stringstream ss(line);
        vector<float> v;
        float x;
        int j=0;
        while(ss>>x){
            v.push_back(x);
            j++;
        } //for
        /*controllo se la riga attuale ha un numero di canali minore o maggiore di nChannels
        in caso lancio InvalidFile perchè il file non è totalmente integro*/
        if(j>nChannels || j<nChannels)
            throw InvalidFile();
        nextData.push_back(v);
    } //for
} //fillNextData

vector<vector<float>> Reader::readAndSend(){
    inputFile=ifstream(fileName);
    inputFile.seekg(position,inputFile.beg);
    if(!currentData.empty()){
        currentData.swap(nextData);
        nextData.clear();
        fillNextData();
    } //if
    if(currentData.empty() && nextData.empty()){
        fillCurrentData();
        fillNextData();
    } //if
    position=inputFile.tellg();
    inputFile.close();
    return currentData;
} //readAndSend

void Reader::rewind(){
    position=0;
} //rewind

int Reader::getNChannels(){
    return nChannels;
} //getNChannels

Reader::~Reader(){
    inputFile.close();
    currentData.clear();
    nChannels=0;
    position=0;
    fileName="";
} //~Reader
