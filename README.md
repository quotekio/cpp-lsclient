cpp-lsclient 0.2
================

cpp-lsclient is a C++ implementation of a Lightstreamer Client.
So far it only implements a subset of Lightstreamer client features but it already
works fine for objects subscribing and receiving data. cpp-lsclient is built arround libcurl and POSIX threads, and makes extensive use of std::vector data structures because of their pretty good performance ( O(1) accessors, O(n) modifiers).

Changelog
---------

### 0.2:

* Bugfix: processing of multiple rows in a single payload
* Auto rebinding of the stream connection in case of closing ( reached Content-Length, etc..)
* Replacement of POSIX threads usage by C++11 std::thread for easier portability (windows)
* Improved documentation

Dependancies
------------

* libcurl4-dev with TLS/SSL Support (OpenSSL/GNUTLS/..)

* cmake 2.6+

* any fully C++11 compliant compiler (clang++, g++ 4.7+)

Status
------

* Features: ~50%

* Doc: 60%

* Unit Tests: 0% (Code Coverage: 0%)


Compile & Install [Linux/BSD/MacOS]
-----------------------------------

Clone this repository with the following command:

```shell
git clone http://github.com/digination/cpp-lsclient
```
Then, create a new directory for building puproses:

```Shell
mkdir cpp-lsclient-build

cd cpp-lsclient-build

cmake ../cpp-lsclient

make && make install.
```

Compile & Install [Windows]
---------------------------

Building cpp-lsclient on windows platforms can be a bit more tricky.

* Your first option (and best bet) is to install a cygwin environment on your windows machine,
and select the following packages during setup: cmake, libcurl, gcc, g++.
Then, you just have to follow the Linux compile and install intructions above. 

* The second option is to install the libcurl windows port ( can be found at http://curl.haxx.se/download.html) and build the LSClient library in your favorite IDE.


Sample Usage
------------

A sample App is provided along with the library in order to let you test it and learn how it works.

Here is a hint about how to use it:

```
./sample MyIGAccount 'CST-6bc9d12be108b8a4216e68c0b4b08e04db87f3b4afd2328bac14c9|XST-6895c30224df50889c1ac9cc5263e9864d0cc96404' 'https://demo-apd.marketdatasystems.com'
```



Usage Example
-------------

```C++

#include <lsclient/lsclient.hpp>

//instanciates new LSClient object with connection parameters
LSClient* lsc = new LSClient(ls_url, ls_username, ls_password);


//Adds a new subscription with 2 items to the client.
lsc->addSubscription(new LSSubscription("MARKET", 
                                        {"MARKET:IX.D.CAC.IMF.IP", 
                                         "MARKET:IX.D.DAX.IMF.IP"},
                                        {"BID","OFFER"}));


//starts stream connection thread
lsc->start();

//waits until LSClient stream is connected.
while ( lsc->getStatus() != LS_STATUS_CONNECTED ) {
  sleep(1);
}


//Sends added subscriptions to Lightstreamer control connection.
lsc->subscribeAll();

//gets back data
LSTable* t = lsc->getSubscriptions()->at(0)->table_ref;


}
```

TODO
----

* Replacement of libcurl by cpp-netlib in order to be fully portable (coming soon)
* Implementation of unit and functional tests.
* Full Lightstreamer protocol support.
* Improve LSTable data accessor to be more easy/convenient.
