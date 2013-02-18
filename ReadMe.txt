========================================================================
    DYNAMIC LINK LIBRARY : KalBot Project Overview
========================================================================


GUI Folder:
Contains "MainWindow" class which is responsible for showing output of intercepted data.

Hooked
There are two seperate functions that was hooked in order to get transfered data between client and server.
These are just my own copies with logging addition so I can respond to server's requests.

PE Interact
Not used. It has ability to read PE Header and IAT/EAT tables of .dll/.exe files.

Tools
Main hack classes that are responsible for hooking function and directly interacting with integrated process
memory.

KalBot.cpp contains main hack thread.