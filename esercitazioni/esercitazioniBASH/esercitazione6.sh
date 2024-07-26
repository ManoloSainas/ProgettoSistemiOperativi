#!/bin/bash


# ● Usare il comando ls insieme al comando sort e al meccanismo di redirezione per 
# creare un file output1.txt contenente l'elenco dei file della cartella di sistema /lib
# ordinati alfabeticamente in ordine inverso sulla base del loro nome

ls /lib | sort -r > output1.txt


#########################################################################################


# ● Utilizzare il comando ls insieme al comando cut e al meccanismo di redirezione per 
# creare un file output2.txt contenente i permessi del proprietario e del suo 
# gruppo di tutti i file della cartella di sistema /lib/
# ○ È possibile avvalersi di un file temporaneo intermedio

ls -l /lib/ | cut -c2-7 > output2.txt


#########################################################################################


# ● Osservare il contenuto del file output2.txt appena creato
# ○ Ci sono dei problemi nelle informazioni riportate? 
# ● Come si possono modificare i precedenti comandi per risolverli? 
# ○ Salvare l’output corretto sul file output3.txt

ls -l /lib | cut -c2-7 > output2.txt
tail -n +2 output2.txt > output3.txt


#########################################################################################


# ● In modo simile a quanto fatto negli esercizi precedenti, creare un file output4a.txt
# che contenga tutte le informazioni dei file presenti nella propria home directory
# ○ Per esteso, senza alcuna selezione specifica con il comando cut
# ○ Risolvere il problema della prima riga, come richiesto in precedenza 
# ● A partire da output4a.txt, individuare il comando o i comandi da usare per 
# generare un file output4b.txt contenente i soli file regolar

ls -l ~ > tmp.txt
tail -n +2 tmp.txt > output4a.txt
grep ‘^-’ output4.txt > output4b.txt 


#########################################################################################



# ● Premesso che /etc/passwd è un file di sistema che contiene alcune informazioni 
# sugli utenti del sistema Linux utilizzato:
# ○ A partire da questo file generare un file output5.txt contenente il solo nome 
# (username) di ciascun utente

cut -d':' -f1 /etc/passwd > output5.txt


#########################################################################################



# ● Generare un file dal nome output6.txt contenente il nome di tutti i file presenti 
# nella cartella di sistema /dev che:
# ○ Hanno lunghezza esatta di 4 caratteri
# ○ Iniziano con la lettera t


 ls /dev | grep '^t...$' > output6.txt


#########################################################################################



# ● Scrivere un comando in grado di inserire in un file denominato output7.txt il percorso 
# e il nome di tutte le cartelle presenti nella vostra home directory
# ○ In modo ricorsivo, includendo quindi anche tutte le sottocartelle presenti a partire 
# dalla home directory
# ○ Gestendo gli eventuali messaggi di errore, che devono essere esclusi da output7.txt


find ~ -type d -print > output7.txt 2> /dev/null


#########################################################################################



# ● Dopo avere consultato attentamente il manuale del comando find:
# ○ Spostarsi sulla propria home directory
# ○ Provare a scrivere un comando in grado di modificare i permessi dei soli file presenti 
# nella cartella (e sottocartelle) correnti con privilegi 644, cambiandoli in 640
# ■ In pratica, togliendo il permesso di lettura al gruppo di utenti “others”, ma nei soli file 
# con permessi 644 di partenza


chmod 640 `find . -type f -perm 644`
chmod o-g `find . -type f -perm 644`
find . -type f -perm 644 -print -exec chmod 640 {} \;
find . -type f -perm 644 -exec chmod o-g {} \;
find . -type f -perm 644 -print -exec chmod 640 {} \;
find . -type f -perm 644 -print -exec chmod o-g {} \;


#########################################################################################



# ● Creare un file di testo input9.txt con il seguente contenuto:
# Questo è un [testo di prova] per svolgere l'esercizio [numero 9]
# ● Utilizzare uno dei comandi visti a lezione per generare il file output9.txt
# contenente il testo del file input9.txt con le parentesi quadre sostituite da 
# parentesi tonde


echo “Questo è un [testo di prova] per svolgere l'esercizio [numero 9]” > input9.txt
tr '[]' '()' < input9.txt > output8.txt


#########################################################################################



# ● Dal file di testo input1.txt precedentemente creato, utilizzando uno dei comandi 
# visti a lezione, eliminare tutti i caratteri numerici
# ○ Salvare l’output sul file output10a.txt
# ● Adattare il comando usato al punto precedente per rimuovere tutti i caratteri a 
# eccezione di quelli numerici
# ○ Salvare l’output sul file output10b.txt


tr -d [:digit:] < input1.txt > output10a.txt
tr -cd [:digit:] < input1.txt > output10b.txt
tr -d 0-9 < input1.txt > output10a.txt
tr -cd 0-9 < input1.txt > output10b.txt


#########################################################################################



# ● Creare due file di testo:
# ○ Contenuto file input11a.txt
# uno
# due
# tre
# ○ Contenuto file input11b.txt
# quattro
# cinque
# ● Definire adesso un comando in grado di mostrare in output il numero di righe totali 
# dei due file

cat input2.txt input3.txt | wc -l


#########################################################################################


# ● Riprendendo la soluzione adottata per l’esercizio precedente, definire un comando in 
# grado di mostrare il numero di file presenti nella cartella di sistema /dev

ls -l /dev | wc -l


#########################################################################################


