#define DEBUG
#include <iostream>
#include <fstream>
using namespace std;

//--------------------------------CSS-----------------------------------------//

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
  #if defined(DEBUG)
   cout << "Importados en map baseDatos "  << baseDatos.size() << endl;
   cout << "Importados en multimap DateAccess " << DateAccess.size() << endl;
   cout << "Importados en map IUCRAccess " << IUCRAccess.size() << endl;


  cout << "Importados en unordered_map index " << index.size() << endl;


  #endif

   fe.close();

}

void css::insert( const crimen & x){
   unsigned int idcrimen = x.getID();
   pair<ID,crimen> crimenx(idcrimen,x);

   // Inserto crimen en el map ordenado por ID
   map<ID,crimen>::iterator it_insert = (baseDatos.insert(crimenx)).first;

   //Inserto en el multimap ordenado por fecha
   pair<fecha,map<ID,crimen>::iterator> crimenconfecha(x.getDate(),it_insert);
   DateAccess.insert(crimenconfecha);

   //Inserto en map ordenado por IUCR
   set<ID> sid;
   sid.insert(x.getID());
   pair<IUCR,set<ID>> iucrd(x.getIucr(),sid);
   IUCRAccess.insert(iucrd);

   //Inserto en unordered_map
   set<ID> usid;
   usid.insert(x.getID());
   pair<Termino,set<ID>> umap(x.getDescription,usid);
   index.insert()umap;

}

//-----------------------------------------------------------------------------//


//-----------------------------------ITERATOR-----------------------------------//

//-----------------------------------------------------------------------------//
