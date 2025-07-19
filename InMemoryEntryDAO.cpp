#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <stdexcept>         
#include <string>
#include <iostream>

#include "InMemoryEntryDAO.hpp"
#include "Carteira.hpp"
#include "Entry.hpp"
#include "Date.hpp"
#include "MovementType.hpp"

InMemoryEntryDAO::InMemoryEntryDAO() : nextCarteiraId(1), nextMovimentacaoId(1) {
    loadCarteirasFromFile();
    loadMovimentacoesFromFile();
    loadCotacoesFromFile();

    for (const auto& pair : carteiras) {
        if (pair.first >= nextCarteiraId) {
            nextCarteiraId = pair.first + 1;
        }
    }
    for (const auto& pair : movimentacoes) {
        if (pair.first >= nextMovimentacaoId) {
            nextMovimentacaoId = pair.first + 1;
        }
    }
}

void InMemoryEntryDAO::loadCarteirasFromFile() {
    std::ifstream file(CARTEIRAS_FILE);
    if (!file.is_open()) {
        std::cerr << "AVISO: Arquivo de carteiras não encontrado: " << CARTEIRAS_FILE << ". Iniciando com dados vazios.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> parts;

        while(std::getline(ss, segment, ',')) {
            parts.push_back(segment);
        }

        if (parts.size() == 3) {
            try {
                int id = std::stoi(parts[0]);
                std::string titular = parts[1];
                std::string corretora = parts[2];
                carteiras[id] = std::unique_ptr<Carteira>(new Carteira(id, titular, corretora));
            } catch (const std::exception& e) {
                std::cerr << "ERRO ao parsear linha de carteira: " << line << " - " << e.what() << "\n";
            }
        } else {
            std::cerr << "AVISO: Linha de carteira mal formatada: " << line << "\n";
        }
    }
    file.close();
}

void InMemoryEntryDAO::loadMovimentacoesFromFile() {
    std::ifstream file(MOVIMENTACOES_FILE);
    if (!file.is_open()) {
        std::cerr << "AVISO: Arquivo de movimentações não encontrado: " << MOVIMENTACOES_FILE << ". Iniciando com dados vazios.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> parts;

        while(std::getline(ss, segment, ',')) {
            parts.push_back(segment);
        }

        if (parts.size() == 5) { 
            try {
                int id = std::stoi(parts[0]);
                int idCarteira = std::stoi(parts[1]);
                Date date(parts[2]);
                MovementType type = static_cast<MovementType>(std::stoi(parts[3]));
                double quantidade = std::stod(parts[4]);

                movimentacoes[id] = std::unique_ptr<Entry>(new Entry(idCarteira, date, type, quantidade));
                movimentacoes[id]->setId(id);
            } catch (const std::exception& e) {
                std::cerr << "ERRO ao parsear linha de movimentação: " << line << " - " << e.what() << "\n";
            }
        } else {
            std::cerr << "AVISO: Linha de movimentação mal formatada (esperado 5 campos): " << line << "\n";
        }
    }
    file.close();
}

void InMemoryEntryDAO::loadCotacoesFromFile() {
    std::ifstream file(COTACOES_FILE);
    if (!file.is_open()) {
        std::cerr << "AVISO: Arquivo de cotações não encontrado: " << COTACOES_FILE << ". Iniciando com dados vazios.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date_str;
        std::string cotacao_str;

        if (std::getline(ss, date_str, ',') && std::getline(ss, cotacao_str)) {
            try {
                double cotacao = std::stod(cotacao_str);
                cotacoesOraculo[date_str] = cotacao;
            } catch (const std::exception& e) {
                std::cerr << "ERRO ao parsear linha de cotação: " << line << " - " << e.what() << "\n";
            }
        } else {
            std::cerr << "AVISO: Linha de cotação mal formatada: " << line << "\n";
        }
    }
    file.close();
}

void InMemoryEntryDAO::saveCarteirasToFile() const {
    std::ofstream file(CARTEIRAS_FILE);
    if (!file.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo de carteiras para escrita: " << CARTEIRAS_FILE << "\n";
        return;
    }
    for (const auto& pair : carteiras) {
        file << pair.second->getId() << ","
             << pair.second->getTitular() << ","
             << pair.second->getCorretora() << "\n";
    }
    file.close();
}

void InMemoryEntryDAO::saveMovimentacoesToFile() const {
    std::ofstream file(MOVIMENTACOES_FILE);
    if (!file.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo de movimentacoes para escrita: " << MOVIMENTACOES_FILE << "\n";
        return;
    }
    for (const auto& pair : movimentacoes) {
        file << pair.second->getId() << ","
             << pair.second->getIdCarteira() << ","
             << pair.second->getDate().getIsoFormat() << ","
             << static_cast<int>(pair.second->getType()) << ","
             << pair.second->getQuantidade() << "\n";
    }
    file.close();
}

void InMemoryEntryDAO::saveCotacoesToFile() const {
    std::ofstream file(COTACOES_FILE);
    if (!file.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo de cotacoes para escrita: " << COTACOES_FILE << "\n";
        return;
    }
    for (const auto& pair : cotacoesOraculo) {
        file << pair.first << ","
             << pair.second << "\n";
    }
    file.close();
}

std::vector<Entry*> InMemoryEntryDAO::getAllEntries()
{
    std::vector<Entry*> result;
    for (auto& pair : movimentacoes) 
    {
        result.push_back(pair.second.get());
    }
    return result;
}

std::vector<Entry*> InMemoryEntryDAO::getFilteredEntries(Date &begin, Date &end)
{
    std::vector<Entry*> result;
    for (auto& pair : movimentacoes) {
        if (pair.second->getDate() >= begin && pair.second->getDate() <= end) {
            result.push_back(pair.second.get());
        }
    }
    return result;
}
void InMemoryEntryDAO::clear() {
    movimentacoes.clear();
    carteiras.clear();
    cotacoesOraculo.clear();
    nextMovimentacaoId = 1;
    nextCarteiraId = 1;

    saveCarteirasToFile();
    saveMovimentacoesToFile();
    saveCotacoesToFile();
}

std::vector<Entry*> InMemoryEntryDAO::getFilteredEntries(Date &begin, Date &end, MovementType type)
{
    std::vector<Entry*> result;
    for (auto& pair : movimentacoes) {
        if (pair.second->getDate() >= begin && pair.second->getDate() <= end && pair.second->getType() == type) {
            result.push_back(pair.second.get());
        }
    }
    return result;
}

Entry* InMemoryEntryDAO::getEntryById(int entryId)
{
    auto it = movimentacoes.find(entryId);
    if (it != movimentacoes.end()) {
        return it->second.get();
    }
    return nullptr;
}

void InMemoryEntryDAO::addEntry(Entry *entry)
{
    entry->setId(nextMovimentacaoId++);
    movimentacoes[entry->getId()] = std::unique_ptr<Entry>(entry);
    saveMovimentacoesToFile();
}

void InMemoryEntryDAO::deleteEntry(int entryId)
{
    movimentacoes.erase(entryId);
    saveMovimentacoesToFile();
}

double InMemoryEntryDAO::getBalance()
{
    double total = 0.0;
    for (auto& pair : movimentacoes) {
        total += (pair.second->getType() == MovementType::Compra ? 1 : -1) * pair.second->getQuantidade();
    }
    return total;
}

double InMemoryEntryDAO::getBalance(Date &date)
{
    double total = 0.0;
    for (auto& pair : movimentacoes) {
        if (pair.second->getDate() <= date) {
            total += (pair.second->getType() == MovementType::Compra ? 1 : -1) * pair.second->getQuantidade();
        }
    }
    return total;
}

int InMemoryEntryDAO::addCarteira(const Carteira& carteira) {
    int id = nextCarteiraId++;
    carteiras[id] = std::unique_ptr<Carteira>(new Carteira(id, carteira.getTitular(), carteira.getCorretora()));
    saveCarteirasToFile();
    return id;
}

std::unique_ptr<Carteira> InMemoryEntryDAO::getCarteiraById(int id) {
    auto it = carteiras.find(id);
    if (it != carteiras.end()) {
        return std::unique_ptr<Carteira>(new Carteira(*(it->second)));
    }
    return nullptr;
}

void InMemoryEntryDAO::updateCarteira(const Carteira& carteira) {
    auto it = carteiras.find(carteira.getId());
    if (it != carteiras.end()) {
        it->second->setTitular(carteira.getTitular());
        it->second->setCorretora(carteira.getCorretora());
        saveCarteirasToFile();
    } else {
        throw std::runtime_error("Carteira com ID " + std::to_string(carteira.getId()) + " nao encontrada para atualizacao.");
    }
}

void InMemoryEntryDAO::deleteCarteira(int id) {
    auto it = carteiras.find(id);
    if (it != carteiras.end()) {
        carteiras.erase(it);
        saveCarteirasToFile();
    } else {
        throw std::runtime_error("Carteira com ID " + std::to_string(id) + " nao encontrada para exclusao.");
    }
}

std::vector<std::unique_ptr<Carteira>> InMemoryEntryDAO::getAllCarteiras() {
    std::vector<std::unique_ptr<Carteira>> result;
    for (const auto& pair : carteiras) {
        result.push_back(std::unique_ptr<Carteira>(new Carteira(*(pair.second))));
    }
    std::sort(result.begin(), result.end(), [](const std::unique_ptr<Carteira>& a, const std::unique_ptr<Carteira>& b) {
        return a->getId() < b->getId();
    });
    return result;
}

std::vector<std::unique_ptr<Carteira>> InMemoryEntryDAO::getAllCarteirasOrdenadasPorNome() {
    std::vector<std::unique_ptr<Carteira>> result;
    for (const auto& pair : carteiras) {
        result.push_back(std::unique_ptr<Carteira>(new Carteira(*(pair.second))));
    }
    std::sort(result.begin(), result.end(), [](const std::unique_ptr<Carteira>& a, const std::unique_ptr<Carteira>& b) {
        return a->getTitular() < b->getTitular();
    });
    return result;
}

int InMemoryEntryDAO::addMovimentacao(const Entry& entry) {
    int id = nextMovimentacaoId++;
    movimentacoes[id] = std::unique_ptr<Entry>(new Entry(entry.getIdCarteira(), entry.getDate(), entry.getType(), entry.getQuantidade()));
    movimentacoes[id]->setId(id);
    saveMovimentacoesToFile();
    return id;
}

std::unique_ptr<Entry> InMemoryEntryDAO::getMovimentacaoById(int entryId) {
    auto it = movimentacoes.find(entryId);
    if (it != movimentacoes.end()) {
        return std::unique_ptr<Entry>(new Entry(*(it->second)));
    }
    return nullptr;
}

void InMemoryEntryDAO::deleteMovimentacao(int entryId) {
    auto it = movimentacoes.find(entryId);
    if (it != movimentacoes.end()) {
        movimentacoes.erase(it);
        saveMovimentacoesToFile();
    } else {
        throw std::runtime_error("Movimentacao com ID " + std::to_string(entryId) + " nao encontrada para exclusao.");
    }
}

std::vector<std::unique_ptr<Entry>> InMemoryEntryDAO::getMovimentacoesByCarteiraId(int idCarteira) {
    std::vector<std::unique_ptr<Entry>> result;
    for (const auto& pair : movimentacoes) {
        if (pair.second->getIdCarteira() == idCarteira) {
            result.push_back(std::unique_ptr<Entry>(new Entry(*(pair.second))));
        }
    }
    
    std::sort(result.begin(), result.end(), [](const std::unique_ptr<Entry>& a, const std::unique_ptr<Entry>& b) {
        return a->getDate() < b->getDate();
    });
    return result;
}

double InMemoryEntryDAO::getTotalCompradoCarteira(int idCarteira) {
    double total = 0.0;
    for (const auto& pair : movimentacoes) {
        if (pair.second->getIdCarteira() == idCarteira && pair.second->getType() == MovementType::Compra) {
            total += pair.second->getQuantidade();
        }
    }
    return total;
}

double InMemoryEntryDAO::getTotalVendidoCarteira(int idCarteira) {
    double total = 0.0;
    for (const auto& pair : movimentacoes) {
        if (pair.second->getIdCarteira() == idCarteira && pair.second->getType() == MovementType::Venda) {
            total += pair.second->getQuantidade();
        }
    }
    return total;
}

void InMemoryEntryDAO::addCotacaoOraculo(const Date& date, double cotacao) {
    cotacoesOraculo[date.getIsoFormat()] = cotacao;
    saveCotacoesToFile();
}

double InMemoryEntryDAO::getCotacaoOraculoByDate(const Date& date) {
    auto it = cotacoesOraculo.find(date.getIsoFormat());
    if (it != cotacoesOraculo.end()) {
        return it->second;
    }
    throw std::runtime_error("Cotacao para a data " + date.getIsoFormat() + " nao encontrada no oraculo em memoria.");
}