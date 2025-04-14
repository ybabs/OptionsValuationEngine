#include "BlackScholesPricer.hpp"
#include <memory>
class ValuationEngine
{

public:
    ValuationEngine(std::unique_ptr<IOptionPricer> pricer) : pricer_(std::move(pricer))
    {

    }

    double getEuropeanCallPrice() const
    {
        return pricer_->priceCall();
    }

    double getEuropeanPutPrice() const
    {
        return pricer_->pricePut();
    }

    // TODO Retrieve greeks here


private:
    std::unique_ptr<IOptionPricer> pricer_;
};