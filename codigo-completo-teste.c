#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Adicionado para tolower, para garantir compilação se ainda não estiver nos seus includes

// ALTERAÇÕES FEITAS 31-07: modularizei a maior parte do codigo, e tambem iniciei a geracao de CPF, falta apenas fazer a parte de validacao e tals.
//  ALTERAÇÕES FEITAS 7:08: conta corrente feita. Arrumei tambem os codigos de clientes, pois estava faltando umas coisas. amem
/*
12.800 de 1 real
6.400 de 2 reais
3.200 de 5 reais
1.600 de 10 reais
800 de 20 reais
400 de 50 reais
200 de 250 reais
100 de 450 reais
*/
// --- Constantes Globais Clientes ---
#define MAX_CLIENTES 50
#define TAM_CPF 15     // "999.999.999-99\0" (14 chars + 1 \0)
#define TAM_CONTA 11   // "999.999-X\0" (9 chars + 1 \0)
#define TAM_RESPOSTA 5 // Para "SIM\0" ou "NAO\0"

// --- Para o Historico de Saque (se for por cliente) ---
#define MAX_SAQUES_POR_CLIENTE 10 // Maximo de saques que um cliente pode ter registrado
#define MAX_TRANSACAO_INFO 50     // Tamanho da string para descrever cada transacao.

// --- Para o Estoque de Cedulas do Caixa ---
#define NUM_CEDULAS 8
#define limites_saques_por_clientes 100
int valor_cedulas[NUM_CEDULAS] = {1, 2, 5, 10, 20, 50, 250, 450};                        // Valores das cedulas
int estoque_cedulas_atuais[NUM_CEDULAS] = {12800, 6400, 3200, 1600, 800, 400, 200, 100}; // Quantidade inicial

// --- Matrizes para os Dados dos Clientes (Globais) ---
// Cada linha 'i' de cada matriz corresponde ao cliente 'i'
char clientes_cpf[MAX_CLIENTES][TAM_CPF];              // clientes_cpf[0][057.957321-41]
char clientes_conta_corrente[MAX_CLIENTES][TAM_CONTA]; // clientes_conta[0][XXX.XXXX-J]
double total_money_retirado_por_cliente[MAX_CLIENTES]; // clientes_saldo[0] = 200 reais
int clientes_numero_saques_realizados[MAX_CLIENTES];   // Contador de saques para cada cliente || //clientes_numeros_saques_realizados[0] = nº saques
int clientes_ativo[MAX_CLIENTES];                      // 1 para ativo, 0 para inativo (exclusao logica) || clientes_ativo[0] = 0
// se (clientes_ativo[i] = 0) entao
// clientes_ativo[i] = vai receber novo cliente
// Variável para manter o controle do número atual de clientes ativos
int quantidade_clientes = 0;

// --- Matrizes para o Historico de Saques POR CLIENTE ---
// Historico detalhado por cliente
// 1. Conta corrente        cpf         valor
// Cada cliente_historico_saque[i][j] possui o valor do j saque do cliente i
// E clientes_saques_count[i] possui quantos saques o cliente i já realizou
double clientes_historico_saques_valores[MAX_CLIENTES][MAX_SAQUES_POR_CLIENTE]; // historico dos valores dos saques por clientes
int clientes_saques_contador[MAX_CLIENTES];                                     // Quantos saques cada cliente realizou (usado como contador de indices)

const char *unidades[] = {"", "Um", "Dois", "Tres", "Quatro", "Cinco", "Seis", "Sete", "Oito", "Nove"};
const char *dezenas[] = {"", "", "Vinte", "Trinta", "Quarenta", "Cinquenta", "Sessenta", "Setenta", "Oitenta", "Noventa"};
const char *especiais[] = {"Dez", "Onze", "Doze", "Treze", "Quatorze", "Quinze", "Dezesseis", "Dezessete", "Dezoito", "Dezenove"};
const char *centenas[] = {"", "Cento", "Duzentos", "Trezentos", "Quatrocentos", "Quinhentos", "Seiscentos", "Setecentos", "Oitocentos", "Novecentos"};

void inicializar_historico_saque();
char geraAlfabeto();
char geraNumero();
void geraContaCorrente(char c[]);
void insere_pontuacao_cpf(char cpf_origem[], char cpf_destino[]);
int obtem_primeiro_digito_verificador(char cpf[]);
int obtem_segundo_digito_verificador(char cpf[]);
void gera_cpf_valido(char cpf[]);
int verifica_cpf_valido(char cpf[]);
void valorSacado();
void valorSaldoExistente();
void QtdCedulasExistentes();
void ExibirMenuSaque();
void MenuRelatorios(int opcaoRelatorio);
void ExibirMenuRelatorios();
int encontrarClientePorConta(const char *conta);
int cliente_duplicado(const char *cpf, const char *conta);
void realizarCadastroClienteInterno();
void incluirClientes();
void mostrarClientes();
void alterarClientes();
void excluirClientes();
void menuCliente(int opcao_cliente);
void exibirMenuCliente();
void MENU_PRINCIPAL(int opcao_principal);
void exibirMenu();
void numeroPorExtenso(int numero);
int fazerSaque(int saqueCliente, int *estoque_cedulas, int indice_cliente);

// objetivo:--
// parâmetros: nenhum
// retorno:nenhum
void inicializar_historico_saque() {
    for (int i = 0; i < MAX_CLIENTES; i++) {
        clientes_saques_contador[i] = 0;
        for (int j = 0; j < MAX_SAQUES_POR_CLIENTE; j++) {
            clientes_historico_saques_valores[i][j] = 0.0;
        }
    }
}

// objetivo:escolhe dentre as letras do alfabeto ('a'..'z') uma letra aleatoriamente
// parametros: nenhum
// retorno:a letra do alfabeto
char geraAlfabeto() {
    int i;
    char letras[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    i = rand() % 26;
    return (letras[i] - 32);
}

// objetivo:escolhe dentre os numeros ('0'..'9') uma numero aleatoriamente
// parametros: nenhum
// retorno:o numero
char geraNumero() {
    int i;
    char numeros[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    i = rand() % 10;
    return (numeros[i]);
}

// objetivo:gera aleatoriamente um numero de conta corrente no formato 999.999-X
// parametros: c onde armazera a conta gerada
// retorno:nenhum
void geraContaCorrente(char c[]) {
    for (int i = 0; i < 3; i++) c[i] = geraNumero();
    c[3] = '.';
    for (int i = 4; i < 7; i++) c[i] = geraNumero();
    c[7] = '-';
    c[8] = geraAlfabeto();
    c[9] = '\0';
}

// objetivo:insere pontuacoes '.' e '- ' em um cpf
// parametros: cpf_origem o cpf recebido no format 99999999999
//             cpf_destino o cpf com as pontuacoes inseridas no formato 999.999.999-99
void insere_pontuacao_cpf(char cpf_origem[], char cpf_destino[]) {
    int iOrigem = 0, jDestino = 0;
    while (iOrigem < 11) {
        cpf_destino[jDestino] = cpf_origem[iOrigem];
        iOrigem++;
        jDestino++;
        if (iOrigem == 3 || iOrigem == 6) {
            cpf_destino[jDestino] = '.';
            jDestino++;
        } else if (iOrigem == 9) {
            cpf_destino[jDestino] = '-';
            jDestino++;
        }
    }
    cpf_destino[jDestino] = '\0';
}

// objetivo:calcula o primeiro digito verificador de um cpf no formato 999999999
// parametros: cpf o cpf sem os digitos verificadores
// retorno: o calculo do primeiro digito verificador
int obtem_primeiro_digito_verificador(char cpf[]) {
    int digito;
    int soma = 0;
    int peso = 10;
    for (int i = 0; i < 9; i++) {
        int numero_cpf_separado = cpf[i] - '0';
        soma += numero_cpf_separado * peso;
        peso--;
    }
    int resto = soma % 11;
    if (resto < 2) {
        digito = 0;
    } else {
        digito = 11 - resto;
    }
    return digito;
}

// objetivo:calcula o segundo digito verificador de um cpf no formato 999999999
// parametros: cpf sem os digitos verificadores
// retorno: o calculo do segundo digito verificador
int obtem_segundo_digito_verificador(char cpf[]) {
    int digito;
    int soma = 0;
    int peso = 11;
    for (int i = 0; i < 10; i++) {
        int numero_cpf_separado = cpf[i] - '0';
        soma += numero_cpf_separado * peso;
        peso--;
    }
    int resto = soma % 11;
    if (resto < 2) {
        digito = 0;
    } else {
        digito = 11 - resto;
    }
    return digito;
}

// objetivo:gera aleatoriamente um cpf valido no formato 999.999.999-99
// parametros: cpf onde sera armazenado o cpf valido
// retorno: nenhum
void gera_cpf_valido(char cpf[]) {
    char cpf_temp_9_digitos[10];
    char cpf_temp_10_digitos[11];
    char cpf_temp_11_digitos[12];
    int primeiro_verificador;
    int segundo_verificador;
    for (int i = 0; i < 9; i++) {
        cpf_temp_9_digitos[i] = (rand() % 10) + '0';
    }
    cpf_temp_9_digitos[9] = '\0';
    primeiro_verificador = obtem_primeiro_digito_verificador(cpf_temp_9_digitos);
    strncpy(cpf_temp_10_digitos, cpf_temp_9_digitos, 9);
    cpf_temp_10_digitos[9] = (char)(primeiro_verificador + '0');
    cpf_temp_10_digitos[10] = '\0';
    segundo_verificador = obtem_segundo_digito_verificador(cpf_temp_10_digitos);
    strncpy(cpf_temp_11_digitos, cpf_temp_10_digitos, 10);
    cpf_temp_11_digitos[10] = (char)(segundo_verificador + '0');
    cpf_temp_11_digitos[11] = '\0';
    insere_pontuacao_cpf(cpf_temp_11_digitos, cpf);
}

// objetivo:verifica se um cpf no formato 999.999.999-99 e valido
// parametros: cpf a ser verificado
// retorno: 1 se cpf e valido ou 0 se cpf nao e valido
int verifica_cpf_valido(char cpf[]) {
    int valido = 1;
    char cpf_apenas_digito[12];
    int i = 0;
    int j = 0;
    while (cpf[i] != '\0') {
        if (cpf[i] >= '0' && cpf[i] <= '9') {
            cpf_apenas_digito[j] = cpf[i];
            j++;
        }
        i++;
    }
    cpf_apenas_digito[j] = '\0';
    if (strlen(cpf_apenas_digito) != 11) {
        valido = 0;
    }
    if (valido == 1) {
        int todos_digitos_iguais = 1;
        for (int k = 1; k < 11; k++) {
            if (cpf_apenas_digito[k] != cpf_apenas_digito[0]) {
                todos_digitos_iguais = 0;
                break;
            }
        }
        if (todos_digitos_iguais) {
            valido = 0;
        }
    }
    if (valido == 1) {
        char cpf9_digitos_temp[10];
        strncpy(cpf9_digitos_temp, cpf_apenas_digito, 9);
        cpf9_digitos_temp[9] = '\0';
        int primeiro_dv_calculado = obtem_primeiro_digito_verificador(cpf9_digitos_temp);
        int primeiro_dv_fornecido = cpf_apenas_digito[9] - '0';
        if (primeiro_dv_fornecido != primeiro_dv_calculado) {
            valido = 0;
        }
    }
    if (valido == 1) {
        char cpf10_digitos_temp[11];
        strncpy(cpf10_digitos_temp, cpf_apenas_digito, 10);
        cpf10_digitos_temp[10] = '\0';
        int segundo_dv_calculado = obtem_segundo_digito_verificador(cpf10_digitos_temp);
        int segundo_dv_fornecido = cpf_apenas_digito[10] - '0';
        if (segundo_dv_fornecido != segundo_dv_calculado) {
            valido = 0;
        }
    }
    return valido;
}

// objetivo:converte um número inteiro para sua representação por extenso em português
// parâmetros: numero o número a ser convertido
// retorno: nenhum
void numeroPorExtenso(int numero) {
    if (numero == 0) return;
    int digitos[6] = {0}, contador = 0, temp = numero;
    while (temp > 0) {
        digitos[contador++] = temp % 10;
        temp /= 10;
    }
    int precisa_e = 0;
    if (contador >= 6 && digitos[5]) {
        printf("%s", digitos[5] == 1 && !digitos[4] && !digitos[3] && !digitos[2] && !digitos[1] && !digitos[0] ? "Cem Mil" : centenas[digitos[5]]);
        precisa_e = digitos[4] || digitos[3] || digitos[2] || digitos[1] || digitos[0];
    }
    if (precisa_e) printf(" e ");
    if (contador >= 5 && digitos[4]) {
        if (digitos[4] == 1) {
            printf("%s Mil", especiais[digitos[3]]);
        } else {
            printf("%s", dezenas[digitos[4]]);
            if (digitos[3]) printf(" e %s Mil", unidades[digitos[3]]);
            else if (digitos[4] > 1) printf(" Mil");
        }
        precisa_e = digitos[2] || digitos[1] || digitos[0];
    } else if (contador >= 4 && digitos[3]) {
        printf("%s Mil", digitos[3] == 1 && !digitos[5] && !digitos[4] ? "Um" : unidades[digitos[3]]);
        precisa_e = digitos[2] || digitos[1] || digitos[0];
    }
    if (precisa_e && (digitos[2] || digitos[1] || digitos[0])) printf(" e ");
    if (contador >= 3 && digitos[2]) {
        printf("%s", digitos[2] == 1 && !digitos[1] && !digitos[0] ? "Cem" : centenas[digitos[2]]);
        precisa_e = digitos[1] || digitos[0];
    }
    if (precisa_e && (digitos[1] || digitos[0])) printf(" e ");
    if (contador >= 2 && digitos[1]) {
        if (digitos[1] == 1) {
            printf("%s", especiais[digitos[0]]);
        } else {
            printf("%s", dezenas[digitos[1]]);
            if (digitos[0]) printf(" e %s", unidades[digitos[0]]);
        }
    } else if (contador >= 1 && digitos[0]) {
        printf("%s", unidades[digitos[0]]);
    }
    printf("\n");
}

// objetivo:realiza o saque, calcula as cédulas necessárias, atualiza o estoque e os dados do cliente
// parâmetros: saqueCliente valor do saque, estoque_cedulas array de estoque de cédulas, indice_cliente índice do cliente
// retorno: 1 se o saque for bem-sucedido, 0 caso contrário
int fazerSaque(int saqueCliente, int *estoque_cedulas, int indice_cliente) {
    if (saqueCliente <= 0) {
        printf("Valor de saque inválido!\n");
        return 0;
    }
    int cedulas_dadas[NUM_CEDULAS] = {0}, temp_saque = saqueCliente;
    for (int i = NUM_CEDULAS - 1; i >= 0; i--) {
        cedulas_dadas[i] = temp_saque / valor_cedulas[i];
        if (cedulas_dadas[i] > estoque_cedulas[i]) {
            cedulas_dadas[i] = estoque_cedulas[i];
        }
        temp_saque -= cedulas_dadas[i] * valor_cedulas[i];
    }
    if (temp_saque != 0) {
        printf("Saque impossível com as cédulas disponíveis!\n");
        return 0;
    }
    printf(" Valor solicitado: R$ %d,00\n", saqueCliente);
    numeroPorExtenso(saqueCliente);
    for (int i = NUM_CEDULAS - 1; i >= 0; i--) {
        if (cedulas_dadas[i] > 0) {
            printf(" %d cédula(s) de %d\n", cedulas_dadas[i], valor_cedulas[i]);
        }
    }
    char confirmacao[TAM_RESPOSTA];
    printf("Confirmar saque? (SIM / NAO)\n");
    while (1) {
        scanf(" %s", confirmacao);
        for (int i = 0; confirmacao[i]; i++) confirmacao[i] = tolower(confirmacao[i]);
        if (strcmp(confirmacao, "sim") == 0) {
            for (int i = 0; i < NUM_CEDULAS; i++) {
                estoque_cedulas[i] -= cedulas_dadas[i];
            }
            clientes_numero_saques_realizados[indice_cliente]++;
            total_money_retirado_por_cliente[indice_cliente] += saqueCliente;
            if (clientes_saques_contador[indice_cliente] < MAX_SAQUES_POR_CLIENTE) {
                clientes_historico_saques_valores[indice_cliente][clientes_saques_contador[indice_cliente]] = saqueCliente;
                clientes_saques_contador[indice_cliente]++;
            } else {
                printf("Aviso: Limite de registros de saques individuais para este cliente atingido.\n");
            }
            printf("\nSaque de R$ %d,00 realizado com sucesso!\n", saqueCliente);
            printf("Você realizou %d de %d saques disponíveis.\n", clientes_numero_saques_realizados[indice_cliente], limites_saques_por_clientes);
            return 1;
        } else if (strcmp(confirmacao, "nao") == 0) {
            printf("Saque cancelado pelo usuário\n");
            return 0;
        } else {
            printf("Entrada inválida! Por favor, digite 'SIM' ou 'NAO'.\n");
        }
    }
}

// objetivo:--
// parâmetros: nenhum
// retorno:nenhum
void ExibirMenuSaque() {
    if (quantidade_clientes == 0) {
        printf("\n---Nenhum cliente registrado para poder efetuar o saque.---\n");
        return;
    }
    char conta_digitada[TAM_CONTA];
    int indice_do_cliente;
    double valor_saque;
    do {
        printf("\n--- Realizar Saque ---\n");
        printf("Digite o número da conta corrente (formato 999.999-x): ");
        if (scanf(" %10s", conta_digitada) != 1) {
            printf("Entrada inválida! Por favor, digite a conta no formato pedido.\n");
            while (getchar() != '\n');
            continue;
        }
        indice_do_cliente = encontrarClientePorConta(conta_digitada);
        if (indice_do_cliente == -1) {
            printf("ERRO: Conta corrente não encontrada ou inativa. Tente novamente\n");
        } else if (clientes_numero_saques_realizados[indice_do_cliente] >= limites_saques_por_clientes) {
            printf("Erro: O cliente já atingiu o limite de %d saques.\n", limites_saques_por_clientes);
            return;
        } else {
            break;
        }
    } while (1);
    do {
        printf("Digite o valor do saque: R$ ");
        if (scanf("%lf", &valor_saque) != 1 || valor_saque != (int)valor_saque) {
            printf("Valor inválido! Por favor, digite um valor numérico inteiro.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (valor_saque <= 0) {
            printf("Erro: O valor de saque precisa ser maior que ZERO. Tente novamente!\n");
            continue;
        }
        printf("\n---Confirmação do saque---\n");
        printf(" Cliente: %s (CPF: %s)\n", clientes_conta_corrente[indice_do_cliente], clientes_cpf[indice_do_cliente]);
        if (fazerSaque((int)valor_saque, estoque_cedulas_atuais, indice_do_cliente)) {
            break;
        }
    } while (1);
}

// INICIO FUNCOES RELATORIOS
// objetivo: Exibe o relatório de valores sacados por todos os clientes
// parâmetros: nenhum
// retorno: nenhum
void valorSacado() {
    double total_geral = 0.0;
    int clientes_com_saques = 0;
    printf("\n--- Relatório Valores Sacados ---\n");
    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (clientes_ativo[i] == 1 && clientes_saques_contador[i] > 0) {
            clientes_com_saques++;
            double total_cliente = 0.0;
            printf("conta-corrente %s cpf %s\n", clientes_conta_corrente[i], clientes_cpf[i]);
            for (int j = 0; j < clientes_saques_contador[i]; j++) {
                total_cliente += clientes_historico_saques_valores[i][j];
                printf("     R$ %.2f\n", clientes_historico_saques_valores[i][j]);
            }
            printf("     R$ total_1 %.2f\n", total_cliente);
            total_geral += total_cliente;
        }
    }
    if (clientes_com_saques > 0) {
        printf("R$ valor_total_geral ");
        numeroPorExtenso((int)total_geral);
        printf("     R$ %.2f\n", total_geral);
    } else {
        printf("Nenhum saque registrado.\n");
    }
}

// objetivo: Exibe o relatório do valor do saldo existente de todos os clientes ativos
// parâmetros: nenhum
// retorno: nenhum
void valorSaldoExistente() {
    printf("\n--- Relatório Valor do Saldo Existente ---\n");
    int clientes_com_saldo = 0;
    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (clientes_ativo[i] == 1 && total_money_retirado_por_cliente[i] > 0) {
            clientes_com_saldo++;
            printf("R$ ");
            numeroPorExtenso((int)total_money_retirado_por_cliente[i]);
            printf("     R$ %.2f\n", total_money_retirado_por_cliente[i]);
        }
    }
    if (clientes_com_saldo == 0) {
        printf("Nenhum saldo existente.\n");
    }
}

// objetivo: Exibe o relatório da quantidade de cédulas existentes no caixa
// parâmetros: nenhum
// retorno: nenhum
void QtdCedulasExistentes() {
    printf("\n--- Relatório Quantidade de Cédulas Existentes ---\n");
    int tem_cedulas = 0;
    for (int i = 0; i < NUM_CEDULAS; i++) {
        if (estoque_cedulas_atuais[i] > 0) {
            tem_cedulas = 1;
            printf("Cédula %d quantidade ", valor_cedulas[i]);
            numeroPorExtenso(estoque_cedulas_atuais[i]);
            printf("     %d\n", estoque_cedulas_atuais[i]);
        }
    }
    if (!tem_cedulas) {
        printf("Nenhuma cédula existente.\n");
    }
}

// --- FUNÇÕES AUXILIARES PARA CLIENTES ---
// Função auxiliar para encontrar o índice de um cliente ativo pela conta.
// Retorna o índice se o cliente for encontrado e ativo, -1 caso contrário.
int encontrarClientePorConta(const char *conta) {
    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (clientes_ativo[i] == 1 && strcmp(clientes_conta_corrente[i], conta) == 0) {
            return i;
        }
    }
    return -1;
}

// Função auxiliar para verificar se o CPF ou a conta já existem entre clientes ATIVOS.
// Retorna 1 se for duplicado, 0 caso contrário.
int cliente_duplicado(const char *cpf, const char *conta) {
    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (clientes_ativo[i] == 1) {
            if (strcmp(cpf, clientes_cpf[i]) == 0 || strcmp(conta, clientes_conta_corrente[i]) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

// Função interna para realizar o processo de cadastro de um único cliente
// É chamada por incluirClientes
void realizarCadastroClienteInterno() {
    char novo_cpf_para_cliente[TAM_CPF];
    char nova_conta_para_cliente[TAM_CONTA];
    int indice_disponivel = -1;
    int deve_gerar_dados = 1;
    char resposta[TAM_RESPOSTA];
    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (clientes_ativo[i] == 0) {
            indice_disponivel = i;
            break;
        }
    }
    if (indice_disponivel == -1) {
        printf("Erro: Não há slots disponíveis para novos clientes. Máximo de clientes atingido.\n");
        return;
    }
    while (1) {
        if (deve_gerar_dados) {
            gera_cpf_valido(novo_cpf_para_cliente);
            geraContaCorrente(nova_conta_para_cliente);
            if (cliente_duplicado(novo_cpf_para_cliente, nova_conta_para_cliente)) {
                printf("CPF %s ou Conta %s já existem. Gerando novos dados...\n", novo_cpf_para_cliente, nova_conta_para_cliente);
            } else {
                printf("Dados gerados e únicos:\n");
                printf("  CPF: %s\n", novo_cpf_para_cliente);
                printf("  Conta: %s\n", nova_conta_para_cliente);
                deve_gerar_dados = 0;
            }
        }
        printf("Deseja continuar?(SIM / NAO)\n");
        if (scanf(" %s", resposta) != 1) {
            printf("Entrada inválida! Por favor, digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n');
            continue;
        }
        for (int i = 0; resposta[i]; i++) {
            resposta[i] = tolower(resposta[i]);
        }
        if (strcmp(resposta, "sim") == 0) {
            strcpy(clientes_cpf[indice_disponivel], novo_cpf_para_cliente);
            strcpy(clientes_conta_corrente[indice_disponivel], nova_conta_para_cliente);
            total_money_retirado_por_cliente[indice_disponivel] = 0.0;
            clientes_numero_saques_realizados[indice_disponivel] = 0;
            clientes_saques_contador[indice_disponivel] = 0;
            clientes_ativo[indice_disponivel] = 1;
            quantidade_clientes++;
            printf("Cliente cadastrado com sucesso na posição %d!\n", indice_disponivel);
            break;
        } else if (strcmp(resposta, "nao") == 0) {
            printf("Cadastro cancelado...\n");
            break;
        } else {
            printf("Resposta inválida. Por favor, digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n');
        }
    }
}

// objetivo:--
// parâmetros: nenhum
// retorno:nenhum
void incluirClientes() {
    char resposta[TAM_RESPOSTA];
    int resposta_valida = 0;
    if (quantidade_clientes >= MAX_CLIENTES) {
        printf("Numero maximo de clientes atingido. Não é possível incluir mais clientes.\n");
        return;
    }
    do {
        printf("\nGostaria de incluir um novo cliente? (SIM / NAO)\n");
        if (scanf(" %s", resposta) != 1) {
            printf("Entrada inválida! Por favor, digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n');
            continue;
        }
        for (int i = 0; resposta[i]; i++) {
            resposta[i] = tolower(resposta[i]);
        }
        if (strcmp(resposta, "sim") == 0) {
            printf("Iniciando o processo de inclusão de cliente...\n");
            realizarCadastroClienteInterno();
            resposta_valida = 1;
        } else if (strcmp(resposta, "nao") == 0) {
            printf("Inclusão de cliente cancelada.\n");
            resposta_valida = 1;
            break;
        } else {
            printf("Resposta inválida. Por favor, digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n');
        }
    } while (resposta_valida == 0);
}

// objetivo:--
// parâmetros: nenhum
// retorno:nenhum
void mostrarClientes() {
    printf("\n--- Lista de Clientes Cadastrados ---\n");
    int clientes_exibidos = 0;
    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (clientes_ativo[i] == 1) {
            printf("Cliente na Posição %d:\n", i);
            printf("CPF: %s\n", clientes_cpf[i]);
            printf("Conta: %s\n", clientes_conta_corrente[i]);
            printf("Saldo de saques: %.2f\n", total_money_retirado_por_cliente[i]);
            printf("Número de Saques Realizados: %d\n", clientes_numero_saques_realizados[i]);
            printf("--------------------------\n");
            clientes_exibidos++;
        }
    }
    if (clientes_exibidos == 0) {
        printf("Nenhum cliente ativo para exibir.\n");
    }
}

// objetivo:---
// parâmetros: nenhum
// retorno:nenhum
void alterarClientes() {
    char resposta[TAM_RESPOSTA];
    char conta_alvo[TAM_CONTA];
    if (quantidade_clientes == 0) {
        printf("Nenhum cliente cadastrado para alterar.\n");
        return;
    }
    printf("Gostaria de alterar conta e cpf? (SIM / NAO)\n");
    scanf(" %s", resposta);
    for (int i = 0; resposta[i]; i++) {
        resposta[i] = tolower(resposta[i]);
    }
    if (strcmp(resposta, "sim") == 0) {
        printf("Digite o número da conta atual do cliente: ");
        scanf(" %s", conta_alvo);
        int indice_cliente = encontrarClientePorConta(conta_alvo);
        if (indice_cliente == -1 || clientes_ativo[indice_cliente] == 0) {
            printf("Conta não encontrada ou cliente inativo.\n");
            return;
        }
        if (clientes_numero_saques_realizados[indice_cliente] > 0) {
            printf("Alteração não permitida: cliente já realizou saques.\n");
            return;
        }
        int resposta_alterar;
        int duplicada;
        char novo_cpf[TAM_CPF];
        char nova_conta[TAM_CONTA];
        printf("O que deseja modificar?\n");
        printf("1- CPF\n");
        printf("2- Conta Corrente\n");
        scanf("%d", &resposta_alterar);
        switch (resposta_alterar) {
            case 1:
                do {
                    gera_cpf_valido(novo_cpf);
                    duplicada = 0;
                    for (int i = 0; i < quantidade_clientes; i++) {
                        if (i != indice_cliente && clientes_ativo[i] == 1 && strcmp(clientes_cpf[i], novo_cpf) == 0) {
                            duplicada = 1;
                            break;
                        }
                    }
                } while (duplicada == 1);
                strcpy(clientes_cpf[indice_cliente], novo_cpf);
                printf("CPF alterado com sucesso\n");
                printf("Conta Corrente: %s\n", clientes_conta_corrente[indice_cliente]);
                printf("Novo CPF: %s\n", clientes_cpf[indice_cliente]);
                break;
            case 2:
                do {
                    geraContaCorrente(nova_conta);
                    duplicada = 0;
                    for (int i = 0; i < quantidade_clientes; i++) {
                        if (i != indice_cliente && clientes_ativo[i] == 1 && strcmp(clientes_conta_corrente[i], nova_conta) == 0) {
                            duplicada = 1;
                            break;
                        }
                    }
                } while (duplicada == 1);
                strcpy(clientes_conta_corrente[indice_cliente], nova_conta);
                printf("Conta Corrente alterada com sucesso!\n");
                printf("Antiga Conta Corrente do usuario: %s\n", conta_alvo);
                printf("Nova Conta Corrente: %s\n", clientes_conta_corrente[indice_cliente]);
                break;
            default:
                printf("Opção digitada inválida, tente novamente\n");
                break;
        }
    } else if (strcmp(resposta, "nao") == 0) {
        printf("Nada alterado.\n");
    } else {
        printf("Resposta inválida. Por favor, digite 'sim' ou 'nao'.\n");
    }
}

// objetivo: ---
// parâmetros: nenhum
// retorno:nenhum
void excluirClientes() {
    char resposta[TAM_RESPOSTA];
    char conta_alvo[TAM_CONTA];
    int resposta_valida_encontrada = 0;
    if (quantidade_clientes == 0) {
        printf("Nenhum cliente cadastrado para exclusão.\n");
        return;
    }
    printf("\n--- Excluir Cliente ---\n");
    printf("Digite o número da conta corrente a ser excluída: ");
    scanf(" %s", conta_alvo);
    int indice_cliente = encontrarClientePorConta(conta_alvo);
    if (indice_cliente == -1) {
        printf("Erro: Conta corrente não encontrada ou já inativa.\n");
        return;
    }
    if (clientes_saques_contador[indice_cliente] > 0) {
        printf("Erro: Não é possível excluir cliente com saques realizados.\n");
        return;
    }
    do {
        printf("Gostaria de excluir a conta %s (CPF: %s)? (SIM / NAO)\n", clientes_conta_corrente[indice_cliente], clientes_cpf[indice_cliente]);
        if (scanf(" %s", resposta) != 1) {
            printf("Entrada inválida! Por favor, digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n');
            continue;
        }
        for (int i = 0; resposta[i]; i++) {
            resposta[i] = tolower(resposta[i]);
        }
        if (strcmp(resposta, "sim") == 0) {
            clientes_ativo[indice_cliente] = 0;
            quantidade_clientes--;
            printf("Conta excluída com sucesso.\n");
            resposta_valida_encontrada = 1;
        } else if (strcmp(resposta, "nao") == 0) {
            printf("Conta não excluída.\n");
            resposta_valida_encontrada = 1;
        } else {
            printf("Resposta inválida. Por favor, digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n');
        }
    } while (resposta_valida_encontrada == 0);
}

// objetivo:Gerenciar o menu de opções do cliente.
//          Permite ao usuário selecionar ações como incluir, mostrar, alterar e excluir clientes.
//          Continua exibindo o menu até que o usuário escolha a opção 'Voltar'.
// parâmetros: nenhum
// retorno:nenhum
void menuCliente(int opcao_cliente) {
    switch (opcao_cliente) {
        case 1:
            printf("Incluir Cliente\n");
            incluirClientes();
            break;
        case 2:
            printf("Mostrar Clientes\n");
            mostrarClientes();
            break;
        case 3:
            printf("Alterar Cliente\n");
            alterarClientes();
            break;
        case 4:
            printf("Excluir Cliente\n");
            excluirClientes();
            break;
        case 5:
            printf("Voltando...\n");
            break;
        default:
            printf("Opção Inválida!\n");
            break;
    }
}

// objetivo:Exibir o menu de opções do cliente.
// parámetros: nenhum
// retorno:nenhum
void exibirMenuCliente() {
    int opcaoCliente;
    do {
        printf("\nMenu Cliente\n");
        printf("1 - Incluir\n");
        printf("2 - Mostrar\n");
        printf("3 - Alterar\n");
        printf("4 - Excluir\n");
        printf("5 - Voltar\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcaoCliente) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida! Tente novamente.\n");
            continue;
        }
        menuCliente(opcaoCliente);
    } while (opcaoCliente != 5);
}

// objetivo:Gerenciar o menu de opções do relatorio
//          Permite ao usuário selecionar ações como valor sacado, saldo existente etc
//          Continua exibindo o menu até que o usuário escolha a opção 'Voltar'.
// parâmetros: nenhum
// retorno:nenhum
void MenuRelatorios(int opcaoRelatorio) {
    switch (opcaoRelatorio) {
        case 1:
            valorSacado();
            break;
        case 2:
            valorSaldoExistente();
            break;
        case 3:
            QtdCedulasExistentes();
            break;
        case 4:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opção Inválida!\n");
            break;
    }
}

// objetivo:Exibir o menu de Relatorios
// parámetros: nenhum
// retorno:nenhum
void ExibirMenuRelatorios() {
    int opcaoRelatorio;
    do {
        printf("\nRelatórios\n");
        printf("1 - Valores sacados\n");
        printf("2 - Valor do saldo existente\n");
        printf("3 - Quantidade de cédulas existentes\n");
        printf("4 - Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcaoRelatorio) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida! Tente novamente.\n");
            continue;
        }
        MenuRelatorios(opcaoRelatorio);
    } while (opcaoRelatorio != 4);
}

// objetivo:Gerenciar o menu de opções do menu principal
//          Permite ao usuário gerenciar os outros menus
//          Continua exibindo o menu até que o usuário escolha a opção 'finalizar'.
// parámetros: nenhum
// retorno:nenhum
void MENU_PRINCIPAL(int opcao_principal) {
    switch (opcao_principal) {
        case 1:
            exibirMenuCliente();
            break;
        case 2:
            ExibirMenuSaque();
            break;
        case 3:
            ExibirMenuRelatorios();
            break;
        case 4:
            printf("Caixa Finalizado!\n");
            break;
        default:
            printf("Opção Inválida!\n");
            break;
    }
}

// objetivo:Exibir o menu de opções do menu principal
// parámetros: nenhum
// retorno:nenhum
void exibirMenu() {
    int opcao_printar;
    do {
        printf("\nMENU PRINCIPAL\n");
        printf("1 - Cliente\n");
        printf("2 - Saque\n");
        printf("3 - Relatórios\n");
        printf("4 - Finalizar\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao_printar) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida! Tente novamente.\n");
            continue;
        }
        MENU_PRINCIPAL(opcao_printar);
    } while (opcao_printar != 4);
}

int main() {
    srand(time(NULL));
    inicializar_historico_saque();
    printf("Bem-Vindo ao Sistema!\n");
    exibirMenu();
    printf("\nSistema Encerrado.\n");
    return 0;
}