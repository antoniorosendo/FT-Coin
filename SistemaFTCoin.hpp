#ifndef SISTEMA_FTCOIN_HPP
#define SISTEMA_FTCOIN_HPP

#include <string>
#include <memory>
#include <vector>

#include "EntryDBDAO.hpp"
#include "AbstractEntryDAO.hpp"
#include "Carteira.hpp"
#include "MovementType.hpp"
#include "Entry.hpp"
#include "Date.hpp"

class SistemaFTCoin 
{
private:
    std::shared_ptr<AbstractEntryDAO> dbDAO;

public:
    SistemaFTCoin();

    void setDBDAO(std::shared_ptr<AbstractEntryDAO> dao);
    std::shared_ptr<AbstractEntryDAO> getDBDAO() const;

    int adicionarCarteira(const std::string& titular, const std::string& corretora);
    Carteira getCarteira(int id);
    void editarCarteira(int id, const std::string& novoTitular, const std::string& novaCorretora);
    void excluirCarteira(int id);
    void listarCarteirasOrdenadasPorId() const;
    void listarCarteirasOrdenadasPorNome() const;

    void registrarMovimentacao(int idCarteira, MovementType tipo, double quantidade);
    std::vector<std::unique_ptr<Entry>> getHistoricoMovimentacoes(int idCarteira) const;
    double calcularSaldo(int idCarteira) const;

    double consultarPrecoOraculo() const;

    void exibirGanhoOuPerda() const;
};

#endif