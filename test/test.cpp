#include <iostream>
#include <queue>
#include <thread>
#include "../src/container/vector.hpp"
using namespace ministl;

int main() {
  vector<int> a;
  vector<int> b(10);
  vector<int> c(1, 2);
  std::cout << a.size() << " \n";
  std::cout << b.size() << " \n";
  std::cout << c.size() << " \n";
  for (auto i = b.begin(); i < b.end(); i++) {
    std::cout << *i << " ";
  }
  std::cout << std::endl;
  for (auto i = c.begin(); i < c.end(); i++) {
    std::cout << *i << " ";
  }
  std::cout << std::endl;
  vector<int> d = c;
  d.push_back(1);
  d.push_back(2);
  d.push_back(3);
  d.push_back(4);
  for (auto i = d.begin(); i < d.end(); i++) {
    std::cout << *i << " ";
  }
  std::cout << std::endl;
  std::cout << d.empty() << "\n";
  std::cout << d.size() << "\n";
  std::cout << d[2] << "\n";
  auto i = d.begin();
  auto j = d.begin() + 2;
  i += 4;
  std::cout << *i << "\n";
  b.insert(b.begin() + 2, j, i);
  for (auto i = b.begin(); i < b.end(); i++) {
    std::cout << *i << " ";
  }
  return 0;
}
