#include "Controller.hpp"
#include "InMemoryEntryDAO.hpp"
#include "EntryDBDAO.hpp"
#include "DataBaseSelector.hpp"
#include "Menu.hpp"
#include "Utils.hpp"
#include "SysInfo.hpp"
#include "ServerDBConnection.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

void welcome() {
    Utils::printMessage(SysInfo::getFullVersion());
    Utils::printMessage("Running now...");
}

void bye() {
    Utils::printMessage("Finished");
}

int main() {
    Utils::clearScreen();
    welcome();

    std::shared_ptr<ServerDBConnection> dbConnection = nullptr;
    std::shared_ptr<AbstractEntryDAO> mainDAO;

    dbConnection = std::make_shared<ServerDBConnection>();
    
    if (dbConnection->getConnection() != nullptr && dbConnection->getConnection()->isValid()) {
        mainDAO = std::make_shared<EntryDBDAO>(dbConnection.get());
        Utils::printMessage("\033[32mPersistencia MariaDB selecionada e conectada.\033[0m");
    }
    else {
        Utils::printMessage("\033[33mPersistencia em Memória selecionada.\033[0m");
        mainDAO = std::make_shared<InMemoryEntryDAO>();
    }

    if (!mainDAO) {
        std::cerr << "Erro fatal: DAO principal não foi inicializado." << std::endl;
        return 1;
    }

    Utils::pause();

    Controller appController;
    appController.getSistemaFTCoin().setDBDAO(mainDAO);

    try {
        appController.start();
    } catch (const std::runtime_error &e) {
        std::cerr << "Erro de execucao: " << e.what() << std::endl;
        Utils::printMessage("Problema durante a execucao. Programa abortado.");
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Ocorreu um erro inesperado: " << e.what() << std::endl;
        Utils::printMessage("Um erro inesperado ocorreu. Programa abortado.");
        return 1;
    }

    bye();
    return 0;
}