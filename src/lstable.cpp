/*
This is a Quick'n'Dirty C++ implementation of LightStreamer Client
Copyright(c) 2015 Clément Gamé.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS 
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE,EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "lstable.hpp"

std::vector<LSTable*> LSTable::table_list;

LSTable::LSTable(int nb_items, int nfields) {

  for (int i=0; i< nb_items;i++ ) {
    std::vector<std::string> v;
    data.push_back(v);
  }

  nb_fields = nfields;
}

std::vector< std::vector<std::string> >* LSTable::getData() {
  return &data;
}

std::vector<std::string>* LSTable::getItemData(int item_num) {
  return &data[item_num];
}

int LSTable::appendData(int item_num, std::vector<std::string> item_data) {
  if ( item_data.size() != nb_fields ) return 1;

  /*
  //debug
  for (int i=0;i<item_data.size();i++) {
    if ( item_data[i] == "" ) cout << "WARNING: item value " << i << " is NULL" << endl;
  }
  */

  data[item_num] = item_data;
  return 0;
}


/* STATIC METHODS */

int LSTable::getTableSequence() {
  return LSTable::table_list.size() -1;
}

LSTable* LSTable::getTable(int tnum) {

  if ( tnum >= LSTable::table_list.size() ) {
    return NULL;
  }
  return LSTable::table_list[tnum];
}

LSTable* LSTable::addTable(int nb_items, int nb_fields) {

  	LSTable* t = new LSTable(nb_items, nb_fields);
  	LSTable::table_list.push_back(t);
    return t;
}

int LSTable::removeTable(int tnum) {

  if ( tnum >= LSTable::table_list.size() ) {
    return 1;
  }

  LSTable::table_list.erase(LSTable::table_list.begin() + tnum);
  return 0;
}

int LSTable::append(int tnum, int item_num, std::vector<std::string> item_data) {
  LSTable* s = LSTable::getTable(tnum);
  if (s!= NULL) {
    return s->appendData(item_num,item_data);
  }

  return 1;
}




