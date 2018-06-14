#include <iostream>
#include <random>
#include <chrono>
#include <ctime>
#include <array>

template <typename T, size_t dim>
class RandomNumberGenerator {
private:
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::array<T,dim> lowerLimits;
	std::array<T,dim> upperLimits;
    
public:
    RandomNumberGenerator(const std::array<T,dim>& lower_bonds, const std::array<T,dim>& upper_bonds)
        : lowerLimits(lower_bonds), upperLimits(upper_bonds) {}
    
    T randomRange(size_t wymiar) {
        std::uniform_real_distribution<> disT(lowerLimits[wymiar], upperLimits[wymiar]);
        return disT(gen);
    }
    
    T stepForward(T& niepodatnosc, size_t wymiar, const T& polozenie) {
        auto upper = (upperLimits[wymiar]-polozenie)/niepodatnosc;
        auto lower = (polozenie-lowerLimits[wymiar])/niepodatnosc;
        std::uniform_real_distribution<> disP(lower, upper);
        return disP(gen);
    }
    
    T choice() {
        return dis(gen);
    }
};

template <typename T, size_t dim)
class Termalizer {
public:
    T operator()(T& niepodatnosc, size_t numerKroku, size_t liczbaKrokow, size_t liczba_wymiarow) {
        
    }
};

int main() {
    auto start = std::chrono::system_clock::now();
    doSomething();
    auto stop = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
