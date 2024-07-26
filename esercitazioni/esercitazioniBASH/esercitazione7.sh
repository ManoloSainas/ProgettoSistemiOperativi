#!/bin/bash


# ● Creare uno script che esegua un backup della home directory dell'utente
# ○ Creando un file .tar.bz2 il cui nome che segua lo schema
# backup-<nomeutente>-<data>.tar.bz2
# ○ Per la data utilizzare il formato YYYY-MM-DD



2 cd
3 tar -cjf backup -`whoami` -`date +%F`.tar.bz2 *


###############################################################################



# ● Realizzare uno script che archivi e comprima (formato tar.gz) tutti i file modificati 
# nelle ultime 24 ore presenti nella home dell'utente che esegue lo script
# ○ Opzionalmente si preveda l'esclusione dei file temporanei
# ● Consiglio: si consulti il manuale dei comandi tar e find


2 tar -cvf archivio.tar `find ~ -mtime -1 -type f | grep -v '~$'`
3 gzip -f archivio.tar # gzip aggiunge in automatico l’estensione .gz


###############################################################################



# ● Creare uno script che prenda in input il nome di una cartella, e quindi:
# ○ Archivi (tar) e comprima (sia in bzip2 sia in gzip) la cartella fornita in input
# ■ Utilizzare le opzioni di compressione previste dal comando tar
# ○ Confronti al termine le dimensioni dei file ottenuti ed indichi all'utente quale tipo di 
# compressione sia più efficiente (minore dimensione)


# Compressione
tar -czf ${1}.tar.gz $1
tar -cjf ${1}.tar.bz2 $1
# Ottenimento dimensione archivi con wc ed isolamento info con cut
gzipSize=`wc -c $1.tar.gz | cut -d " " -f1`
bzip2Size=`wc -c $1.tar.bz2 | cut -d " " -f1`
# Confronto dimensioni e stampa output
if [ $gzipSize -lt $bzip2Size ]; then
        echo "gzip è più efficiente"
elif [ $gzipSize -gt $bzip2Size ]; then
        echo "bzip2 è più efficiente"
else
        echo "i due metodi di compressione sono equivalenti"
fi


###############################################################################



# ● Scrivere uno script che accetti in input due parametri numerici e stampi “grande” se la 
# somma di questi è maggiore di 10, altrimenti stampi “piccolo”
# ● Includere un sistema di controllo che restituisca un messaggio di errore nei casi in cui:
# ○ Il primo e il secondo parametro non esistono
# ○ È stato specificato un terzo parametro


ERR_ARGUMENTS=65
n=$#
if [ $n -gt 2 ]; then # Controlla che siano passati solo due argomenti
    echo "Errore: inserisci solo due argomenti"
    exit $ERR_ARGUMENTS
fi
if [ -z $2 ]; then # Controlla che i due argomenti non siano nulli
    echo "Errore: inserisci due argomenti"
    exit $ERR_ARGUMENTS
fi
if [ $(($1+$2)) -gt 10 ]; then # Stampa output
    echo grande
else
    echo piccolo
fi
exit 0


###############################################################################



# ● Scrivere uno script che trasformi tutte le lettere di un file da minuscole in maiuscole
# ○ Suggerimento: usare il comando tr
# ● Includere un sistema di controllo che restituisca un messaggio di errore nei casi in cui:
# ○ Il nome non corrisponde ad un file esistente


E_ERR_ARG=65
if [ ! -e $1 ]; then
    echo Errore: Nessun file specificato
    echo Utilizzo: `basename $0` nomefile
    exit $E_ERR_ARG
fi
tr a-z A-Z < $1
exit 0


###############################################################################



# ● Scrivere uno script che svolga un conto alla rovescia
# ○ Lo script deve chiedere all’utente il numero di secondi da cui partire
# ■ Prestare attenzione ai casi in cui inserisce un numero di secondi iniziali negativo
# ○ Ogni secondo viene stampato il numero di secondi restanti
# ○ Al termine del conteggio stampi “Ring Ring” e produca un alert sonoro (BEL)
# ● Consiglio: si consulti il manuale dei comandi sleep e echo



echo "Settare i secondi del timer:"
read secs
while ((secs > 0)); do
    echo "Mancano $secs secondi"
    ((--secs))
    sleep 1
done
echo -e Ring Ring\\a


###############################################################################



# ● Scrivere uno script che implementi il number guessing game
# ○ Generare un numero casuale tra 1 e 20
# ○ Chiedere all’utente un numero finché non indovina il numero o finisce i tentativi
# ■ Ad ogni tentativo errato fornire una indicazione, ovvero dire se il numero appena 
# inserito è più grande o piccolo di quello che si sta cercando di indovinare
# ■ L’utente ha massimo 4 tentativi
# ● Consiglio: la variabile RANDOM contiene un numero casuale tra 0 e 32767


let n=$RANDOM%20+1
echo "Numero casuale tra 1 e 20 generato: prova a indovinarlo in 4 tentativi!"
g=0; c=1
until [ $c -eq 5 ]; do
    echo "Tentativo $c, fai la tua scommessa"
    read g
    if [ $g -eq $n ]; then
        echo "Sei fortunato, il numero era proprio $n"
        exit 0
    elif [ $g -lt $n ]; then
        echo "Hai puntato basso"
    else
        echo "Voli alto bello"
    fi
    c=`expr $c + 1`
done
echo "Hai finito i tentativi, forse andrà meglio la prossima volta"
exit 1