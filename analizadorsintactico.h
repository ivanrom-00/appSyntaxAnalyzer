#ifndef ANALIZADORSINTACTICO_H
#define ANALIZADORSINTACTICO_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QStringList>
using namespace std;

#define MAXTOKEN 255  // --> Ya no se usa
#define NUMPALRES 4
#define MAX 50

class AnalizadorSintactico
{
private:

    // arreglo de tokens (deben ser lo mismos que en el Léxico)
    char token[22][8] = {"x",";",",","*","id","[","]","num","char","int","float",
                         "puts","(",")","Cte.Lit","=","+","-","/","{","}","real"};

    // arreglo de variables sintácticas que indicarán las producciones
    char varsint[13][3]={"x","D","L","L'","I","I'","A","A'","K","T","F","E","P"};

    // Tabla M con todas las producciones
    int tablaM[35][8]= {
                {1, 8 , 1, 9, 2, -1, 999,999},  // D con char      D -> TL;
                {1, 9 , 1, 9, 2, -1, 999,999},  // D con int       D -> TL;
                {1, 10, 1, 9, 2, -1, 999,999},  // D con float     D -> TL;
                {2, 3 , 2, 4, 3, 999, 999, 999}, // L con *        L -> IL'
                {2, 4 , 2, 4, 3, 999, 999, 999}, // L con ID       L -> IL'
     /* 5 */    {3, 1 , 3, 999,999,999,999,999}, // L' con ;       L' -> e
                {3, 2 , 3, -2, 4, 3, 999,999},  // L' con ,        L' -> ,IL'
                {4, 3, 4, -3, -4, 5, 999,999},  // I con *         I -> *IDI'
                {4, 4, 4, -4, 5, 999,999,999},  // I con ID        I -> IDI'
                {5, 1, 5, 999,999,999,999,999}, // I' con ;        I' -> e
    /* 10 */    {5, 2, 5, 999,999,999,999,999},  // I' con ,       I' -> e
                {5, 5, 5, 6, 999,999,999,999},  // I' con [        I' -> A
                {5, 3, 5, -3, 8, 5,999,999},   //I' con *          I' -> *KI'
                {5, 15, 5, -15, 8, 5,999,999}, //I' con =          I' -> =KI'
                {5, 16, 5, -16, 8, 5,999,999}, //I' con +          I' -> +KI'
                {5, 17, 5, -17, 8, 5,999,999}, //I' con -          I' -> -KI'
                {5, 18, 5, -18, 8, 5,999,999}, //I' con /          I' -> /KI'
                {6, 5, 6, -5, 8,-6, 5, 999 },   // A con [         A -> [K]I'
                {6, 2, 6, -2, 8, 6, 999, 999 },   // A con ,       A -> ,KA
                {6, 20, 6, -20, 999, 999, 999, 999 },   // A con }       A -> }
                {7, 1, 7, 999,999,999,999,999}, // A' con ;        A' -> e
                {7, 2, 7, 999,999,999,999,999}, // A' con ,        A' -> e
    /* 15 */    {7, 5, 7, -5, 8, -6, 7, 999},   // A' con [        A' -> [K]A'  (corregido)
                {8, 4, 8, -4, 999, 999,999,999}, // K con ID       K -> ID
                {8, 7, 8, -7, 999,999,999,999}, // K con num       K -> num
                {8, 19, 8, -19, 8, 6,999,999}, // K con {          K -> {KA
                {8, 14, 8, -14, 999,999,999,999}, // K con Cte.Lit       K -> Cte.Lit
                {8, 21, 8, -21, 999,999,999,999}, // K con real    K -> real
                {9, 8, 9, -8,999,999,999,999},  // T con char      T -> char
     /* -> */   {9, 9, 9, -9,999,999,999,999},  // T con int       T -> int
    /* 20 */    {9,10,9,-10,999,999,999,999},   // T con float     T -> float

                /*segunda gramatica */
                {10,11,10,-11, 11,-1,999,999},  // F con puts      F -> puts E ;
                {11, 12, 11,-12,12,-13,999,999}, // E con (        E -> (P)
                {12,14,12,-14,999,999,999,999}, // P con Cte       P -> Cte
                {12, 4, 12, -4,999,999,999,999}}; // P con ID      P -> ID

    // Listas:
    QStringList asTokens;      // --> asTokens que será obtenida del Léxico
    QStringList variables;     // --> Lista de variables sintácticas utilizadas
    QStringList producciones;  // --> Lista de producciones realizadas

    char pilac[MAX][10];
    int cima = -1;

public:
    AnalizadorSintactico();
    bool analizador(QStringList tokensLexico);

    void insertarPila(string elem);
    void eliminarPila();

    int buscaTabla(char a[], char x[]);

    int estoken(char x[]);

    // Getters de las listas para generar las tablas
    QStringList getVariables();
    QStringList getProducciones();
};

#endif // ANALIZADORSINTACTICO_H
