Autor: andygodov2@gmail.com

The program was tested on SDL2 version 2-28-0

The following libraries are required for the program to work under Windows:
 - SDL2.dll
 - SDL2_image.dll
 - SDL2_mixer.dll
 - SDL2_ttf.dll
 
You can download it from the official website: https://www.libsdl.org/

You also need to download the header files from
the official website and place them in the include/SDL2 directory

To run the program under Linux, you need to install the following libraries:
 - sudo apt install libsdl2-dev
 - sudo apt install libsdl2-image-dev
 - sudo apt install libsdl2-mixer-dev
 - sudo apt install libsdl2-ttf-dev

To build a program for Windows, enter the command: make win

To build a program for Linux, enter the command: make lin

As a result, a bin directory will be created and executable files will be created in it