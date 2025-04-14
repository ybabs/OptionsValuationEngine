#include "BlackScholesPricer.hpp"
#include "AmericanOptionsPricer.hpp"
#include "OptionTypes.hpp"
#include "events/EventManager.hpp"
#include "OptionBuilder.hpp"
#include <memory>
#include <stdexcept>

#include "events/OptionEvent.hpp"




OptionBuilder::OptionBuilder(): 
                        style_(OptionStyle::European), 
                        type_(OptionType::Call),   
                        S_(0.0),
                        K_(0.0),
                        r_(0.0),
                        sigma_(0.0),
                        T_(0.0),
                        useDates_(false),
                        steps_(1000),
                        eventManager_(nullptr)
{

}

OptionBuilder& OptionBuilder::setStyle(OptionStyle style)
{
    style_ = style;
    return *this;
}

OptionBuilder& OptionBuilder::useDates(bool useDate)
{
    useDates_ = useDate;
    return *this;
}
OptionBuilder& OptionBuilder::setType(OptionType type)
{
    type_ = type;
    return *this;
}

OptionBuilder& OptionBuilder::setEventManager(std::shared_ptr<EventManager> manager)
{
    eventManager_ = manager;
    return *this;
}

OptionBuilder& OptionBuilder::setUnderlying(double S)
{
    S_ = S;
    return *this;
}
OptionBuilder& OptionBuilder::setStrike(double K)
{
    K_ = K;
    return *this;
}
OptionBuilder& OptionBuilder::setRiskFreeRate(double r)
{
    r_ = r;
    return *this;
}
OptionBuilder& OptionBuilder::setVolatility(double sigma)
{
    sigma_ = sigma;
    return *this;
}

// Allow setting time to maturity directly abd using calendar dates
OptionBuilder& OptionBuilder::setTimeToMaturity(double T)
{
    T_ = T;
    return *this;
}
OptionBuilder& OptionBuilder::setExpirationDate(const std::chrono::year_month_day& expirationDate)
{
    expirationDate_ = expirationDate;
    return *this;
}
OptionBuilder& OptionBuilder::setValuationDate(const std::chrono::year_month_day& valuationDate)
{
    valuationDate_ = valuationDate;
    return *this;
}

// DO i want to set steps for American options
OptionBuilder& OptionBuilder::setSteps(int steps)
{
    steps_ = steps;
    return *this;
}

std::unique_ptr<IOptionPricer> OptionBuilder::build() const
{
    if(S_ <= 0.0 || K_ <= 0.0 )
    {
        throw std::runtime_error("Underlying price and strike must be positive");
    }

    std::unique_ptr<IOptionPricer> pricer;

    if(style_ == OptionStyle::European)
    {
        if(useDates_)
        {
            pricer = std::make_unique<BlackScholesOptionsPricer>(S_, K_, expirationDate_, r_, sigma_, valuationDate_ );
        }
        else 
        {
            pricer = std::make_unique<BlackScholesOptionsPricer>(S_, K_, T_, r_, sigma_);
        }
    }

    else 
    {
        if (useDates_) {
            pricer = std::make_unique<AmericanOptionsPricer>(S_, K_, expirationDate_, r_, sigma_,  valuationDate_, steps_);
        } else {
            pricer = std::make_unique<AmericanOptionsPricer>(S_, K_, T_, r_, sigma_, steps_);
        }
    }

    // hmm pricer classes offer seprate functions for pricing a call or a put

    if(eventManager_)
    {
        OptionEvent event{EventType::OptionBuilt, "Option Successfully Built"};
        eventManager_->notify(event);
    }
    
    return pricer;
}