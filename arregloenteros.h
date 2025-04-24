#ifndef __ARREGLOENTEROS_H__
#define __ARREGLOENTEROS_H__
typedef struct{
    int* direccion;
    int capacidad;
}ArregloEnteros;

/**
 * Devuelve un arreglo vacío.
 */
ArregloEnteros* crear_arreglo(int size);

/**
 * Destruye el arreglo.
 */
void destruir_arreglo(ArregloEnteros* arreglo);

/**
 * Agrega el dato ingresado a la posicion indicada.
 */
ArregloEnteros* arreglo_agregar_dato(ArregloEnteros* arreglo, int dato,int pos);

/**
 * Duplica el tamaño del arreglo.
 */
ArregloEnteros* arreglo_ajustar(ArregloEnteros* arreglo,int size);

/**
 * Recorre e imprime los datos del arreglo de enteros.
 */
void arreglo_enteros_imprimir(ArregloEnteros* arreglo);
#endif /* __ARREGLOENTEROS_H__ */