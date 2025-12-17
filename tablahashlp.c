#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>
#define FACTOR_CARGA(numElms, casillas) ((float)(numElms)/(casillas))
#define LIMITE 0.7
//IMPLEMENTACION DE TABLA HASH CON DIRECCIONAMIENTO ABIERTO (LINEAL PROOBING).//
/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void *dato;
  int estado; // 0 si casilla está libre, 1 si está ocupada, -1 si fue eliminada.
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
                          FuncionHash hash)
{
  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla);
  tabla->elems = malloc(sizeof(CasillaHash)*capacidad);
  assert(tabla->elems);
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;
  tabla->numElems = 0;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
  {
    tabla->elems[idx].dato = NULL;
    tabla->elems[idx].estado = 0;
  }
  
  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) {return tabla->numElems;}

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla){return tabla->capacidad;}

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla){
  for (int idx = 0; idx < tabla->capacidad; ++idx)
  {
    if (tabla->elems[idx].dato != NULL)
      tabla->destr(tabla->elems[idx].dato);
  }
  free(tabla->elems);
  free(tabla);
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
static void tablahash_insertar_aux(TablaHash tabla, void* dato, unsigned idx,
                                                                    int indice_borrado)
{
  CasillaHash* casilla = &tabla->elems[idx];
  // Caso en que la casilla esté vacia.
  if (casilla->estado == 0)
  {
    unsigned idx_to_put = (indice_borrado != -1) ? indice_borrado : idx; //verifico si encontre una posicion eliminada.
    tabla->elems[idx_to_put].dato = tabla->copia(dato);
    tabla->elems[idx_to_put].estado = 1;
    tabla->numElems++;
    return;
  }
  //Caso en que la casilla ya esté ocupada por un dato repetido.
  if (casilla->estado == 1 && tabla->comp(casilla->dato,dato) == 0)
  {
    tabla->destr(casilla->dato);
    casilla->dato = tabla->copia(dato);
    return;
  }
  //Caso en que la casilla haya sido eliminada y sea la primera encontrada.
  if (casilla->estado == -1 && indice_borrado == -1)
    indice_borrado = idx;
  
  unsigned nuevo_idx = (idx + 1) % tabla->capacidad;
  tablahash_insertar_aux(tabla, dato, nuevo_idx,indice_borrado);
}
void tablahash_insertar(TablaHash tabla, void *dato){
  if (FACTOR_CARGA(tabla->numElems,tabla->capacidad) > LIMITE)
    tablahash_redimensionar(tabla);

  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  
  tablahash_insertar_aux(tabla, dato, idx, -1);
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
static void* tablahash_buscar_aux(TablaHash tabla, void* dato, unsigned indice, unsigned indice_incio){
  //Caso en que la casilla esta vacia, el dato no se encuentra.
  if (tabla->elems[indice].estado == 0)
    return NULL;
  //Caso en que haya encontrado el dato
  if (tabla->elems[indice].estado == 1 && tabla->comp(tabla->elems[indice].dato,dato) == 0)
    return tabla->elems[indice].dato;
  
  indice = (indice + 1) % tabla->capacidad;
  if (indice == indice_incio)//Verifico que no este en el mismo indice donde comence.
    return NULL;
  
  return tablahash_buscar_aux(tabla,dato,indice,indice_incio);    
}
void *tablahash_buscar(TablaHash tabla, void *dato){
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  return tablahash_buscar_aux(tabla, dato, idx, idx);
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
static void tablahash_eliminar_aux(TablaHash tabla, void* dato, unsigned indice, unsigned indice_incio){
  //Caso en que la casilla estaba vacia.
  if (tabla->elems[indice].estado == 0)
    return;
  //Caso en que encuentra al dato a eliminar.
  if (tabla->elems[indice].estado == 1 && tabla->comp(tabla->elems[indice].dato,dato) == 0){
    tabla->destr(tabla->elems[indice].dato);
    tabla->elems[indice].dato = NULL;
    tabla->elems[indice].estado = -1;
    tabla->numElems--;
    return;
  }
  indice = (indice + 1) % tabla->capacidad;
  if (indice == indice_incio)//Verifico no volver a empezar.
    return;
  
  tablahash_eliminar_aux(tabla,dato,indice,indice_incio); 
}
void tablahash_eliminar(TablaHash tabla, void *dato){
  unsigned indice = tabla->hash(dato) % tabla->capacidad;
  tablahash_eliminar_aux(tabla,dato,indice,indice);
}

/**
 * Duplica la capacidad de la tabla y reposiciona todos los elementos de acuerdo a la nueva
 * posicion que le asigne la funcion de hash.
 */
void tablahash_redimensionar(TablaHash tabla){
  //Guardo la informacion de la tabla.
  CasillaHash* elmsAnteriores = tabla->elems;
  unsigned capacidadAnterior = tabla->capacidad;
  unsigned numelmsAnteriores = tabla->numElems;
  //inicializo las nuevas casillas de la tabla redimensionada.
  tabla->capacidad*=2;
  CasillaHash* nuevaTabla = malloc(sizeof(CasillaHash) * tabla->capacidad);
  assert(nuevaTabla);
  for (unsigned i = 0; i < tabla->capacidad; i++)
  {
    nuevaTabla[i].dato = NULL;
    nuevaTabla[i].estado = 0;
  }
  tabla->numElems = 0;
  //Reubico los elementos anteriores en la nueva tabla.
  for (unsigned i = 0; i < capacidadAnterior; i++)
  {
    if (elmsAnteriores[i].dato != NULL)
    {
      void* dato = elmsAnteriores[i].dato;
      unsigned h = tabla->hash(dato) % tabla->capacidad;
      unsigned idx = h;
      unsigned j = 0;
      while (nuevaTabla[idx].estado == 1) {
        j++;
        idx = (h + j) % tabla->capacidad;
      }
    nuevaTabla[idx].dato = dato; // No se copia, ya existe
    nuevaTabla[idx].estado = 1;
    tabla->numElems++;
  }
  }
  free(elmsAnteriores);
  tabla->elems = nuevaTabla;
}