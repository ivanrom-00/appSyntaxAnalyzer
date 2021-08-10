#include "analizadorlexico.h"

AnalizadorLexico::AnalizadorLexico(FILE *FileOrigin)
{
    Fd = FileOrigin;
}

void AnalizadorLexico::analizador(int numBytesArch)
{
    char caracter;  // --> cadena que se irá analizando
    indice = inicioToken = k = 0;  // --> Inicializar los índices
    inicializarEstados();  // --> Inicializar los estados

    while(indice < numBytesArch || estadoDeAceptacion())
    {
        switch(estadoActual){

        // ------ NÚMEROS ------

        case 0:
            caracter = leerCaracter();
            if (strchr(num,caracter)) {
                estadoActual = 1;
                cadena += caracter;  // --> Se va agregando a la cadena
            }
            else
                fallo();
            break;

        case 1:
            caracter=leerCaracter();
            if(strchr(num,caracter)){
                estadoActual=1;
                cadena += caracter;
            }
            else{
                if(caracter=='.'){  // --> Detectar punto para los números reales
                    estadoActual = 2;
                    cadena += caracter;
                }
                else{
                    estadoActual = 3;
                }
            }
            break;

        case 2:
            caracter=leerCaracter();
            if(strchr(num,caracter)) {
                estadoActual=2;
                cadena += caracter;
            }
            else
                estadoActual=4;
            break;

        // ------ NÚMERO ENTERO ------

        case 3:
            retrocedeCaracter();
            tokensResult.append("num");  // -> Agergar el tipo de token
            valoresResult.append(cadena);  // -> Agregar el valor de la cadena
            cadena = "";  // -> Reiniciar cadena
            if (indice >= numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ NÚMERO REAL ------

        case 4:
            retrocedeCaracter();
            tokensResult.append("real");
            valoresResult.append(cadena);
            cadena = "";
            if (indice >= numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ OPERADORES ------

        case 5:
            caracter=leerCaracter();
            switch(caracter){
            case '+': tokensResult.append("+"); cadena += "+"; estadoActual = 6; break;
            case '-': tokensResult.append("-"); cadena += "-"; estadoActual = 6; break;
            case '*': tokensResult.append("*"); cadena += "*"; estadoActual = 6; break;
            case '/': tokensResult.append("/"); cadena += "/"; estadoActual = 6; break;
            case '=': tokensResult.append("="); cadena += "="; estadoActual = 6; break;
            default: fallo(); break;
            }
            break;

        // ------ ESTADO DE ACEPTACIÓN DE LOS OPERADORES ------

        case 6:
            valoresResult.append(cadena);
            cadena = "";
            if (indice>=numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ IDENTIFICADORES ------

        case 7:
            caracter=leerCaracter();
            if ((isalpha(caracter) || caracter=='_')) {
                estadoActual=8;
                cadena += caracter;
            }
            else
                fallo();
            break;

        case 8:
            caracter=leerCaracter();
            if ((isalpha(caracter)|| caracter=='_') || isdigit(caracter)) {
                estadoActual=8;
                cadena += caracter;
            }
            else
                estadoActual= 9;
            break;

        case 9:
            retrocedeCaracter();
            if(esId()) {
                tokensResult.append("id");
                valoresResult.append(cadena);
                cadena = "";
            }
            else {
                tokensResult.append(sLexema);
                valoresResult.append(sLexema);
                cadena = "";
            }

            if(indice>=numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ FINAL DE LÍNEA (;) ------

        case 10:
            caracter=leerCaracter();
            if(caracter==';') {
                estadoActual=11;
                cadena += ";";
            }
            else
                fallo();
            break;

        case 11:
            tokensResult.append(";");
            valoresResult.append(cadena);
            cadena = "";
            if (indice >= numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ CORCHETES [ ] ------

        case 12:
            caracter=leerCaracter();
            if(caracter=='[') {
                estadoActual=13;
                cadena += "[";
            }
            else
                fallo();
            break;

        case 13:
            tokensResult.append("[");
            valoresResult.append(cadena);
            cadena = "";
            if (indice >= numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        case 14:
            caracter=leerCaracter();
            if(caracter==']') {
                estadoActual=15;
                cadena += "]";
            }
            else
                fallo();
            break;

        case 15:
            tokensResult.append("]");
            valoresResult.append(cadena);
            cadena = "";
            if (indice >= numBytesArch)
                return;
            inicioToken = indice;
            inicializarEstados();
            break;

        // ------ COMA , ------

        case 16:
            caracter = leerCaracter();
            if(caracter == ',') {
                estadoActual=17;
                cadena += ",";
            }
            else
                fallo();
            break;

        case 17:
            tokensResult.append(",");
            valoresResult.append(cadena);
            cadena = "";
            if (indice >= numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ COMILLAS Y CTE.LIT  ------

        case 18:
            caracter = leerCaracter();
            if (('"' == caracter) && caracter) {
                estadoActual=19;
                cadena += caracter;
            }

            else
                fallo();
            break;

        case 19:
            caracter = leerCaracter();
            if (('"' != caracter) && caracter) {
                estadoActual=19;
                cadena += caracter;
            }
            else
                if(caracter) {
                    estadoActual=20;
                    cadena += caracter;
                }
            break;

        case 20:
            tokensResult.append("Cte.Lit");
            valoresResult.append(cadena);
            cadena = "";
            if (indice >= numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ PARÉNTESIS ( )  ------

        case 21:
            caracter=leerCaracter();
            if(caracter == '(') {
                estadoActual=22;
                cadena += "(";
            }
            else
                fallo();
            break;

        case 22:
            tokensResult.append("(");
            valoresResult.append(cadena);
            cadena = "";
            if (indice>=numBytesArch)
                return;
            inicioToken = indice;
            inicializarEstados();
            break;

        case 23:
            caracter=leerCaracter();
            if(caracter == ')') {
                estadoActual=24;
                cadena += ")";
            }
            else
                fallo();
            break;

        case 24:
            tokensResult.append(")");
            valoresResult.append(cadena);
            cadena = "";
            if (indice>=numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;

        // ------ LLAVES { } ------

        case 25:
            caracter=leerCaracter();
            if(caracter == '{') {
                estadoActual=26;
                cadena += "{";
            }
            else
                fallo();
            break;

        case 26:
            tokensResult.append("{");
            valoresResult.append(cadena);
            cadena = "";
            if (indice>=numBytesArch)
                return;
            inicioToken = indice;
            inicializarEstados();
            break;

        case 27:
            caracter=leerCaracter();
            if(caracter == '}') {
                estadoActual=28;
                cadena += "}";
            }
            else
                fallo();
            break;

        case 28:
            tokensResult.append("}");
            valoresResult.append(cadena);
            cadena = "";
            if (indice>=numBytesArch)
                return;
            inicioToken=indice;
            inicializarEstados();
            break;
        }  /*fin de switch*/
    }  /*fin de while*/

    // Cuando ya no hay nada que analizar, entonces se agrega el $ a los tokens

    tokensResult.append("$");
    valoresResult.append("$");
}

void AnalizadorLexico::inicializarEstados()
{
    // Reiniciar los estados para volver a analizar
    estadoActual=0;
    estadoInicial=0;
}

bool AnalizadorLexico::estadoDeAceptacion()
{

    // Conocer cuáles estados son de acpetación

    switch (estadoActual)
    {
    case 3:
    case 4:
    case 6:
    case 9:
    case 11:
    case 13:
    case 15:
    case 17:
    case 20:
    case 22:
    case 24:
    case 26:
        return true;
    default : return false;
    }
}

void AnalizadorLexico::fallo()
{

    // Si un estado no se cumple entonces se salta al siguiente para analizar
    // al final se llama al método recuperaError

    switch (estadoInicial){

    case 0 :
        estadoInicial=5;
        indice=inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 5 :
        estadoInicial=7;
        indice=inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 7 :
        estadoInicial=10;
        indice=inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 10 :
        estadoInicial=12;
        indice = inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 12 :
        estadoInicial=14;
        indice = inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 14 :
        estadoInicial=16;
        indice=inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 16 :
        estadoInicial=18;
        indice = inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 18 :
        estadoInicial=21;
        indice = inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 21 :
        estadoInicial=23;
        indice = inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 23 :
        estadoInicial=25;
        indice = inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 25 :
        estadoInicial=27;
        indice = inicioToken;
        fseek(Fd, (long)inicioToken, SEEK_SET);
        break;

    case 27: recuperaError();
    }
    estadoActual=estadoInicial;
}

void AnalizadorLexico::recuperaError()
{
    // Reinicar el inicioToken e inicializar nuevamente los estados para volver a analizar
    inicioToken = indice;
    inicializarEstados();
}

int AnalizadorLexico::esId()
{
    // Cuando se presume que la cadena analizada es un ID, se comprueba antes si
    // es una palabra reservada

    int n,m,found = false;
    fseek(Fd,(long)inicioToken,SEEK_SET);
    for (m=inicioToken, n=0; m<indice ; m++, n++)
        fread(&sLexema[n],sizeof(char),1, Fd);
    sLexema[n]= '\0';
    for (m=0 ; m < NUMPALRES && !found ;)
        if (strcmp(PalabrasReservadas[m], sLexema)==0)
            found = true;
        else
            m++;
    return (found ? 0 : 1);
}

char AnalizadorLexico::leerCaracter()
{
    // Leer el siguiente caracter del archivo
    // aumentar el índice

    char cAux;
    fread(&cAux,sizeof(cAux),1,Fd);
    indice++;
    return cAux;
}

void AnalizadorLexico::retrocedeCaracter()
{
    // Regresar al caracter anterior del archivo
    // reducir el índice

    indice--;
    fseek(Fd,(long)indice,SEEK_SET);
}

// Getters de las listas de tokens y valores:

QStringList AnalizadorLexico::getTokenResult()
{
    return tokensResult;
}

QStringList AnalizadorLexico::getValoresResult()
{
    return valoresResult;
}
