#!/bin/bash

echo "Settare i secondi del timer:"
read secs
while ((secs > 0)); do
echo "Mancano $secs secondi"
((--secs))
sleep 1
done
echo -e Ring Ring "\a"