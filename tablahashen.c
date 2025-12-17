#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>
#define FACTOR_CARGA(numElms, casillas) ((float)(numElms)/(casillas))
#define LIMITE 0.7

typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);

typedef enum {
  AVL_RECORRIDO_IN,  /** Inorden */
  AVL_RECORRIDO_PRE, /** Preorden */
  AVL_RECORRIDO_POST /** Postorden */
} AVLRecorrido;

/**
 * Estructura del nodo del arbol AVL.
 * Tiene un puntero al dato (dato),
 * un puntero al nodo raiz del subarbol izquierdo (izq),
 * un puntero al nodo raiz del subarbol derecho (der), y
 * un entero para representar la altura del arbol (altura)
 */
typedef struct _AVL_Nodo {
  void* dato;
  struct _AVL_Nodo* izq, * der;
  int altura;
} AVL_Nodo;

/**
 * Estructura del arbol AVL.
 * Tiene un puntero al nodo raiz (raiz),
 * un puntero a funcion (copia) que recibe un dato y retorna un puntero con la
 * direccion de memoria de una copia fisica del dato,
 * un puntero a funcion (comp) que compara dos datos y retorna un entero
 * negativo si el primero es menor que el segundo, 0 si son iguales, y un entero
 * positivo en caso contrario,
 * y un puntero a una funcion (destr) que recibe un dato y lo destruye.
 * En esta implementación, los punteros a funcion necesarios para manipular los
 * datos se mantienen en la estructura para evitar pasarlos por parametro a las
 * demas funciones.
 */
struct _AVL {
  AVL_Nodo* raiz;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
};

typedef struct _AVL* AVL;

/**
 * Retorna un arbol AVL vacio
 */
AVL avl_crear(FuncionCopiadora copia, FuncionComparadora comp, FuncionDestructora destr){
  AVL arbol = malloc(sizeof(struct _AVL));
  assert(arbol);
  arbol->comp = comp;
  arbol->copia = copia;
  arbol->destr = destr;
  arbol->raiz = NULL;
  return arbol;
}

/**
 * Destruye el arbol y sus datos.
 */
static void avl_nodo_destruir(AVL_Nodo* raiz, FuncionDestructora destr){
  if (raiz != NULL)
  {
    avl_nodo_destruir(raiz->izq, destr);
    avl_nodo_destruir(raiz->der, destr);
    destr(raiz->dato);
    free(raiz);
  }
  
}
void avl_destruir(AVL arbol){
  avl_nodo_destruir(arbol->raiz,arbol->destr);
  free(arbol);
}

/**
 * Retorna 1 si el dato se encuentra y 0 en caso contrario
 */
static int avl_nodo_buscar(AVL_Nodo* raiz, FuncionComparadora comp, void* dato){
  if (raiz == NULL)
    return 0;
  else if (comp(raiz->dato, dato) == 0)
    return 1;
  else if (comp(raiz->dato, dato) > 0)
    return avl_nodo_buscar(raiz->izq, comp, dato);
  else
    return avl_nodo_buscar(raiz->der, comp, dato);
}
int avl_buscar(AVL arbol, void * dato){
  return avl_nodo_buscar(arbol->raiz, arbol->comp, dato);
}

/**
 * avl_nodo_altura: Funcion interna que retorna la altura del arbol.
 * La altura del arbol vacio se define como -1.
 */
static int avl_nodo_altura(AVL_Nodo* raiz) {
  return (raiz == NULL) ? -1 : raiz->altura;
}

/**
 * avl_nodo_max_altura_hijos: Funcion interna que retorna la maxima altura de
 * los hijos.
 */
static unsigned int avl_nodo_max_altura_hijos(AVL_Nodo* raiz) {
  assert(raiz);
  int alturaIzq = avl_nodo_altura(raiz->izq);
  int alturaDer = avl_nodo_altura(raiz->der);
  return (alturaIzq > alturaDer) ? alturaIzq : alturaDer; 
}

/**
 * avl_nodo_factor_balance: Funcion interna que retorna el factor de balance de
 * un nodo.
 */
static int avl_nodo_factor_balance(AVL_Nodo* raiz) {
  assert(raiz);
  int factor = avl_nodo_altura(raiz->der) - avl_nodo_altura(raiz->izq);
  assert(-2 <= factor || factor <= 2);
  return factor;
}

/**
 * avl_nodo_rotacion_simple_izq: Funcion interna que realiza una rotacion simple
 * a izquierda y retorna la nueva raiz.
 */
static AVL_Nodo* avl_nodo_rotacion_simple_izq(AVL_Nodo* raiz) {
  AVL_Nodo* hijoder = raiz->der;
  assert(hijoder);
  
  raiz->der = hijoder->izq;
  hijoder->izq = raiz;

  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoder->altura = 1 + avl_nodo_max_altura_hijos(hijoder);
  return hijoder; 
}

/**
 * avl_nodo_rotacion_simple_der: Funcion interna que realiza una rotacion simple
 * a derecha y retorna la nueva raiz.
 */
static AVL_Nodo* avl_nodo_rotacion_simple_der(AVL_Nodo* raiz) {
  AVL_Nodo* hijoizq = raiz->izq;
  assert(hijoizq);

  raiz->izq = hijoizq->der;
  hijoizq->der = raiz;

  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoizq->altura = 1 + avl_nodo_max_altura_hijos(hijoizq);
  return hijoizq;
}
/**
 * avl_balancear: balancea el avl dado
 */
static AVL_Nodo* avl_balancear_arbol(AVL_Nodo* raiz){
  int balance = avl_nodo_factor_balance(raiz);
  if (balance > 1)
  {
    if (avl_nodo_factor_balance(raiz->der) < 0)
      raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
    return avl_nodo_rotacion_simple_izq(raiz);
  }
  if (balance < -1)
  {
    if (avl_nodo_factor_balance(raiz->izq) > 0)
      raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
    return avl_nodo_rotacion_simple_der(raiz);
  }
  return raiz;
}
void avl_balancear(AVL arbol){
  arbol->raiz = avl_balancear_arbol(arbol->raiz);
}

/**
 * Inserta un dato no repetido en el arbol, manteniendo la propiedad de los
 * arboles AVL.
 */
static AVL_Nodo* avl_nodo_crear(void* dato, FuncionCopiadora copy){
  AVL_Nodo* nuevo_nodo = malloc(sizeof(AVL_Nodo));
  assert(nuevo_nodo);
  nuevo_nodo->dato = copy(dato);
  nuevo_nodo->altura = 0;
  nuevo_nodo->der = nuevo_nodo->izq =  NULL;
  return nuevo_nodo;
}
static AVL_Nodo* avl_nodo_insertar(AVL_Nodo* raiz, void* dato, 
  FuncionComparadora comp, FuncionCopiadora copy){
    if (raiz == NULL)
      return avl_nodo_crear(dato, copy);
    else if (comp(raiz->dato,dato) > 0)
      raiz->izq = avl_nodo_insertar(raiz->izq, dato, comp, copy);
    else if (comp(raiz->dato, dato) < 0)
      raiz->der = avl_nodo_insertar(raiz->der, dato, comp, copy);
    else
      return raiz;
    
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    
    return avl_balancear_arbol(raiz);
}
void avl_insertar(AVL arbol, void *dato){
  arbol->raiz = avl_nodo_insertar(arbol->raiz, dato, arbol->comp, arbol->copia);
}

/**
 * Retorna 1 si el arbol cumple la propiedad de los arboles AVL, y 0 en caso
 * contrario.
 */
static int avl_validar_abb(AVL_Nodo* raiz, FuncionComparadora comp, void* min, void* max){
  if (raiz == NULL)
    return 1;
  else
  {
    if (min != NULL && comp(raiz->dato, min) <= 0)
      return 0;
    if (max != NULL && comp(raiz->dato, max) >= 0)
      return 0;
    return (avl_validar_abb(raiz->izq, comp, min, raiz->dato)) && (avl_validar_abb(raiz->der, comp, raiz->dato, max));
  }
}
static int avl_validar_altura_balance(AVL_Nodo* raiz){
  if (raiz == NULL)
    return 1;
  
  int avlIzq = avl_validar_altura_balance(raiz->izq);
  int avlDer = avl_validar_altura_balance(raiz->der);
  if (avlIzq && avlDer)
  {
    int altura = 1 + avl_nodo_max_altura_hijos(raiz);
    int factor = avl_nodo_factor_balance(raiz);
    if(altura == raiz->altura && factor >= 1 && factor <=1)
      return 1;
  }
  return 0;
}
int avl_validar(AVL arbol){
  return (avl_validar_abb(arbol->raiz, arbol->comp, NULL, NULL)) && (avl_validar_altura_balance(arbol->raiz)); 
}

/**
 * Recorrido DSF del arbol
 */
static void avl_nodo_recorrer(AVL_Nodo* raiz, AVLRecorrido orden, FuncionVisitanteExtra visita, void* extra){
  if (raiz != NULL)
  {
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
void avl_recorrer(AVL arbol, AVLRecorrido orden, FuncionVisitanteExtra visita, void* extra){
  avl_nodo_recorrer(arbol->raiz, orden, visita, extra);
}

/**
 * avl_eliminar: Elimina el dato indicado en el avl, manteniendo la condicion de AVL
 */
static AVL_Nodo* avl_min(AVL_Nodo* raiz){
  if (raiz->izq == NULL)
    return raiz;
  return avl_min(raiz->izq);
}
static AVL_Nodo* avl_nodo_eliminar(AVL_Nodo* raiz, void* dato, FuncionComparadora comp, FuncionDestructora destr){
  if (raiz == NULL )
    return NULL;
  else if (comp(raiz->dato, dato) > 0)
    raiz->izq = avl_nodo_eliminar(raiz->izq, dato, comp, destr);
  else if (comp(raiz->dato,dato) < 0)
    raiz->der = avl_nodo_eliminar(raiz->der, dato, comp, destr);
  else{
    if (!raiz->izq || !raiz->der)
    {
      AVL_Nodo* temp = (raiz->izq) ?  raiz->izq : raiz->der;
      destr(raiz->dato);
      free(raiz);
      return temp;
    }
    else
    {
      AVL_Nodo* sucesor = avl_min(raiz->der);
      void* dato_temp = raiz->dato;
      raiz->dato = sucesor->dato;
      sucesor->dato = dato_temp;
      raiz->der = avl_nodo_eliminar(raiz->der, sucesor->dato, comp, destr);
    }
  }
  return avl_balancear_arbol(raiz);
}
AVL avl_eliminar(AVL arbol, void* dato){
  arbol->raiz = avl_nodo_eliminar(arbol->raiz, dato, arbol->comp, arbol->destr);
  return arbol;
}

/**
 * avl_obtener_dato: retorna el puntero del dato que se busca
 */
static void* avl_nodo_obtener(AVL_Nodo* raiz, FuncionComparadora comp, void* dato){
  if (raiz == NULL)
    return NULL;
  else if (comp(raiz->dato, dato) == 0)
    return raiz->dato;
  else if (comp(raiz->dato, dato) > 0)
    return avl_nodo_obtener(raiz->izq, comp, dato);
  else
    return avl_nodo_obtener(raiz->der, comp, dato);
}
void* avl_obtener(AVL arbol, void * dato){
  return avl_nodo_obtener(arbol->raiz, arbol->comp, dato);
}


//IMPLEMENTACION DE TABLA HASH CON ENCADENAMIENTO DE ESTRUCTURA AUXILIAR (AVL).//
/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  AVL casilla;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].casilla = avl_crear(copia, comp, destr);
  }

  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx){
    if (tabla->elems[idx].casilla != NULL)
      avl_destruir(tabla->elems[idx].casilla);
  }
  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {
  if(FACTOR_CARGA(tabla->numElems,tabla->capacidad) >= LIMITE)
    tablahash_redimensionar(tabla);
  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  AVL casilla = tabla->elems[idx].casilla;
  
  if (avl_buscar(casilla,dato))//verificar si hay colision.
  {
    avl_eliminar(casilla,dato);
    tabla->numElems--;
  }

  avl_insertar(casilla,dato);
  tabla->numElems++;
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  AVL casilla = tabla->elems[idx].casilla;
  // Retornar el dato de la casilla si hay concidencia.
  return avl_obtener(casilla,dato);
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  AVL casilla=tabla->elems[idx].casilla;
  if (avl_buscar(casilla,dato))//en caso de encontrarse el dato en la tabla.
  {
    avl_eliminar(casilla,dato);
    tabla->numElems--;
  }

}
/**
 * Duplica la capacidad de la tabla y reposiciona todos los elementos de acuerdo a la nueva
 * posicion que le asigne la funcion de hash.
 */
static void destr_sin_datos(AVL_Nodo* raiz){
  if (raiz != NULL)
  {
    destr_sin_datos(raiz->izq);
    destr_sin_datos(raiz->der);
    free(raiz);
  }
}
static void avl_insertar_sin_copiar(AVL arbol, void* dato);
static AVL_Nodo* avl_nodo_insertar_sin_copiar(AVL_Nodo* raiz, void* dato, FuncionComparadora comp);
static void tablahash_insertar_directo(TablaHash tabla, void* dato);
static void reinsertar_sin_copiar(void* dato, void* extra);

static void avl_insertar_sin_copiar(AVL arbol, void* dato) {
    arbol->raiz = avl_nodo_insertar_sin_copiar(arbol->raiz, dato, arbol->comp);
    avl_balancear(arbol);
}
static AVL_Nodo* avl_nodo_insertar_sin_copiar(AVL_Nodo* raiz, void* dato, FuncionComparadora comp) {
    if (!raiz) {
        AVL_Nodo* nuevo = malloc(sizeof(AVL_Nodo));
        nuevo->dato = dato;    
        nuevo->izq = nuevo->der = NULL;
        nuevo->altura = 0;
        return nuevo;
    }
    int c = comp(dato, raiz->dato);
    if (c < 0)
        raiz->izq = avl_nodo_insertar_sin_copiar(raiz->izq, dato, comp);
    else if (c > 0)
        raiz->der = avl_nodo_insertar_sin_copiar(raiz->der, dato, comp);
    else {
        // reemplazo: destruyo dato viejo y pongo nuevo puntero.
        raiz->dato = dato;
    }
    return raiz;
}
static void tablahash_insertar_directo(TablaHash tabla, void* dato) {
    unsigned idx = tabla->hash(dato) % tabla->capacidad;
    avl_insertar_sin_copiar(tabla->elems[idx].casilla, dato);
    tabla->numElems++;
}
static void reinsertar_sin_copiar(void* dato, void* extra) {
    TablaHash tabla = extra;
    tablahash_insertar_directo(tabla, dato);
}
void tablahash_redimensionar(TablaHash tabla) {
    unsigned cap_ant = tabla->capacidad;
    CasillaHash* viejos = tabla->elems;

    tabla->capacidad *= 2;
    tabla->elems = malloc(sizeof(CasillaHash) * tabla->capacidad);
    for (unsigned i = 0; i < tabla->capacidad; i++)
        tabla->elems[i].casilla = avl_crear(tabla->copia, tabla->comp, tabla->destr);

    unsigned num_ant = tabla->numElems;
    tabla->numElems = 0;

    for (unsigned i = 0; i < cap_ant; i++) {
        if (viejos[i].casilla) {
            avl_recorrer(viejos[i].casilla, AVL_RECORRIDO_IN, reinsertar_sin_copiar, tabla);
            destr_sin_datos(viejos[i].casilla->raiz);
            free(viejos[i].casilla);
        }
    }
    free(viejos);
    // assert(tablahash_nelems(tabla) == num_ant);
}


