#include "sglist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Devuelve una lista vacía.
*/

SGList sglist_crear() { return NULL; }

/**
 * Destruccion de la lista.
 * destroy es una función que libera el dato almacenado.
 */
void sglist_destruir(SGList list, FuncionDestructora destroy) {
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
int sglist_vacia(SGList list) { return (list == NULL); }

/**
 * Agrega un elemento al inicio de la lista.
 * copy es una función que retorna una copia física del dato.
 */
SGList sglist_agregar_inicio(SGList list, void *data, FuncionCopia copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = copy(data);
  return newNode;
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void sglist_recorrer(SGList list, FuncionVisitante visit) {
  for (GNode *node = list; node != NULL; node = node->next)
    visit(node->data);
}

/**
 * Filtrado de la lista, utilizando la función y predicado pasado.
 */
SGList sglist_filtrar(SGList lista, FuncionCopia c, Predicado p){
  if (lista==NULL)
    return lista;
  SGList newlist=sglist_crear();
  for (; lista!=NULL; lista=lista->next){
    if (p(lista->data))
      newlist=sglist_agregar_inicio(newlist,lista->data,c);}
  return newlist;
}   

/**
 * Insertar nuevo dato a la lista, utilizando la funcion comparadora y copia dada.
 */
SGList sglist_insertar(SGList lista, void *dato, FuncionCopia copiar, FuncionComparadora comparar){
    if (lista==NULL)
    {
        lista=sglist_agregar_inicio(lista,dato,copiar);
        return lista;
    }
    GNode* temp=lista;
    GNode* nuevo_nodo=malloc(sizeof(GNode));
    assert(nuevo_nodo);
    nuevo_nodo->data=dato; 
    for(; temp!=NULL;temp=temp->next){
        if (comparar(dato,temp->data)==1)
        {
            nuevo_nodo->next=temp->next;
            temp->next=nuevo_nodo;
        }
    }
    return lista;
}

/**
 * Busca un determinado dato dentro de la lista, utilizando la función comparadora para verificar la igualdad con el dato dado.
 */
int sglist_buscar(SGList lista, void *dato,FuncionComparadora comparar){
  if (lista==NULL)
    return 0;
  else if (comparar(dato,lista->data))
    return 1;
  sglist_buscar(lista->next,dato,comparar);
}
