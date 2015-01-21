#ifndef UTILS_H
#define UTILS_H

#include  <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <regex>
#include <iterator>


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

std::string &ltrim(std::string &s);
std::string &rtrim(std::string &s);
std::string &trim(std::string &s);

std::string int2string(int i);
int roundint(double r);
std::string &upper(std::string &s);
std::string sreplace(std::string &s,
                     std::string toReplace,
                     std::string replaceWith);


size_t curl_write_handler(void*, size_t, size_t, std::string stream);


#endif