#include "OptionBuilder.hpp"
#include "OptionTypes.hpp"
#include <iostream>
#include <memory>
#include <chrono>


int main() {
    std::cout << "Option Pricing Engine Initialized" << std::endl;


    double S = 100.0;   // Underlying asset price
    double K = 100.0;   // Strike price
    double T = 1.0;     // Time to maturity (1 year)
    double r = 0.05;    // Risk-free interest rate (5%)
    double sigma = 0.2; // Volatility (20%)

    std::chrono::year_month_day expirationDate{std::chrono::year{2025}, std::chrono::month{12}, std::chrono::day{31}};
    std::chrono::sys_days today = floor<std::chrono::days>(std::chrono::system_clock::now());
    std::chrono::year_month_day valuationDate{today};


    OptionBuilder builder;
    builder.setStyle(OptionStyle::European)
           .setType(OptionType::Call)
           .setUnderlying(S)
           .setStrike(K)
           .setRiskFreeRate(r)
           .setVolatility(sigma)
           .setExpirationDate(expirationDate)
           .setValuationDate(valuationDate);

    std::unique_ptr<IOptionPricer> pricer = builder.build();

    // Retrieve and display option prices.
    std::cout << "European Call Option Price: " << pricer->priceCall() << std::endl;
    std::cout << "European Put Option Price: " << pricer->pricePut() << std::endl;

    return 0;
}