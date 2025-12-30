#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

Merkelmain::Merkelmain()
{


}

void Merkelmain::init()
{
    
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertcurrency("BTC", 10);

    while(true)
    {
        printMenu();
        input = getuserOption();
        processUserOption(input);
    }
}

void Merkelmain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;

    std::cout << "============== " << std::endl;\

    std::cout << "Current time is: " << currentTime <<std::endl;
}

void Merkelmain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void Merkelmain::printMarketStats()
{
    for (std::string const p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookentry> entries = orderBook.getOrders(OrderBooktype::ask,
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() <<std::endl;
        std::cout << "Max ask: " << Orderbook::gethighprice(entries) <<std::endl;
        std::cout << "Min ask: " << Orderbook::getlowprice(entries) <<std::endl;
    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookentry& e : orders)
    // {
    //     if (e.ordertype == OrderBooktype::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.ordertype == OrderBooktype::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void Merkelmain::EnterAsk()
{
    std::cout << "Make an ask - enter the amount: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::getline(std::cin, input);
    
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() !=3)
    {
        std::cout << "Merkelmain::EnterAsk Bad input! " << input << std::endl;
    }
    else{
        try{
            OrderBookentry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBooktype::ask
            );
            obe.username = "simuser";
            if (wallet.canFulfillorder(obe))
            {
                std::cout << "Wallet looks good" << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds. " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << "Merkelmain::EnterAsk Bad input " << std::endl;
        }
    }
}

void Merkelmain::enterBid()
{
        std::cout << "Make an bid - enter the amount: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::getline(std::cin, input);
    
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() !=3)
    {
        std::cout << "Merkelmain::EnterBid Bad input! " << input << std::endl;
    }
    else{
        try{
            OrderBookentry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBooktype::bid
                
            );
            obe.username = "simuser";
            if (wallet.canFulfillorder(obe))
            {
                std::cout << "Wallet looks good" << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds. " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << "Merkelmain::EnterBid Bad input " << std::endl;
        }
    }
}

void Merkelmain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
void Merkelmain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
    std::cout << "matching " << p << std::endl;
    std::vector<OrderBookentry> sales = orderBook.matchAskstoBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookentry& sale : sales)
    {
        std::cout << "Sale price: " << sale.price << "Amount " << sale.amount << std::endl;
        if (sale.username == "simuser")
        {
            wallet.processSale(sale);
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}
}
 
int Merkelmain::getuserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    userOption = std::stoi(line);
    std::cin >> userOption;
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void Merkelmain::processUserOption(int userOption)
{
    if (userOption < 1 || userOption > 6) 
    {
        std::cout << "Wrong choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        EnterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }       
}
 
