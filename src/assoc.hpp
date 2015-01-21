#ifndef ASSOC_H
#define ASSOC_H

#include <iostream>
#include <vector>
using namespace std;

		  template <class T>
          class AssocArray
          {
	          private:
		          typedef struct _Data
		          {
			          T data;
			          string name;
		          } Data ;
		          vector<Data> stack;
	          public:
		          long Size()
		          {
			          return stack.size();
		          }

                  vector<string> keys() {

                  	vector<string> res;
                    for(int i=0;i<Size();i++) {
                       res.push_back(stack[i].name);
                    }
                    return res;
                  }

		          bool IsItem(string name)
		          {
			          for(int i=0; i<Size(); i++)
			          {
				          if(stack[i].name == name)
					          return true;
			          }
			          return false;
		          }

        		  bool AddItem(string name, T data)
		          {
			          if(IsItem(name))
				          return false;
			          Data d;
			          d.name = name;
			          d.data = data;
			          stack.push_back(d);
			          return true;
		          }


                  T& at(long idx) {

                    for(int i=0; i<Size(); i++) {
				      if(i == idx)
					  return stack[i].data;
			        }
                    return stack[Size()-1].data;
                  }

                  T&  get(string name) {
			          for(int i=0; i<Size(); i++)
			          {
				          if(stack[i].name == name)
					          return stack[i].data;
			          }
			          long idx = Size();
			          Data d;
			          d.name = name;
			          stack.push_back(d);
			          return stack[idx].data;
		          }
                  
  		          T& operator [] (string name)
		          {
			          for(int i=0; i<Size(); i++)
			          {
				          if(stack[i].name == name)
					          return stack[i].data;
			          }
			          long idx = Size();
			          Data d;
			          d.name = name;
			          stack.push_back(d);
			          return stack[idx].data;
		          }

  		          string GetItemName(long index)
		          {
			          if(index<0)
				          index = 0;
			          for(int i=0; i<Size(); i++)
				          if(i == index)
					          return stack[i].name;
			          return "";
		          }

                  string reverse(T obj) {

                    for(int i=0;i<Size();i++) {

                      if (stack[i].data == obj) {
                        return stack[i].name;
                      }
                    }

                    return "NULL";                    
                  }

  		          T& operator [] (long index)
		          {
			          if(index < 0)
				          index = 0;
			          for(int i=0; i<Size(); i++)
			          {
				          if(i == index)
					          return stack[i].data;
			          }
			          return stack[0].data;
		          } 

            };

#endif