#pragma once

#include "IOptionPricer.hpp"
#include "OptionTypes.hpp"
#include <chrono>
#include  <memory>

#include "events/EventManager.hpp"


class OptionBuilder
{
public:
    
    OptionBuilder();
    OptionBuilder& setStyle(OptionStyle style);
    OptionBuilder& setType(OptionType type);

    OptionBuilder& setUnderlying(double S);
    OptionBuilder& setStrike(double K);
    OptionBuilder& setRiskFreeRate(double r);
    OptionBuilder& setVolatility(double sigma);

    // Allow setting time to maturity directly abd using calendar dates
    OptionBuilder& setTimeToMaturity(double T);
    OptionBuilder& setExpirationDate(const std::chrono::year_month_day& expirationDate);
    OptionBuilder& setValuationDate(const std::chrono::year_month_day& valuationDate);
    OptionBuilder& useDates(bool useDate);

    // DO i want to set steps for American options
    OptionBuilder& setSteps(int steps);

    OptionBuilder& setEventManager(std::shared_ptr<EventManager> manager);

    std::unique_ptr<IOptionPricer> build() const;

private:
    OptionStyle style_ = OptionStyle::European;
    OptionType type_ = OptionType::Call;
    double S_ = 0.0;
    double K_ = 0.0;
    double r_ = 0.0;
    double sigma_ = 0.0;
    double T_ = 0.0; // time to maturity in years

    bool useDates_ = false;
    std::chrono::year_month_day expirationDate_;
    std::chrono::year_month_day valuationDate_;

    std::shared_ptr<EventManager> eventManager_ = nullptr;

    int steps_ = 1000;

};


