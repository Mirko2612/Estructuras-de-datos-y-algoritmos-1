#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "BinaryHeap.h"

#define RIGHT(i) (2 * (i + 1))
#define LEFT(i) ((2 * i) + 1)
#define PARENT(i) ((i-1)/2)

#define HEAP_SIZE 1024

void swap(void** a, void** b){
    void* aux = *a;
    *a = *b;
    *b = aux;
}

BHeap bheap_crear(FuncionComparadora comp, FuncionCopiadora copy,FuncionDestructora destr){
    BHeap bHeap = malloc(sizeof(struct _BHeap));
    bHeap->arr = malloc(sizeof(void*)*HEAP_SIZE);
    bHeap->capacidad = HEAP_SIZE;
    bHeap->comp = comp;
    bHeap->destr = destr;
    bHeap->copy = copy;
    bHeap->ultimo = -1;
    return bHeap;
}

void bheap_destruir(BHeap bHeap){
    bheap_recorrer(bHeap->destr, bHeap);
    free(bHeap->arr);
    free(bHeap);
}

int bheap_es_vacio(BHeap bHeap){
    return bHeap->ultimo == -1;
}

void bheap_recorrer(FuncionVisitante visit, BHeap bHeap){
    for(int i = 0; i <= bHeap->ultimo; i++){
        visit(bHeap->arr[i]);
    }
}

void flotar(BHeap bHeap, int i){
    if(bHeap==NULL || i<=0) return;
    int padre = PARENT(i);
    while (i>0 && bHeap->comp(bHeap->arr[i],bHeap->arr[padre])<0)
    {
        void* temp= bHeap->arr[i];
        bHeap->arr[i]=bHeap->arr[padre];
        bHeap->arr[padre]=temp;
        
        i=padre;
        padre= PARENT(i);
    }
}

void bheap_insertar(void* data,BHeap bHeap){
    if(bHeap->ultimo + 1 == bHeap->capacidad){ 
            bHeap->arr = realloc(bHeap->arr, sizeof(void*)*bHeap->capacidad*2);
        bHeap->capacidad *= 2;
    }
    // Insertamos el elemento al final del Heap
    bHeap->arr[++bHeap->ultimo] = bHeap->copy(data);
    // Flotamos el elemento hasta su posición final
    flotar(bHeap, bHeap->ultimo);
}  

void hundir(BHeap bHeap, int i){
    if(bHeap==NULL) return;
    int hijoder=RIGHT(i);
    int hijoizq=LEFT(i);
    int tam=bHeap->ultimo;
    int menor=i;
    while (i<= bHeap->ultimo)
    {
        if (hijoder<=tam && bHeap->comp(bHeap->arr[menor],bHeap->arr[hijoder])<0)
            menor=hijoder;
        if (hijoizq<=tam && bHeap->comp(bHeap->arr[menor],bHeap->arr[hijoizq])<0)
            menor=hijoizq;
        if (menor==i)
            i=bHeap->ultimo +1;
        else{
        void* temp=bHeap->arr[i];
        bHeap->arr[i]=bHeap->arr[menor];
        bHeap->arr[menor]=temp;
        i=menor;
        }
    }
}


void bheap_eliminar(void* data, BHeap bHeap){
    int pos = -1;
    int i = 0;

    // Buscamos la primera aparicion del elemento
    while (pos == -1 && i <= bHeap->ultimo) {

        if (bHeap->comp(data, bHeap->arr[i]) == 0) {
            pos = i;
        }

        i++;

    }

    if(pos == -1){
        printf("El elemento no se encuentra en el arbol\n");
        return;
    }

    //Si el elemento estaba en el arbol, lo quitamos y lo reemplazamos por el ultimo.
    void* elem = bHeap->arr[pos];
    bHeap->arr[pos] = bHeap->arr[bHeap->ultimo--];

    // Destruimos el dato
    bHeap->destr(elem);

    // Ubicamos el dato segun corresponda
    if (pos > 0 && bHeap->comp(bHeap->arr[pos], bHeap->arr[PARENT(pos)]) > 0) {
        flotar(bHeap, pos);
    } 
    else {
        hundir(bHeap, pos);
    }
}

BHeap bheap_crear_desde_arr(void **arr, int largo, FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destr){

    BHeap bHeap = malloc(sizeof(struct _BHeap));
    bHeap->comp = comp;
    bHeap->copy = copy;
    bHeap->destr = destr;
    bHeap->capacidad = largo;
    bHeap->ultimo = largo-1;
    bHeap->arr = arr;
    
    // Convertimos en bheap. Notar que la ultima mitad del array son todas hojas
    for(int i = largo / 2; i >= 0; i --){
        hundir(bHeap, i);
    }
    return bHeap;
}

void* dummy_copy(void* dato){
    return dato;
}

void dummy_destr(void* dato){
    (void) dato;
    return;
}

void* bheap_pop(BHeap bHeap){
    void* max = bHeap->arr[0];
    // Reemplazamos la raiz por el ultimo
    bHeap->arr[0] = bHeap->arr[bHeap->ultimo--];
    // Hundimos la nueva raiz hasta su posición final
    hundir(bHeap, 0);
    return max;
}

void heapSort(void** arr, int largo, FuncionComparadora comp){
    BHeap bHeap = bheap_crear_desde_arr(arr, largo, comp, dummy_copy, dummy_destr);
    for(int i = largo - 1; i >= 0; i--){
        void* max = bheap_pop(bHeap);
        bHeap->arr[i] = max;
    }
    free(bHeap);
}

