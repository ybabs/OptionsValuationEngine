#include "BlackScholesPricer.hpp"
#include <cmath>


// Constructor to initialize the option parameters.
    // Parameters:
    //   S     : Underlying asset price
    //   K     : Strike price
    //   T     : Time to maturity (in years)
    //   r     : Risk-free interest rate (annualized)
    //   sigma : Volatility (annualized)
BlackScholesOptionsPricer::BlackScholesOptionsPricer(double S, double K, double T, double r, double sigma):
S_(S), K_(K), T_(T), r_(r), sigma_(sigma)
{

}

double BlackScholesOptionsPricer::priceEuropeanCall() const
{
    auto d = computeD1D2();

    // Black Scholes Call Formula
    // Call = S * N (d1) - K * exp(-r * T) * N(d2)
    double call = S_* normCdf(d.first) - K_ * std::exp(-r_ * T_) * normCdf(d.second);

    return call;
}

double BlackScholesOptionsPricer::priceEuropeanPut() const 
{
    auto d = computeD1D2();

    // Black Scholes put formula 
    //Put = K * exp(-r * T) * N(-d2) - S * N(-d1)
    double put = K_ * std::exp(-r_ * T_) * normCdf(-d.second) - S_ * normCdf(-d.first);
    return put;
}


//   d1 = [ln(S/K) + (r + sigma^2/2)*T] / (sigma * sqrt(T))
//   d2 = d1 - sigma * sqrt(T)
std::pair<double, double> BlackScholesOptionsPricer::computeD1D2() const
{
    double sqrtT = std::sqrt(T_);
    double d1 = (std::log(S_ / K_) + (r_ + 0.5 * sigma_ * sigma_) * T_) / (sigma_ * sqrtT);
    double d2 = d1 - sigma_ * sqrtT;
    return {d1, d2};
}

double BlackScholesOptionsPricer::normCdf(double x) const
{
    return 0.5 * std::erfc(-x/std::sqrt(2.0));
}






