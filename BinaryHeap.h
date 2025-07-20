#include <stdlib.h>

#ifndef BINARYHEAP_H
#define BINARYHEAP_H

typedef int (*FuncionComparadora) (void* dato1, void* dato2);
typedef void* (*FuncionCopiadora) (void* dato);
typedef void (*FuncionDestructora) (void* dato);
typedef void (*FuncionVisitante) (void* dato);

typedef struct _BHeap{
    void **arr;
    int capacidad;
    int ultimo;
    FuncionComparadora comp;
    FuncionCopiadora copy;
    FuncionDestructora destr;
} *BHeap;


// Crea un BHeap binario vacío
BHeap bheap_crear(FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destr);

// Determina si el BHeap es vacio
int bheap_es_vacio(BHeap heap);

// Inserta un elemento en un BHeap
void bheap_insertar(void* data, BHeap heap);

// Retorna y elimina el elemento en el tope del BHeap
void* bheap_pop(BHeap heap);

// Recorre el BHeap
void bheap_recorrer(FuncionVisitante visit, BHeap heap);

// Elimina un elemento dado del BHeap
void bheap_eliminar(void* dato, BHeap heap);

// Destruye un BHeap
void bheap_destruir(BHeap heap);

// Crea un BHeap desde un array.
// No crea una copia del array, utiliza la misma referencia.
BHeap bheap_crear_desde_arr(void **arr, int largo,FuncionComparadora comp, FuncionCopiadora copiar, FuncionDestructora destr);

// Heapsort
void heapSort(void** arr, int largo, FuncionComparadora comp);
#endif