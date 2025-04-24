#include "arregloenteros.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Devuelve un arreglo vacío.
 */
ArregloEnteros* crear_arreglo(int size){
    if(size <=0) return NULL;
    ArregloEnteros* arreglo=malloc(sizeof(ArregloEnteros));
    assert(arreglo);
    arreglo->direccion=malloc(sizeof(int)*size);
    assert(arreglo->direccion);
    arreglo->capacidad=size;
    return arreglo;
}

/**
 * Destruye el arreglo.
 */
void destruir_arreglo(ArregloEnteros* arreglo){
    free(arreglo->direccion);
    free(arreglo);
}

/**
 * Agrega el dato ingresado a la posicion indicada.
 */
ArregloEnteros* arreglo_agregar_dato(ArregloEnteros* arreglo,int dato,int pos){
    arreglo->direccion[pos]=dato;
    return arreglo;
}

/**
 * Duplica el tamaño del arreglo.
 */
ArregloEnteros* arreglo_ajustar(ArregloEnteros* arreglo,int size){
    arreglo->capacidad=size*2;
    arreglo->direccion=realloc(arreglo->direccion,sizeof(int)*(arreglo->capacidad));
    assert(arreglo->direccion);
    return arreglo;
}

/**
 * Recorre e imprime los datos del arreglo de enteros.
 */
void arreglo_enteros_imprimir(ArregloEnteros* arreglo){
    for(int i=0;i<arreglo->capacidad;i++)
        printf("%d\n",(arreglo->direccion)[i]);
}