#include "BlackScholesPricer.hpp"
#include <chrono>
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

BlackScholesOptionsPricer::BlackScholesOptionsPricer(double S, double K, const std::chrono::year_month_day &expirationDate, double r, 
    double sigma, const std::chrono::year_month_day& valuationDate):S_(S), K_(K), r_(r), sigma_(sigma)
{
    std::chrono::sys_days expSysDays{expirationDate};
    std::chrono::sys_days valSysDays{valuationDate};

    // compute the difference in days
    auto diff = expSysDays - valSysDays;
    T_ = static_cast<double>(diff.count()) / AVERAGE_DAYS_IN_YEAR;
    
}

double BlackScholesOptionsPricer::priceCall() const
{
    auto d = computeD1D2();

    // Black Scholes Call Formula
    // Call = S * N (d1) - K * exp(-r * T) * N(d2)
    double call = S_* normCdf(d.first) - K_ * std::exp(-r_ * T_) * normCdf(d.second);

    return call;
}

double BlackScholesOptionsPricer::pricePut() const 
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

// Delta of the call option is the rate of change of the option price wrt changes in the underlyinf asset's price
// Using Black scholes, apply the cumulative NDF to d1
double BlackScholesOptionsPricer::deltaCall() const
{
    auto d = computeD1D2();
    return normCdf(d.first);
}

// delta of put option is always negative// should be between -1 and 0
double BlackScholesOptionsPricer::deltaPut() const
{
    auto d = computeD1D2();
    return -normCdf(-d.first);
}

// second derivative of option price
double BlackScholesOptionsPricer::gamma() const 
{
    auto d = computeD1D2();
    double pdf = std::exp(-0.5 * d.first * d.first) / std::sqrt(2 * M_PI);

    return pdf / (S_ * sigma_ * std::sqrt(T_));

}
double BlackScholesOptionsPricer::vega() const  
{
    auto d = computeD1D2();
    double pdf = std::exp(-0.5 * d.first * d.first) / std::sqrt(2 * M_PI);
    return S_ * std::sqrt(T_) * pdf;
}
double BlackScholesOptionsPricer::thetaCall() const 
{
    auto d = computeD1D2();
    double pdf = std::exp(-0.5 * d.first * d.first) / std::sqrt(2 * M_PI);
    // Theta (per year)
    return - (S_ * pdf * sigma_) / (2 * std::sqrt(T_))
           - r_ * K_ * std::exp(-r_ * T_) * normCdf(d.second);
}
double BlackScholesOptionsPricer::rhoCall() const
{
    auto d = computeD1D2();
    return K_ * T_ * std::exp(-r_ * T_) * normCdf(d.second);
}




