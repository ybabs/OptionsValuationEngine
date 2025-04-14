#pragma once

#include <cmath>
#include <chrono>
#include "IOptionPricer.hpp"


// Encapsulate the logic to price both European and American call and put options using
// the Black Scholes model.  
class BlackScholesOptionsPricer : public IOptionPricer
{

public:
    BlackScholesOptionsPricer(double S, double K, double T, double r, double sigma);
    BlackScholesOptionsPricer(double S, double K, const std::chrono::year_month_day &expirationDate, double r, 
                                double sigma, const std::chrono::year_month_day& valuationDate);
    double priceCall() const override;
    double pricePut() const override;
    double deltaCall() const override;
    double deltaPut() const override;
    double gamma() const  override;
    double vega() const   override;
    double thetaCall() const  override;
    double rhoCall() const override;


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