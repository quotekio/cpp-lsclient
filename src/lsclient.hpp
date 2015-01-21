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

#include <vector>
#include <string>
#include <pthread.h>
#include "lssubscription.hpp"
#include "lstable.hpp"
#include "utils.hpp"
#include "assoc.hpp"

#define LS_STATUS_INITIALIZED 0x01
#define LS_STATUS_CONNECTED 0x02
#define LS_STATUS_RECEIVING 0x03

#define LS_STATUS_CONNECT_ERROR 0x40


/** LSClient class is the main project Class.
 *
 */
class LSClient {

  public:

    /**
     * LSClient Object constructor 1.
     * @param url the lightstreamer server endpoint URL.
     * @param username the lightstreamer account username.
     * @param password the lightstreamer account password.
     * @return a new LSClient object.
     */
    LSClient(std::string url, 
    		     std::string username, 
    		     std::string password);

    /**
     * LSClient Object constructor 2.
     * @param url the lightstreamer server endpoint URL.
     * @param username the lightstreamer account username.
     * @param password the lightstreamer account password.
     * @param adapter the lightstreamer adapter set to use.
     * @return a new LSClient object.
     */
    LSClient(std::string url, 
             std::string username, 
             std::string password,
             std::string adapter);

    /** This method starts the Lightstreamer Stream Connection Thread.
     *  It starts LSClient::streamThreadWrapper asynchronously.
     */
    void start();

    /** This Initiates the Lightstreamer Stream Connection. If you plan to use this method manually,
     *  wrap it in a thread.
     */
    int connect();

    /** Sets the Lightstreamer Client status. It not meant to be used directly but needs to
     *  remain public.
     */
    void setStatus(int st);

    /** Retrieves the status of the LSClient connection.
     *  @return the status of LSClient as an integer.
     */
    int getStatus();
    int addSubscription(LSSubscription*);
    int remSubscription(std::string);
    std::vector<LSSubscription*>* getSubscriptions();

    int subscribe(int, 
                  std::string, 
                  std::string, 
                  std::string);
    
    /** Gives all the LSSubscriptions added to the LSClient object to Lightstreamer
     *  server control.
     *  @return 0 if subscriptions are ok, 1 otherwise.
     */

    int subscribeAll();

    //callback wrappers

    /** This is the main function of LSClient, the LS Stream connection
     *  parsing loop. 
     */
    static size_t streamCallbackWrapper(void*, size_t, size_t, void*);
    static void* streamThreadWrapper(void*);
    
    /** Sets the Lightstreamer Client Session ID.
     *  @param sessid The Sesion ID returned by the Stream connection.
     */
    void setSessionId(std::string);

    /** Sets the lightstream control endpoint ( if different from lightstreamer endpoint)
     *  @param ctl_endpoint
     */
    void setControlEndpoint(std::string);
    
  private:
    
  	std::string ls_endpoint;
    std::string ls_control_endpoint;
  	std::string ls_username;
  	std::string ls_password;
    std::string ls_session_id;
    std::string ls_adapter_set;
  	std::vector<LSSubscription*> ls_subscriptions;
    int ls_status;
    pthread_t stream_thread;
};