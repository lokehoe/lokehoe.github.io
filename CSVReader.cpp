#include "CSVReader.h"
#include <fstream>
#include <iostream>

std::vector<OrderBookentry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookentry> entries;
    std::ifstream csvFile(csvFilename);
    std::string line;

    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try
            {
                entries.push_back(stringsToOBE(tokenise(line, ',')));
            }
            catch (...)
            {
            }
        }
    }
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    size_t start = 0, end;

    while ((end = csvLine.find(separator, start)) != std::string::npos)
    {
        tokens.push_back(csvLine.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(csvLine.substr(start));
    return tokens;
}

OrderBookentry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    if (tokens.size() != 5)
        throw std::exception{};

    return OrderBookentry{
        std::stod(tokens[3]),
        std::stod(tokens[4]),
        tokens[0],
        tokens[1],
        OrderBookentry::stringToOrderBooktype(tokens[2])
    };
}

OrderBookentry CSVReader::stringsToOBE(std::string priceString,
                                      std::string amountString,
                                      std::string timestamp,
                                      std::string product,
                                      OrderBooktype ordertype)
{
    return OrderBookentry{
        std::stod(priceString),
        std::stod(amountString),
        timestamp,
        product,
        ordertype
    };
}
