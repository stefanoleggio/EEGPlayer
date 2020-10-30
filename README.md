# EEGPLAYER Ghidoni

## Assegnamento dei compiti

+ **Manuel Barusco**: Input e struct Data
+ **Stefano Leggio**: Lettore
+ **Marco Martini**: Interfaccia grafica

## Input

Creazione oggetto `DataSet` (buffer)

+ Leggere da file (Gestire vari stream di input e chiuderli)
+ Controlli formato del file
+ Il DataSet contiene solo una parte dei dati (*e.g 2048 dati*) 
+ Deve avere una funzione che fa l'output di un `array` di `float` dove ogni cella contiene il valore di **1** canale

## Lettore

Creazione oggetto `Player`

+ Pausa
+ Start
+ Funzione scaricamento da oggetto Reader
+ Funzione caricamento a oggetto Chart

Creazione struct `Data`

+ Colore
+ Numero canale
+ Valore assoluto
