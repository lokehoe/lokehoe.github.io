#include <iostream>
#include <string>
#include <map>  
#include "OrderBookentry.h"


class Wallet
{
    public:
        Wallet();
        void insertcurrency(std::string type, double amount);
        bool removecurrency(std::string type, double amount);
        bool containscurrency(std::string type, double amount);
        bool canFulfillorder(OrderBookentry order);
        void processSale (OrderBookentry& sale);
        std::string toString();
    
    private:
        std::map<std::string,double> currencies;
};
