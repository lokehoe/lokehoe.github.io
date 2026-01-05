#pragma once

#include "OrderBookentry.h"
#include <vector>
#include <string>

class CSVReader
{
    public:
     CSVReader();

     static std::vector<OrderBookentry> readCSV(std::string csvFile);
     static std::vector<std::string> tokenise(std::string csvLine, char separator);
     static OrderBookentry stringsToOBE(std::string price,
                                        std::string amount,
                                        std::string timestamp,
                                        std::string product,
                                        OrderBooktype OrderBooktype);

    private:
     static OrderBookentry stringsToOBE(std::vector<std::string> tokens);
     
}; 
