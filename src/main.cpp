#include "BlackScholesPricer.hpp"
#include "ValuationEngine.h"
#include <iostream>
#include <memory>


int main() {
    std::cout << "Option Pricing Engine Initialized" << std::endl;


    double S = 100.0;   // Underlying asset price
    double K = 100.0;   // Strike price
    double T = 1.0;     // Time to maturity (1 year)
    double r = 0.05;    // Risk-free interest rate (5%)
    double sigma = 0.2; // Volatility (20%)


    std::unique_ptr<IOptionPricer> bsPricer = std::make_unique<BlackScholesOptionsPricer>(S, K, T, r, sigma);

    ValuationEngine engine(std::move(bsPricer));

    std::cout << "European Call Option Price: " << engine.getEuropeanCallPrice() << std::endl;
    std::cout << "European Put Option Price: " << engine.getEuropeanPutPrice() << std::endl;

    return 0;
}