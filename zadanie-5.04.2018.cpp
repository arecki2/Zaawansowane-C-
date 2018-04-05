#include <iostream>
#include <random>
#include "smartvector.h"

struct Change{
  std::mt19937 generator;
  std::uniform_int_distribution<> distribution;
 
  Change() : distribution(std::uniform_int_distribution<>(-5,5)){}
  void operator()(int& i){ i+=distribution(generator);}
};

/*******************************************************************
 *******************************************************************
 *******************************************************************/
 
struct Gt0{
  bool operator()(const int& i){ return i > 0;}
} gt0;

/*******************************************************************
 *******************************************************************
 *******************************************************************/
 
int main(){
  // generator liczb pseudolosowych
  std::mt19937 generator;
  std::uniform_int_distribution<> distribution(-5,5);
   
  Change f1;
  SmartVector<int> Is;
  SmartVector<int> Js;

  std::cout<<"Generating vector:"<<std::endl;
  for(unsigned i=20U; i>0; --i) Is.push_back(distribution(generator));
  std::cout<<Is<<std::endl; // Wyświetlanie wektora
  
  std::cout<<"Applying function:"<<std::endl;
  Is.apply(f1); // Działanie f1
  std::cout<<Is<<std::endl;

  std::cout<<"Searching for ==0:"<<std::endl;
  auto zeros = Is.search(0); // wyszukiwanie po obiekcie
  for(const auto& x : zeros)
    std::cout<<"0 in "<<x - Is.begin()<<std::endl;

  std::cout<<"Searching for >0:"<<std::endl;

  auto zeros = Is.search(gt0); // wyszukiwanie po funktorze
  for(const auto& x : zeros)
    std::cout<<"0 in "<<x - Is.begin()<<std::endl;
    
  std::cout<<"Copying >0:"<<std::endl;
  Js = Is.copy(gt0); // tworzenie kopii elementów spełniających warunek
  std::cout<<Js<<std::endl;

  std::cout<<"Sorting copy:"<<std::endl;
  Js.sort(); // sortowanie
  std::cout<<Js<<std::endl;

  std::cout<<"Sorting original:"<<std::endl;
  Is.sort()
  std::cout<<Is<<std::endl;


  return 0;
}
