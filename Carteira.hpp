#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include <string>
#include <iostream>

class Carteira 
{
private:
    int id;
    std::string titular;
    std::string corretora;
    double saldo;

public:
    Carteira(int id, const std::string& titular, const std::string& corretora);
    Carteira();

    int getId() const;
    const std::string& getTitular() const;
    const std::string& getCorretora() const;
    double getSaldo() const;

    void setTitular(const std::string& novoTitular);
    void setCorretora(const std::string& novaCorretora);

    void adicionarSaldo(double valor);
    void removerSaldo(double valor);

    static void imprimirCabecalhoTabelaCarteira(); 
    void imprimirSaldo(double saldoCalculado) const;
    void imprimir(double saldoAtual) const;
};

#endif
