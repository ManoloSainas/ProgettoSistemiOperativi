#!/bin/bash

# Creare all’interno della propria home directory una cartella denominata doctest
# Muoversi all'interno della cartella doctest e creare un file denominato 
# newfile.txt
# Visualizzare le proprietà di newfile.txt con il comando ls -l
# Eseguire touch newfile.txt
# Descrivere l'effetto di questo comando sul file newfile.txt

mkdir ~/doctest
cd ~/doctest
touch newfile.txt
ls -l
touch newfile.txt
ls -l

# Visualizzare i file della propria home directory ordinati in base alla dimensione
# Visualizzare i file nascosti della propria home 
# Cosa hanno di particolare questi file?
# Suggerimento:
# Utilizzare man ls per trovare le opzioni richieste

ls -lS
ls -l --sort size
ls -a
ls -all

# Creare nella home directory una cartella libri
# Dentro la cartella libri, creare due file: libro1 e libro2
# Revocare i permessi di lettura alla cartella libri
# Che tipo di operazioni è possibile eseguire in tale directory? 
# Quali sono le operazioni negate?
# Revocare i permessi di esecuzione  alla cartella libri
# Che tipo di operazioni è possibile eseguire in tale directory? 
# Quali sono le operazioni negate?

mkdir libri
touch libri/file1 libri/file2
chmod -r libri
chmod -x libri

# Qual è l’effetto prodotto dalla seguente sequenza di comandi?
# Identificare il risultato di ogni azione e il motivo per cui si ottiene il risultato

cd
mkdir d1
chmod 444 d1
cd d1