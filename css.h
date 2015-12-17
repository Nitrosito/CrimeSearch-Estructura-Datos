/*
   *css.h
 */

#ifndef __CSS_H
#define __CSS_H
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>
#include "crimen.h"
/* 3 TIPOS DE DATOS
   iterator que nos permite iterar por los delitos en orden creciente de ID
   Date_iterator que nos permite iterar por los delitos en orden creciente de fecha, del más antiguo al más reciente.
   IUCR_iterator que nos permite iterar según el valor del IUCR
 */

typedef float Longitud;
typedef float Latitud;
typedef unsigned int ID;
typedef string Termino;
typedef string IUCR;

class css{
public:

  //-----------------------------iterator.--------------------------------------//
  class iterator {
  private:
  /* @brief it  itera sobre los ID del map
   */
  map<ID,crimen>::iterator it;
  public:
  pair<const ID, crimen > & operator*();
  bool operator!=(css::iterator it);
  bool operator==(css::iterator it);
  iterator operator++(int);
  iterator operator++();


  friend css;
  };
  //-----------------------------------------------------------------------------//




  //------------------------------IUCR_iterator----------------------------------//
  class IUCR_iterator {
  private:
  css *pcss;
  /* @brief it_m itera sobre los IUCR del map
   */
  map<IUCR,set<ID> >::iterator it_m;
  /* @brief it_s itera sobre los ID del set
   */
  set<ID>::iterator it_s;
  public:
  pair<const ID, crimen > & operator*();
  bool operator!=(css::IUCR_iterator it);
  bool operator==(css::IUCR_iterator it);
  IUCR_iterator operator++(int);
  IUCR_iterator operator++();

  friend css;
  };
  //-----------------------------------------------------------------------------//



  //------------------------------Date_iterator----------------------------------//
  class Date_iterator {
  private:
  multimap<fecha, map<ID,crimen>::iterator>::iterator it_mm;
  public:
  pair<const ID, crimen > & operator*();
  friend css;

  };
  //-----------------------------------------------------------------------------//



// METODOS CSS

/**
 * @brief Lee los elementos dado un fichero e inserta la informacion en la BD
 * @param nombreDB nombre del fichero
 */
void load(string nombreDB);

/**
 * @brief Inserta un nuevo crimen en la base de datos
 * @pre El crimen no debe estar almacenado en el conjunto
 * @param x Crimen a insertar
 */
void insert( const crimen & x);

/**
 * @brief Borra un crimen de la base de datos dado su ID, y todas sus referencias
 * @param ID del crimen a borrar
 * @return true si lo ha borrado, false si no
 */
bool erase( unsigned int ID);

/**
 * @brief Busca el crimen identificado por ID
 * @param ID del crimen a buscar
 * @return iterator a crimen encontrado. Si no encuentra devuelbe end()
 */
iterator find_ID(const unsigned int ID);

/**
 * @brief Modifica el campo arrest de un crimen
 * @param ID crimen a modificar
 * @param value  valor nuevo para arrest
 */
void setArrest(const unsigned int ID, bool value);

/**
 * @brief Dada una consulta, expresada mediante un conjunto de términos en q, el
 sistema devuelve un vector ordenado con los k mejores ID.
 * @param q conjunto de terminos
 * @param k cantidad de IDs a buscar
 * @return vector ordenado con los k mejores ID
 */
vector<pair<ID,float> > Query(list<string> & q, int k);

/**
 * @brief Busca delitos que se han cometido dentro de un area geometrica concreta
 * @param x1,y1,x2,y2 longitud y latitud para localizar el area
 * @return Lista de crimenes cometidos en esa area
 */
list<ID> inArea(Longitud x1, Latitud y1, Longitud x2, Latitud y2 );


/* Métodos relacionados con los iteradores */

/**
 * @return Iterador correspondiente al primer delito que se encuentra
 */
IUCR_iterator ibegin();

/**
 * @return Iterador correspondiente al primer delito que se encuentra
 */
Date_iterator dbegin();

/**
 * @return Iterador correspondiente al primer delito que se encuentra
 */
iterator begin();

/**
 * @return Iterador que apunta al elemento siguiente al ultimo delito en CSS segun
 el criterio sobre el que se itera
 */
Date_iterator dend();

/**
 * @return Iterador que apunta al elemento siguiente al ultimo delito en CSS segun
 el criterio sobre el que se itera
 */
IUCR_iterator iend();


/**
 * @return Iterador que apunta al elemento siguiente al ultimo delito en CSS segun
 el criterio sobre el que se itera
 */
iterator end();

/**
 * @brief Hacer busqueda por rango, considerando IUCR
 * @param IUCR de delitos a buscar
 * @return iterador que apunta al primer delito con IUCR mayor o igual a i
 */
IUCR_iterator lower_bound(IUCR i);

/**
 * @brief Hacer busqueda por rango, considerando IUCR
 * @param IUCR de delitos a buscar
 * @return iterador que apunta al ultimo delito con IUCR estrictamente mayor que d
 */
IUCR_iterator upper_bound(IUCR d);

/**
 * @brief Hacer busqueda por rango
 * @param i fecha de delitos a buscar
 * @return iterador que apunta al primer delito con Fecha mayor o igual a i
 */
Date_iterator lower_bound(fecha i);

/**
 * @brief Hacer busqueda por rango
 * @param d fecha de delitos a buscar
 * @return iterador que apunta al ultimo delito con fecha estrictamente mayor que d
 */
Date_iterator upper_bound(fecha d);





// Private CSS
private:
map<ID,crimen> baseDatos;
multimap<fecha, map<ID,crimen>::iterator> DateAccess;
map<IUCR,set<ID> > IUCRAccess;
unordered_map<Termino, set<ID> > index;
map<Longitud,multimap<Latitud, ID> > posicionGeo;
};

#include "css.hxx"
#endif
