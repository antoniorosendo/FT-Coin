#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

#include "EntryDBDAO.hpp"
#include "Carteira.hpp"
#include "Entry.hpp"
#include "Date.hpp"
#include "MovementType.hpp"

std::vector<Entry*> EntryDBDAO::getAllEntries() {
    throw std::runtime_error("getAllEntries não implementado.");
}

std::vector<Entry*> EntryDBDAO::getFilteredEntries(Date &begin, Date &end) {
    throw std::runtime_error("getFilteredEntries não implementado.");
}

std::vector<Entry*> EntryDBDAO::getFilteredEntries(Date &begin, Date &end, MovementType type) {
    throw std::runtime_error("getFilteredEntries não implementado.");
}

Entry* EntryDBDAO::getEntryById(int entryId) {
    throw std::runtime_error("getEntryById não implementado.");
}

void EntryDBDAO::addEntry(Entry* entry) {
    throw std::runtime_error("addEntry não implementado.");
}

void EntryDBDAO::deleteEntry(int entryId) {
    throw std::runtime_error("deleteEntry não implementado.");
}

double EntryDBDAO::getBalance() {
    throw std::runtime_error("getBalance não implementado.");
}

double EntryDBDAO::getBalance(Date &date) {
    throw std::runtime_error("getBalance não implementado.");
}

//Definição das Strings SQL para Carteira
const std::string EntryDBDAO::SQL_CARTEIRA_ADD =
    "INSERT INTO CARTEIRA (Titular, Corretora) VALUES (?, ?)";
const std::string EntryDBDAO::SQL_CARTEIRA_GET_BY_ID =
    "SELECT IdCarteira, Titular, Corretora FROM CARTEIRA WHERE IdCarteira = ?";
const std::string EntryDBDAO::SQL_CARTEIRA_UPDATE =
    "UPDATE CARTEIRA SET Titular = ?, Corretora = ? WHERE IdCarteira = ?";
const std::string EntryDBDAO::SQL_CARTEIRA_DELETE =
    "DELETE FROM CARTEIRA WHERE IdCarteira = ?";
const std::string EntryDBDAO::SQL_CARTEIRA_GET_ALL =
    "SELECT IdCarteira, Titular, Corretora FROM CARTEIRA ORDER BY IdCarteira ASC";
const std::string EntryDBDAO::SQL_CARTEIRA_GET_ALL_ORDER_BY_NOME =
    "SELECT IdCarteira, Titular, Corretora FROM CARTEIRA ORDER BY Titular ASC";

//Definição das Strings SQL para Movimentacao (Entry)
const std::string EntryDBDAO::SQL_MOVIMENTACAO_ADD =
    "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) VALUES (?, ?, ?, ?)"; 
const std::string EntryDBDAO::SQL_MOVIMENTACAO_GET_ALL =
    "SELECT IdMovimento, IdCarteira, Data, TipoOperacao, Quantidade FROM MOVIMENTACAO ORDER BY Data DESC, IdMovimento DESC";
const std::string EntryDBDAO::SQL_MOVIMENTACAO_GET_BY_ID =
    "SELECT IdMovimento, IdCarteira, Data, TipoOperacao, Quantidade FROM MOVIMENTACAO WHERE IdMovimento = ?";
const std::string EntryDBDAO::SQL_MOVIMENTACAO_DELETE =
    "DELETE FROM MOVIMENTACAO WHERE IdMovimento = ?";

const std::string EntryDBDAO::SQL_MOVIMENTACAO_GET_BY_CARTEIRA_ID =
    "SELECT IdMovimento, IdCarteira, Data, TipoOperacao, Quantidade FROM MOVIMENTACAO WHERE IdCarteira = ? ORDER BY Data ASC, IdMovimento ASC";

const std::string EntryDBDAO::SQL_MOVIMENTACAO_GET_TOTAL_COMPRADO_CARTEIRA =
    "SELECT SUM(Quantidade) FROM MOVIMENTACAO WHERE IdCarteira = ? AND TipoOperacao = 'C'";
const std::string EntryDBDAO::SQL_MOVIMENTACAO_GET_TOTAL_VENDIDO_CARTEIRA =
    "SELECT SUM(Quantidade) FROM MOVIMENTACAO WHERE IdCarteira = ? AND TipoOperacao = 'V'";


//Definição das Strings SQL para Oraculo
const std::string EntryDBDAO::SQL_ORACULO_GET_COTACAO_BY_DATE =
    "SELECT Cotacao FROM ORACULO WHERE Data = ?";
const std::string EntryDBDAO::SQL_ORACULO_ADD_COTACAO =
    "INSERT INTO ORACULO (Data, Cotacao) VALUES (?, ?)";


//Construtor e Destrutor
EntryDBDAO::EntryDBDAO(ServerDBConnection *serverDBConnection)
    : serverDBConnection(serverDBConnection) {}
EntryDBDAO::~EntryDBDAO() {}

//Métodos Auxiliares de Conversão
char EntryDBDAO::movementTypeToChar(MovementType type) const {
    return (type == MovementType::Compra ? 'C' : 'V');
}
MovementType EntryDBDAO::charToMovementType(char typeChar) const {
    if (typeChar == 'C') return MovementType::Compra;
    if (typeChar == 'V') return MovementType::Venda;
    throw std::runtime_error("Tipo de movimento invalido no banco de dados.");
}

//Métodos Auxiliares de Criação de Objetos a partir de ResultSet
Carteira* EntryDBDAO::createCarteiraFromResultSet(sql::ResultSet* res) const {
    int id = res->getInt("IdCarteira");
    std::string titular = static_cast<std::string>(res->getString("Titular"));
    std::string corretora = static_cast<std::string>(res->getString("Corretora"));
    return new Carteira(id, titular, corretora);
}
Entry* EntryDBDAO::createMovimentacaoFromResultSet(sql::ResultSet* res) const {
    int id = res->getInt("IdMovimento"); 
    int idCarteira = res->getInt("IdCarteira");
    Date date(static_cast<std::string>(res->getString("Data")));
    MovementType type = charToMovementType(res->getString("TipoOperacao")[0]);
    double quantidade = res->getDouble("Quantidade");
    Entry* entry = new Entry(idCarteira, date, type, quantidade);
    entry->setId(id);
    return entry;
}

//Implementação dos Métodos para Carteira
int EntryDBDAO::addCarteira(const Carteira& carteira) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_CARTEIRA_ADD));
        stmnt->setString(1, carteira.getTitular());
        stmnt->setString(2, carteira.getCorretora());
        stmnt->execute();
        std::unique_ptr<sql::Statement> stmt_last_id(serverDBConnection->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> res_last_id(stmt_last_id->executeQuery("SELECT LAST_INSERT_ID()"));
        if (res_last_id->next()) {
            return res_last_id->getInt(1);
        }
        return 0;
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao inserir nova carteira: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao adicionar carteira ao banco de dados.");
    }
}
std::unique_ptr<Carteira> EntryDBDAO::getCarteiraById(int id) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_CARTEIRA_GET_BY_ID));
        stmnt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
        if (res->next()) {
            return std::unique_ptr<Carteira>(createCarteiraFromResultSet(res.get()));
        }
        return nullptr;
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao buscar carteira por ID: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao buscar carteira por ID no banco de dados.");
    }
}
void EntryDBDAO::updateCarteira(const Carteira& carteira) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_CARTEIRA_UPDATE));
        stmnt->setString(1, carteira.getTitular());
        stmnt->setString(2, carteira.getCorretora());
        stmnt->setInt(3, carteira.getId());
        stmnt->execute();
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao atualizar carteira: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao atualizar carteira no banco de dados.");
    }
}
void EntryDBDAO::deleteCarteira(int id) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_CARTEIRA_DELETE));
        stmnt->setInt(1, id);
        stmnt->execute();
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao excluir carteira: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao excluir carteira do banco de dados.");
    }
}
std::vector<std::unique_ptr<Carteira>> EntryDBDAO::getAllCarteiras() {
    std::vector<std::unique_ptr<Carteira>> carteirasDB;
    try {
        std::unique_ptr<sql::Statement> stmnt(serverDBConnection->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(SQL_CARTEIRA_GET_ALL));
        while (res->next()) {
            carteirasDB.push_back(std::unique_ptr<Carteira>(createCarteiraFromResultSet(res.get())));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao selecionar todas as carteiras: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao recuperar carteiras do banco de dados.");
    }
    return carteirasDB;
}
std::vector<std::unique_ptr<Carteira>> EntryDBDAO::getAllCarteirasOrdenadasPorNome() {
    std::vector<std::unique_ptr<Carteira>> carteirasDB;
    try {
        std::unique_ptr<sql::Statement> stmnt(serverDBConnection->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery(SQL_CARTEIRA_GET_ALL_ORDER_BY_NOME));
        while (res->next()) {
            carteirasDB.push_back(std::unique_ptr<Carteira>(createCarteiraFromResultSet(res.get())));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao selecionar carteiras ordenadas por nome: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao recuperar carteiras ordenadas por nome do banco de dados.");
    }
    return carteirasDB;
}

//Implementação dos Métodos para Movimentacao (Entry)
int EntryDBDAO::addMovimentacao(const Entry& entry) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_MOVIMENTACAO_ADD));
        stmnt->setInt(1, entry.getIdCarteira());
        stmnt->setDateTime(2, entry.getDate().getIsoFormat());
        stmnt->setString(3, std::string(1, movementTypeToChar(entry.getType())));
        stmnt->setDouble(4, entry.getQuantidade());
        stmnt->execute();
        std::unique_ptr<sql::Statement> stmt_last_id(serverDBConnection->getConnection()->createStatement());
        std::unique_ptr<sql::ResultSet> res_last_id(stmt_last_id->executeQuery("SELECT LAST_INSERT_ID()"));
        if (res_last_id->next()) {
            return res_last_id->getInt(1);
        }
        return 0;
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao inserir nova movimentacao: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao adicionar movimentacao ao banco de dados.");
    }
}
std::unique_ptr<Entry> EntryDBDAO::getMovimentacaoById(int entryId) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_MOVIMENTACAO_GET_BY_ID));
        stmnt->setInt(1, entryId);
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
        if (res->next()) {
            return std::unique_ptr<Entry>(createMovimentacaoFromResultSet(res.get()));
        }
        return nullptr;
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao buscar movimentacao por ID: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao buscar movimentacao por ID no banco de dados.");
    }
}
void EntryDBDAO::deleteMovimentacao(int entryId) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_MOVIMENTACAO_DELETE));
        stmnt->setInt(1, entryId);
        stmnt->execute();
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao excluir movimentacao: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao excluir movimentacao do banco de dados.");
    }
}

std::vector<std::unique_ptr<Entry>> EntryDBDAO::getMovimentacoesByCarteiraId(int idCarteira) {
    std::vector<std::unique_ptr<Entry>> entriesDB;
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_MOVIMENTACAO_GET_BY_CARTEIRA_ID));
        stmnt->setInt(1, idCarteira);
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
        while (res->next()) {
            entriesDB.push_back(std::unique_ptr<Entry>(createMovimentacaoFromResultSet(res.get())));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao obter movimentacoes para carteira ID " << idCarteira << ": " << e.what() << std::endl;
        throw std::runtime_error("Falha ao recuperar historico de movimentacoes do banco de dados.");
    }
    return entriesDB;
}
double EntryDBDAO::getTotalCompradoCarteira(int idCarteira) {
    double total = 0.0;
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_MOVIMENTACAO_GET_TOTAL_COMPRADO_CARTEIRA));
        stmnt->setInt(1, idCarteira);
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
        if (res->next()) {
            total = res->getDouble(1);
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao obter total comprado da carteira (FTC): " << e.what() << std::endl;
        throw std::runtime_error("Falha ao obter total comprado do banco de dados.");
    }
    return total;
}

double EntryDBDAO::getTotalVendidoCarteira(int idCarteira) {
    double total = 0.0;
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_MOVIMENTACAO_GET_TOTAL_VENDIDO_CARTEIRA));
        stmnt->setInt(1, idCarteira);
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
        if (res->next()) {
            total = res->getDouble(1);
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao obter total vendido da carteira (FTC): " << e.what() << std::endl;
        throw std::runtime_error("Falha ao obter total vendido do banco de dados.");
    }
    return total;
}

void EntryDBDAO::addCotacaoOraculo(const Date& date, double cotacao) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_ORACULO_ADD_COTACAO));
        stmnt->setDateTime(1, date.getIsoFormat());
        stmnt->setDouble(2, cotacao);
        stmnt->execute();
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao adicionar cotacao para a data " << date.getIsoFormat() << ": " << e.what() << std::endl;
    }
}
double EntryDBDAO::getCotacaoOraculoByDate(const Date& date) {
    double cotacao = 0.0;
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_ORACULO_GET_COTACAO_BY_DATE));
        stmnt->setDateTime(1, date.getIsoFormat());
        std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery());
        if (res->next()) {
            cotacao = res->getDouble("Cotacao");
        } else {
            throw std::runtime_error("Cotacao nao encontrada para a data: " + date.getIsoFormat());
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao buscar cotacao por data: " << e.what() << std::endl;
        throw std::runtime_error("Falha ao buscar cotacao no banco de dados.");
    }
    return cotacao;
}