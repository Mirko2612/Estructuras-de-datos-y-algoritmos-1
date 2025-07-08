#include "bstree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * bstee_crear: Retorna un arbol de busqueda binaria vacio
 */
BSTree bstee_crear() { return NULL; }

/**
 * bstree_destruir: Destruye el arbol y sus datos
 */
void bstree_destruir(BSTree raiz, FuncionDestructora destr) {
  if (raiz != NULL) {
    bstree_destruir(raiz->izq, destr);
    bstree_destruir(raiz->der, destr);
    destr(raiz->dato);
    free(raiz);
  }
};

/**
 * bstree_buscar: Retorna 1 si el dato se encuentra y 0 en caso
 * contrario
 */
int bstree_buscar(BSTree raiz, void *dato, FuncionComparadora comp) {
  if (raiz == NULL)
    return 0;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return 1;
  else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    return bstree_buscar(raiz->izq, dato, comp);
  else // raiz->dato < dato
    return bstree_buscar(raiz->der, dato, comp);
}

/**
 * bstree_insertar: Inserta un dato no repetido en el arbol, manteniendo la
 * propiedad del arbol de busqueda binaria
 */
BSTree bstree_insertar(BSTree raiz, void *dato, FuncionCopiadora copia,
                       FuncionComparadora comp) {
  if (raiz == NULL) { // insertar el dato en un nuevo nodo
    struct _BST_Nodo *nuevoNodo = malloc(sizeof(struct _BST_Nodo));
    assert(nuevoNodo != NULL);
    nuevoNodo->dato = copia(dato);
    nuevoNodo->izq = nuevoNodo->der = NULL;
    return nuevoNodo;
  } else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    raiz->izq = bstree_insertar(raiz->izq, dato, copia, comp);
  else if (comp(dato, raiz->dato) > 0) // raiz->dato < dato
    raiz->der = bstree_insertar(raiz->der, dato, copia, comp);
  // si el dato ya se encontraba, no es insertado
  return raiz;
}

/**
 * bstree_recorrer: Recorrido DSF del arbol
 */
void bstree_recorrer(BSTree raiz, BSTreeRecorrido orden,
                     FuncionVisitanteExtra visita, void *extra) {
  if (raiz != NULL) {
    if (orden == BTREE_RECORRIDO_PRE)
      visita(raiz->dato, extra);
    bstree_recorrer(raiz->izq, orden, visita, extra);
    if (orden == BTREE_RECORRIDO_IN)
      visita(raiz->dato, extra);
    bstree_recorrer(raiz->der, orden, visita, extra);
    if (orden == BTREE_RECORRIDO_POST)
      visita(raiz->dato, extra);
  }
}

BSTree max_min(BSTree raiz){
  if (raiz->izq == NULL ) return raiz ; 
  return max_min(raiz->izq) ; 
}

BSTree bstree_eliminar(BSTree arbol, void *dato, FuncionComparadora comp, FuncionDestructora destroy)
{
  if (arbol == NULL)
    return arbol;                  // caso base, no se encontro el dato a eliminar
  if (comp(dato, arbol->dato) < 0) // dato < arbol->dato
  {
    // buscamos en el subarbol izquierdo
    arbol->izq = bstree_eliminar(arbol->izq, dato, comp, destroy);
  }
  else if (comp(dato, arbol->dato) > 0)
  {
    // buscamos en el subarbol derecho
    arbol->der = bstree_eliminar(arbol->der, dato, comp, destroy);
  }
  else
  {
    // caso en el que encontramos el nodo a eliminar
    // es una hoja
    if (arbol->izq == NULL)
    {
      BSTree temp = arbol->der;
      destroy(arbol->dato);
      free(arbol);
      return temp;
    }
    if (arbol->der == NULL)
    {
      BSTree temp = arbol->izq;
      destroy(arbol->dato);
      free(arbol);
      return temp;
    }
    BSTree min_max = max_min(arbol->der);
    void *dato_a_eliminar = arbol->dato;
    arbol->dato = min_max->dato;
    // eliminamos el nodo que contiene el dato a eliminar
    // ahora eliminamos el nodo que contiene el dato a eliminar
    // en el subarbol derecho
    arbol->der = bstree_eliminar(arbol->der, dato_a_eliminar, comp, destroy);
  }

  return arbol;
}

int btree_validar(BSTree arbol ,FuncionComparadora comp){
  if (arbol == NULL) return 1 ; 
  if (arbol->izq != NULL && comp(arbol->izq->dato , arbol->dato) > 0 )
    return 0 ;   
  if (arbol->der != NULL && comp(arbol->der->dato , arbol->dato) < 0 )
    return 0 ;  
  return btree_validar(arbol->der , comp) && btree_validar(arbol->izq , comp) ; 
}
