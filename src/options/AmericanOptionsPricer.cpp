#include "AmericanOptionsPricer.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

AmericanOptionsPricer::AmericanOptionsPricer(double S, double K, double T, double r, double sigma, int steps):
S_(S), K_(K), T_(T), sigma_(sigma), r_(r), steps_(steps)
{

}

AmericanOptionsPricer::AmericanOptionsPricer(double S, double K, const std::chrono::year_month_day &expirationDate, double r, 
    double sigma, const std::chrono::year_month_day& valuationDate, int steps): S_(S), K_(K), sigma_(sigma), r_(r), steps_(steps)
{
    std::chrono::sys_days expSysDays{expirationDate};
    std::chrono::sys_days valSysDays{valuationDate};

    // compute the difference in days
    auto diff = expSysDays - valSysDays;
    T_ = static_cast<double>(diff.count()) / AVERAGE_DAYS_IN_YEAR;
}

// For non-dividend paying assets, American calls are the same as European calls
double AmericanOptionsPricer::priceCall() const
{
    if(T_ <= 0 || steps_ <= 0)
    {
        return 0.0;
    }

    // For american calls on a non-dividend paying stock, early exericse is actually not optimal
    // SO you can actually use Black Scholes price as a shortcut here. But Let us still build the tree
    return binomialTreePrice(true);
}

double AmericanOptionsPricer::pricePut() const
{
    if(T_ <= 0 || steps_ <= 0)
    {
        return 0.0;
    }

    return binomialTreePrice(false);
}

double AmericanOptionsPricer::deltaPut() const
{
    // small pertubation relative to the current asset price
    double h = 0.01 * S_;
    double priceUp = pricePutFD(S_ + h);
    double priceDown = pricePutFD(S_ - h);
    return (priceUp - priceDown) / (2 * h);
}

// Use Finite difference.
// if params are not provided, use the object's values
double AmericanOptionsPricer::pricePutFD(double S, double sigma, double T, double r) const
{

    if(sigma < 0)
    {
        sigma = sigma_;
    }

    if(T < 0)
    {
        T = T_;
    }

    if( r < 0)
    {
        r = r_;
    }

    AmericanOptionsPricer tempPricer(S, K_, T, r, sigma, steps_);
    return tempPricer.pricePut();
}



// Use finite difference/ central difference approximation for the greeks here
double AmericanOptionsPricer::deltaCall() const
{
    // 1% is  good enough value to get a good approximation here.
    // We could potentially improve the accuracy of this by using a smaller h, 
    // Or a higher order finite method.
    // potentially an adaptive method that adjusts h??
    double h = 0.01 * S_; 
    double priceUp = priceCallFD(S_ + h);
    double priceDown = priceCallFD(S_ - h);
    return (priceUp - priceDown) / (2 * h);

}

double AmericanOptionsPricer::gamma() const
{
    double h = 0.01 * S_; 
    double priceUp = priceCallFD(S_ + h);
    double priceMid = priceCallFD(S_);
    double priceDown = priceCallFD(S_ - h);

    return (priceUp - 2 * priceMid + priceDown) / (h * h);
}

double AmericanOptionsPricer::vega() const
{
    double h = 0.01 * sigma_;
    double priceUp = priceCallFD(S_ , sigma_ + h);
    double priceDown = priceCallFD(S_, sigma_ - h);
    return (priceUp - priceDown) / (2 * h);
}

double AmericanOptionsPricer::thetaCall() const
{
    double h = 1.0/365; // for one day
    double priceNow = priceCallFD(S_, sigma_, T_);
    double priceLater = priceCallFD(S_, sigma_, T_ - h);
    return (priceNow - priceLater) / h;
}

double AmericanOptionsPricer::rhoCall() const
{
    double h = 0.0001;
    double priceUp = priceCallFD(S_, sigma_, T_, r_ + h);
    double priceDown = priceCallFD(S_, sigma_, T_, r_ - h);
    return (priceUp - priceDown) / (2 * h);
}

// Finite Difference pricing for approximating Greeks
// Using the binomial tree for american options
double AmericanOptionsPricer::priceCallFD(double S, double sigma, double T, double r) const
{

    if(sigma < 0)
    {
       sigma = sigma_; 
    }

    if (T < 0) {
        T = T_;
    }

    if(r < 0)
    {
        r = r_;
    }

    AmericanOptionsPricer tempPricer(S, K_, T, r, sigma, steps_);
    return tempPricer.priceCall();

}

// Cox-Ross-Rubinstein binomial tree model
double AmericanOptionsPricer::binomialTreePrice(bool isCall) const
{
    double dt = T_/steps_;
    double upFactor = std::exp(sigma_ * std::sqrt(dt));  // up factor, % increase in the asset price if it goes up
    double downFactor = 1.0/upFactor;   // down factor
    double discountFactor = std::exp(-r_ * dt);  // Discount future cash floes to the present value
    double p = (std::exp(r_ * dt) - downFactor) / (upFactor - downFactor);  // risk neutral probability of an upward move


    // Terminal PAyoff array
    // Calculate the option's payoff at expiration T
    std::vector<double> optionPrices(steps_ + 1);
    for(int i = 0; i <= steps_; i++)
    {
        // stock price at the end of the tree
        double ST = S_ * std::pow(upFactor, steps_ - i) * std::pow(downFactor, i);
        double intrinsic = isCall ? std::max(0.0, ST - K_) : std::max(0.0, K_ - ST);
        optionPrices[i] = intrinsic;
    }

    // Backward induction for options price
    for(int step = steps_ - 1; step >= 0; step --)
    {
        bool allContinuation = true; // track if all nodes are continuations
        for(int i = 0; i <= step; i++)
        {
            double ST = S_ * std::pow(upFactor, step -i) * std::pow(downFactor, i);

            // continuation value is the value of holding the option while the intrinsic is the immediate payoff 
            // from exercising the option. as we can exercise this at anytime, we need to keep comparing both
            // values at each step
            // Continuation is also the Discounted EV if it's held from more than one time step
            double continuation = discountFactor * (p * optionPrices[i] + (1-p) * optionPrices[i + 1]);
            double intrinsic = isCall ? std::max(0.0, ST - K_) : std::max(0.0, K_ - ST);
            // For american options, choose the max of intrinsic and continuation
            double nodeValue = std::max(intrinsic, continuation);

            if(nodeValue == intrinsic)
            {
                allContinuation = false;
            }
        }
        if (isCall && allContinuation) 
        {
           break;
        }
    }
    // return option at the root of the tree
    return optionPrices[0];

}
