#include <string>
#include "OrderBookentry.h"

OrderBookentry::OrderBookentry(double price,
                        double amount,
                        std::string timestamp,
                        std::string product,
                        OrderBooktype ordertype,
                        std:: string username)
:price(price),
amount(amount), 
timestamp(timestamp),
product(product), 
ordertype(ordertype),
username(username)

{

}

OrderBooktype OrderBookentry::stringToOrderBooktype(std::string s)
{
    if (s == "ask")
    {
        return OrderBooktype::ask;
    }
    if (s == "bid")
    {
        return OrderBooktype::bid;
    }
    return OrderBooktype::unknown;
    
}