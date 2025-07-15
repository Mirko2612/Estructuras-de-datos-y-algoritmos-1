#include "avl.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * avl_crear: Retorna un arbol AVL vacio
 */
AVL avl_crear(FuncionCopiadora copia, FuncionComparadora comp,
  FuncionDestructora destr) {
  AVL arbol = malloc(sizeof(struct _AVL));
  assert(arbol != NULL);
  arbol->raiz = NULL;
  arbol->copia = copia;
  arbol->comp = comp;
  arbol->destr = destr;
  return arbol;
}

/**
 * avl_destruir: Destruye el arbol y sus datos.
 */
static void avl_nodo_destruir(AVL_Nodo* raiz, FuncionDestructora destr) {
  if (raiz != NULL) {
    // destruir los nodos en postorder
    avl_nodo_destruir(raiz->izq, destr);
    avl_nodo_destruir(raiz->der, destr);
    destr(raiz->dato);
    free(raiz);
  }
}
void avl_destruir(AVL arbol) {
  avl_nodo_destruir(arbol->raiz, arbol->destr);
  free(arbol);
}

/**
 * avl_buscar: Retorna 1 si el dato se encuentra y 0 en caso contrario
 */
static int avl_nodo_buscar(AVL_Nodo* raiz, FuncionComparadora comp,
  void* dato) {
  if (raiz == NULL)
    return 0;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return 1;
  else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    return avl_nodo_buscar(raiz->izq, comp, dato);
  else // raiz->dato < dato
    return avl_nodo_buscar(raiz->der, comp, dato);
}
int avl_buscar(AVL arbol, void* dato) {
  return avl_nodo_buscar(arbol->raiz, arbol->comp, dato);
}

/**
 * avl_nodo_altura: Funcion interna que retorna la altura del arbol.
 * La altura del arbol vacio se define como -1.
 */
static int avl_nodo_altura(AVL_Nodo* raiz) {
  return (raiz == NULL ? -1 : raiz->altura);
}

/**
 * avl_nodo_max_altura_hijos: Funcion interna que retorna la maxima altura de
 * los hijos.
 */
static unsigned int avl_nodo_max_altura_hijos(AVL_Nodo* raiz) {
  assert(raiz != NULL);
  int alturaIzq = avl_nodo_altura(raiz->izq);
  int alturaDer = avl_nodo_altura(raiz->der);
  return (alturaIzq < alturaDer ? alturaDer : alturaIzq);
}

/**
 * avl_nodo_factor_balance: Funcion interna que retorna el factor de balance de
 * un nodo.
 */
static int avl_nodo_factor_balance(AVL_Nodo* raiz) {
  assert(raiz != NULL);
  int factor = avl_nodo_altura(raiz->der) - avl_nodo_altura(raiz->izq);
  assert(-2 <= factor || factor <= 2);
  return factor;
}

/**
 * avl_nodo_rotacion_simple_izq: Funcion interna que realiza una rotacion simple
 * a izquierda y retorna la nueva raiz.
 */
static AVL_Nodo* avl_nodo_rotacion_simple_izq(AVL_Nodo* raiz) {
  AVL_Nodo* hijoDer = raiz->der;
  assert(hijoDer != NULL);
  // actualizar punteros
  raiz->der = hijoDer->izq;
  hijoDer->izq = raiz;
  // actualizar alturas
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoDer->altura = 1 + avl_nodo_max_altura_hijos(hijoDer);
  return hijoDer;
}

/**
 * avl_nodo_rotacion_simple_der: Funcion interna que realiza una rotacion simple
 * a derecha y retorna la nueva raiz.
 */
static AVL_Nodo* avl_nodo_rotacion_simple_der(AVL_Nodo* raiz) {
  AVL_Nodo* hijoIzq= raiz->izq;
  assert(hijoIzq != NULL);
  // actualizar punteros
  raiz->izq=hijoIzq->der;
  hijoIzq->der=raiz;
  // actualizar alturas
  raiz->altura= 1 + avl_nodo_max_altura_hijos(raiz);
  hijoIzq->der= 1 + avl_nodo_max_altura_hijos(hijoIzq);
  return hijoIzq;
}

/**
 * avl_nodo_crear: Funcion interna que crea un nuevo nodo y lo retorna.
 * La altura de un nodo hoja es 0.
 */
static AVL_Nodo* avl_nodo_crear(void* dato, FuncionCopiadora copia) {
  AVL_Nodo* nuevoNodo = malloc(sizeof(AVL_Nodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->dato = copia(dato);
  nuevoNodo->izq = nuevoNodo->der = NULL;
  nuevoNodo->altura = 0;
  return nuevoNodo;
}

/**
 * avl_insertar: Inserta un dato no repetido en el arbol, manteniendo la
 * propiedad de los arboles AVL.
 */
static AVL_Nodo* avl_nodo_insertar(AVL_Nodo* raiz, void* dato,
  FuncionCopiadora copia, FuncionComparadora comp) {
  if (raiz == NULL) // insertamos el nuevo elemento
    return avl_nodo_crear(dato, copia);
  else if (comp(dato, raiz->dato) < 0) { // el dato debe ir en el subarbol izq
    raiz->izq = avl_nodo_insertar(raiz->izq, dato, copia, comp);
    // chequear balance
    if (avl_nodo_factor_balance(raiz) == -2) {
      // casos 1 o 2
      if (avl_nodo_factor_balance(raiz->izq) == 1) // caso 2
        raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
      raiz = avl_nodo_rotacion_simple_der(raiz); // caso 1
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  }
  else if (comp(raiz->dato, dato) > 0) { // el dato debe ir en el subarbol der
    raiz->der=avl_nodo_insertar(raiz->der, dato, copia, comp);
    // chequear balance
    if (avl_nodo_factor_balance(raiz)== 2){
      // casos 3 o 4
      if (avl_nodo_factor_balance(raiz->der) == -1)
        raiz->der=avl_nodo_rotacion_simple_der(raiz->der); // caso 3
      raiz=avl_nodo_rotacion_simple_izq(raiz);
    }
    raiz->altura=1+ avl_nodo_max_altura_hijos(raiz);
    return raiz;
  }
  else // no agregar elementos repetidos
    return raiz;
}
void avl_insertar(AVL arbol, void* dato) {
  arbol->raiz = avl_nodo_insertar(arbol->raiz, dato, arbol->copia, arbol->comp);
}

/**
 * avl_validar: Retorna 1 si el arbol cumple la propiedad de los arboles AVL,
 * y 0 en caso contrario.
 * avl_nodo_validar_abb: Funcion interna que retorna 1 si el arbol cumple la
 * propiedad de los arboles BB, y 0 en caso contrario.
 * avl_nodo_validar_altura_y_balance: Funcion interna que retorna 1 si la
 * altura y el factor de balance de los nodos son correctos, y 0 en caso
 * contrario.
 */
static int avl_nodo_validar_abb(AVL_Nodo* raiz, void* min, void* max,
  FuncionComparadora comp) {
  // si la raiz es vacia, retornar exitosamente
  if (raiz == NULL)
    return 1;
  else {
    // sino, validar intervalo
    if (min != NULL && comp(raiz->dato, min) <= 0)
      return 0;
    if (max != NULL && comp(max, raiz->dato) <= 0)
      return 0;
    // y validar subarboles recursivamente
    return (avl_nodo_validar_abb(raiz->izq, min, raiz->dato, comp) &&
      avl_nodo_validar_abb(raiz->der, raiz->dato, max, comp));
  }
}
static int avl_nodo_validar_altura_y_balance(AVL_Nodo* raiz) {
  // si la raiz es vacia, retornar exitosamente
  if (raiz == NULL)
    return 1;
  // sino, validar subarboles recursivamente
  int ret1 = avl_nodo_validar_altura_y_balance(raiz->izq);
  int ret2 = avl_nodo_validar_altura_y_balance(raiz->der);
  if (ret1 && ret2) {
    // si ambos subarboles son validos, validar altura y balance de raiz
    int altura = 1 + avl_nodo_max_altura_hijos(raiz);
    int balance = avl_nodo_factor_balance(raiz);
    if ((raiz->altura == altura) && (balance >= -1) && (balance <= 1))
      return 1;
  }
  // en cualquier otro caso, retornar falso
  return 0;
}
int avl_validar(AVL arbol) {
  return (avl_nodo_validar_altura_y_balance(arbol->raiz) &&
    avl_nodo_validar_abb(arbol->raiz, NULL, NULL, arbol->comp));
}

/**
 * avl_recorrer: Recorrido DSF del arbol
 */
static void avl_nodo_recorrer(AVL_Nodo* raiz, AVLRecorrido orden,
  FuncionVisitanteExtra visita, void* extra) {
  if (raiz != NULL) {
    if (orden == AVL_RECORRIDO_PRE)
      visita(raiz->dato, extra);
    avl_nodo_recorrer(raiz->izq, orden, visita, extra);
    if (orden == AVL_RECORRIDO_IN)
      visita(raiz->dato, extra);
    avl_nodo_recorrer(raiz->der, orden, visita, extra);
    if (orden == AVL_RECORRIDO_POST)
      visita(raiz->dato, extra);
  }
}
void avl_recorrer(AVL arbol, AVLRecorrido orden, FuncionVisitanteExtra visita,
  void* extra) {
  avl_nodo_recorrer(arbol->raiz, orden, visita, extra);
}

/**
 * avl_balancear: balancea el avl dado
 */
static AVL_Nodo* avl_balancear_arbol(AVL_Nodo* raiz){
  int balance= avl_nodo_factor_balance(raiz);
  if(balance > 1){
    if (avl_nodo_factor_balance(raiz->izq < 0))//caso rotacion doble
      raiz->izq=avl_nodo_rotacion_simple_izq(raiz->izq);
    return avl_nodo_rotacion_simple_der(raiz);
  }
  if (balance < -1)
  {
    if(avl_nodo_factor_balance(raiz->der)> 0)//caso rotacion doble
     raiz->der=avl_nodo_rotacion_simple_der(raiz->der);
    return avl_nodo_rotacion_simple_izq(raiz);
  }
  return raiz;
}

AVL avl_balancear(AVL arbol){
  return avl_balancear_arbol(arbol->raiz);
}


/**
 * avl_eliminar: Elimina el dato indicado en el avl, manteniendo la condicion de AVL
 */
AVL max_min(AVL_Nodo* raiz){
  if (raiz->izq == NULL ) return raiz ; 
  return max_min(raiz->izq) ; 
}
static AVL_Nodo* avl_eliminar_dato(AVL_Nodo* raiz, FuncionComparadora comp, 
  FuncionDestructora destr, void* dato){
  if (raiz==NULL)
    return NULL;
  else if (comp(raiz->dato, dato)<0)
    avl_eliminar_dato(raiz->der, comp, destr, dato);
  else if (comp(raiz->dato, dato)>0)
    avl_eliminar_dato(raiz->izq, comp, destr, dato);
  else{
    if (!raiz->izq || !raiz->der)
    {
      AVL_Nodo* temp= raiz->izq ? raiz->izq : raiz->der;
      destr(raiz->dato);
      free(raiz);
      return temp;
    }
    else{
      AVL_Nodo* sucesor=max_min(raiz->der);
      void* dato_temp=raiz->dato;
      raiz->dato=sucesor->dato;
      sucesor->dato=dato_temp;
      raiz->der=avl_eliminar_dato(raiz->der,comp,destr,sucesor->dato);
    }
  }
  return avl_balancear_arbol(raiz);
}
AVL avl_eliminar(AVL arbol, void* dato){
  return avl_eliminar_dato(arbol->raiz,arbol->comp,arbol->destr,dato);
}
/**
 * avl_obtener_dato: retorna el puntero del dato que se busca
 */
void* avl_obtener_dato(AVL_Nodo* nodo, void* dato, FuncionComparadora comp) {
  if (nodo == NULL) return NULL;

  int c = comp(dato, nodo->dato);
  if (c == 0) return nodo->dato;
  else if (c < 0) return avl_obtener_dato(nodo->izq, dato, comp);
  else return avl_obtener_dato(nodo->der, dato, comp);
}
void* avl_obtener(AVL arbol, void* dato) {
  return avl_obtener_dato(arbol->raiz, dato, arbol->comp);
}
