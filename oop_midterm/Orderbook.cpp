#include "Orderbook.h"
#include "CSVReader.h"
#include <string>
#include <iostream>
#include <map>
#include <algorithm>


Orderbook::Orderbook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

std::vector<std::string> Orderbook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookentry& e: orders)
    {
        prodMap[e.product] = true;
    }

    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

std::vector<OrderBookentry> Orderbook::getOrders(OrderBooktype type, 
                                        std::string product,
                                        std::string timestamp)
{
    std::vector<OrderBookentry> order_sub;
    for(OrderBookentry& e : orders)
    {
        if(e.ordertype == type &&
           e.product == product &&
           e.timestamp == timestamp)
           {
                order_sub.push_back(e);
           }
    }
    return order_sub;
}

double Orderbook::gethighprice(std::vector<OrderBookentry>& orders)
{
    double max = orders[0].price;
    for(OrderBookentry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}

double Orderbook::getlowprice(std::vector<OrderBookentry>& orders)
{
    double min = orders[0].price;
    for(OrderBookentry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;
}

std::string Orderbook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string Orderbook::getNextTime(std::string timestamp)
{
  std::string next_timestamp;
  for (OrderBookentry& e : orders)
  {
    if (e.timestamp > timestamp)
    {
        next_timestamp = e.timestamp;
        break;
    }
  }
  if (next_timestamp == "")
  {
    next_timestamp = orders[0].timestamp;
  }
  return next_timestamp;
}

void Orderbook::insertOrder(OrderBookentry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookentry::compareBytimestamp);
}

std::vector<OrderBookentry> Orderbook::matchAskstoBids(std::string product, std::string timestamp)
{
    std::vector<OrderBookentry> asks = getOrders(OrderBooktype::ask,
                                                 product,
                                                timestamp);

    std::vector<OrderBookentry> bids = getOrders(OrderBooktype::ask,
                                                    product,
                                                    timestamp);

    std::vector<OrderBookentry> sales;

    for (OrderBookentry& ask : asks)
    {
        for (OrderBookentry& bid : bids)
        {
           if (bid.price >= ask.price && bid.amount > 0 && ask.amount > 0)
            {
                OrderBookentry sale{
                    ask.price,
                    std::min(ask.amount, bid.amount),
                    timestamp,
                    product,
                    OrderBooktype::asksale
                };

                bid.amount -= sale.amount;
                ask.amount -= sale.amount;

                sales.push_back(sale);

                if (ask.amount == 0)
                    break;
            }
        }
    }
    return sales;
}
