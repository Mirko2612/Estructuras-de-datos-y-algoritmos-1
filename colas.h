#ifndef __COLAS_H__
#define __COLAS_H__
typedef struct _Nodo{
    void* dato;
    struct _Nodo* sig;
}Nodo;

typedef struct {
  Nodo* primero;
  Nodo* ultimo;  
}_Cola;

typedef _Cola* Cola;
typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*Predicado)(void *dato);

/**
 * Crea e inicializa una nueva cola vacía con la capacidad dada.
 */
Cola cola_crear();

/**
 * Libera la memoria requerida para la cola.
 */
void cola_destruir(Cola cola);

/**
 * Retorna 1 si la cola está vacía, 0 en caso contrario. 
 */
int cola_es_vacia(Cola cola);

/**
 * Retorna el elemento que se encuentre en el inicio de la cola, sin eliminarlo.
 */
void* cola_inicio(Cola cola);

/**
 * Inserta un elemento en el casillero siguiente al último de la cola.
 */
void cola_encolar(Cola cola,void* dato,FuncionCopia copy);

/**
 * Elimina el primer elemento de la cola.
 */
void cola_desencolar(Cola cola,FuncionDestructora destroy);

/**
 * Imprime en orden los elementos de la cola.
 */
void cola_imprimir(Cola cola,FuncionVisitante visitar);


#endif /*__COLAS_H__*/
