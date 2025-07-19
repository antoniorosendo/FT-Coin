#ifndef ABSTRACT_ENTRY_DAO_HPP
#define ABSTRACT_ENTRY_DAO_HPP

#include <vector>
#include "Entry.hpp"
#include "Date.hpp"
#include "MovementType.hpp"
#include "Carteira.hpp"
#include <memory>


class AbstractEntryDAO {
public:
    virtual ~AbstractEntryDAO() = default;

    virtual std::vector<Entry*> getAllEntries() = 0;
    virtual std::vector<Entry*> getFilteredEntries(Date &begin, Date &end) = 0;
    virtual std::vector<Entry*> getFilteredEntries(Date &begin, Date &end, MovementType type) = 0;
    virtual Entry* getEntryById(int entryId) = 0;
    virtual void addEntry(Entry* entry) = 0;
    virtual void deleteEntry(int entryId) = 0;
    virtual double getBalance() = 0;
    virtual double getBalance(Date &date) = 0;

    virtual int addCarteira(const Carteira& carteira) = 0;
    virtual std::unique_ptr<Carteira> getCarteiraById(int id) = 0;
    virtual void updateCarteira(const Carteira& carteira) = 0;
    virtual void deleteCarteira(int id) = 0;
    virtual std::vector<std::unique_ptr<Carteira>> getAllCarteiras() = 0;
    virtual std::vector<std::unique_ptr<Carteira>> getAllCarteirasOrdenadasPorNome() = 0;

    virtual int addMovimentacao(const Entry& entry) = 0;
    virtual std::unique_ptr<Entry> getMovimentacaoById(int entryId) = 0;
    virtual void deleteMovimentacao(int entryId) = 0;
    virtual std::vector<std::unique_ptr<Entry>> getMovimentacoesByCarteiraId(int idCarteira) = 0;

    virtual double getTotalCompradoCarteira(int idCarteira) = 0;
    virtual double getTotalVendidoCarteira(int idCarteira) = 0;

    virtual void addCotacaoOraculo(const Date& date, double cotacao) = 0;
    virtual double getCotacaoOraculoByDate(const Date& date) = 0;
};

#endif