#include "colas.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

Cola cola_crear(int size){
    if (size <=0) return NULL;
    Cola cola=malloc(sizeof(struct _Cola)*size);
    assert(cola);
    cola->primero=NULL;
    cola->ultimo=NULL;
    return cola;
}

void cola_destruir(Cola cola,FuncionDestructora destroy){
    Nodo* Nodo_a_eliminar;
    Nodo* temp=cola->primero;
    while (temp!=NULL)
    {
        Nodo_a_eliminar=temp;
        temp=temp->sig;
        destroy(Nodo_a_eliminar->dato);
        free(Nodo_a_eliminar);
    }
    free(cola);
}

int cola_es_vacia(Cola cola){
    return cola->primero==NULL;
}

void* cola_inicio(Cola cola){
    if (cola==NULL ||cola->primero==NULL)
        return NULL;
    return cola->primero->dato;
}

void cola_encolar(Cola cola, void* dato, FuncionCopia copy){
    if (cola==NULL)
        return;
    Nodo* nuevo_nodo=malloc(sizeof(Nodo));
    assert(nuevo_nodo);
    nuevo_nodo->dato=copy(dato);
    nuevo_nodo->sig=NULL;
    if (cola->ultimo != NULL)
        cola->ultimo->sig=nuevo_nodo;
    else
        cola->primero=nuevo_nodo;
    cola->ultimo=nuevo_nodo;
}

void cola_desencolar(Cola cola,FuncionDestructora destroy){
    if (cola->primero==NULL)
        return;
    Nodo* nodo_a_eliminar=cola->primero;
    cola->primero=cola->primero->sig;
    if (cola->primero==NULL)
        cola->ultimo=NULL;
    destroy(nodo_a_eliminar->dato);
    free(nodo_a_eliminar);

}

void cola_imprimir(Cola cola,FuncionVisitante visitar){
    for (Nodo* temp = cola->primero; temp!=NULL; temp=temp->sig)
        visitar(temp->dato);
}