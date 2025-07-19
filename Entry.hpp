#ifndef ENTRY_HPP_
#define ENTRY_HPP_

#include "Date.hpp"
#include "MovementType.hpp"

class Entry
{
private:
    int id;
    int idCarteira;
    Date date;
    MovementType type;
    double quantidade;

public:
    Entry(int idCarteira, Date data, MovementType tipo, double quantidade);

    int getId() const;
    void setId(int id);

    int getIdCarteira() const;

    const Date& getDate() const;
    MovementType getType() const;

    double getQuantidade() const;

    static void imprimirCabecalhoTabelaMov(); 
    void imprimir() const;
};

#endif