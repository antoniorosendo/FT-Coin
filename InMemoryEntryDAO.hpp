#ifndef INMEMORYENTRYDAO_HPP_
#define INMEMORYENTRYDAO_HPP_

#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include "Carteira.hpp"
#include "AbstractEntryDAO.hpp"

const std::string CARTEIRAS_FILE = "data/carteiras.csv";
const std::string MOVIMENTACOES_FILE = "data/movimentacoes.csv";
const std::string COTACOES_FILE = "data/cotacoes.csv";

class InMemoryEntryDAO : public AbstractEntryDAO
{
private:
    std::map<int, std::unique_ptr<Carteira>> carteiras;
    std::map<int, std::unique_ptr<Entry>> movimentacoes;
    std::map<std::string, double> cotacoesOraculo;
    int nextCarteiraId = 1;
    int nextMovimentacaoId = 1;

    void loadCarteirasFromFile();
    void saveCarteirasToFile() const;
    void loadMovimentacoesFromFile();
    void saveMovimentacoesToFile() const;
    void loadCotacoesFromFile();
    void saveCotacoesToFile() const;

public:
    InMemoryEntryDAO();

    std::vector<Entry*> getAllEntries();
    std::vector<Entry*> getFilteredEntries(Date &begin, Date &end);
    std::vector<Entry*> getFilteredEntries(Date &begin, Date &end, MovementType type);
    Entry* getEntryById(int entryId);
    void addEntry(Entry* entry);
    void deleteEntry(int entryId);
    double getBalance();
    double getBalance(Date &date);

    int addCarteira(const Carteira& carteira);
    std::unique_ptr<Carteira> getCarteiraById(int id);
    void updateCarteira(const Carteira& carteira);
    void deleteCarteira(int id);
    std::vector<std::unique_ptr<Carteira>> getAllCarteiras();
    std::vector<std::unique_ptr<Carteira>> getAllCarteirasOrdenadasPorNome();

    int addMovimentacao(const Entry& entry);
    std::unique_ptr<Entry> getMovimentacaoById(int entryId);
    void deleteMovimentacao(int entryId);
    std::vector<std::unique_ptr<Entry>> getMovimentacoesByCarteiraId(int idCarteira);

    double getTotalCompradoCarteira(int idCarteira);
    double getTotalVendidoCarteira(int idCarteira);

    void addCotacaoOraculo(const Date& date, double cotacao);
    double getCotacaoOraculoByDate(const Date& date);
    
    void clear();
};

#endif