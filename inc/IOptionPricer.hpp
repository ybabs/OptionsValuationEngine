#pragma once

#include <cmath>


static constexpr double AVERAGE_DAYS_IN_YEAR = 365.25;

// Abstact Base class for Options Pricers.
//This should serve as a common interface for all pricing models
class IOptionPricer
{
    public:

    virtual ~IOptionPricer()
    {

    }


    // Pure virtual function for pricing European Call and Put Options
    virtual double priceCall() const = 0;
    virtual double pricePut() const = 0;

    
    virtual double deltaCall() const = 0;
    virtual double deltaPut() const = 0;
    virtual double gamma() const = 0;
    virtual double vega() const  = 0;
    virtual double thetaCall() const = 0;
    virtual double rhoCall() const = 0;
};