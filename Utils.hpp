#ifndef UTILS_H_
#define UTILS_H_

#include <string> 
#include <iostream> 
#include <limits> 


namespace Utils
{
    
    void printMessage(std::string text);
    void printFramedMessage(std::string text, std::string symbol, int length);
    std::string replicate(std::string text, int times);

    void imprimirCabecalhoTabelaMovimentacao();
    void clearInputBuffer(); 
    void pause();     
    void clearScreen();       
}

#endif