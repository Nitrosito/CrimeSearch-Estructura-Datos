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

bool css::erase( unsigned int ID){
  crimen crimenborrar = baseDatos[ID];
  //borro de contenedor por IUCR
  if(!IUCRAccess.erase(crimenborrar.getIucr()))
    return false;

  //borro date access
  css::Date_iterator borrar;
  borrar.it_mm=DateAccess.find(crimenborrar.getDate());
  //Se podria hacer mas eficiente limitando la busqueda
  //desde el dia del crimen al siguiente...
  for(;borrar.it_mm!=DateAccess.end();borrar++){
    if((*borrar).first==ID){
      DateAccess.erase(borrar.it_mm);
    }
  }

  //borro de index
  string descripcion = crimenborrar.getDescription();
  string s;
  int ini = 0;
  int fin = descripcion.find_first_of(" ");
  s = descripcion.substr(ini,fin);
  if(!s.empty()){
    index[s].erase(crimenborrar.getID());
    if(index[s].size()==0)
      index.erase(s);
  }
  while(fin!=-1){
    ini = fin+1;
    fin = descripcion.find_first_of(" /:",ini);
    s = descripcion.substr(ini,fin-ini);
    if(s.size()>1){
      index[s].erase(crimenborrar.getID());
      if(index[s].size()==0)
        index.erase(s);
   }
  }

  //borro en posicionGeo
  posicionGeo[crimenborrar.getLongitude()].erase(crimenborrar.getLatitude());
  if(posicionGeo[crimenborrar.getLongitude()].size()==0)
    posicionGeo.erase(crimenborrar.getLongitude());

  //borro de baseDatos
  baseDatos.erase(ID);
}

list<ID> css::inArea(Longitud x1, Latitud y1, Longitud x2, Latitud y2){
  list<ID> res;
  map<Longitud,multimap<Latitud,ID>>::iterator it;
  multimap<Latitud,ID>::iterator ot;
  it = posicionGeo.lower_bound(x1);
  if(it!=posicionGeo.end()){
      ot =(*it).second.lower_bound(y1);
      if(ot!=(*it).second.end()){

        for(;(*it).first <= x2 && it!=posicionGeo.end() ;it++){
          ot =(*it).second.lower_bound(y1);
          for(;(*ot).first <= y2 && ot!=(*it).second.end();ot++){
              res.push_back((*ot).second);
          }
        }
    }
  }
  return res;
}

void css::mostrarTamanios(){
  cout << "Tamaño baseDatos: " << baseDatos.size() << endl;
  cout << "Tamaño DateAccess: " << DateAccess.size() << endl;
  cout << "Tamaño IUCRAccess: " << IUCRAccess.size() << endl;
  cout << "Tamaño index: " << index.size() << endl;
  cout << "Tamaño posicionGeo: " << posicionGeo.size() << endl << endl;
}

css::iterator css::begin(){
  css::iterator res;
  res.it=baseDatos.begin();
  return res;
}

css::iterator css::end(){
  css::iterator res;
  res.it=baseDatos.end();
  return res;
}

css::IUCR_iterator css::ibegin(){
  css::IUCR_iterator res;
  res.pcss=this;
  res.it_m=IUCRAccess.begin();
  res.it_s=res.it_m->second.begin();
  return res;
}

css::IUCR_iterator css::iend(){
  css::IUCR_iterator res;
  res.pcss=this;
  res.it_m=IUCRAccess.end();
  res.it_s=res.it_m->second.end();
  return res;
}

css::Date_iterator css::dbegin(){
  css::Date_iterator res;
  res.pcss=this;
  res.it_mm = DateAccess.begin();
  return res;
}

css::Date_iterator css::dend(){
  css::Date_iterator res;
  res.pcss=this;
  res.it_mm = DateAccess.end();
  return res;
}

css::iterator css::find_ID(const unsigned int ID){
  css::iterator res;
  res.it=baseDatos.find(ID);
  return res;
}

void css::setArrest(const unsigned int ID, bool value){
  baseDatos[ID].setArrest(value);
}

css::IUCR_iterator css::lower_bound(IUCR i){
  css::IUCR_iterator res;
  res.pcss =this;
  res.it_m= IUCRAccess.lower_bound(i);
  res.it_s= res.it_m->second.begin();
  return res;
}

css::IUCR_iterator css::upper_bound(IUCR i){
  css::IUCR_iterator res;
  res.pcss =this;
  res.it_m= IUCRAccess.upper_bound(i);
  res.it_s= res.it_m->second.begin();
  return res;
}

css::Date_iterator css::lower_bound(fecha i){
  css::Date_iterator res;
  res.pcss =this;
  res.it_mm= DateAccess.lower_bound(i);
  return res;
}

css::Date_iterator css::upper_bound(fecha i){
  css::Date_iterator res;
  res.pcss =this;
  res.it_mm= DateAccess.upper_bound(i);
  return res;
}

vector<pair<ID,float> > css::Query(list<string> & q, int k){
  pair<ID,float> inres;
  vector<pair<ID,float>> res;
  //UN TERMINO
  if(q.size()==1){
      unordered_map<Termino,set<ID>>::iterator primero;
      vector<pair<ID,float>> res;
      pair<ID,float> inres;

      primero=index.find((*q.begin()));
      set<ID>::iterator primerid;
      primerid = (*primero).second.begin();

      for(;primerid != (*primero).second.end(); primerid++){
        inres.first=(*primerid);
        inres.second=1.0;
        res.push_back(inres);
      }
      return res;
  }
  
}



//-----------------------------------------------------------------------------//


//-----------------------------------ITERATOR-----------------------------------//
bool css::iterator::operator!=(css::iterator in){
  if(it!=in.it)
    return true;

  return false;
}

bool css::iterator::operator==(css::iterator in){
  if(it==in.it)
    return true;

  return false;
}

pair<const ID,crimen>& css::iterator::operator*(){
  return *it;
}

css::iterator css::iterator::operator++(int){    //Post incremento
  css::iterator aux(*this);
  ++it;
  return aux;
}

css::iterator css::iterator::operator++(){    //Post incremento
  ++it;
  return *this;
}
//-----------------------------------------------------------------------------//



//-----------------------------IUCR_ITERATOR-----------------------------------//

bool css::IUCR_iterator::operator!=(css::IUCR_iterator in){
  if(it_s!=in.it_s && it_m!=in.it_m)
    return true;

  return false;
}

bool css::IUCR_iterator::operator==(css::IUCR_iterator in){
  if(it_s==in.it_s)
    return true;

  return false;
}

pair<const ID,crimen>& css::IUCR_iterator::operator*(){
  return *(pcss->baseDatos.find(*it_s));
}

css::IUCR_iterator css::IUCR_iterator::operator++(int){    //Post incremento
   css::IUCR_iterator aux(*this);
   it_s++;
   if(it_s==it_m->second.end()){
     it_m++;
     it_s= it_m->second.begin();
   }
   return aux;
}

css::IUCR_iterator css::IUCR_iterator::operator++(){    //Post incremento
  ++it_s;
  return *this;
}
//-----------------------------------------------------------------------------//


//------------------------------Date_iterator----------------------------------//
pair<const ID,crimen> &css::Date_iterator::operator*(){
  return *it_mm->second;
}

bool css::Date_iterator::operator!=(css::Date_iterator it){
  if( (* (*this) ).first != (*it).first )
    return true;

  return false;
}

bool css::Date_iterator::operator==(css::Date_iterator it){
  if( (* (*this) ).first == (*it).first )
    return true;

  return false;
}

css::Date_iterator css::Date_iterator::operator++(int){
  css::Date_iterator aux(*this);
  it_mm++;
  return *this;
}

css::Date_iterator css::Date_iterator::operator++(){
  it_mm++;
  return *this;
}

//-----------------------------------------------------------------------------//
