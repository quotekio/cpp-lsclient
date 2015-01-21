#ifndef HTTP_H
#define HTTP_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string>
#include "assoc.hpp"

class http {

  private:
  	CURL* ch;
  	struct curl_slist *headers;
    size_t (*write_cb)(void*,size_t,size_t, void*);
    void* write_data;

  public:
  	http();
  	void destroy();
  	void add_header(std::string);
  	std::string get(std::string url);
  	std::string post(std::string,std::string);
  	std::string post(std::string url,AssocArray<std::string> post_data);
    void post2(std::string url,AssocArray<std::string> post_data);
  	std::string del(std::string url);
    CURL* get_curl_handler();

};

#endif