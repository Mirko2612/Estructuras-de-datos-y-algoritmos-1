#ifndef __SGLIST_H__
#define __SGLIST_H__

typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*Predicado)(void *dato);
typedef int (*FuncionComparadora)(void *dato1, void*dato2);

typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef GNode *GList;

typedef GList SGList;

/**
 * Devuelve una lista vacía.
 */
SGList sglist_crear();

/**
 * Destruccion de la lista.
 */
void sglist_destruir(SGList lista, FuncionDestructora destruir);

/**
 * Determina si la lista es vacía.
 */
int sglist_vacia(SGList lista);

/**
 * Agrega un elemento al inicio de la lista.
 */
SGList sglist_agregar_inicio(SGList lista, void *dato, FuncionCopia copiar);

/**
 * Recorrido de la lista, utilizando la función pasada.
 */
void sglist_recorrer(SGList lista, FuncionVisitante visitar);

/**
 * Filtrado de la lista, utilizando la función y predicado pasado.
 */
SGList sglist_filtrar(SGList lista, FuncionCopia c,Predicado p);

/**
 * Insertar nuevo dato a la lista, utilizando la función comparadora y copia dada.
 */
SGList sglist_insertar(SGList lista, void *dato, FuncionCopia copiar, FuncionComparadora comparar);

/**
 * Busca un determinado dato dentro de la lista, utilizando la función comparadora para verificar la igualdad con el dato dado.
 */
int sglist_buscar(SGList lista, void *dato,FuncionComparadora comparar);
#endif /* __SGLIST_H__ */