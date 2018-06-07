#include <iostream>
#include <random>
#include "helloer.h"

int main(){
	const int N = 100; // liczba elementów do porównania
	std::random_device rd{};
	std::uniform_int_distribution<> choice(0,2);

	     Helloer<double> A;
	Rude_Helloer<double> B;
	Mean_Helloer<double> C;

	Helloer<double>* ptrI;
	Helloer<double>* ptrJ;

	for(int i=N; i>0; --i){
		switch(choice(rd)){
			case 0:
				ptrI = new Helloer<double>;
				break;
			case 1:
				ptrI = new Rude_Helloer<double>;
				break;
			case 2:
				ptrI = new Mean_Helloer<double>;
				break;
			default:
				ptrI = nullptr;
		}
	
	  for(int j=N; j>0; --j){
		switch(choice(rd)){
			case 0:
				ptrJ = new Helloer<double>;
				break;
			case 1:
				ptrJ = new Rude_Helloer<double>;
				break;
			case 2:
				ptrJ = new Mean_Helloer<double>;
				break;
			default:
				ptrJ = nullptr;
		}
	   std::cout<<std::boolalpha<<"ptrI*ptrJ = "<<(*ptrI)*(*ptrJ)<<std::endl;
	  }
	}

	return 0;
}
