#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "btree.h"
#include <math.h>
/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear() { return NULL; }

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo) {
  if (nodo != NULL) {
    btree_destruir(nodo->left);
    btree_destruir(nodo->right);
    free(nodo);
  }
}

/**
 * Indica si el árbol es vacío.
 */
int btree_empty(BTree nodo) { return nodo == NULL; }

/**
 * Crea un nuevo arbol, con el dato dado en el nodo raiz, y los subarboles dados
 * a izquierda y derecha.
 */
BTree btree_unir(int dato, BTree left, BTree right) {
  BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->dato = dato;
  nuevoNodo->left = left;
  nuevoNodo->right = right;
  return nuevoNodo;
}

/**to
 * Recorrido del arbol, utilizando la funcion pasada.
 */
void btree_recorrrer_in(BTree arbol, FuncionVisitante2 visit) {
  if (btree_empty(arbol)) return;
  btree_recorrrer_in(arbol->left,visit);
  visit(arbol->dato);
  btree_recorrrer_in(arbol->right,visit);
}

void btree_recorrrer_pre(BTree arbol, FuncionVisitante2 visit) {
  if (btree_empty(arbol)) return;
  visit(arbol->dato);
  btree_recorrrer_pre(arbol->left,visit);
  btree_recorrrer_pre(arbol->right ,visit);
}

void btree_recorrrer_post(BTree arbol, FuncionVisitante2 visit) {
  if (btree_empty(arbol)) return;
  btree_recorrrer_post(arbol->left,visit);
  btree_recorrrer_post(arbol->right,visit);
  visit(arbol->dato);
}


void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden,FuncionVisitante2 visit) {
  if (orden == BTREE_RECORRIDO_IN) return btree_recorrrer_in(arbol, visit);
  else if (orden == BTREE_RECORRIDO_PRE) return btree_recorrrer_pre(arbol, visit);
  else return btree_recorrrer_post(arbol, visit);
  return;
}

int btree_nnodos(BTree arbol){
  if (btree_empty(arbol)) return 0;
  return 1 + btree_nnodos(arbol->left) + btree_nnodos(arbol->right);
}

//Suponiendo que no se repiten elementos
int btree_buscar(BTree arbol, int dato){
  if (arbol == NULL) return 0;
  if (arbol->dato == dato) return 1;
  if (btree_buscar(arbol->left,dato)) return 1;
  return btree_buscar(arbol->right, dato);
  // otra opcion que anda por le cortocircuito del or
  // return btree_buscar(arbol->left, dato) || btree_buscar(arbol->right, dato)
}


BTree btree_copiar(BTree arbol){

  if (btree_empty(arbol)) return NULL;
  BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->dato = arbol->dato;
  nuevoNodo->left = btree_copiar(arbol->left);
  nuevoNodo->right = btree_copiar(arbol->right);
  return nuevoNodo;
}



int max(int a,int b){
  return (a>b)? a:b ; 
}

int btree_altura(BTree arbol){
  if(btree_empty(arbol)) return 0;
  return max(btree_altura(arbol->left),btree_altura(arbol->right))+1  ; 
}


int btree_nnodos_profundidad(BTree arbol , int p){
  if(btree_empty(arbol)) return 0 ;   
  if(p == 0) return 1 ; 
  return  btree_nnodos_profundidad(arbol->left,p-1) +btree_nnodos_profundidad(arbol->right,p-1);
}


int btree_profundidad(BTree arbol , int dato ){
  if(btree_empty(arbol)) return -1 ;   
  if(arbol->dato == dato) return 0 ; 
  int p = btree_profundidad(arbol->left,dato);
  if(p != -1) return p +1 ; 
  p = btree_profundidad(arbol->right,dato);
  if(p != -1) return p +1 ; 
  return -1 ;
}

int btree_sumar(BTree arbol){
  if(btree_empty(arbol)) return 0;
  return arbol->dato + btree_sumar(arbol->left)+btree_sumar(arbol->right);
}

void btree_recorrer_extra(BTree arbol, BTreeOrdenDeRecorrido orden,
FuncionVisitanteExtra visit, void *extra){
  if (orden==BTREE_RECORRIDO_IN) 
    return btree_recorrrer_in(arbol,visit);
  else if (orden=BTREE_RECORRIDO_POST)
    return btree_recorrrer_post(arbol,visit);
  else 
    return btree_recorrrer_pre(arbol,visit);
  return;
}
  
/*
void btree_bfs_iterativo(BTree arbol,FuncionCopia copy,FuncionDestructora destroy,FuncionVisitante2 visit){
  if (arbol==NULL) return;
  Cola cola_arbol=cola_crear();
  cola_encolar(cola_arbol,arbol,copy);
  while (!cola_es_vacia(cola_arbol))
  {
    void* dato = cola_inicio(cola_arbol);
    BTree nodo_actual = (BTree) dato;
    visit(nodo_actual->dato);
    cola_desencolar(cola_arbol, destroy);
    if (nodo_actual->left !=NULL)
      cola_encolar(cola_arbol,nodo_actual->left,copy);
    if (nodo_actual->right !=NULL)
      cola_encolar(cola_arbol,nodo_actual->right,copy);
  }
  cola_destruir(cola_arbol,NULL);
}*/


BTree btree_mirror(BTree arbol){
  if (arbol== NULL) return NULL;
  BTree mirror=malloc(sizeof(struct _BTNodo));
  assert(mirror);
  mirror->dato=arbol->dato;
  mirror->left=btree_mirror(arbol->right);
  mirror->right=btree_mirror(arbol->left);

  return mirror;
}

int contar_nodos(BTree arbol) {
    if (arbol == NULL) return 0;
    return 1 + contar_nodos(arbol->left) + contar_nodos(arbol->right);
}

static int es_completo_aux(BTree arbol, int index, int total_nodos) {
    if (arbol == NULL) return 1;

    if (index >= total_nodos)
        return 0;

    return es_completo_aux(arbol->left, 2 * index + 1, total_nodos) &&
           es_completo_aux(arbol->right, 2 * index + 2, total_nodos);
}

int btree_validar_completo(BTree arbol) {
    int total = contar_nodos(arbol);
    return es_completo_aux(arbol, 0, total);
}

static void btree_to_arr_aux(BTree arbol, void arr, int* i){
  if (arbol == NULL) return; 
  btree_to_arr_aux(arbol->izq, arr, i);
  arr[i] = arbol->dato;
  *(i)++;
  btree_to_arr_aux(arbol->der, arr, i);
}
static void** btree_to_arr(BTree arbol){
  int cantidad = bstree_nnodos(arbol);
  void** arr = malloc(sizeof(void)*cantidad);
  int i = 0;
  btree_to_arr_aux(arbol, arr, &i);
  return arr;
}
static BTree arr_to_bstree(void* arr[],int cantidad,FuncionComparadora comp,BTree arbol_completo){
  if (arbol_completo ==NULL) 
    arbol_completo->dato=arr[]
  
  return 0;
}
BTree btree_a_bstree(BTree arbol){
  if(btree_validar_completo(arbol)) return arbol;

}