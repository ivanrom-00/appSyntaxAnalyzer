#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
using namespace std;

#include <string.h>

class FileManager
{
private:
    FILE *Fd;
public:
    FileManager();
    void generarArchivo(char *contenido);
    void abrirArchivo();
    int  bytesarch();
    FILE *getFd() const;
};

#endif // FILEMANAGER_H
