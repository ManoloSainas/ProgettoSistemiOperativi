#!/bin/bash

# Creare un file di nome bin-f contenente i soli comandi della directory /bin che 
# iniziano con la lettera ‘f’
# Partendo dall’output del comando ls
# Usando il comando find con l’opzione adatta alla selezione dei file con nome indicato
# Ripetere per la directory /usr/bin chiamando il file usr-bin-f

ls /bin/ | grep "^f" > ~/bin-f
ls /usr/bin/ | grep "^f" > ~/usr-bin-f

find /bin/ -name f* > ~/bin-f
find /usr/bin/ -name f* > ~/bin-f

# Creare due cartelle chiamate ok e ko
# Rimuovere qualsiasi permesso alla cartella ko
# Usare il comando ls:
# Su entrambe le cartelle contemporaneamente
# In versione estesa (long) e includendo anche possibili file nascosti
# Salvare l’output nello standard output all’interno del file result.out
# Salvare l’output nello standard error all’interno del file error.log
# Visualizzare a video il contenuto di result.out e error.log
# Confrontare l’output del comando ls indicato, ma eseguito senza reindirizzamento, 
# con il contenuto dei file result.out e error.log

mkdir ok ko
chmod 000 ko
ls –al ok ko 2> error.log > result.out 
cat error.log
cat result.out
ls -al ok ko

# Crearsi una copia locale history.txt della history di sistema, adoperando il 
# meccanismo della redirezione dell'output, quindi eseguire un comando che visualizzi 
# le prime 15 righe di tale file
# Eseguire un comando che visualizzi le sole righe del file history.txt che finiscono 
# con il carattere t
# Eseguire un comando che memorizzi in un file info.txt la dimensione in byte ed il 
# numero di parole del file history.txt

history > history.txt 
head -n15 history.txt
grep 't$' history.txt
wc -cw history.txt > info.txt
cat info.txt

# Elencare i file presenti nella propria home directory in ordine alfabetico
# Elencare i file presenti nella propria home directory per data di ultima modifica e 
# salvare il risultato nel file data.log

ls | sort
ls -tl > data.log
cat data.log

# Individuare le opzioni utili per visualizzare tutte le informazioni disponibili sul sistema 
# su cui state operando
# Riferirsi alle informazioni potenzialmente restituibili dal comando uname
# Avvalendosi del comando cut, visualizzare la sola release e il nome del kernel

uname -a | cut -d" " -f1,3

# Riprendendo quanto richiesto nell’esercizio 01
# Creare un file di nome bin-l-f contenente i soli comandi della directory /bin che 
# iniziano con la lettera ‘f’
# Partendo dall’output del comando ls -l
# Ripetere per la directory /usr/bin chiamando il file usr-bin-l-f

ls -l /bin/ | tr -s " " | cut -d" " -f9 | grep "^f" > bin-l-f
ls -l /usr/bin/ | tr -s " " | cut -d" " -f9 | grep "^f" > usr-bin-l-f