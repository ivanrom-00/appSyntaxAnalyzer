#include "pila.h"
template<class T>
Pila<T>::Pila(int n)
{
            tam = n;
           // vec = new T[tam+1];
           // tope = 1;
            vec = new T[tam];
            tope = 0;
            vacia = true;
            llena = false;
}

template<class T>
void Pila<T>::push(T valor)
        {
            vacia = false;
            vec[tope++] = valor;
            if (tope == tam)
                llena = true;

        }
template<class T>
T Pila<T>::pop()
{
            /*if (--tope == 0)
                vacia = true;
            */
            llena = false;
            if (--tope == 0)
            {
                vacia = true;
                      // return vec[tope];
            }
            /*if (tope == -1)
                tope = 0;
                */
            return vec[tope];


}
template<class T>
bool Pila<T>::esta_vacia()
{
    return vacia;
}
template<class T>
bool Pila<T>::esta_llena()
{
    return llena;
}
//SOLICITAMOS INFO. DEL DATO QUE ESTA EN EL TOPE DE LA PILA
template<class T>
T Pila<T>::Tope()
{
    return vec[tope - 1];
}
