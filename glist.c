#include "glist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Devuelve una lista vacía.
*/
GList glist_crear() { return NULL; }

/**
 * Destruccion de la lista.
 * destroy es una función que libera el dato almacenado.
 */
void glist_destruir(GList list, FuncionDestructora destroy) {
  GNode *nodeToDelete;
  while (list != NULL) {
    nodeToDelete = list;
    list = list->next;
    destroy(nodeToDelete->data);
    free(nodeToDelete);
  }
}

/**
 * Determina si la lista es vacía.
 */
int glist_vacia(GList list) { return (list == NULL); }

/**
 * Agrega un elemento al inicio de la lista.
 * copy es una función que retorna una copia física del dato.
 */
GList glist_agregar_inicio(GList list, void *data, FuncionCopia copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = copy(data);
  return newNode;
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list, FuncionVisitante visit) {
  for (GNode *node = list; node != NULL; node = node->next)
    visit(node->data);
}

/**
 * Filtrado de la lista, utilizando la función y predicado pasado.
 */
GList glist_filtrar(GList lista, FuncionCopia c, Predicado p){
  if (lista==NULL)
    return lista;
  GList newlist=glist_crear();
  for (; lista!=NULL; lista=lista->next){
    if (p(lista->data))
      newlist=glist_agregar_inicio(newlist,lista->data,c);}
  return newlist;
}

/**
 * Mapeado de la lista, Aplica la función pasada en cada elemento de la lista.
 */
GList glist_map(GList lista,FuncionMapeadora map){
  if (lista==NULL) 
    return NULL;
  GNode* nuevo_nodo=malloc(sizeof(GNode));
  nuevo_nodo->data=map(lista->data);
  nuevo_nodo->next=glist_map(lista->next,map);
  return nuevo_nodo;
}
