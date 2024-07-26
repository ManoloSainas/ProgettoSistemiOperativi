#!/bin/bash

# Using the three commands man, apropos and whatis:
# What type of information does each of these return?
# Are there differences between the returned information? If yes, which ones?

man cat
apropos cat
whatis cat

# Move to your home directory
# Create the directory archive
# Create the subdirectory work within archive
# Create the subdirectory home within archive
# Verify the creation of the folders using the ls command

cd ~ 
mkdir archivio
mkdir archivio/casa
mkdir archivio/lavoro
ls archivio

# In reference to the previous exercise 04
# Create within the work folder a file called doc1, one called doc2 and 
# one called doc3
# Rename the file doc1 to doc1b
# Remove the file doc2 from the work folder
# Move the file doc3 from the work folder to the home folder
# Verify that in the work folder only the file doc1b remains and that in the home folder 
# there is only the file doc3

cd archivio/lavoro
touch doc1 doc2 doc3
mv doc1 doc1b
rm doc2
mv doc3 ../casa
ls .
ls ../casa

# In reference to the previous exercises 04 and 05
# Create within your home directory a folder named backup
# Copy the archive folder into the backup folder
# Verify the copy
# Remove the archive folder from your home directory

cd ~
mkdir backup
cp -r archivio backup
ls backup
rm -r archivio

# Do everything using relative paths and not absolute ones!
# Create within your home directory a folder named objects
# Create within the objects folder the following subfolders:
# list1, list2, list3, list4
# Within the subfolder list4 create the following sub-subfolders:
# class1, class2
# Create two files named file1 and file2 within the class2 folder
# Move file1 inside the list1 folder
# Copy the file file2 into the list4 folder

mkdir oggetti
cd oggetti
mkdir lista1 lista2 lista3 lista4
cd lista4
mkdir classe1 classe2
cd classe2
touch file1 file2
mv file1 ../../lista1
cp file2 ../..

# Create a DATA folder in your home directory
# Create inside it the files file1 and file2
# Remove the folder using the rmdir command
# Is it possible to execute the command?
# If not, explain the reason and indicate the possible solutions to the problem

cd ~
mkdir DATA
touch DATA/file1 DATA/file2
rmdir DATA || echo "Cannot remove 'DATA/': Directory not empty"

# In relation to the myfile.txt file from the previous exercise 10:
# Use the strings command and explain the type of information obtained
# Use the file command and explain the type of information obtained

strings myfile.txt
file myfile.txt