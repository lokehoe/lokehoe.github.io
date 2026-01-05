#include <iostream>
#include <vector>
#include <limits>
#include "OrderBookentry.h"
#include "Merkelmain.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include <random>
#include <ctime>
#include <map>
#include <algorithm>
#include <iomanip>  

const std::vector<std::string> VALID_CURRENCIES = {"BTC", "USDT", "ETH", "DOGE"};
std::string getCurrentTimestamp() {
    std::time_t t = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", std::localtime(&t));
    return std::string(buffer);
}

Merkelmain::Merkelmain() : currentWallet(nullptr)
{
}

void Merkelmain::init()
{
    userAuthenticationMenu();

    currentTime = orderBook.getEarliestTime();
    if (userManager.getCurrentUser()) {
        currentWallet = &userManager.getCurrentUser()->getWallet();
        currentWallet->insertcurrency("BTC", 10);
    }

    int input;
    while (true)
    {
        if (!userManager.getCurrentUser()) {
            std::cout << "You are not logged in. Please log in to continue." << std::endl;
            userAuthenticationMenu();
            if(userManager.getCurrentUser())
            {
                currentWallet = &userManager.getCurrentUser()->getWallet();
            }
        }
        printMenu();
        input = getuserOption();
        processUserOption(input);
    }
}

//My part
void Merkelmain::userAuthenticationMenu() {
    while (!userManager.getCurrentUser()) {
        std::cout << "================================== " << std::endl;
        std::cout << "      Authentication Page      " << std::endl;    
        std::cout << "================================== " << std::endl;
        std::cout << "1: Login" << std::endl;
        std::cout << "2: Register" << std::endl;
        std::cout << "3: Reset Password" << std::endl;
        std::cout << "4: Exit" << std::endl;
        std::cout << "================================== " << std::endl;

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string username, password;
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            if (userManager.login(username, password)) {
                std::cout << "Login successful!" << std::endl;
                break;
            } else {
                std::cout << "Invalid username or password." << std::endl;
            }
        } else if (choice == 2) {
            std::string fullName, email, password;
            std::cout << "Enter full name: ";
            std::getline(std::cin, fullName);
            std::cout << "Enter email: ";
            std::getline(std::cin, email);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            userManager.registerUser(fullName, email, password);
        } else if (choice == 3) {
            std::string email, newPassword;
            std::cout << "Enter your email address: ";
            std::getline(std::cin, email);
            std::cout << "Enter your new password: ";
            std::getline(std::cin, newPassword);
            userManager.resetPassword(email, newPassword);
        }
        else if (choice == 4) {
            exit(0);
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void Merkelmain::printMenu()
{
    std::cout << "================================== " << std::endl;
    std::cout << "      Trading Platform     " << std::endl;    
    std::cout << "================================== " << std::endl;
    std::cout << "1: Print help " << std::endl;
    std::cout << "2: Print exchange stats" << std::endl;
    std::cout << "3: Make an offer " << std::endl;
    std::cout << "4: Make a bid " << std::endl;
    std::cout << "5: Print wallet " << std::endl;
    std::cout << "6: Generate Candlestick Data" << std::endl;
    std::cout << "7: Simulate New Trades" << std::endl;
    std::cout << "8: Deposit Funds" << std::endl;
    std::cout << "9: Withdraw Funds" << std::endl;
    std::cout << "10: View Transaction History" << std::endl;
    std::cout << "11: View Trading Statistics" << std::endl;
    std::cout << "12: Logout" << std::endl;
    std::cout << "================================== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void Merkelmain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void Merkelmain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookentry> entries = orderBook.getOrders(OrderBooktype::ask,
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << Orderbook::gethighprice(entries) << std::endl;
        std::cout << "Min ask: " << Orderbook::getlowprice(entries) << std::endl;
    }
}

void Merkelmain::EnterAsk()
{
    std::cout << "Make an ask - enter the amount: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad input! " << input << std::endl;
        return;
    }

    try {
        OrderBookentry obe = CSVReader::stringsToOBE(
            tokens[1], tokens[2], currentTime, tokens[0], OrderBooktype::ask
        );
        obe.username = userManager.getCurrentUser()->getUsername();

        if (currentWallet->canFulfillorder(obe))
        {
            std::cout << "Wallet looks good" << std::endl;

            std::string base = tokens[0].substr(0, tokens[0].find('/'));
            currentWallet->removecurrency(base, obe.amount);

            orderBook.insertOrder(obe);

            std::string quote = tokens[0].substr(tokens[0].find('/') + 1);
            double received = obe.price * obe.amount;
            currentWallet->insertcurrency(quote, received);

            std::cout << "Trade completed: Sold " << obe.amount << " " << base
                      << " for " << received << " " << quote << std::endl;
        }
        else
        {
            std::cout << "Wallet has insufficient funds." << std::endl;
        }
    } catch (const std::exception& e)
    {
        std::cout << "Wrong input" << std::endl;
    }
}


void Merkelmain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad input! " << input << std::endl;
        return;
    }

    try {
        OrderBookentry obe = CSVReader::stringsToOBE(
            tokens[1], tokens[2], currentTime, tokens[0], OrderBooktype::bid
        );
        obe.username = userManager.getCurrentUser()->getUsername();

        std::string base = tokens[0].substr(0, tokens[0].find('/'));
        std::string quote = tokens[0].substr(tokens[0].find('/') + 1);
        double cost = obe.price * obe.amount;

        if (currentWallet->removecurrency(quote, cost))
        {
            std::cout << "Wallet looks good, placing bid..." << std::endl;
            orderBook.insertOrder(obe);

            currentWallet->insertcurrency(base, obe.amount);
            std::cout << "Trade completed: Bought " << obe.amount << " " << base
                      << " for " << cost << " " << quote << std::endl;
        }
        else
        {
            std::cout << "Wallet has insufficient funds to buy." << std::endl;
        }
    } catch (const std::exception& e)
    {
        std::cout << "Wrong input" << std::endl;
    }
}



void Merkelmain::printWallet()
{
    std::cout << currentWallet->toString() << std::endl;
}

void Merkelmain::generateCandlestickData()
{
    std::string product, timeframe;
    std::cout << "Product: ";
    std::getline(std::cin, product);
    std::cout << "Timeframe (daily/monthly/yearly): ";
    std::getline(std::cin, timeframe);

    auto data = orderBook.getCandlestickData(
    product,
    timeframe,
    OrderBooktype::ask);

    auto biddata = orderBook.getCandlestickData(
    product,
    timeframe,
    OrderBooktype::bid);


    if (data.empty()) {
        std::cout << "No data found for product " << product << " in the given timeframe." << std::endl;
        return;
    }

    const int dateWidth = 28;
    const int priceWidth = 12;

    std::cout << "\n" << std::left 
              << std::setw(dateWidth)  << "Date" 
              << std::setw(priceWidth) << "Open" 
              << std::setw(priceWidth) << "High" 
              << std::setw(priceWidth) << "Low" 
              << std::setw(priceWidth) << "Close" 
              << std::endl;
    

    std::cout << std::string(dateWidth + priceWidth * 4, '-') << std::endl;

    for (auto& c : data)
    {
        std::cout << std::left 
                  << std::setw(dateWidth)  << c.getTimestamp()
                  << std::setw(priceWidth) << c.getOpen()
                  << std::setw(priceWidth) << c.getHigh()
                  << std::setw(priceWidth) << c.getLow()
                  << std::setw(priceWidth) << c.getClose()
                  << std::endl;
    }
    std::cout << std::endl; 
}
//My part
void Merkelmain::simulateNewTrades()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> price_dist(0.95, 1.05);
    std::uniform_real_distribution<> amount_dist(0.1, 1.0);

    for (const std::string& product : orderBook.getKnownProducts()) {
        std::vector<OrderBookentry> last_orders = orderBook.getOrders(OrderBooktype::ask, product, orderBook.getLatestTime());
        if (last_orders.empty()) {
            last_orders = orderBook.getOrders(OrderBooktype::bid, product, orderBook.getLatestTime());
        }
        double last_price = last_orders.empty() ? 100.0 : last_orders.back().price;

        for (int i = 0; i < 5; ++i) {
            std::string timestamp = getCurrentTimestamp();
            
            double ask_price = last_price * price_dist(gen);
            double ask_amount = amount_dist(gen);
            OrderBookentry ask_order{ask_price, ask_amount, timestamp, product, OrderBooktype::ask, userManager.getCurrentUser()->getUsername()};
            orderBook.insertOrder(ask_order);

            double bid_price = last_price * price_dist(gen);
            double bid_amount = amount_dist(gen);
            OrderBookentry bid_order{bid_price, bid_amount, timestamp, product, OrderBooktype::bid, userManager.getCurrentUser()->getUsername()};
            orderBook.insertOrder(bid_order);
        }
    }
    orderBook.saveOrders();
    std::cout << "Simulated 5 new ask and bid orders for all products." << std::endl;
}

void Merkelmain::depositFunds()
{
    std::string currency;
    double amount;
    std::cout << "Enter currency to deposit: ";
    std::getline(std::cin, currency);
    if (std::find(VALID_CURRENCIES.begin(), VALID_CURRENCIES.end(), currency) == VALID_CURRENCIES.end()) {
    std::cout << "Invalid currency. Please choose from BTC, USDT, ETH, DOGE." << std::endl;
    return;
    }
    std::cout << "Enter amount to deposit: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    currentWallet->insertcurrency(currency, amount);
    Transaction transaction(getCurrentTimestamp(), userManager.getCurrentUser()->getUsername(), TransactionType::Deposit, currency, amount);
    transactionManager.saveTransaction(transaction);
    std::cout << "Deposited " << amount << " " << currency << std::endl;
}

void Merkelmain::withdrawFunds()
{
    std::string currency;
    double amount;
    std::cout << "Enter currency to withdraw: ";
    std::getline(std::cin, currency);
    if (std::find(VALID_CURRENCIES.begin(), VALID_CURRENCIES.end(), currency) == VALID_CURRENCIES.end()) {
    std::cout << "Invalid currency. Please choose from BTC, USDT, ETH, DOGE." << std::endl;
    return;
    }
    std::cout << "Enter amount to withdraw: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (currentWallet->removecurrency(currency, amount)) {
        Transaction transaction(getCurrentTimestamp(), userManager.getCurrentUser()->getUsername(), TransactionType::Withdrawal, currency, amount);
        transactionManager.saveTransaction(transaction);
        std::cout << "Withdrew " << amount << " " << currency << std::endl;
    } else {
        std::cout << "Insufficient funds to withdraw " << amount << " " << currency << std::endl;
    }
}

void Merkelmain::viewTransactionHistory()
{
    std::cout << "Transaction History (Last 5 transactions):" << std::endl;
    std::vector<Transaction> recentTransactions = transactionManager.getRecentTransactions(userManager.getCurrentUser()->getUsername(), 5);
    for (const auto& transaction : recentTransactions) {
        std::cout << transaction.getTimestamp() << " - ";
        switch (transaction.getType()) {
            case TransactionType::Deposit:
                std::cout << "Deposit: ";
                break;
            case TransactionType::Withdrawal:
                std::cout << "Withdrawal: ";
                break;
            case TransactionType::Trade:
                std::cout << "Trade: ";
                break;
        }
        std::cout << transaction.getAmount() << " " << transaction.getCurrency();
        if (transaction.getType() == TransactionType::Trade) {
            std::cout << " @ " << transaction.getPrice();
        }
        std::cout << std::endl;
    }
}

void Merkelmain::viewTradingStatistics()
{
    std::cout << "Trading Statistics:" << std::endl;
    std::vector<OrderBookentry> userOrders;
    std::vector<std::string> timestamps;
    for(const auto& p : orderBook.getKnownProducts())
    {
    }
    
    int askCount = 0;
    int bidCount = 0;
    double totalSpent = 0.0;
    std::map<std::string, int> asksPerProduct;
    std::map<std::string, int> bidsPerProduct;

    for (const auto& order : userOrders) {
        if (order.ordertype == OrderBooktype::ask) {
            askCount++;
            asksPerProduct[order.product]++;
        } else if (order.ordertype == OrderBooktype::bid) {
            bidCount++;
            bidsPerProduct[order.product]++;
        }
    }

    std::cout << "Total Asks: " << askCount << std::endl;
    std::cout << "Total Bids: " << bidCount << std::endl;

    std::cout << "\nAsks per product:" << std::endl;
    for (const auto& pair : asksPerProduct) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nBids per product:" << std::endl;
    for (const auto& pair : bidsPerProduct) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}


int Merkelmain::getuserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-12" << std::endl;
    std::getline(std::cin, line);
    while (true) {
        try {
            userOption = std::stoi(line);
            break;
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::getline(std::cin, line);
        } catch (const std::out_of_range& e) {
            std::cout << "Input out of range. Please enter a valid number." << std::endl;
            std::getline(std::cin, line);
        }
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void Merkelmain::processUserOption(int userOption)
{
    if (userOption < 1 || userOption > 12)
    {
        std::cout << "Invalid choice. Choose 1-12" << std::endl;
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
        generateCandlestickData();
    }
    if (userOption == 7)
    {
        simulateNewTrades();
    }
    if (userOption == 8)
    {
        depositFunds();
    }
    if (userOption == 9)
    {
        withdrawFunds();
    }
    if (userOption == 10)
    {
        viewTransactionHistory();
    }
    if (userOption == 11)
    {
        viewTradingStatistics();
    }
    if (userOption == 12)
    {
        userManager.logout();
        currentWallet = nullptr;
    }
}
