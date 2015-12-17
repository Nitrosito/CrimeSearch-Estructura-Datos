#include "crimen.h"
#include "fecha.h"
#include "css.h"
#include <iostream>
#include <fstream>
using namespace std;
int main(){
    css database;
    database.load("crimenes.csv");

    cout << " -----------------------testing iteradores-----------------------" << endl;

    cout << "--CSS::ITERATOR--" << endl;
    css::iterator re;
    re=database.begin();
    for(;re != database.end();re++)
      cout << (*re).first << " " << (*re).second << endl;

    cout << "--CSS::IUCR_ITERATOR--" << endl;
    css::IUCR_iterator iui;
    iui=database.ibegin();
    for(;iui!=database.iend();iui++){
      cout << "(Orden IUCR) " << (*iui).second << endl;
    }

}
