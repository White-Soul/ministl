#include <iostream>
#include <queue>
#include <thread>
#include "../ministl/vector.hpp"
#include "../ministl/list.hpp"
#include "../ministl/string.hpp"
#include "../ministl/deque.hpp"
#include "../ministl/stack.hpp"
#include "../ministl/set.hpp"
#include "../ministl/map.hpp"
using namespace ministl;
void print(list<int> &a);
void print(string &a);
void print(deque<int> &a);
void print(map<int, int> &a);
int main()
{
  map<int, int> a;
  
  a[1] = 1;
  a[2] = 2;
  a[3] = 3;
  a[3] = 4;
   
  print(a);
  return 0;
}

void print(map<int, int> &a){
  for(auto b = a.begin(); b!=a.end(); ++b){
    std::cout << b->first<< "==" << b->second << std::endl;
  }
}
