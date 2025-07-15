#ifndef __AVL_H__
#define __AVL_H__

typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);
typedef void (*FuncionDestructora)(void *dato);
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

int avl_balance;

/**
 * Retorna un arbol AVL vacio
 */
AVL avl_crear(FuncionCopiadora copia, FuncionComparadora comp, FuncionDestructora destr);

/**
 * Destruye el arbol y sus datos.
 */
void avl_destruir(AVL arbol);

/**
 * Retorna 1 si el dato se encuentra y 0 en caso contrario
 */
int avl_buscar(AVL arbol, void * dato);

/**
 * Inserta un dato no repetido en el arbol, manteniendo la propiedad de los
 * arboles AVL.
 */
void avl_insertar(AVL arbol, void *dato);

/**
 * Retorna 1 si el arbol cumple la propiedad de los arboles AVL, y 0 en caso
 * contrario.
 */
int avl_validar(AVL arbol);

/**
 * Recorrido DSF del arbol
 */
void avl_recorrer(AVL arbol, AVLRecorrido orden, FuncionVisitanteExtra visita, void * extra);

/**
 * avl_balancear: balancea el avl dado
 */
AVL avl_balancear(AVL arbol);

/**
 * avl_eliminar: Elimina el dato indicado en el avl, manteniendo la condicion de AVL
 */
AVL avl_eliminar(AVL arbol, void* dato);

/**
 * avl_obtener_dato: retorna el puntero del dato que se busca
 */
void* avl_obtener(AVL arbol, void* dato);
#endif /* __AVL_H__*/