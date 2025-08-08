# trabalho3-nilton
Trabalho de faculdade.
=====================================================================================================================================

Sistema de Gerenciamento de Clientes e Caixa Eletrônico
Um sistema em C para simular o gerenciamento de clientes de um banco e operações básicas de caixa eletrônico, incluindo cadastro de clientes, controle de saldos, registro de saques e relatórios.

=====================================================================================================================================

🚀 Funcionalidades
Este sistema oferece as seguintes funcionalidades:

👤 Clientes
Incluir Cliente: Cadastra novos clientes no sistema.

Mostrar Clientes: Lista todos os clientes ativos com seus dados (CPF, Conta Corrente, Saldo e Saques Realizados).

Alterar Cliente: (A ser implementado) Permite modificar dados de clientes existentes.

Excluir Cliente: Realiza a exclusão lógica de um cliente, caso ele não tenha saques registrados.

=====================================================================================================================================

💸 Saque
Realizar Saque: (Em desenvolvimento) Funcionalidade para que os clientes possam sacar dinheiro, com controle de cédulas.

=====================================================================================================================================

📊 Relatórios
Valores Sacados: Exibe o histórico de valores sacados por cada cliente.

Valor do Saldo Existente: Mostra o saldo atual de todos os clientes ativos.

Quantidade de Cédulas Existentes: Apresenta o estoque atual de cédulas disponíveis no caixa.

=====================================================================================================================================

🔒 Validação e Geração de CPF
Inclui funções para validar CPFs (verificando formato, dígitos e casos especiais de CPFs com todos os dígitos iguais).

Capacidade de gerar CPFs válidos aleatoriamente, com formatação e cálculo dos dígitos verificadores.

=====================================================================================================================================

📋 Variáveis, Vetores e Matrizes Globais
O sistema utiliza diversas variáveis, vetores e matrizes globais para armazenar e gerenciar os dados.

⚙️ Constantes Globais
MAX_CLIENTES: Define o número máximo de clientes que o sistema pode gerenciar (atualmente 50).

TAM_CPF: Tamanho máximo da string para armazenar um CPF formatado ("999.999.999-99\0"), incluindo o terminador nulo (\0), totalizando 15 caracteres.

TAM_CONTA: Tamanho máximo da string para armazenar um número de conta corrente ("999.999-X\0"), incluindo o terminador nulo, totalizando 11 caracteres.

TAM_RESPOSTA: Tamanho máximo para strings de resposta do usuário ("SIM\0" ou "NAO\0"), 5 caracteres.

MAX_SAQUES_POR_CLIENTE: Define o número máximo de saques que podem ser registrados por cada cliente (atualmente 10).

MAX_TRANSACAO_INFO: (Não utilizado diretamente no código fornecido para este propósito, mas declarado) Destinado ao tamanho da string para descrever transações, 50 caracteres.

NUM_CEDULAS: Número de diferentes tipos de cédulas disponíveis no caixa (atualmente 8).

=====================================================================================================================================

💰 Estoque de Cédulas do Caixa
valor_cedulas[NUM_CEDULAS]: Um vetor que armazena os valores nominais de cada cédula disponível no caixa.

Exemplo: {1, 2, 5, 10, 20, 50, 250, 450}.

estoque_cedulas_atuais[NUM_CEDULAS]: Um vetor que armazena a quantidade atual de cada tipo de cédula no caixa. O índice corresponde ao valor_cedulas.

Exemplo: {12800, 6400, 3200, 1600, 800, 400, 200, 100} para 1 real, 2 reais, etc.

=====================================================================================================================================

👤 Dados dos Clientes
As seguintes matrizes armazenam as informações dos clientes. Cada índice i em todas essas matrizes corresponde aos dados de um único cliente.

clientes_cpf[MAX_CLIENTES][TAM_CPF]: Uma matriz de caracteres para armazenar o CPF de cada cliente, já formatado.

clientes_conta_corrente[MAX_CLIENTES][TAM_CONTA]: Uma matriz de caracteres para armazenar o número da conta corrente de cada cliente.

clientes_saldo[MAX_CLIENTES]: Um vetor de tipo double para armazenar o saldo atual de cada cliente.

clientes_numero_saques_realizados[MAX_CLIENTES]: Um vetor de tipo int que conta o número total de saques que cada cliente já realizou.

clientes_ativo[MAX_CLIENTES]: Um vetor de tipo int que indica o status do cliente.

1: Cliente ativo.

0: Cliente inativo (utilizado para exclusão lógica).

quantidade_clientes: Uma variável inteira que rastreia o número total de clientes ativos cadastrados no sistema.

=====================================================================================================================================

📚 Histórico de Saques por Cliente
clientes_historico_saques_valores[MAX_CLIENTES][MAX_SAQUES_POR_CLIENTE]: Uma matriz de tipo double que armazena os valores de cada saque individual realizado por cada cliente.

clientes_historico_saques_valores[i][j] representa o valor do j-ésimo saque do cliente i.

clientes_saques_contador[MAX_CLIENTES]: Um vetor de tipo int que funciona como um contador para o número de saques já registrados para cada cliente específico. Ele também atua como o próximo índice disponível para registrar um novo saque para aquele cliente.

=====================================================================================================================================
🗺️ Estrutura do Código
O código está organizado em funções modularizadas para facilitar a manutenção e leitura:

main(): Ponto de entrada do programa, inicializa o gerador de números aleatórios e exibe o menu principal.

exibirMenu() / MENU_PRINCIPAL(): Gerenciam o fluxo do menu principal do sistema.

exibirMenuCliente() / menuCliente(): Funções para o submenu de gerenciamento de clientes.

incluirClientes() / mostrarClientes() / alterarClientes() / excluirClientes(): Implementam as operações CRUD para clientes.

encontrarClientePorConta() / cliente_duplicado(): Funções auxiliares para buscar e verificar a unicidade de clientes.

geraAlfabeto() / geraNumero() / geraContaCorrente(): Funções para geração de dados (ainda em desenvolvimento para conta corrente).

insere_pontuacao_cpf(): Formata um CPF numérico com pontos e hífen.

obtem_primeiro_digito_verificador() / obtem_segundo_digito_verificador(): Calculam os dígitos verificadores de um CPF.

gera_cpf_valido(): Gera um CPF completo e válido.

verifica_cpf_valido(): Valida um CPF dado, incluindo formato e dígitos verificadores.

ExibirMenuSaque(): (Em desenvolvimento) Função para o menu de saques.

ExibirMenuRelatorios() / MenuRelatorios(): Funções para o submenu de relatórios.

valorSacado() / valorSaldoExistente() / QtdCedulasExistentes(): Implementam os relatórios específicos.

💡 Próximos Passos (Desenvolvimento Futuro)
Completar a lógica da função realizarCadastroClienteInterno() para permitir a inclusão real de clientes com CPF e conta válidos.

Implementar a funcionalidade de alterarClientes().

Desenvolver a lógica completa da função ExibirMenuSaque() para processar saques, verificar saldo e estoque de cédulas.

Melhorar a apresentação dos relatórios, talvez com mais detalhes ou filtros.

Tratamento de erros mais robusto para entradas do usuário.
