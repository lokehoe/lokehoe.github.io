#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "Orderbook.h"
#include "wallet.h"

class Merkelmain
{
    public:
        Merkelmain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void EnterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int  getuserOption();
        void processUserOption(int userOption);

        std::string currentTime;
        Orderbook orderBook{"20200317.csv"};
        std::vector<OrderBookentry> orders;

        Wallet wallet;

}; 