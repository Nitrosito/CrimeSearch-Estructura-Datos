#include <iostream>
#include <fstream>
using namespace std;

void css::load(string nombreDB){
  ifstream fe;
  string cadena;
  cout << "Abrimos" << nombreDB << endl;
  fe.open(nombreDB.c_str(),ifstream::in);
  if (fe.fail()){
    cerr << "Error al abrir el fichero " << nombreDB << endl;
  }
  else{
    getline(fe,cadena,'\n'); //leo la cabecera del fichero
    crimen aux;
    int i = 0;
     while ( /*!fe.eof()*/i<10 ){
        getline(fe,cadena,'\n');
        	if (!fe.eof()){
             aux.setCrimen(cadena);
            //cout << cadena << endl;
             insert(aux);
             i++;
          }
      }
  }
   fe.close();
}

void css::insert( const crimen & x){
   baseDatos.insert(pair<ID,crimen>(x.getID(),x));
}
