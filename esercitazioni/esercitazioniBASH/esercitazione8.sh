#!/bin/bash


# ● Scrivere uno script che svolga il seguente compito:
# ○ Acquisisca un indirizzo internet come parametro in input e scarichi il sorgente della 
# pagina corrispondente (HTML o altro)
# ○ Non visualizzi a schermo eventuali warnings o errori
# ○ Consenta la visualizzazione del file sorgente, a prescindere dalla sua lunghezza
# ● Consiglio: consultare il manuale del comando wget


wget -O - $1 2>/dev/null | less


###############################################################################



# ● Scrivere uno script che svolga il seguente compito:
# ○ Acquisita una lettera dell’alfabeto corrispondente ad una delle opzioni del comando 
# uname visualizzi le relative informazioni sul sistema spiegando cosa si sta mostrando
# ■ Non c’è bisogno di includere esattamente tutte le opzioni!
# ○ Se la lettera non corrisponde a una delle opzioni brevi del comando uname visualizzi 
# un messaggio di avvertimento per l’utente


echo "Digita una delle seguenti lettere 'a', 's', 'v', 'm', 'o'"
read var
res=`uname -$var 2> /dev/null` # Scarta i messaggi di errore di opzioni invalide
case "$var" in
    "s" ) echo "Il nome del kernel utilizzato è: $res" ;;
    "v" ) echo "La versione del kernel utilizzata è $res" ;;
    "o" ) echo "Il sistema operativo utilizzato è $res" ;;
    "m" ) echo "L'hardware utilizzato è: $res" ;;
    "a" ) echo "Le caratteristiche generali del sistema sono: $res" ;;
    * ) echo "Hai digitato una lettera non valida!" ;;
esac


###############################################################################



# ● Dato il file rubrica.txt le cui righe hanno la seguente struttura:
# <ruolo> <nome> <cognome> <e-mail>
# ● Scrivere uno script che svolga il seguente compito:
# ○ Stampi a video un riepilogo che mostri il numero di persone raggruppate per ruolo nel 
# formato <conteggio occorrenze> <ruolo>
# ○ Stampi a video tutte le persone che non hanno come ruolo Ricercatore
# ○ Inserisca in un file italia.txt tutte le righe il cui indirizzo e-mail termina con .it
# ● Consiglio: si può risolvere senza iterazione!


echo "1) Riepilogo contatti per ruolo:"
cut -d' ' -f1 rubrica.txt | sort | uniq -c
echo ""
echo "2) Contatti diversi da 'Ricercatore':"
grep -v "Ricercatore" rubrica.txt
echo ""
echo "3) Indirizzi italiani:"
grep "\.it$" rubrica.txt | tee italia.txt


###############################################################################



# ● Dato il precedente file rubrica.txt le cui righe hanno la seguente struttura:
# <ruolo> <nome> <cognome> <e-mail>
# ● Scrivere uno script che svolga il seguente compito:
# ○ Modifichi le occorrenze di Raffaella in Antonella, e salvi la versione modificata in 
# rubrica_due.txt
# ○ Escluda le prime tre righe di rubrica_due.txt e salvi tutto in rubrica_tre.txt
# ○ Si utilizzi il comando sed per entrambe le due operazioni precedenti!
# ● Si consiglia in aggiunta a man sed dell’altra documentazione relativa a sed

# Modifica tutte le occorrenze di 'Raffaella' in 'Antonella' 
sed s/Raffaella/Antonella/ rubrica.txt > rubrica_due.txt
# Cancella le prime tre righe
sed '1,3d' rubrica_due.txt > rubrica_tre.txt



###############################################################################



# ● Scrivere uno script che svolge il seguente compito:
# ○ Acquisisca da tastiera un nome e controlli che questo corrisponda ad un file esistente
# ○ Se il file non esiste, viene aperto un file con gedit avente tale nome
# ○ Se il file esiste e ha dimensione maggiore di zero byte visualizzi il numero di linee, 
# parole e bytes, e imposti i permessi a lettura ed esecuzione per tutti (u-g-o)
# ○ Altrimenti, se il file esiste ma ha dimensione pari a zero byte, lo rimuova

echo "Inserisci il nome di un file"
read name
if test ! -e $name; then # se non esiste...
    gedit $name
elif test -s $name; then # se ha dimensione superiore a zero
    chmod 555 $name # attribuisci permessi r e x 
    info=` wc f2 | tr -s ' ' | sed s/'^ '/''/ | cut -d' ' -f1-3` # isola info
    echo "Il numero di linee, parole e byte del file è: $info" # stampa info
else # se ha dimensione minore uguale 0
    rm $name
fi


###############################################################################


# ● Scrivere in forma di script un comando di rimozione file nominato cancella.sh che 
# svolga i seguenti compiti:
# ○ Accetti più nomi di file/cartelle passati come argomenti da riga di comando
# ■ E restituisca un errore in caso di assenza di argomenti passati
# ○ Archivi e comprima ogni file con gzip (se non lo è già), rimuovendo l’originale
# ■ Per verificare se un file sia già compresso o meno si usi file
# ○ Sposti i file compressi in una cartella nascosta /home/<nome utente>/.Trash
# ■ Si consiglia di approfondire il comando du
# ■ Se la directory .Trash supera i 100Kb la svuota


# Verifica esistenza argomenti
if [ ! -e $1 ]; then
    echo "Manca argomento"
    echo "Uso corretto: `basename $0` dir1 dir2 … "
    exit 1
fi
# Creazione cartella .Trash se inesistente
if [ ! -e $HOME/.Trash ]; then
    mkdir $HOME/.Trash
    echo "Creo il cestino"
fi

for arg in $@; do # Ciclo sui file passati come argomento
    if file $arg | grep gzip > /dev/null ; then # Se già compresso sposta
        mv $arg $HOME/.Trash
    else # Altrimenti comprimi e sposta
        tar cfz $arg.gzip $arg
        rm -rf $arg
        mv $arg.gzip $HOME/.Trash
    fi
echo "File compressi spostati nel cestino"
size=`du -ks $HOME/.Trash/ | cut -f1` # Calcola dimensioni directory
if [ $size -gt 100 ]; then # Se .Trash supera il limite svuota
    rm $HOME/.Trash/*
    echo "Svuoto il cestino"
else
    echo "Il cestino non è ancora pieno"
fi
done