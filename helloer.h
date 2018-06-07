#ifndef _HELLOER_H
#define _HELLOER_H

#include <iostream>
#include <random>
#include <memory>

template<typename T=double>
class Helloer{
protected:
    std::mt19937 generator;
    std::normal_distribution<T> gauss;
    std::shared_ptr<T> myID;
    std::shared_ptr<T> value;

public:
    void randomize(){
	    std::random_device rd{};
	    std::uniform_int_distribution<> choice(0,3);
	    switch(choice(rd)){
		    case 0:
			    myID = value;
			    break;
		    case 1:
			    value = myID;
			    break;
		    case 2:
			    myID.reset();
			    break;
		    case 3:
			    value.reset();
			    break;
		    default:
			    myID.reset();
			    value.reset();
			    break;
	}

    }
    Helloer(): generator(std::random_device()()),
	       gauss(0.0,1.0){
	    myID = std::make_shared<T>(gauss(generator));
	    value = std::make_shared<T>(gauss(generator));
	    std::cout<<"Hello!   I am  "<<*myID<<"."<<std::endl;
    }

    virtual ~Helloer(){
	    std::cout<<"Goodbye! I was "<<*myID<<"."<<std::endl;
    }	    

    bool operator*(const Helloer& rhs){
	    value = myID;
	    return true;
    }


};	

template<typename T=double>
class Rude_Helloer : public Helloer<T>{
public:
    Rude_Helloer() : Helloer<T>(){
	    std::cout<<"I belive you are a silly person!"<<std::endl;
    }

    ~Rude_Helloer(){
	    std::cout<<"Goodbye silly person!"<<std::endl;
    }	    

    bool operator*(const Helloer<T>& rhs){
	    //Helloer<T>::randomize();
	    return false;
    }

    friend bool operator*(const Helloer<T>& lhs,const Rude_Helloer& rhs){
	    return false;
    }

    bool operator*(const Rude_Helloer& rhs){
	    //Helloer<T>::randomize();
	    return true;
    }
};

template<typename T=double>
class Mean_Helloer : public Rude_Helloer<T>{
public:
    Mean_Helloer() : Rude_Helloer<T>(){
	    std::cout<<"And I dislike you."<<std::endl;
    }

    ~Mean_Helloer(){
	    std::cout<<"Goodbye silly person! I disliked you."<<std::endl;
    }	    

    bool operator*(const Helloer<T>& rhs){
	    //Helloer<T>::randomize();
	    return false;
    }

    friend bool operator*(const Helloer<T>& lhs,const Mean_Helloer& rhs){
	    return false;
    }

    bool operator*(const Rude_Helloer<T>& rhs){
	    //Helloer<T>::randomize();
	    return false;
    }

    friend bool operator*(const Rude_Helloer<T>& lhs,const Mean_Helloer& rhs){
	    return false;
    }

    bool operator*(const Mean_Helloer& rhs){
	    //Helloer<T>::randomize();
	    return true;
    }
};
#endif
