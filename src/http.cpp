#include "http.hpp"

size_t curl_writehandle(void *ptr,
	                    size_t size,
	                    size_t nmemb,
	                    void* s)
{
  
    std::string &stream = *static_cast<std::string*>(s);

    if (ptr != NULL) {
      std::string temp(static_cast<const char*>(ptr), size * nmemb);
      stream += temp;
    }
    return size*nmemb;
}

CURL* http::get_curl_handler()  {
  return ch;
}

http::http() {
  ch = curl_easy_init();
  headers = NULL;
  write_cb = curl_writehandle;
}

void http::destroy() {
 curl_easy_cleanup(ch);
 delete this;
}

void http::add_header(std::string header) {
  headers = curl_slist_append(headers, header.c_str());
}

std::string http::get(std::string url) {

  std::string wdata = "";
  write_data = &wdata;
  curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
  curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,write_cb);
  curl_easy_setopt(ch,CURLOPT_WRITEDATA,write_data);
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  curl_easy_perform(ch);

  return wdata;
}


std::string http::post(std::string url,std::string pdata) {

  std::string wdata = "";
  curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
  curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_writehandle);
  curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);
  curl_easy_setopt(ch,CURLOPT_POST,1);
  curl_easy_setopt(ch,CURLOPT_POSTFIELDS,pdata.c_str());
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  curl_easy_perform(ch);

  return wdata;
 
}


std::string http::post(std::string url, AssocArray<std::string> post_data) {

  std::string wdata = "";
  std::string wpost = "";

  for(int i=0;i<post_data.Size();i++) {
    wpost += post_data.GetItemName(i) + "=" + post_data[i];
    if (i != post_data.Size() -1) wpost += "&";
  }

  curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
  curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_writehandle);
  curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);
  curl_easy_setopt(ch,CURLOPT_POST,1);
  curl_easy_setopt(ch,CURLOPT_POSTFIELDS,wpost.c_str());
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  curl_easy_perform(ch);

  return wdata;

}

void http::post2(std::string url, AssocArray<std::string> post_data) {

  std::string wpost = "";

  for(int i=0;i<post_data.Size();i++) {
    wpost += post_data.GetItemName(i) + "=" + post_data[i];
    if (i != post_data.Size() -1) wpost += "&";
  }

  curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
  curl_easy_setopt(ch,CURLOPT_POST,1);
  curl_easy_setopt(ch,CURLOPT_POSTFIELDS,wpost.c_str());
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  curl_easy_perform(ch);

}




std::string http::del(std::string url) {

  std::string wdata = "";
  write_data = &wdata;
  curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
  curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,write_cb);
  curl_easy_setopt(ch,CURLOPT_WRITEDATA,write_data);
  curl_easy_setopt(ch,CURLOPT_CUSTOMREQUEST,"DELETE");
  curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
  curl_easy_perform(ch);
  return wdata;
  
}
