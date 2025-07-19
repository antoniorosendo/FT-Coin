#define CONTROLLER_H

#include <string>
#include <vector>
#include <memory>

#include "SistemaFTCoin.hpp"
#include "Menu.hpp"           
#include "TextFromFile.hpp"   
#include "Utils.hpp"          
#include "SysInfo.hpp"     

class Controller
{
private:
    SistemaFTCoin sistemaFTCoin;
    void launchActions(std::string title, std::vector<std::string> menuItens, std::vector<void (Controller::*)()> functions, bool isRootMenu = false);

    void actionCarteira(void);
    void actionMovimentos(void);
    void actionRelatorios(void);
    void actionAjuda(void);


    void newCarteira(void);
    void getCarteiraById(void);
    void editCarteira(void);
    void deleteCarteira(void);

    void newCompra(void);
    void newVenda(void);

    void listarCarteirasPorId(void);
    void listarCarteirasPorNome(void);
    void exibirSaldoCarteira(void);
    void exibirHistoricoCarteira(void);
    void ganhoOuPerdaCarteiras(void);

    void mostrarAjuda(void);
    void mostrarCreditos(void);

public:
    SistemaFTCoin& getSistemaFTCoin();
    Controller();
    ~Controller();

    void start();
};