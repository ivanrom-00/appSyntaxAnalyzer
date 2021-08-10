#include "filemanager.h"

FILE *FileManager::getFd() const
{
    return Fd;
}

FileManager::FileManager()
{

}

void FileManager::generarArchivo(char *contenido)
{
    char car;
    char nomArch[10];

    strcpy(nomArch, "x.dat"); // --> Nombre por default

    Fd = fopen(nomArch,"w+b"); // --> Abrir archivo

    for(int i = 0; i < strlen(contenido) + 1; i++) {
        car = contenido[i];
        if(car == 0)
            i++;

        fwrite(&car,sizeof(car),1,Fd); // --> Escribir en el archivo
    }

    fclose(Fd);

}

void FileManager::abrirArchivo()
{
    // Abrir el archivo con el nombre por default

    char nomArch[10];

    strcpy(nomArch, "x.dat");

    Fd = fopen(nomArch,"r+b");
}

int FileManager::bytesarch()
{
    // Conocer los bytes del archivo

    int aux;
    fseek(Fd,0L,SEEK_END);
    aux = (int) ftell(Fd);
    fseek(Fd,0L,SEEK_SET);
    return aux;
}
