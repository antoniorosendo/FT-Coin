#include <iostream>   
#include <limits>     
#include <string>     
#include <iomanip> 
#include "Utils.hpp"    
#include "SysInfo.hpp"  

void Utils::printMessage(std::string text)
{
    std::cout << "* " << SysInfo::getSystemName() << " message: " << text << std::endl;
}

std::string Utils::replicate(std::string text, int times)
{
    std::string buffer;
    for (int count = 0; count < times; count++)
    {
        buffer += text;
    }
    return (buffer);
}

void Utils::printFramedMessage(std::string text, std::string symbol, int length)
{
    std::cout << replicate(symbol, length) << std::endl;
    std::cout << text << std::endl;
    std::cout << replicate(symbol, length) << std::endl;
}

void Utils::clearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Utils::imprimirCabecalhoTabelaMovimentacao() {
    std::cout << std::setfill('-') << std::setw(60) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left
              << std::setw(5) << "ID"
              << std::setw(12) << "DATA"
              << std::setw(10) << "TIPO"
              << std::setw(15) << "QUANTIDADE"
              << std::endl;
    std::cout << std::setfill('-') << std::setw(60) << "" << std::endl;
}

void Utils::pause()
{
    std::cout << "\nPressione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Utils::clearScreen() 
{
    system("clear");
}