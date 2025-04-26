#ifndef __PILAS_H__
#define __PILAS_H__
#include "arregloenteros.h"
struct _Pila{
    ArregloEnteros* arr;
    int ultimo;
};
typedef struct _Pila *Pila;

/**
 * Crea e inicializa una nueva pila vac´ıa con la capacidad dada.
 */
Pila pila_crear(int size);

/**
 * Libera la memoria requerida para la pila.
 */
void pila_destruir(Pila pila);

/**
 * Retorna 1 si la pila est´a vac´ıa, 0 en caso contrario.
 */
int pila_es_vacia(Pila pila);

/**
 * Retorna el elemento que se encuentre en el tope de la pila, sin eliminarlo.
 */
int pila_tope(Pila pila);

/**
 * Inserta un elemento en el tope de la pila, en caso que la pila se encuentre llena, deber´a aumentar al doble la capacidad del arreglo.
 */
void pila_apilar(Pila pila, int dato);

/**
 * Elimina el elemento que se encuentra en el tope de la pila.
 */
void pila_desapilar(Pila pila);

/**
 * Imprime en orden los elementos de la pila.
 */
void pila_imprimir(Pila pila);
#endif /* __PILAS_H__ */