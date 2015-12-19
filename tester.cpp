#include "crimen.h"
#include "fecha.h"
#include "css.h"
#include <iostream>
#include <fstream>
using namespace std;
int main(){
    css database;
    database.load("crimenes.csv"); //QUITAR Define DEBUG si no se quiere DEBUG (css.hxx)

    cout << " -----------------------testing iteradores-----------------------" << endl;

    cout << "--CSS::ITERATOR--" << endl;
    css::iterator re;
    re=database.begin();
    for(;re != database.end();re++)
      cout << "(ORDEN ID) "<< (*re).first << " " << (*re).second << endl;

    cout << "--CSS::IUCR_iterator--" << endl;
    css::IUCR_iterator iui;
    iui=database.ibegin();
    for(;iui!=database.iend();iui++){
      cout << "(Orden IUCR) " << (*iui).second << endl;
    }

    cout << "--CSS::Date_iterator--" << endl;
    css::Date_iterator di;
    di=database.dbegin();
    for(;di!=database.dend();di++){
    cout << "(Orden FECHA) " << (*di).second << endl;
    }
    cout << "-------------------------" << endl;

    cout << "\n------find_ID----10231612-------" << endl;
    css::iterator buscar = database.find_ID(10231612);
    if(buscar!=database.end()){
      cout << "ECONTRADO " << (*buscar).second << endl;
    }
    else
      cout << "NO ENCONTRADO" << endl;

      cout << "\n------find_ID----123456-------" << endl;
      buscar = database.find_ID(123456);
      if(buscar!=database.end()){
        cout << "ECONTRADO " << (*buscar).second << endl;
      }
      else
        cout << "NO ENCONTRADO" << endl;

    cout << "\n\n -----set Arrest-----" << endl;
    cout << "antes " << (*database.begin()).second << endl;
    cout << "arrestado? " << (*database.begin()).second.getArrest() << endl;
    database.setArrest((*database.begin()).second.getID(),false);
    cout << "despues " << (*database.begin()).second << endl;
    cout << "arrestado? " << (*database.begin()).second.getArrest() << endl;


    cout << "\n\n -----lower_bound--IUCR 0337---" << endl;
    css::IUCR_iterator lb;
    lb = database.lower_bound("0337");
    cout << (*lb).second << endl;

    cout << "\n\n -----upper_bound--IUCR 0337---" << endl;
    css::IUCR_iterator ub;
    ub = database.upper_bound("0337");
    cout << (*ub).second << endl;

    cout << "\n\n -----upper_bound--FECHA " << (*ub).second.getDate() <<  endl;
    fecha mifecha;
    mifecha = (*ub).second.getDate();
    css::Date_iterator dil;
    dil=database.upper_bound(mifecha);
    cout << (*dil).second << endl;

    cout << "\n\n -----lower_bound--FECHA " << (*ub).second.getDate() <<  endl;
    css::Date_iterator dilb;
    dilb=database.lower_bound(mifecha);
    cout << (*dilb).second << endl;

    cout << "\n\n -----ERASE--ID 10231612" <<  endl;
    cout << "antes borrar: " << endl;
    database.mostrarTamanios();
    bool borrado = database.erase(10230979);
    if(borrado){
      cout << "BORRADO CON EXITO" << endl;
      database.mostrarTamanios();
    }
    else
      cout << "No se ha podido borrar" << endl;




}
