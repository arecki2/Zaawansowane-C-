#ifndef _ASA_H
#define _ASA_H

#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

/* Wymagania parametrów:
 *
 * dim:				liczba wymiarów minimalizacji
 * T:				typ arytmetyczny
 * NSolvers:			liczba niezależnych solwerów
 * Landscape:			klasa wykonywalna przechowująca minimalizaowaną funkcję
 * RandomNumberGenerator:	klasa zajmująca się generowanie liczb pseudoloswych
 * Termalizer:			klasa zawiadująca podatnością na zmiany
 *
 * Landscape:
 * 	posiada: T operator()(const std::array<T,dim>&)const
 *
 * RandomNumberGenerator:
 * 	posiada: konstruktor przechowujący więzy RandomNumberGenerator(const std::array<T,dim>&, const std::array<T,dim>&)
 * 		 T randomRange(size_t wymiar) zwracającą losową koordynatę w wymiarze [więzy_dolne[wymiar],więzy_górne[wymiar]]
 * 		 T stepForward(T& niepodatnosc, size_t wymiar, const T& polozenie) zwracajce nowe polozenie w wymiarze wymiar, wzgledem zmiennej polozenie
 * 		 T choice() zwracającą liczbę z rozkładu jednorodnego [0,1]
 *
 * Termalizer:
 * 	posiada: T operator()(T& niepodatnosc, size_t numerKroku, size_t liczbaKrokow, size_t liczba_wymiarow) opisującą rosnącą w numerKroku funkcję niepodatnosci
 *
 * niepodatnosc jest w przybliżeniu prawdopodobieństwem, że układ zdecyduje się na kosztowny ruch (i.e., pójdzie "pod górę")
 */


template<class Landscape, size_t dim, class RandomNumberGenerator,
	 class Termalizer, typename T = double>
class AdaptiveSimulatedAnnealing {
	T beta;					//susceptibility to change
	T beta0;

	std::array<T,dim> lowerLimits;
	std::array<T,dim> upperLimits;		// lower and upper limits of minimization procedure

	std::array<T,dim> variables;		// variables to optimize
	T value;				// value

	Landscape landscape;			// minimization function
	RandomNumberGenerator generator;	// random number generator
	Termalizer termalizer;			// termalization function

	size_t initialPoints;			// number of points for variation analysis
	std::vector<T> memory;			// last initialPoints* records

public:
	AdaptiveSimulatedAnnealing(std::array<T,dim> _lowerLimits, std::array<T,dim> _upperLimits,
				   size_t _initialPoints = 100U, T _beta=T(1.0001)):
	       			   beta(_beta), beta0(_beta), lowerLimits(_lowerLimits), upperLimits(_upperLimits),
				   generator(lowerLimits,upperLimits), initialPoints(_initialPoints){
					   memory = std::vector<T>(initialPoints,T(0.0));
					   reset();
				}

private:
	void reset(){ // reseting move
		beta=beta0;

		for(size_t i=0U; i<dim; ++i)
			variables[i] = generator.randomRange(i);

		value = landscape(variables);
	}

	std::pair<T,size_t> metropolis(const size_t max_steps, const T eps){ // Metropolis algorithm with max_steps - maximum steps, and eps - accuracy
		T varianceSq(-1.0);
		size_t steps = max_steps;

		auto normalization = std::pow(static_cast<T>(initialPoints),-0.5);

		for (size_t i=0U; i<max_steps; ++i){
			std::array<T,dim> newVariables;
			for(size_t k=0U; k<dim; ++k)
				newVariables[k] = generator.stepForward(beta, k, variables[k]); // moving forward

			T newValue = landscape(newVariables);

			if(newValue < value ){ // always accept better step
				for(size_t k=0U; k<dim; ++k)
					variables[k] = newVariables[k];

				value = newValue;

				beta = termalizer(beta,i,max_steps,dim);

			}
			else if ( std::exp( beta*(value - newValue) ) >= generator.choice()){ // accept with Metropolis probability
				for(size_t k=0U; k<dim; ++k)
					variables[k] = newVariables[k];
				
				value = newValue;
			}
			
			memory[i % initialPoints] = value;
			if(i > initialPoints){ // calculate squared variance
				std::vector<T> varianceTmp;
				std::transform(memory.begin(),memory.end(), std::back_inserter(varianceTmp),
					       [&](T& x) -> T {return pow(x-value,2);});
				varianceSq = normalization*std::pow(std::accumulate(varianceTmp.begin(),varianceTmp.end(),T(0.0)),0.5);
				if(varianceSq < eps){
					steps = i;
					break;
				}
			}
		}


		return std::make_pair(varianceSq,steps);
	}


	public:
	// getting only the result for the best walker
	std::pair<T,size_t> run(const size_t max_steps, const T eps, T& point_value, std::array<T,dim>& points){
		reset();

		auto result = metropolis(max_steps,eps);

		point_value = value;
		points = variables;

		return result; 
;
	}

	virtual ~AdaptiveSimulatedAnnealing(){}
};

#endif
