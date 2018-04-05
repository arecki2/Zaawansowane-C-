#ifndef __SMARTVECTOR_H__
#define __SMARTVECTOR_H__

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

//typedef std::iterator<std::input_iterator_tag, T> iterator;

template <typename T>
class SmartVector {
private:
	std::vector<T> vector;
public:
	SmartVector(std::initializer_list<T> list) : vector(list) {}
	SmartVector(const SmartVector<T> & sv) : std::vector<T>(sv.vector) {}
	
	class iterator : public std::iterator<std::input_iterator_tag, T> {
		
	};
	
	iterator begin() {
		return vector.begin();
	}
	
	iterator end() {
		return vector.end();
	}
	
	iterator rbegin() {
		return vector.rbegin();
	}
	
	iterator rend() {
		return vector.rend();
	}
	
	void print_all_elems() {
		for(T elem : vector)
			std::cout << elem << "\t";
		std::cout << std::endl;
	}
	
	template <class Function>
	void apply(Function f) {
		std::for_each(begin(), end(), f);
	}
	
	template <class UnaryPredicate>
	iterator search(UnaryPredicate p) {
		std::vector<T> found();
		iterator curr = begin();
		while(curr != end()) {
			iterator f = std::find_if(curr, end(), p);
			found.push_back(*f);
			if(curr != end())
				curr = f.next();
		}
		return found.iterator();
	}
	
	template <class UnaryPredicate>
	iterator copy(UnaryPredicate p) {
		SmartVector<T> output();
		return std::copy_if(begin(), end(), output.iterator(), p);
	}
	
	void sort() {
		if(std::is_sorted(begin(), end())) {
			std::cout << "Vector is already sorted" << std::endl;
		} else {
			std::cout << "Vector will be sorted now" << std::endl;
			std::sort(begin(), end());
		}
	}
};

#endif
