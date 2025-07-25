# FT Coin - Gerenciador de Carteira Virtual em C++**
Este projeto, FT Coin, é uma aplicação de linha de comando desenvolvida em C++11 para o gerenciamento de carteiras de uma moeda virtual hipotética (FTC). Ele oferece funcionalidades robustas para que os usuários possam acompanhar suas movimentações financeiras, consultar saldos e analisar o desempenho de suas carteiras.

# Visão Geral
O FT Coin foi projetado com um sistema de persistência flexível, priorizando a conexão com um banco de dados MariaDB para armazenamento de dados completo. Em caso de falha na conexão com o MariaDB, o sistema realiza um fallback automático para um modo de persistência em memória, que salva os dados em arquivos CSV/TXT locais, garantindo a continuidade da operação.

# Funcionalidades Principais
**Gerenciamento de Carteiras:**
- Crie novas carteiras com titular e corretora.
- Consulte detalhes de carteiras por ID.
- Edite informações de carteiras existentes.
- Exclua carteiras.
- Liste todas as carteiras, ordenadas por ID ou nome do titular, com o saldo atual.

**Registro de Movimentações:**
- Registre operações de compra e venda da moeda FTC para uma carteira específica.
- Validação de saldo para operações de venda.

**Oráculo de Cotações:**
- Simula a cotação da moeda FTC em BRL.
- Busca cotações por data (se persistidas) ou gera valores aleatórios para datas sem cotação, persistindo-os para uso futuro.

**Relatórios Financeiros:**
- Exiba o saldo atual de moedas em uma carteira específica.
- Consulte o histórico detalhado de movimentações de uma carteira.
- Visualize um relatório completo de ganho ou perda por carteira, incluindo lucro/prejuízo realizado e o valor estimado das moedas em carteira com base na cotação atual.

# Persistência de Dados:
- MariaDB: Armazenamento robusto e completo de carteiras, movimentações e cotações.
- Fallback em Memória (CSV/TXT): Em caso de falha na conexão com o MariaDB, os dados são automaticamente salvos e carregados de arquivos carteiras.csv, movimentacoes.csv e cotacoes.csv, permitindo que o aplicativo continue funcionando.

# Tecnologias Utilizadas
Linguagem: C++11
Compilador: g++
Gerenciamento de Dependências/Compilação: Makefile
Banco de Dados: MariaDB (via MariaDB Connector/C++)
Persistência Local: Arquivos .csv / .txt

# Como Compilar e Executar
Pré-requisitos:
Um compilador C++ (g++ recomendado) com suporte a C++11.
MariaDB Connector/C++ instalado e suas bibliotecas e cabeçalhos acessíveis (geralmente em /usr/include e /usr/lib/x86_64-linux-gnu).

# Clonar o Repositório:
git clone https://github.com/antoniorosendo/FT-Coin
cd ftcoin

# Compilar:
Utilize o Makefile fornecido para compilar o projeto. Os arquivos objeto (.o) serão gerados na subpasta build/.

# Executar:
Após a compilação bem-sucedida, o executável ftcoin será criado no diretório raiz do projeto.

./ftcoin

O programa tentará se conectar ao MariaDB. Se a conexão falhar, ele informará e usará a persistência em memória (criando ou lendo os arquivos CSV no diretório de execução, ou em data/ se configurado).

# Estrutura do Projeto
- src/: Contém os arquivos .cpp e .hpp do projeto.
- build/: Diretório onde os arquivos objeto (.o) são gerados durante a compilação.
- Makefile: Script para automatizar o processo de compilação.
- carteiras.csv, movimentacoes.csv, cotacoes.csv: Arquivos de dados para a persistência em memória (podem ser criados automaticamente ou em uma pasta data/ dependendo da configuração).

# Contribuições
Sinta-se à vontade para explorar o código, relatar problemas ou sugerir melhorias.
