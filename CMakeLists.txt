cmake_minimum_required(VERSION 2.6)

project(lsclient)

SET (CMAKE_CXX_FLAGS       "-g -O3 -std=c++11 -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_library(
        lsclient SHARED
        src/utils.cpp
        src/lsclient.cpp
        src/lstable.cpp
        src/lssubscription.cpp
        src/http.cpp
)

add_executable(
  sample
  sample.cpp
)

find_library (PTHREAD pthread) 
find_library (CURL curl)

target_link_libraries(
  lsclient
  curl
  pthread
)

target_link_libraries(
  sample
  lsclient
)

install (TARGETS lsclient DESTINATION lib)
install (FILES "src/assoc.hpp" DESTINATION include/lsclient/)
install (FILES "src/utils.hpp" DESTINATION include/lsclient/)
install (FILES "src/lsclient.hpp" DESTINATION include/lsclient/)
install (FILES "src/lssubscription.hpp" DESTINATION include/lsclient/)
install (FILES "src/lstable.hpp" DESTINATION include/lsclient/)

