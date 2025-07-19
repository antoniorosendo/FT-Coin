#ifndef ENTRYDBDAO_H_
#define ENTRYDBDAO_H_

#include "AbstractEntryDAO.hpp"
#include "ServerDBConnection.hpp"
#include "Carteira.hpp"
#include "Entry.hpp"
#include "Date.hpp"
#include "MovementType.hpp"

#include <vector>
#include <string>
#include <memory>
#include <mariadb/conncpp.hpp>

class EntryDBDAO final : public AbstractEntryDAO {
private:
    ServerDBConnection *serverDBConnection;

    //Strings SQL para TABELA: Carteira
    static const std::string SQL_CARTEIRA_ADD;
    static const std::string SQL_CARTEIRA_GET_BY_ID;
    static const std::string SQL_CARTEIRA_UPDATE;
    static const std::string SQL_CARTEIRA_DELETE;
    static const std::string SQL_CARTEIRA_GET_ALL;
    static const std::string SQL_CARTEIRA_GET_ALL_ORDER_BY_NOME;

    //Strings SQL para TABELA: Movimentacao
    static const std::string SQL_MOVIMENTACAO_ADD;
    static const std::string SQL_MOVIMENTACAO_GET_ALL;
    static const std::string SQL_MOVIMENTACAO_GET_BY_ID;
    static const std::string SQL_MOVIMENTACAO_DELETE;
    static const std::string SQL_MOVIMENTACAO_GET_BY_CARTEIRA_ID;
    static const std::string SQL_MOVIMENTACAO_GET_TOTAL_COMPRADO_CARTEIRA; 
    static const std::string SQL_MOVIMENTACAO_GET_TOTAL_VENDIDO_CARTEIRA;

    //Strings SQL para TABELA: Oraculo
    static const std::string SQL_ORACULO_GET_COTACAO_BY_DATE;
    static const std::string SQL_ORACULO_ADD_COTACAO;

    //Métodos auxiliares de conversão
    char movementTypeToChar(MovementType type) const;
    MovementType charToMovementType(char typeChar) const;

    //Métodos auxiliares de criação de objetos a partir de ResultSet
    Carteira* createCarteiraFromResultSet(sql::ResultSet* res) const;
    Entry* createMovimentacaoFromResultSet(sql::ResultSet* res) const;

public:
    EntryDBDAO(ServerDBConnection *serverDBConnection);
    virtual ~EntryDBDAO();

    //Implementação dos métodos da interface AbstractEntryDAO
    std::vector<Entry*> getAllEntries() override;
    std::vector<Entry*> getFilteredEntries(Date &begin, Date &end) override;
    std::vector<Entry*> getFilteredEntries(Date &begin, Date &end, MovementType type) override;
    Entry* getEntryById(int entryId) override;
    void addEntry(Entry* entry) override;
    void deleteEntry(int entryId) override;
    double getBalance() override;
    double getBalance(Date &date) override;

    //Métodos para a entidade Carteira
    int addCarteira(const Carteira& carteira) override;
    std::unique_ptr<Carteira> getCarteiraById(int id) override;
    void updateCarteira(const Carteira& carteira) override;
    void deleteCarteira(int id) override;
    std::vector<std::unique_ptr<Carteira>> getAllCarteiras() override;
    std::vector<std::unique_ptr<Carteira>> getAllCarteirasOrdenadasPorNome() override;

    //Métodos para a entidade Movimentacao (Entry)
    int addMovimentacao(const Entry& entry);
    std::unique_ptr<Entry> getMovimentacaoById(int entryId);
    void deleteMovimentacao(int entryId);
    std::vector<std::unique_ptr<Entry>> getMovimentacoesByCarteiraId(int idCarteira);
    
    //Métodos para cálculos financeiros
    double getTotalCompradoCarteira(int idCarteira) override;
    double getTotalVendidoCarteira(int idCarteira) override;

    //Métodos para a entidade Oraculo
    void addCotacaoOraculo(const Date& date, double cotacao) override;
    double getCotacaoOraculoByDate(const Date& date) override;
};

#endif