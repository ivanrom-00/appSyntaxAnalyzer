#include "analizadorsintactico.h"

AnalizadorSintactico::AnalizadorSintactico()
{

}
                                    // obtener el asTokens del Léxico
bool AnalizadorSintactico::analizador(QStringList tokensLexico)
{
    int ip=0,i,j;  // -> Índices
    int renglon, iast;
    char x[50], a[50];  // -> X y A para hacer las comparaciones

    asTokens = tokensLexico; // Aquí se inicializa el asTokens con los tokens analizados

    insertarPila("$"); // Inicializar la pila con $

    // Conocer si se usará la primera o segunda gramática
    // La segunda gramática se usa cuando se analiza "puts"
    if(asTokens[ip] == "puts")
        insertarPila("F");
    else
        insertarPila("D");

    do
    {
        strcpy(x,pilac[cima]); // -> Obtener x de la cima de la pila
        strcpy(a,asTokens[ip].toStdString().c_str()); // -> Obtener a del actual token

        if(estoken(x) || (strcmp(x,"$")== 0)){
            // Si la x ES UN TOKEN o $, entonces se va al siguiente ip
            if(strcmp(x,a)==0){
                // Cuando x = a:
                eliminarPila(); // --> Se elimina la cima de la pila
                ip++; // --> Se aumenta el ip para conocer lo siguiente a analizar
            }
            else{
                // Cuando x != a:
                // Se compara otra vez para conocer la gramática
                if(asTokens[ip] == "puts"){
                    insertarPila("F");
                }
                else{
                    insertarPila("D");
                }
                // x = cima de la pila y se vuelve a analizar
                strcpy(x,pilac[cima]);

            }
        }
        else{
            // Si la x NO ES TOKEN
            renglon = buscaTabla(a,x); // se busca una producción
            if(renglon != 999)
            {
                // Cuando se encuentra una producción:

                eliminarPila(); // Se elimina la cima de la pila
                iast = 0;

                // Se obtiene la variable sintáctica de la Tabla M
                // Se agrega a la lista de variables
                variables.append(varsint[tablaM[renglon][0]]);

                // Comienza la producción:

                QString produccion = "";

                for(j= 3; iast != 999; j++){ // -> Mientras haya producción
                    iast = tablaM[renglon][j];
                    if(iast < 0){
                        // Cuando es negativo es un token y
                        // se obtiene del arreglo de tokens
                        iast *= -1;
                        produccion += token[iast]; // Se va agregando al QString

                    }
                    else{
                        if(iast !=999)
                            // Cuando es positivo es una variable sintáctica y
                            // se obtiene del arreglo de varsint
                            produccion += varsint[iast]; // Se va agregando al QString
                    }
                }

                // Se agrega la producción a la lista de producciones
                producciones.append(produccion);

                // Se agrega a la pila la producción
                for(i = j-2; i > 2; i--){
                    iast = tablaM[renglon][i];
                    if(iast < 0){
                        iast *= -1;
                        insertarPila(token[iast]);
                    }
                    else{
                        insertarPila(varsint[iast]);
                    }
                }

            } // --> Fin de if
            else{
                // Cuando se encuentra una producción 999
                // es un error de sintaxis
                return false;
            }
        }
    } while(strcmp(x,"$")!= 0); // Mientras x != $, ya que $ indica el final

    return true; // --> Sintaxis válida

}

void AnalizadorSintactico::insertarPila(string elem)
{
    // Agregar a la cima de la pila

    if(cima == -1)
    {
        cima = 0;
        strcpy(pilac[cima],elem.c_str());
    }
    else
    {
        if(cima == MAX - 1){
            puts("Pila LLena");
        }
        else{
            cima++;
            strcpy(pilac[cima],elem.c_str());
        }
    }

}

void AnalizadorSintactico::eliminarPila()
{
    // Eliminar la cima de la pila

    if(cima == -1){
        puts("Pila Vacia");
    }
    else{
        strcpy(pilac[cima],"");
        cima--;
    }

}

int AnalizadorSintactico::buscaTabla(char a[], char x[])
{

    // Buscar x y a en los arreglos y conocer la producción correspondiente

    int indx=0, inda=0, i;

    // Conocer el token que indica a
    for(i = 0; i < 22; i++) {
        if(strcmp(a,token[i])==0) {
            inda = i;
            break;
        }
    }

    // Conocer la variable que indica x
    for(i = 0; i < 13; i++) {
        if(strcmp(x,varsint[i])==0) {
            indx=i;
            break;
        }
    }

    // Conocer la producción según x y a
    for(i = 0; i < 35; i++) {
        if(indx == tablaM[i][0]) {
            if(inda == tablaM[i][1]) {
                return i;
            }
        }
    }

    // Si no encuentra una producción retorna 999 (error)
    return 999;

}

int AnalizadorSintactico::estoken(char x[])
{
    // Conocer si x es token

    int i;
    for(i = 0; i < 22; i++) {
        if(strcmp(x,token[i])==0){
            return 1;
        }
    }
    return 0;

}

// Getters de las listas:

QStringList AnalizadorSintactico::getVariables()
{
    return variables;
}

QStringList AnalizadorSintactico::getProducciones()
{
    return producciones;
}
