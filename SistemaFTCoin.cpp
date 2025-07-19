#include "SistemaFTCoin.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>

#include "Carteira.hpp"
#include "Entry.hpp"
#include "Date.hpp"
#include "MovementType.hpp"
#include "Utils.hpp"

SistemaFTCoin::SistemaFTCoin() {}

void SistemaFTCoin::setDBDAO(std::shared_ptr<AbstractEntryDAO> dao) {
    this->dbDAO = dao;
}

std::shared_ptr<AbstractEntryDAO> SistemaFTCoin::getDBDAO() const {
    return this->dbDAO;
}

//Métodos de Carteira
int SistemaFTCoin::adicionarCarteira(const std::string &titular, const std::string &corretora) {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    Carteira novaCarteira(0, titular, corretora);
    return dbDAO->addCarteira(novaCarteira);
}

Carteira SistemaFTCoin::getCarteira(int id) {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    std::unique_ptr<Carteira> carteira = dbDAO->getCarteiraById(id);
    if (!carteira) {
        throw std::runtime_error("Carteira nao encontrada com ID: " + std::to_string(id));
    }
    return *carteira;
}

void SistemaFTCoin::editarCarteira(int id, const std::string &novoTitular, const std::string &novaCorretora) {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    std::unique_ptr<Carteira> carteiraExistente = dbDAO->getCarteiraById(id);
    if (!carteiraExistente) {
        throw std::runtime_error("Carteira com ID " + std::to_string(id) + " nao encontrada para edicao.");
    }
    Carteira carteiraParaAtualizar(id, novoTitular, novaCorretora);
    dbDAO->updateCarteira(carteiraParaAtualizar);
    std::cout << "Carteira " << id << " editada. Novo titular: " << novoTitular << ", Nova corretora: " << novaCorretora << std::endl;
}

void SistemaFTCoin::excluirCarteira(int id) {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    dbDAO->deleteCarteira(id);
    std::cout << "Carteira " << id << " excluida." << std::endl;
}

void SistemaFTCoin::listarCarteirasOrdenadasPorId() const {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    std::vector<std::unique_ptr<Carteira>> lista = dbDAO->getAllCarteiras();
    Carteira::imprimirCabecalhoTabelaCarteira();
    for (const auto& c_ptr : lista) {
        double saldoAtual = calcularSaldo(c_ptr->getId());
        c_ptr->imprimir(saldoAtual);
    }
}

void SistemaFTCoin::listarCarteirasOrdenadasPorNome() const {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    std::vector<std::unique_ptr<Carteira>> lista = dbDAO->getAllCarteirasOrdenadasPorNome();
    Carteira::imprimirCabecalhoTabelaCarteira();
    for (const auto& c_ptr : lista) {
        double saldoAtual = calcularSaldo(c_ptr->getId());
        c_ptr->imprimir(saldoAtual);
    }
}

//Métodos de Movimentacao
void SistemaFTCoin::registrarMovimentacao(int idCarteira, MovementType tipo, double quantidade) {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
     
    Carteira carteira = getCarteira(idCarteira);
    
    if (tipo == MovementType::Compra) {
    } else if (tipo == MovementType::Venda) {
        double saldoAtualDeMoedas = calcularSaldo(idCarteira);
        if (saldoAtualDeMoedas < quantidade) {
            throw std::runtime_error("Quantidade invalida: saldo insuficiente para venda. Saldo atual: " + std::to_string(saldoAtualDeMoedas));
        }
    } else {
        throw std::runtime_error("Tipo de movimentacao invalido.");
    }

    Entry novaEntrada(idCarteira, Date(), tipo, quantidade);
    dbDAO->addMovimentacao(novaEntrada);
}

std::vector<std::unique_ptr<Entry>> SistemaFTCoin::getHistoricoMovimentacoes(int idCarteira) const {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    
    return dbDAO->getMovimentacoesByCarteiraId(idCarteira);
}

double SistemaFTCoin::calcularSaldo (int idCarteira) const{
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    std::vector<std::unique_ptr<Entry>> historico = dbDAO->getMovimentacoesByCarteiraId(idCarteira);
    double saldoCalculado = 0.0;
    for (const auto& mov_ptr : historico) {
        if (mov_ptr->getType() == MovementType::Compra) {
            saldoCalculado += mov_ptr->getQuantidade();
        } else {
            saldoCalculado -= mov_ptr->getQuantidade();
        }
    }
    return saldoCalculado;
}

//Métodos de Oraculo
double SistemaFTCoin::consultarPrecoOraculo() const {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");
    Date dataAtual;
    double cotacao;
    try {
        cotacao = dbDAO->getCotacaoOraculoByDate(dataAtual);
        std::cout << std::fixed << std::setprecision(2) << "[Oraculo] Cotacao DB para " << dataAtual.getIsoFormat() << ": " << cotacao << std::endl;
        return cotacao;
    } catch (const std::runtime_error& e) {
        std::cerr << "Cotacao para a data " << dataAtual.getIsoFormat() << " nao encontrada no DB. Gerando aleatoria..." << std::endl;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(1.0, 10.0);
        cotacao = dist(gen);
        
        dbDAO->addCotacaoOraculo(dataAtual, cotacao); 
        std::cout << std::fixed << std::setprecision(2) << "[Oraculo] Cotacao aleatoria gerada e adicionada para " << dataAtual.getIsoFormat() << ": " << cotacao << std::endl;
        return cotacao;
    }
}

//Relatórios
void SistemaFTCoin::exibirGanhoOuPerda() const {
    if (!dbDAO) throw std::runtime_error("DBDAO nao inicializado.");

    Utils::clearScreen();

    std::vector<std::unique_ptr<Carteira>> todasCarteiras = dbDAO->getAllCarteiras();

    double cotacaoAtual = consultarPrecoOraculo();

    std::cout << "=========================================\n";
    std::cout << "--- Ganho ou perda por carteira ---\n";
    std::cout << "=========================================\n";

    for (const auto& carteira_ptr : todasCarteiras) {
        const Carteira& carteira = *carteira_ptr;

        std::vector<std::unique_ptr<Entry>> historico = dbDAO->getMovimentacoesByCarteiraId(carteira.getId());
        
        double totalComprado = 0.0;
        double totalVendido = 0.0;
        double saldoMoedas = 0.0;

        for (const auto& mov_ptr : historico) {
            double cotacaoDaData = 0.0;
            try {
                cotacaoDaData = dbDAO->getCotacaoOraculoByDate(mov_ptr->getDate());
            } catch (const std::runtime_error& e) {
                cotacaoDaData = 0.0; 
            }

            double valorOperacaoEmBRL = mov_ptr->getQuantidade() * cotacaoDaData;

            if (mov_ptr->getType() == MovementType::Compra) {
                totalComprado += valorOperacaoEmBRL;
                saldoMoedas += mov_ptr->getQuantidade();
            } else {
                totalVendido += valorOperacaoEmBRL;
                saldoMoedas -= mov_ptr->getQuantidade();
            }
        }

        double valorEstimado = saldoMoedas * cotacaoAtual;
        double lucroPrejuizoRealizado = totalVendido - totalComprado;
        double ganhoPerdaTotalEstimado = lucroPrejuizoRealizado + valorEstimado;

        const char* verde = "\033[32m";
        const char* vermelho = "\033[31m";
        const char* reset = "\033[0m";

        std::cout << "-----------------------------------------\n";
        std::cout << "Carteira ID: " << carteira.getId() << "\n";
        std::cout << "Titular: " << carteira.getTitular() << "\n";
        std::cout << "Corretora: " << carteira.getCorretora() << "\n";
        std::cout << "Saldo atual de moedas: " << std::fixed << std::setprecision(4) << saldoMoedas << " FTC\n";
        std::cout << "Valor total comprado (realizado): " << std::fixed << std::setprecision(2) << totalComprado << " BRL\n";
        std::cout << "Valor total vendido (realizado): " << std::fixed << std::setprecision(2) << totalVendido << " BRL\n";
        std::cout << "Lucro/Prejuizo REALIZADO: " 
                  << (lucroPrejuizoRealizado >= 0 ? verde : vermelho)
                  << std::fixed << std::setprecision(2) << lucroPrejuizoRealizado 
                  << " BRL (" << (lucroPrejuizoRealizado >= 0 ? "Ganho" : "Perda") << ")" << reset << "\n";
        std::cout << "Valor estimado das moedas em carteira (cotacao atual): " << std::fixed << std::setprecision(2) << valorEstimado << " BRL\n";
        std::cout << "Ganho/Perda TOTAL ESTIMADO: " 
                  << (ganhoPerdaTotalEstimado >= 0 ? verde : vermelho)
                  << std::fixed << std::setprecision(2) << ganhoPerdaTotalEstimado 
                  << " BRL (" << (ganhoPerdaTotalEstimado >= 0 ? "Ganho" : "Perda") << ")" << reset << "\n";
    }

    std::cout << "=========================================\n";
    Utils::pause();
}