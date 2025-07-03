#ifndef __BTREE_H__
#define __BTREE_H__

typedef void (*FuncionVisitante2)(int dato);
typedef void (*FuncionVisitanteExtra) (int dato, void *extra);
typedef int (*FuncionComparadora) (int dato1, int dato2);
typedef enum {
  BTREE_RECORRIDO_IN,
  BTREE_RECORRIDO_PRE,
  BTREE_RECORRIDO_POST
} BTreeOrdenDeRecorrido;

struct _BTNodo {
  int dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
};

typedef struct _BTNodo *BTree;

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear();

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo);

/**
 * Indica si el árbol es vacío.
 */
int btree_empty(BTree nodo);

/**
 * Crea un nuevo arbol, con el dato dado en el nodo raiz, y los subarboles dados
 * a izquierda y derecha.
 */
BTree btree_unir(int dato, BTree left, BTree right);


/**
* Recorrido del arbol, utilizando la funcion pasada.
*/
void btree_recorrrer_in(BTree arbol, FuncionVisitante2 visit);
void btree_recorrrer_pre(BTree arbol, FuncionVisitante2 visit);
void btree_recorrrer_post(BTree arbol, FuncionVisitante2 visit);

void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden,FuncionVisitante2 visit);

int btree_nnodos(BTree arbol);

int btree_buscar(BTree arbol, int dato);

BTree btree_copiar(BTree arbol);

int btree_altura(BTree arbol) ;

int btree_nnodos_profundidad(BTree arbol , int p);

int btree_profundidad(BTree arbol , int dato );

int btree_sumar(BTree arbol);

void btree_recorrer_extra(BTree arbol, BTreeOrdenDeRecorrido orden,
FuncionVisitanteExtra visit, void *extra);

//void btree_bfs_iterativo(BTree arbol, FuncionCopia copy, FuncionDestructora destroy, FuncionVisitante2 visit);

BTree btree_mirror(BTree arbol);

int contar_nodos(BTree arbol);

int btree_validar_completo(BTree arbol);

BTree btree_a_bstree(BTree arbol);
#endif /* __BTREE_H__ */
