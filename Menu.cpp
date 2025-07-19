#include <iostream> 
#include <limits>   
#include <stdexcept> 
#include <string>   
#include <iomanip>  

#include "Menu.hpp"
#include "Utils.hpp"


Menu::Menu(vector<string> &itens, string title, string message) :
    itens(itens), title(title), message(message), symbol("="), width(0), repeatList(false), zeroForLastOpt(false) {}


Menu::~Menu() {}


const int Menu::getChoice()
{
    int choice;
    while (true) 
    {
        Utils::clearScreen();
        std::cout << makeDecorator() << std::endl;
        std::cout << title << std::endl;
        std::cout << makeDecorator() << std::endl;

        
        for (size_t i = 0; i < itens.size(); ++i)
        {
            if (zeroForLastOpt && i == (itens.size() - 1)) {
                
                std::cout << "0. " << itens[i] << std::endl;
            } else {
                
                std::cout << (i + 1) << ". " << itens[i] << std::endl;
            }
        }
        std::cout << makeDecorator() << std::endl; 
        std::cout << message; 

        std::cin >> choice;

        if (std::cin.fail()) 
        {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Opcao invalida. Por favor, digite um numero.\n";
            Utils::pause();
            continue;
        }

        
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        if (zeroForLastOpt && choice == 0) {
            return itens.size();
        }
        
        else if (choice < 1 || choice > itens.size())
        {
            std::cout << "Opcao invalida. Digite um numero entre 1 e " << itens.size() - 1 << ".\n"; 
            Utils::pause();
            continue; 
        }
        else 
        {
            return choice; 
        }
    }
}

void Menu::setDecorator(const string &decorator, int width)
{
    this->symbol = decorator;
    this->width = width;
}


const string& Menu::getDecorator() const
{
    return symbol;
}


const string& Menu::getMessage() const
{
    return message;
}

const int Menu::getLength() const
{
    return (itens.size());
}

void Menu::setMessage(const string &message)
{
    this->message = message;
}

bool Menu::isRepeatList() const
{
    return repeatList;
}

void Menu::setRepeatList(bool repeatList)
{
    this->repeatList = repeatList;
}

const string Menu::getSymbol() const
{
    return symbol;
}

void Menu::setSymbol(const string &symbol)
{
    this->symbol = symbol;
}

const string& Menu::getTitle() const
{
    return title;
}

void Menu::setTitle(const string &title)
{
    this->title = title;
}

int Menu::getWidth() const
{
    return width;
}

void Menu::setWidth(int width)
{
    this->width = width;
}

bool Menu::isZeroForLastOpt() const
{
    return zeroForLastOpt;
}

void Menu::setZeroForLastOpt(bool zeroForLastOpt)
{
    this->zeroForLastOpt = zeroForLastOpt;
}

const bool Menu::isNotAValidChoice(unsigned long choice) const
{
    return (choice >= itens.size()); 
}

const string Menu::replicate(string text, int times) const
{
    string buffer;
    for (int count = 0; count < times; count++)
    {
        buffer += text;
    }
    return (buffer);
}

const string Menu::makeDecorator()
{
    if (width == 0)
    {
        int max_item_len = 0;
        for (string item : itens)
        {
            if (item.length() > max_item_len)
            {
                max_item_len = item.length();
            }
        }
        
        int min_width = max_item_len + 4; 
        if (itens.size() >= 10) { 
            min_width += 1;
        }

        if (title.length() > min_width) {
            min_width = title.length();
        }

        width = (min_width / symbol.length()) * symbol.length();
        if (width < min_width) {
            width += symbol.length(); 
        }
        if (width == 0) width = 40; 
    }
    return (replicate(symbol, width));
}