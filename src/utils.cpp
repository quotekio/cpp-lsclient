#include "utils.hpp"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

std::string int2string(int i) {
  std::ostringstream os;
  os << i;
  return os.str();
}

int roundint(double r) {
    return (r > 0.0) ? (r + 0.5) : (r - 0.5); 
}

std::string &upper(std::string &s) {
        std::transform(s.begin(), s.end(),s.begin(), ::toupper);
        return s;
}

std::string sreplace(std::string &s,
                      std::string toReplace,
                      std::string replaceWith) {

    if (s.find(toReplace) != std::string::npos ) {
      return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
    }

    else return s;

}

std::string json_escape(const std::string& input) {
    std::ostringstream ss;
    for (auto iter = input.cbegin(); iter != input.cend(); iter++) {
    //C++98/03:
    //for (std::string::const_iterator iter = input.begin(); iter != input.end(); iter++) {
        switch (*iter) {
            case '\\': ss << "\\\\"; break;
            case '"': ss << "\\\""; break;
            case '/': ss << "\\/"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << *iter; break;
        }
    }
    return ss.str();
}

size_t curl_write_handler(void *ptr, size_t size, size_t nmemb, std::string stream)
{
    if (ptr != NULL) {

      std::string temp(static_cast<const char*>(ptr), size * nmemb);
      stream += temp;
    }
    return size*nmemb;
}