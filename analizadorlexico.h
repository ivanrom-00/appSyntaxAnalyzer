#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QStringList>
using namespace std;

#define MAXTOKEN 255 // -----> Ya no se usa
#define NUMPALRES 4
#define MAX 50

class AnalizadorLexico
{
private:
    int k;
    int indice;
    int aux;
    int inicioToken;
    int estadoInicial;
    int estadoActual;
    char num[11]="0123465789";
    char sLexema[127];
    char sLinea[127];
    FILE *Fd;   // --> archivo en el que se analizará el léxico
    char PalabrasReservadas[5][10]= {"char", "float","int","puts"};

    QString cadena; // --> Variable para ir obteniendo el valor completo analizado

    QStringList tokensResult;  // --> asTokens
    QStringList valoresResult; // --> lista de valores para la tabla

public:
    AnalizadorLexico(FILE *FileOrigin);  // Construir con un archivo
    void analizador(int numBytesArch);
    void inicializarEstados();
    bool estadoDeAceptacion();
    void fallo();
    void recuperaError();

    int esId();

    char leerCaracter();
    void retrocedeCaracter();

    // getters de las listas para así poder hacer la tabla:
    QStringList getTokenResult();
    QStringList getValoresResult();

};

#endif // ANALIZADORLEXICO_H
