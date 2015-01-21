#include "src/lsclient.hpp"
#include <iostream>
#include <string>

void usage(char* prgname) {

  cout << "Usage: " << prgname << " <username> <password> <lightstreamer_url>" << endl;
}

int main(int argc, char** argv) {

  if ( argc != 4) {
    usage(argv[0]);
    exit(1);
  }

  std::string ls_username = std::string(argv[1]);
  std::string ls_password = std::string(argv[2]);
  std::string ls_url = std::string(argv[3]);

  //instanciates new LSClient object with connection parameters
  LSClient* lsc = new LSClient(ls_url, ls_username, ls_password);

  //Adds a new subscription with 2 items to the client.
  lsc->addSubscription(new LSSubscription("MARKET", 
                                          {"MARKET:IX.D.CAC.IMF.IP", 
                                           "MARKET:IX.D.DAX.IMF.IP"},
                                          {"BID","OFFER"}));

  cout << "[LSCLIENT] Connecting to LightStreamer.." ;

  //starts stream connection thread
  lsc->start();

  //waits until LSClient stream is connected.
  while ( lsc->getStatus() != LS_STATUS_CONNECTED ) {
    cout << "." ;
    sleep(1);
  }

  cout << "[OK]" << endl;

  //Sends added subscriptions to Lightstreamer control connection.
  lsc->subscribeAll();

  //fetches data
  while(1) {
  
    std::vector<LSSubscription*>* subscriptions = lsc->getSubscriptions();

    for (int i=0;i< subscriptions->size();i++ )  {
      //LSTables are the objects containing the LS Data
      LSTable* t = subscriptions->at(i)->table_ref;
      for (int j=0;j< subscriptions->at(i)->getObjectIds().size(); j++ ) {
        vector<string>* values = t->getItemData(j);

        if (values->size() > 1 ) {
          cout << "ITEM:" <<  subscriptions->at(i)->getObjectIds()[j] << endl;
          cout << "BID:" << values->at(0) << endl;
          cout << "OFFER:" << values->at(1) << endl;
        }
      }
    }
    sleep(1);
  }
}
