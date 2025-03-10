#include <cmath>


// Abstact Base class for Options Pricers.
//This should serve as a common interface for all pricing models
class IOptionPricer
{
    public:

    virtual ~IOptionPricer()
    {

    }


    // Pure virtual function for pricing European Call and Put Options
    virtual double priceEuropeanCall() const = 0;
    virtual double priceEuropeanPut() const = 0;

    
    // Add virtual function for greeks later
};