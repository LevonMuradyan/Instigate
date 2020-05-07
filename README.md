# Instigate
CONTENT 1. INTRODUCTION 2. DIRECTORY STRUCTURE 3. USAGE

INTRODUCTION This is the README file for Score of Sentence project which provides to realize what sentence is about.

DIRECTORY STRUCTURE

src/ : The source codes of the projects.

doc/ : Documentation of project (doc->latex->refman.pdf)

makefile : provides the targets which are described in "USAGE" section.

makefile.prog : make the program

makefile.test : make the tests

doxyfile : documentation config file

USAGE
    1.At first install Boost, write `sudo apt-get install libboost-all-dev` in terminal 
        to install Boost completly
    2.Then write `export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu/"` in terminal
        to set Library path to Boost libs
    3.Write `make` in terminal to build project
    4.Write `./bin/program` in terminal to run the program
    5.Open `logfile.log` to see the results  

usage: make [target]

      where <target> = help | clean | test | prog 
       
     default - build the project

        help - displays information about make usage
       
       clean - cleans all the temporary files created by previous builds

        test - build unit tests

        prog - build the project