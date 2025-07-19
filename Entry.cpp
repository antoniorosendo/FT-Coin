#include "Date.hpp"
#include "MovementType.hpp"
#include <string>
#include "Entry.hpp"
#include <iostream>
#include <iomanip>

Entry::Entry(int idCarteira, Date date, MovementType type, double quantidade)
    : id(0), idCarteira(idCarteira), date(date), type(type),
      quantidade(quantidade) {}

int Entry::getId() const { return id; }
void Entry::setId(int id) { this->id = id; }

int Entry::getIdCarteira() const { return idCarteira; }

const Date& Entry::getDate() const { return date; }

MovementType Entry::getType() const { return type; }

double Entry::getQuantidade() const { return quantidade; }

void Entry::imprimirCabecalhoTabelaMov() {
  std::cout << std::setfill('=') << std::setw(50) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left
              << std::setw(5) << "ID"
              << std::setw(12) << "DATA"
              << std::setw(10) << "TIPO"
              << std::setw(15) << "QUANTIDADE"
              << std::endl;
    std::cout << std::setfill('=') << std::setw(50) << "" << std::endl;
} 

void Entry::imprimir() const {
  std::cout << std::setfill(' ') << std::left
            << std::setw(5) << id
            << std::setw(12) << date.getIsoFormat()
            << std::setw(10) << (type == MovementType::Compra ? "Compra" : "Venda")
            << std::setw(15) << std::fixed << std::setprecision(4) << quantidade
            << std::endl;
}