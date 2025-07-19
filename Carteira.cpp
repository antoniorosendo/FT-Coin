#include "Carteira.hpp"
#include <iostream>
#include <algorithm> 
#include <iomanip>

Carteira::Carteira(int id, const std::string &titular, const std::string &corretora)
    : id(id), titular(titular), corretora(corretora), saldo(0.0) {}

Carteira::Carteira()
    : id(0), titular(""), saldo(0.0) {}

int Carteira::getId() const
{
    return id;
}
    
const std::string& Carteira::getTitular() const
{
    return titular;
}

const std::string& Carteira::getCorretora() const
{
    return corretora;
}

double Carteira::getSaldo() const
{
    return saldo;
}

void Carteira::setTitular(const std::string& novoTitular)
{
    titular = novoTitular;
}

void Carteira::setCorretora(const std::string& novaCorretora)
{
    corretora = novaCorretora;
}

void Carteira::adicionarSaldo(double valor)
{
    saldo += valor;
}

void Carteira::removerSaldo(double valor)
{
    if (valor > saldo)
    {
        std::cerr << "Saldo insuficiente para remover " << valor << " moedas.\n";
    }
    else
    {
        saldo -= valor;
    }
}

void Carteira::imprimirCabecalhoTabelaCarteira() {
    std::cout << std::setfill('=') << std::setw(80) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left
              << std::setw(8) << "ID"
              << std::setw(30) << "TITULAR"
              << std::setw(25) << "CORRETORA"
              << std::setw(15) << "SALDO (FTC)"
              << std::endl;
    std::cout << std::setfill('=') << std::setw(80) << "" << std::endl;
}

void Carteira::imprimirSaldo(double saldoCalculado) const {
    std::cout << std::setfill('=') << std::setw(40) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left
              << std::setw(10) << "ID"
              << std::setw(20) << "SALDO (FTC)"
              << std::endl;
    std::cout << std::setfill('=') << std::setw(40) << "" << std::endl;
    std::cout << std::setfill(' ') << std::left
              << std::setw(10) << id
              << std::setw(20) << std::fixed << std::setprecision(4) << saldoCalculado
              << std::endl;
    std::cout << std::setfill('=') << std::setw(40) << "" << std::endl;
}

void Carteira::imprimir(double saldoAtual) const
{
    std::cout << std::setfill(' ') << std::left
              << std::setw(8) << id
              << std::setw(30) << titular
              << std::setw(25) << corretora
              << std::setw(15) << std::fixed << std::setprecision(4) << saldoAtual
              << std::endl;
}