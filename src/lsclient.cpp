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

#include "lsclient.hpp"
#include "http.hpp"

LSClient::LSClient(std::string url, 
    		       std::string username, 
    		       std::string password) {

  ls_endpoint = url;
  ls_control_endpoint = url;
  ls_username = username;
  ls_password = password;

  ls_status = LS_STATUS_INITIALIZED;
  ls_adapter_set = "";

}

LSClient::LSClient(std::string url, 
               std::string username, 
               std::string password,
               std::string adapter) {

  ls_endpoint = url;
  ls_control_endpoint = url;
  ls_username = username;
  ls_password = password;
  ls_adapter_set = adapter;

  ls_status = LS_STATUS_INITIALIZED;

}



void LSClient::start()  {
  pthread_create(&stream_thread, NULL, LSClient::streamThreadWrapper, this);
}

void* LSClient::streamThreadWrapper(void* lsc) {
  static_cast<LSClient*>(lsc)->connect();
  return NULL;
}

int LSClient::connect() {

  //creates a new LS session
  http* req = new http();
  AssocArray<string> pdata;
  pdata["LS_user"] = ls_username ;
  pdata["LS_password"] = ls_password ;
  if (ls_adapter_set != "") pdata["LS_adapter_set"] = ls_adapter_set;

  std::string create_session_url = ls_endpoint + "/lightstreamer/create_session.txt";  

  //sets stream callback
  CURL* ch = req->get_curl_handler();
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, &LSClient::streamCallbackWrapper);
  curl_easy_setopt(ch,CURLOPT_WRITEDATA, this);
  req->post2(create_session_url, pdata);

  return 0;

}


int LSClient::subscribe(int subscription_index, 
                        std::string table, 
                        std::string operation, 
                        std::string mode) {

  std::string subscribe_url = ( ls_control_endpoint.find("https://") == std::string::npos ) ? "https://" : "" ;
  subscribe_url += ls_control_endpoint + "/lightstreamer/control.txt";

  int nb_errors = 0;

  if ( subscription_index > ls_subscriptions.size() + 1 ) return 1;

  LSSubscription* s = ls_subscriptions[subscription_index];

  http* req2 = new http();

  AssocArray<std::string> pdata;
  pdata["LS_session"] = ls_session_id;
  pdata["LS_op"] = operation;
  pdata["LS_table"] = table;
  pdata["LS_schema"] = "";
  pdata["LS_id"] = "";
  pdata["LS_mode"] = mode;
  
  if (ls_adapter_set != "") pdata["LS_adapter_set"] = ls_adapter_set;

    for (int j=0;j< s->getFields().size();j++ ) {
      pdata["LS_schema"] += s->getFields()[j];
      pdata["LS_schema"] += "+";
    }

    for (int j=0;j< s->getObjectIds().size();j++ ) {
      pdata["LS_id"] += s->getObjectIds()[j];
      pdata["LS_id"] += "+";
    }
    
    std::string resp = req2->post(subscribe_url, pdata);
    trim(resp);

    if ( resp != "OK" ) {
      nb_errors++;
    }
  
  return nb_errors;

}

int LSClient::subscribeAll() {

  std::string subscribe_url = ( ls_control_endpoint.find("https://") == std::string::npos ) ? "https://" : "" ;
  subscribe_url += ls_control_endpoint + "/lightstreamer/control.txt";

  int nb_errors = 0;

  for (int i=0;i< ls_subscriptions.size() ;i++) {

    LSTable* s = LSTable::addTable(ls_subscriptions[i]->getObjectIds().size(),
                                   ls_subscriptions[i]->getFields().size());
    ls_subscriptions[i]->table_ref = s;

    http* req2 = new http();

    AssocArray<std::string> pdata;
    pdata["LS_session"] = ls_session_id;
    pdata["LS_op"] = "add";
    pdata["LS_table"] = int2string(LSTable::getTableSequence()) ;
    pdata["LS_schema"] = "";
    pdata["LS_id"] = "";
    pdata["LS_mode"] = "MERGE";

    if (ls_adapter_set != "") pdata["LS_adapter_set"] = ls_adapter_set;

    for (int j=0;j< ls_subscriptions[i]->getFields().size();j++ ) {
      pdata["LS_schema"] += ls_subscriptions[i]->getFields()[j];
      pdata["LS_schema"] += "+";
    }

    for (int j=0;j< ls_subscriptions[i]->getObjectIds().size();j++ ) {
      pdata["LS_id"] += ls_subscriptions[i]->getObjectIds()[j];
      pdata["LS_id"] += "+";
    }
    
    std::string resp = req2->post(subscribe_url, pdata);
    trim(resp);

    if ( resp != "OK" ) {
      nb_errors++;
    }
  }
  return nb_errors;
}


size_t LSClient::streamCallbackWrapper(void* ptr, size_t size, size_t nmemb, void* obj) {

  LSClient* lsc = static_cast<LSClient*>(obj);

  if (ptr != NULL) {
      std::string ls_stream(static_cast<const char*>(ptr), size * nmemb);

      if (lsc->getStatus() == LS_STATUS_CONNECTED || 
          lsc->getStatus() == LS_STATUS_RECEIVING ) {

        //debug
        //cout << ls_stream << endl;

        //actually parses data comming in stream connection
        if ( ls_stream.find("|") != std::string::npos )  {

          lsc->setStatus(LS_STATUS_RECEIVING);

          std::vector<std::string> values = split(ls_stream,'|');
          std::vector<std::string> values_map = split(values[0],',');

          int tnum = atoi(values_map[0].c_str());
          int indices_num = atoi(values_map[1].c_str()) - 1;

          values.erase(values.begin());
          LSTable::append(tnum, indices_num, values);


        } 

      }
      
      std::vector<std::string> resdata = split(ls_stream,'\n');
      for (int i=0;i<resdata.size();i++) {
        trim(resdata[i]);
      }

      if (  resdata[0] == "OK" ) {
        lsc->setStatus(LS_STATUS_CONNECTED);
        //parse Data
        for (int i=1;i<resdata.size();i++) {
          std::vector<std::string> parsed_line = split(resdata[i],':');

          if (parsed_line.size() > 1) {
            if (parsed_line[0] == "SessionId") {
              lsc->setSessionId(parsed_line[1]);
            }
            else if (parsed_line[0] == "ControlAddress") {
              lsc->setControlEndpoint(parsed_line[1]);
            } 
          }
        }
      }

    }
    return size*nmemb;
}


void LSClient::setSessionId(std::string sessid) {
  ls_session_id = sessid;
}

int LSClient::getStatus()  {
  return ls_status;
}

void LSClient::setStatus(int st) {
  ls_status = st;
}

void LSClient::setControlEndpoint(std::string ctl_endpoint) {
  ls_control_endpoint = ctl_endpoint;
}

int LSClient::addSubscription(LSSubscription* s) {
	ls_subscriptions.push_back(s);
	return 0;
}

int LSClient::remSubscription(std::string object_id) {
  return 0;
}

std::vector<LSSubscription*>* LSClient::getSubscriptions() {
  return &ls_subscriptions;
}




