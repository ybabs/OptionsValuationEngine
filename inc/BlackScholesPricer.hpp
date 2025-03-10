#pragma once

#include <cmath>

#include "IOptionPricer.hpp"


// Encapsulate the logic to price both European and American call and put options using
// the Black Scholes model.  
class BlackScholesOptionsPricer : public IOptionPricer
{

public:
    BlackScholesOptionsPricer(double S, double K, double T, double r, double sigma);
    double priceEuropeanCall() const override;
    double priceEuropeanPut() const override;


private:

    // pre
    double S_; // Underlying asset price;
    double K_; // Strike price;
    double T_;  // Time to Maturity
    double r_; // Risk Free Interest Rate
    double sigma_; // volatility





    double normCdf(double x) const;
    std::pair<double, double> computeD1D2() const;



    
};