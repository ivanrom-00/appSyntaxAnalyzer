#ifndef PILA_H
#define PILA_H

template<class T>
class Pila
{
    T* vec;
    int tam;
    int tope;
    bool vacia;
    bool llena;
public:
    Pila(int n=10);
    void push(T valor);
    T pop();
    bool esta_vacia();
    bool esta_llena();
    T Tope();
};

#endif // PILA_H
