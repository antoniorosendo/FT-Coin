#include "Controller.hpp"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <limits> 
#include <string> 

#include "Carteira.hpp"
#include "SistemaFTCoin.hpp"
#include "MovementType.hpp"

#include "Menu.hpp"
#include "TextFromFile.hpp"
#include "Utils.hpp"
#include "SysInfo.hpp" 

using namespace std;


Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::start()
{
    vector<string> mainMenuItems = {
        "Carteira",
        "Movimentacao",
        "Relatorios",
        "Ajuda",
        "Sair do programa"
    };
    vector<void (Controller::*)()> mainFunctions = {
        &Controller::actionCarteira,
        &Controller::actionMovimentos,
        &Controller::actionRelatorios,
        &Controller::actionAjuda,
        nullptr 
    };

    launchActions("FTCoin System - Menu Principal", mainMenuItems, mainFunctions, true);
}

void Controller::launchActions(std::string title, std::vector<std::string> menuItens, std::vector<void (Controller::*)()> functions, bool isRootMenu)
{
    Menu menu(menuItens, title, "Sua opcao: "); 
    menu.setZeroForLastOpt(true);
    int choice;

    do {
        Utils::clearScreen();
        choice = menu.getChoice(); 

        if (static_cast<size_t>(choice) == menuItens.size() && functions.at(static_cast<size_t>(choice - 1)) == nullptr) {
            if (isRootMenu){
                std::cout << "\nSaindo do programa...\n";
            }
            else
                std::cout << "\nRetornando ao menu anterior...\n";
            break;
        }

        if (choice > 0 && static_cast<size_t>(choice) <= functions.size() && functions.at(static_cast<size_t>(choice - 1)) != nullptr)
        {
            try {
                (this->*functions.at(choice - 1))(); 
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro: " << e.what() << std::endl;
                Utils::pause(); 
            } catch (const std::exception& e) {
                std::cerr << "Ocorreu um erro inesperado: " << e.what() << std::endl;
                Utils::pause(); 
            }
        }
        
    } while (true);
}

void Controller::actionCarteira(void)
{
    Utils::clearScreen();

    vector<string> carteiraMenuItems = {
        "Criar nova carteira",
        "Consultar carteira por ID",
        "Editar carteira",
        "Excluir carteira",
        "Voltar ao menu principal"
    };
    vector<void (Controller::*)()> carteiraFunctions = {
        &Controller::newCarteira,
        &Controller::getCarteiraById,
        &Controller::editCarteira,
        &Controller::deleteCarteira,
        nullptr 
    };

    launchActions("Carteira - Menu", carteiraMenuItems, carteiraFunctions);
}

void Controller::newCarteira(void)
{
    Utils::clearScreen();

    string titular;
    string corretora;
    cout << "--- Criar Nova Carteira ---\n";
    cout << "Digite o nome do titular: ";
    getline(cin, titular);
    cout << "Digite o nome da corretora: ";
    getline(cin, corretora);

    try {
    int novoId = sistemaFTCoin.adicionarCarteira(titular, corretora);
    cout << "Carteira criada com sucesso! ID atribuido: " << novoId << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro ao criar carteira: " << e.what() << std::endl;
    }
    Utils::pause();
}

void Controller::getCarteiraById(void)
{
    Utils::clearScreen();

    int id;
    cout << "--- Consultar Carteira por ID ---\n";
    cout << "Digite o ID da carteira: ";
    while (!(cin >> id)) {
        Utils::clearScreen();
        cout << "--- Consultar Carteira por ID ---\n";
        cout << "Entrada invalida. Digite um numero para o ID: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    try {
        Carteira::imprimirCabecalhoTabelaCarteira();
        const Carteira &carteira = sistemaFTCoin.getCarteira(id);
        double saldo= sistemaFTCoin.calcularSaldo(id); 
        carteira.imprimir(saldo);
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro ao consultar carteira: " << e.what() << std::endl;
    }
    Utils::pause();
}

void Controller::editCarteira(void)
{
    Utils::clearScreen();

    int id;
    string novoTitular;
    string novaCorretora;
    cout << "--- Editar Carteira ---\n";
    cout << "Digite o ID da carteira a ser editada: ";
    while (!(cin >> id)) {
        cout << "Entrada invalida. Digite um numero para o ID: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();
    cout << "Digite o novo nome do titular: ";
    getline(cin, novoTitular);
    cout << "Digite o novo nome da corretora: ";
    getline(cin, novaCorretora);

    try {
        sistemaFTCoin.editarCarteira(id, novoTitular, novaCorretora);
        cout << "Carteira editada com sucesso!\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro ao editar carteira: " << e.what() << std::endl;
    }
    Utils::pause();
}

void Controller::deleteCarteira(void)
{
    Utils::clearScreen();

    int id;
    cout << "--- Excluir Carteira ---\n";
    cout << "Digite o ID da carteira a ser excluida: ";
    while (!(cin >> id)) {
        cout << "Entrada invalida. Digite um numero para o ID: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    try {
        sistemaFTCoin.excluirCarteira(id);
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro ao excluir carteira: " << e.what() << std::endl;
    }
    Utils::pause();
}

void Controller::actionMovimentos(void)
{
    Utils::clearScreen();

    vector<string> movimentosMenuItems = {
        "Registrar nova Compra",
        "Registrar nova Venda",
        "Voltar ao menu principal"
    };
    vector<void (Controller::*)()> movimentosFunctions = {
        &Controller::newCompra,
        &Controller::newVenda,
        nullptr 
    };
    launchActions("Movimentos - Menu", movimentosMenuItems, movimentosFunctions);
}

void Controller::newCompra(void)
{
    Utils::clearScreen();

    int idCarteira;
    double quantidade;

    cout << "--- Registrar Nova Compra ---\n";
    cout << "Digite o ID da carteira: ";
    while (!(cin >> idCarteira)) {
        cout << "Entrada invalida. Digite um numero para o ID: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    cout << "Digite a quantidade a ser comprada: ";
    while (!(cin >> quantidade) || quantidade <= 0) {
        cout << "Quantidade invalida. Digite um numero positivo: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    try {
        sistemaFTCoin.registrarMovimentacao(idCarteira, MovementType::Compra, quantidade);
        cout << "Compra registrada com sucesso!\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro ao registrar compra: " << e.what() << std::endl;
    }
    Utils::pause();
}

void Controller::newVenda(void)
{
    Utils::clearScreen();

    int idCarteira;
    double quantidade;

    cout << "--- Registrar Nova Venda ---\n";
    cout << "Digite o ID da carteira: ";
    while (!(cin >> idCarteira)) {
        cout << "Entrada invalida. Digite um numero para o ID: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    cout << "Digite a quantidade a ser vendida: ";
    while (!(cin >> quantidade) || quantidade <= 0) {
        cout << "Quantidade invalida. Digite um numero positivo: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    try {
        sistemaFTCoin.registrarMovimentacao(idCarteira, MovementType::Venda, quantidade);
        cout << "Venda registrada com sucesso!\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro ao registrar venda: " << e.what() << std::endl;
    }
    Utils::pause();
}

    

    void Controller::actionRelatorios(void)
{
    Utils::clearScreen();

    vector<string> relatoriosMenuItems = {
        "Listar carteiras ordenadas por ID",
        "Listar carteiras ordenadas por nome do titular",
        "Exibir saldo atual de uma carteira especifica",
        "Exibir historico de movimentacao de uma carteira especifica",
        "Apresentar ganho ou perda total de cada carteira",
        "Voltar ao menu principal"
    };

    vector<void (Controller::*)()> relatoriosFunctions = {
        &Controller::listarCarteirasPorId,
        &Controller::listarCarteirasPorNome,
        &Controller::exibirSaldoCarteira,
        &Controller::exibirHistoricoCarteira,
        &Controller::ganhoOuPerdaCarteiras,
        nullptr
    };

    launchActions("Relatorios - Menu", relatoriosMenuItems, relatoriosFunctions);
}

void Controller::listarCarteirasPorId(void)
{
    Utils::clearScreen();

    cout << "--- Listar carteiras ordenadas por ID ---\n";
    sistemaFTCoin.listarCarteirasOrdenadasPorId(); 
    Utils::pause();
}

void Controller::listarCarteirasPorNome(void)
{
    Utils::clearScreen();

    cout << "--- Listar carteiras ordenadas por nome ---\n";
    sistemaFTCoin.listarCarteirasOrdenadasPorNome(); 
    Utils::pause();
}

void Controller::exibirSaldoCarteira(void)
    {
    Utils::clearScreen();

    int id;
    cout << "--- Exibir saldo da carteira ---\n";
    cout << "Digite o ID da carteira: ";
    while (!(cin >> id)) {
        cout << "Entrada invalida. Digite um numero para o ID: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    try {
        double saldo = sistemaFTCoin.calcularSaldo(id);
        Carteira carteira = sistemaFTCoin.getCarteira(id);
        carteira.imprimirSaldo(saldo);
    } catch (const std::runtime_error& e) {
        cerr << "Erro: " << e.what() << "\n";
    }

    Utils::pause();
    }

void Controller::exibirHistoricoCarteira(void)
{
    Utils::clearScreen();

    int id;
    cout << "--- Exibir historico da carteira ---\n";
    cout << "Digite o ID da carteira: ";
    while (!(cin >> id)) {
        cout << "Entrada invalida. Digite um numero para o ID: ";
        Utils::clearInputBuffer();
    }
    Utils::clearInputBuffer();

    try {
        std::vector<std::unique_ptr<Entry>> historico = sistemaFTCoin.getHistoricoMovimentacoes(id);
        
        Entry::imprimirCabecalhoTabelaMov();
        for (const auto& m : historico) {
            m->imprimir();
        }
    } catch (const std::runtime_error& e) {
        cerr << "Erro: " << e.what() << "\n";
    }

    Utils::pause();
}

void Controller::ganhoOuPerdaCarteiras(void)
{
    Utils::clearScreen();

    cout << "--- Ganho ou perda por carteira ---\n";
    sistemaFTCoin.exibirGanhoOuPerda(); 
}

void Controller::actionAjuda(void)
{
    Utils::clearScreen();

    vector<string> ajudaMenuItems = {
        "Mostrar Ajuda",
        "Mostrar Creditos",
        "Voltar ao menu principal"
    };
    vector<void (Controller::*)()> ajudaFunctions = {
        &Controller::mostrarAjuda,
        &Controller::mostrarCreditos,
        nullptr 
    };
    launchActions("Ajuda - Menu", ajudaMenuItems, ajudaFunctions);
}

void Controller::mostrarAjuda(void)
{
    Utils::clearScreen();

    TextFromFile helpFile("txt/FTC_Help.txt");
    cout << "--- Ajuda ---\n";
    cout << helpFile.getFileContent() << endl;
    Utils::pause();
}

void Controller::mostrarCreditos(void)
{
    Utils::clearScreen();

    TextFromFile creditsFile("txt/Creditos.txt");
    cout << "--- Creditos ---\n";
    cout << creditsFile.getFileContent() << endl;
    Utils::pause();
}

SistemaFTCoin& Controller::getSistemaFTCoin() {
    return sistemaFTCoin;
}