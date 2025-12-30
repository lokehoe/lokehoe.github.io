#include "wallet.h"
#include <vector>
#include "CSVReader.h"

Wallet::Wallet()
{

}

void Wallet::insertcurrency(std::string type, double amount)
{
    double balance;
    if (amount < 0)
    {
        throw std::exception{};
    }
    if (currencies.count(type) == 0)
    {
        balance = 0;
    }
    else
    {
        balance = currencies[type];
    }
    balance += amount;
    currencies[type] = balance;
}

bool Wallet::removecurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        return false;
    }
    if (currencies.count(type) == 0)
    {
        return false;
    }
    else
    {
        if (containscurrency(type, amount))
        {
            std::cout <<" Removing " << type << " : " << amount <<std::endl;
            currencies[type] -= amount;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool Wallet::containscurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0)
        return false;
    else
    {
        return currencies[type] >= amount;
    }

}

std::string Wallet::toString()
{
    std::string s;
    for (std::pair<std::string,double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    return s;
}

bool Wallet::canFulfillorder(OrderBookentry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    if(order.ordertype == OrderBooktype::ask)
    {
        double amount = order.amount;
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfillorder " << currency << " : " << amount << std::endl;
        return containscurrency(currency, amount);
    }
    if (order.ordertype == OrderBooktype::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
          std::cout << "Wallet::canFulfillorder " << currency << " : " << amount << std::endl;
        return containscurrency(currency, amount);
    }
    return false;
}

void Wallet::processSale(OrderBookentry& sale)
{
        std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    if(sale.ordertype == OrderBooktype::asksale)
    {
        double outgoingamount = sale.amount;
        std::string outgoingcurrency = currs[0];
        double incomingamount = sale.amount * sale.price;
        std::string incomingcurrency = currs[1];

        currencies[incomingcurrency] += incomingamount;
        currencies[outgoingcurrency] -= outgoingamount;
        
    }
    if (sale.ordertype == OrderBooktype::bidsale)
    {
        double incomingamount = sale.amount;
        std::string incomingcurrency = currs[0];
        double outgoingamount = sale.amount * sale.price;
        std::string outgoingcurrency = currs[1];

        currencies[incomingcurrency] += incomingamount;
        currencies[outgoingcurrency] -= outgoingamount;
    }
}