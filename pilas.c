#include "pilas.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Crea e inicializa una nueva pila vac´ıa con la capacidad dada.
 */
Pila pila_crear(int size){
    if(size <=0) return NULL;
    Pila pila=malloc(sizeof(struct _Pila));
    assert(pila);
    pila->arr=crear_arreglo(size);
    pila->ultimo=-1;
    return pila;
}

/**
 * Libera la memoria requerida para la pila.
 */
void pila_destruir(Pila pila){
    destruir_arreglo(pila->arr);
    free(pila);
}

/**
 * Retorna 1 si la pila est´a vac´ıa, y 0 en caso contrario.
 */
int pila_es_vacia(Pila pila){
    return pila->ultimo==-1;
}

/**
 * Retorna el elemento que se encuentre en el tope de la pila, pero sin eliminarlo.
 */
int pila_tope(Pila pila){
    return pila->arr->direccion[pila->ultimo];
}

/**
 * Inserta un elemento en el tope de la pila, en caso que la pila se encuentre llena, deber´a aumentar al doble la capacidad del arreglo.
 */
void pila_apilar(Pila pila, int dato){
    if(pila->ultimo == (pila->arr->capacidad)-1){
        (pila->ultimo)++;
        pila->arr=arreglo_ajustar(pila->arr,pila->arr->capacidad);
        (pila->arr->direccion)[pila->ultimo]=dato;
        return;
    }
    (pila->ultimo)++;
    (pila->arr->direccion)[pila->ultimo]=dato;
}

/**
 * Elimina el elemento que se encuentra en el tope de la pila.
 */
void pila_desapilar(Pila pila){
    if(pila->ultimo==-1) return;
    
    (pila->arr->capacidad)--;
    (pila->ultimo)--;
}

/**
 * Imprime en orden los elementos de la pila.
 */
void pila_imprimir(Pila pila){
    arreglo_enteros_imprimir(pila->arr);
}
