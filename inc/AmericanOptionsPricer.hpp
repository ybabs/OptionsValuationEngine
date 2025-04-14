#pragma once

#include "IOptionPricer.hpp"
#include <chrono>



class AmericanOptionsPricer : public IOptionPricer
{
public:
    AmericanOptionsPricer(double S, double K, double T, double r, double sigma, int steps = 1000);
    AmericanOptionsPricer(double S, double K, const std::chrono::year_month_day &expirationDate, double r, 
        double sigma, const std::chrono::year_month_day& valuationDate, int steps);

    double priceCall() const override;
    double pricePut() const override;
    double deltaCall() const override;
    double deltaPut() const override;
    double gamma() const  override;
    double vega() const   override;
    double thetaCall() const  override;
    double rhoCall() const override;

    double priceCallFD(double S, double sigma = -1, double T = -1, double r = -1) const;
    double pricePutFD(double S, double sigma = -1, double T = -1, double r = -1) const;
    double binomialTreePrice(bool isCall) const;



    private:
    double S_;
    double K_;
    double T_;
    double r_;
    double sigma_;
    double steps_;

};