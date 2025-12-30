#pragma once
#include "OrderBookentry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class Orderbook
{
    public:
    
        Orderbook(std::string filename);

        std::vector<std::string> getKnownProducts();
    
        std::vector<OrderBookentry> getOrders(OrderBooktype type, 
                                             std::string product,
                                             std::string timestamp);

        std::string getEarliestTime();
        std::string getNextTime(std::string timestamp);

        void insertOrder(OrderBookentry& order);

        std::vector<OrderBookentry> matchAskstoBids(std::string product,std::string timestamp);

        static double gethighprice(std::vector<OrderBookentry>& orders);
        static double getlowprice(std::vector<OrderBookentry>& orders);



    private:
        std::vector<OrderBookentry> orders;   
};
