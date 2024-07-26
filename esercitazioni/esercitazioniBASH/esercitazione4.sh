#!/bin/bash

# Esercizio 01a:
# Creare una cartella nella propria home utente chiamandola myfiles
# Creare al suo interno i file file1, file2 e file3
# Creare un archivio non compresso della cartella contenente i 3 file
# Eliminare la cartella myfiles, quindi ripristinarla estraendo il contenuto dell'archivio 
# precedentemente creato

cd
mkdir myfiles
touch myfiles/file1 myfiles/file2 myfiles/file3
tar -cf archivio_1.tar myfiles
rm -r myfiles
tar -xf archivio_1.tar

# Esercizio 01b: 
# Ripetere quanto appena fatto, ma creando un archivio di tipo compresso

tar -czf archivio_2.tar.gz myfiles
rm -r myfiles
gunzip archivio_2.tar.gz
tar -xf archivio_2.tar

# Ridefinire il comando rm in modo tale che sia chiesta conferma prima della 
# cancellazione dei file
# Definire quindi il comando rmi (rm interattivo)

alias rmi='rm -i'

# Individuare nella directory corrente tutti i file che iniziano con let
# Se non esistono file, creare due file chiamati lettera.txt e lett e rieseguire il 
# comando precedente
# Che altro comando potevamo usare per trovare solo il file lett?
# Rinominare il file lett in lett.txt. Digitare un comando per individuare solo 
# lett.txt

touch lett lettera.txt
ls let*
mv lett lett.txt
ls le??.txt

# Creare la directory esercitazione-<data_odierna> nella vostra home
# Il valore <data_odierna> deve essere sostituito con la data odierna
# L'informazione della data non deve essere scritta manualmente, bensì ricavata dinamicamente

mkdir esercitazione-`date +%F`

# Definire una variabile VAR1 e assegnarle il valore “Il biglietto costa”
# Visualizzare il valore di VAR1 sul terminale
# Definire una variabile VAR2 e assegnarle il valore 50
# Visualizzare i valori delle due variabili VAR1 e VAR2 affiancati

VAR1="Il biglietto costa"
echo $VAR1
VAR2=50
echo $VAR1$VAR2