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
            cout << cadena << endl;
             insert(aux);
             i++;
          }
      }
  }
  #if defined(DEBUG)
   cout << "Importados en map baseDatos "  << baseDatos.size() << endl;
   for(auto& it:baseDatos)
    cout << it.first << " " << it.second << endl;

   cout << "Importados en multimap DateAccess " << DateAccess.size() << endl;
   for(auto& it:DateAccess){
    cout << it.first << " " << endl;
   }

   cout << "Importados en map IUCRAccess " << IUCRAccess.size() << endl;
   for(auto& it:IUCRAccess){
     cout << "------------ \n" << it.first << " IUCR " << endl;
     for(auto& it2:it.second)
     cout << it2 << " ID" << endl;

     cout << "------------" << endl;
   }

  cout << "Importados en unordered_map index " << index.size() << endl;
  for(auto& it:index){
    cout << it.first << " Termino" <<endl;
    for(auto& it2:it.second)
      cout << it2 << " ID" << endl;
  }

  cout << "Importados en map posicionGeo" << posicionGeo.size() << endl;
  for(auto& it:posicionGeo){
    cout << it.first << endl;
    for(auto &it2:it.second)
    cout << it2.first << " " << it2.second << "ID" << endl;
  }


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
   IUCRAccess[x.getIucr()].insert(idcrimen);

   //Inserto en unordered_map
   string descripcion = x.getDescription();
   string s;
   int ini = 0;
   int fin = descripcion.find_first_of(" ");
   s = descripcion.substr(ini,fin);
   if(!s.empty()){
     //cout << s << endl;
     index[s].insert(idcrimen);
   }
   while(fin!=-1){
     ini = fin+1;
     fin = descripcion.find_first_of(" /:",ini);
     s = descripcion.substr(ini,fin-ini);
     if(s.size()>1){
      index[s].insert(idcrimen);
      //cout<< s<<endl;
    }
  }

  //Inserto en map Longitud
  pair<Latitud,ID> coor(x.getLatitude(),idcrimen);
  posicionGeo[x.getLongitude()].insert(coor);



}

//-----------------------------------------------------------------------------//


//-----------------------------------ITERATOR-----------------------------------//

//-----------------------------------------------------------------------------//
