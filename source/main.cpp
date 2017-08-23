#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "mainwindow.h"

int main(int argc,char *argv[])
{
   MainWindow window(800,600,"Resource compiler",argc,argv);
   return Fl::run();
}
