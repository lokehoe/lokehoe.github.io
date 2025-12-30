#include <string>

enum class OrderBooktype{bid,ask,unknown,asksale,bidsale};

class OrderBookentry
{
    public:
        OrderBookentry( double price,
                        double amount,
                        std::string timestamp,
                        std::string product,
                        OrderBooktype ordertype,
                        std::string username = "dataset");

    static OrderBooktype stringToOrderBooktype(std::string s);

    static bool compareBytimestamp(OrderBookentry& e1, OrderBookentry& e2)
    {
        return e1.timestamp < e2.timestamp;
    }
    static bool compareByPriceAsc(OrderBookentry& e1, OrderBookentry& e2)
    {
        return e1.price < e2.price;
    }
        static bool compareByPriceDesc(OrderBookentry& e1, OrderBookentry& e2)
    {
        return e1.price > e2.price;
    }
        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBooktype ordertype;
        std::string username;
};
