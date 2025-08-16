#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Alunos: Ryan Roberto Lima Campos, João Pedro Schons Martins. Vinícius Amâncio Martins Pereira, Thais

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

// objetivo: Inicializa o historico de saques para todos os clientes
// parametros: nenhum
// retorno: nenhum
void inicializar_historico_saque()
{
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        clientes_saques_contador[i] = 0;
        for (int j = 0; j < MAX_SAQUES_POR_CLIENTE; j++)
        {
            clientes_historico_saques_valores[i][j] = 0.0;
        }
    }
}

// objetivo: escolhe dentre as letras do alfabeto ('a'..'z') uma letra aleatoriamente
// parametros: nenhum
// retorno: a letra do alfabeto
char geraAlfabeto()
{
    char letras[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    return letras[rand() % 26] - 32;
}

// objetivo: escolhe dentre os numeros ('0'..'9') um numero aleatoriamente
// parametros: nenhum
// retorno: o numero
char geraNumero()
{
    char numeros[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    return numeros[rand() % 10];
}

// objetivo: gera aleatoriamente um numero de conta corrente no formato 999.999-X
// parametros: c onde armazena a conta gerada
// retorno: nenhum
void geraContaCorrente(char c[])
{
    for (int i = 0; i < 3; i++)
        c[i] = geraNumero();
    c[3] = '.';
    for (int i = 4; i < 7; i++)
        c[i] = geraNumero();
    c[7] = '-';
    c[8] = geraAlfabeto();
    c[9] = '\0';
}

// objetivo: insere pontuacoes '.' e '-' em um cpf
// parametros: cpf_origem o cpf recebido no formato 99999999999
//             cpf_destino o cpf com as pontuacoes inseridas no formato 999.999.999-99
// retorno: nenhum
void insere_pontuacao_cpf(char cpf_origem[], char cpf_destino[])
{
    int i = 0, j = 0;
    while (i < 11)
    {
        cpf_destino[j++] = cpf_origem[i++];
        if (i == 3 || i == 6)
            cpf_destino[j++] = '.';
        else if (i == 9)
            cpf_destino[j++] = '-';
    }
    cpf_destino[j] = '\0';
}

// objetivo: calcula o primeiro digito verificador de um cpf no formato 999999999
// parametros: cpf o cpf sem os digitos verificadores
// retorno: o calculo do primeiro digito verificador
int obtem_primeiro_digito_verificador(char cpf[])
{
    int soma = 0, peso = 10;
    for (int i = 0; i < 9; i++)
    {
        soma += (cpf[i] - '0') * peso--;
    }
    int resto = soma % 11;
    return resto < 2 ? 0 : 11 - resto;
}

// objetivo: calcula o segundo digito verificador de um cpf no formato 999999999
// parametros: cpf sem os digitos verificadores
// retorno: o calculo do segundo digito verificador
int obtem_segundo_digito_verificador(char cpf[])
{
    int soma = 0, peso = 11;
    for (int i = 0; i < 10; i++)
    {
        soma += (cpf[i] - '0') * peso--;
    }
    int resto = soma % 11;
    return resto < 2 ? 0 : 11 - resto;
}

// objetivo: gera aleatoriamente um cpf valido no formato 999.999.999-99
// parametros: cpf onde sera armazenado o cpf valido
// retorno: nenhum
void gera_cpf_valido(char cpf[])
{
    char cpf_temp_9_digitos[10];
    char cpf_temp_10_digitos[11];
    char cpf_temp_11_digitos[12];
    int primeiro_verificador;
    int segundo_verificador;
    int valid_cpf;
    do
    {
        for (int i = 0; i < 9; i++)
        {
            cpf_temp_9_digitos[i] = geraNumero();
        }
        cpf_temp_9_digitos[9] = '\0';
        // Verifica se todos os dígitos são iguais
        int todos_iguais = 1;
        for (int i = 1; i < 9; i++)
        {
            if (cpf_temp_9_digitos[i] != cpf_temp_9_digitos[0])
            {
                todos_iguais = 0;
                break;
            }
        }
        if (todos_iguais)
            continue;
        primeiro_verificador = obtem_primeiro_digito_verificador(cpf_temp_9_digitos);
        strncpy(cpf_temp_10_digitos, cpf_temp_9_digitos, 9);
        cpf_temp_10_digitos[9] = (char)(primeiro_verificador + '0');
        cpf_temp_10_digitos[10] = '\0';
        segundo_verificador = obtem_segundo_digito_verificador(cpf_temp_10_digitos);
        strncpy(cpf_temp_11_digitos, cpf_temp_10_digitos, 10);
        cpf_temp_11_digitos[10] = (char)(segundo_verificador + '0');
        cpf_temp_11_digitos[11] = '\0';
        insere_pontuacao_cpf(cpf_temp_11_digitos, cpf);
        valid_cpf = verifica_cpf_valido(cpf);
    } while (!valid_cpf);
}

// objetivo: verifica se um cpf no formato 999.999.999-99 e valido
// parametros: cpf a ser verificado
// retorno: 1 se cpf e valido ou 0 se cpf nao e valido
int verifica_cpf_valido(char cpf[])
{
    char cpf_digitos[12];
    int j = 0;
    for (int i = 0; cpf[i]; i++)
    {
        if (cpf[i] >= '0' && cpf[i] <= '9')
            cpf_digitos[j++] = cpf[i];
    }
    cpf_digitos[j] = '\0';
    if (strlen(cpf_digitos) != 11)
        return 0;
    int todos_iguais = 1;
    for (int k = 1; k < 11; k++)
    {
        if (cpf_digitos[k] != cpf_digitos[0])
        {
            todos_iguais = 0;
            break;
        }
    }
    if (todos_iguais)
        return 0;
    char cpf9[10];
    strncpy(cpf9, cpf_digitos, 9);
    cpf9[9] = '\0';
    if (obtem_primeiro_digito_verificador(cpf9) != (cpf_digitos[9] - '0'))
        return 0;
    char cpf10[11];
    strncpy(cpf10, cpf_digitos, 10);
    cpf10[10] = '\0';
    if (obtem_segundo_digito_verificador(cpf10) != (cpf_digitos[10] - '0'))
        return 0;
    return 1;
}

// objetivo: converte um numero inteiro para sua representacao por extenso
// parametros: numero o valor a ser convertido
// retorno: nenhum (imprime diretamente)
void numeroPorExtenso(int numero)
{
    if (numero == 0)
    {
        printf("Zero\n");
        return;
    }
    if (numero > 999999)
    {
        printf("Número fora do intervalo suportado (0 a 999999)\n");
        return;
    }
    int digitos[6] = {0}, contador = 0, temp = numero;
    while (temp > 0)
    {
        digitos[contador++] = temp % 10;
        temp /= 10;
    }
    int precisa_e = 0;
    // Centena de milhar
    if (contador >= 6 && digitos[5])
    {
        if (digitos[5] == 1 && !digitos[4] && !digitos[3])
        {
            printf("Cem Mil");
        }
        else
        {
            printf("%s", centenas[digitos[5]]);
            if (digitos[5] > 1)
                printf(" Mil");
        }
        precisa_e = digitos[4] || digitos[3] || digitos[2] || digitos[1] || digitos[0];
    }
    // Dezena de milhar
    if (contador >= 5 && digitos[4])
    {
        if (precisa_e)
            printf(" e ");
        if (digitos[4] == 1 && digitos[3] < 10)
        {
            printf("%s Mil", especiais[digitos[3]]);
        }
        else
        {
            printf("%s", dezenas[digitos[4]]);
            if (digitos[3])
                printf(" e %s Mil", unidades[digitos[3]]);
            else if (digitos[4] >= 2)
                printf(" Mil");
            else if (digitos[4] == 1 && !digitos[3])
                printf(" Mil");
        }
        precisa_e = digitos[2] || digitos[1] || digitos[0];
    }
    else if (contador >= 4 && digitos[3])
    {
        if (precisa_e)
            printf(" e ");
        printf("%s Mil", unidades[digitos[3]]);
        precisa_e = digitos[2] || digitos[1] || digitos[0];
    }
    // Centena
    if (contador >= 3 && digitos[2])
    {
        if (precisa_e)
            printf(" e ");
        printf("%s", digitos[2] == 1 && !digitos[1] && !digitos[0] ? "Cem" : centenas[digitos[2]]);
        precisa_e = digitos[1] || digitos[0];
    }
    // Dezena e unidade
    if (contador >= 2 && digitos[1])
    {
        if (precisa_e)
            printf(" e ");
        if (digitos[1] == 1)
        {
            printf("%s", especiais[digitos[0]]);
        }
        else
        {
            printf("%s", dezenas[digitos[1]]);
            if (digitos[0])
                printf(" e %s", unidades[digitos[0]]);
        }
    }
    else if (contador >= 1 && digitos[0])
    {
        if (precisa_e)
            printf(" e ");
        printf("%s", unidades[digitos[0]]);
    }
    printf("\n");
}

// objetivo: realiza o saque, distribuindo cedulas e confirmando a operacao
// parametros: saqueCliente o valor do saque, estoque_cedulas o estoque atual, indice_cliente o indice do cliente
// retorno: 1 se saque realizado com sucesso, 0 caso contrario
int fazerSaque(int saqueCliente, int *estoque_cedulas, int indice_cliente)
{
    if (saqueCliente <= 0 || saqueCliente > 999999)
    {
        printf("Valor de saque invalido! Deve ser entre 1 e 999999.\n");
        return 0;
    }
    int cedulas_dadas[NUM_CEDULAS] = {0}, temp_saque = saqueCliente;
    for (int i = NUM_CEDULAS - 1; i >= 0; i--)
    {
        cedulas_dadas[i] = temp_saque / valor_cedulas[i];
        if (cedulas_dadas[i] > estoque_cedulas[i])
        {
            cedulas_dadas[i] = estoque_cedulas[i];
        }
        temp_saque -= cedulas_dadas[i] * valor_cedulas[i];
    }
    if (temp_saque != 0)
    {
        printf("Saque impossivel com as cedulas disponiveis!\n");
        return 0;
    }
    printf(" Valor solicitado: R$ %d,00\n", saqueCliente);
    numeroPorExtenso(saqueCliente);
    for (int i = NUM_CEDULAS - 1; i >= 0; i--)
    {
        if (cedulas_dadas[i] > 0)
        {
            printf(" %d cedula(s) de %d\n", cedulas_dadas[i], valor_cedulas[i]);
        }
    }
    char confirmacao[TAM_RESPOSTA];
    printf("Confirmar saque? (SIM / NAO)\n");
    while (1)
    {
        scanf(" %s", confirmacao);
        for (int i = 0; confirmacao[i]; i++)
            confirmacao[i] = tolower(confirmacao[i]);
        if (strcmp(confirmacao, "sim") == 0)
        {
            for (int i = 0; i < NUM_CEDULAS; i++)
            {
                estoque_cedulas[i] -= cedulas_dadas[i];
            }
            clientes_numero_saques_realizados[indice_cliente]++;
            total_money_retirado_por_cliente[indice_cliente] += saqueCliente;
            if (clientes_saques_contador[indice_cliente] < MAX_SAQUES_POR_CLIENTE)
            {
                clientes_historico_saques_valores[indice_cliente][clientes_saques_contador[indice_cliente]] = saqueCliente;
                clientes_saques_contador[indice_cliente]++;
            }
            else
            {
                printf("Aviso: Limite de registros de saques individuais para este cliente atingido.\n");
            }
            printf("\nSaque de R$ %.0f,00 realizado com sucesso!\n", (double)saqueCliente);
            numeroPorExtenso(saqueCliente);
            printf("Voce realizou %d de %d saques disponiveis.\n", clientes_numero_saques_realizados[indice_cliente], limites_saques_por_clientes);
            return 1;
        }
        else if (strcmp(confirmacao, "nao") == 0)
        {
            printf("Saque cancelado pelo usuario\n");
            return 0;
        }
        else
        {
            printf("Entrada invalida! Por favor, digite 'SIM' ou 'NAO'.\n");
        }
    }
}

// objetivo: exibe o menu para realizar saques
// parametros: nenhum
// retorno: nenhum
void ExibirMenuSaque()
{
    if (quantidade_clientes == 0)
    {
        printf("\n---Nenhum cliente registrado para poder efetuar o saque.---\n");
        return;
    }
    char conta_digitada[TAM_CONTA];
    int indice_do_cliente;
    double valor_saque;
    int saque_bem_sucedido = 0;

    do
    {
        printf("\n--- Realizar Saque ---\n");
        printf("Digite o numero da conta corrente (formato 999.999-x): ");
        if (scanf(" %10s", conta_digitada) != 1)
        {
            printf("Entrada invalida! Por favor, digite a conta no formato pedido.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        indice_do_cliente = encontrarClientePorConta(conta_digitada);
        if (indice_do_cliente == -1)
        {
            printf("ERRO: Conta corrente nao encontrada ou inativa. Tente novamente\n");
        }
        else if (clientes_numero_saques_realizados[indice_do_cliente] >= limites_saques_por_clientes)
        {
            printf("Erro: O cliente ja atingiu o limite de %d saques.\n", limites_saques_por_clientes);
            return;
        }
        else
        {
            break;
        }
    } while (1);
    do
    {
        printf("Digite o valor do saque: R$ ");
        if (scanf("%lf", &valor_saque) != 1 || valor_saque != (int)valor_saque)
        {
            printf("Valor invalido! Por favor, digite um valor numerico inteiro.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        while (getchar() != '\n')
            ;
        if (valor_saque <= 0)
        {
            printf("Erro: O valor de saque precisa ser maior que ZERO. Tente novamente!\n");
            continue;
        }
        printf("\n---Confirmacao do saque---\n");
        printf(" Cliente: %s (CPF: %s)\n", clientes_conta_corrente[indice_do_cliente], clientes_cpf[indice_do_cliente]);
        saque_bem_sucedido = fazerSaque((int)valor_saque, estoque_cedulas_atuais, indice_do_cliente);
        if (saque_bem_sucedido == 1 || saque_bem_sucedido == 0)
        {
            break; // Sai do loop de solicitação de valor do saque
        }
    } while (1);
}

// objetivo: exibe o relatorio de valores sacados por todos os clientes
// parametros: nenhum
// retorno: nenhum
void valorSacado()
{
    double total_geral = 0.0;
    int clientes_com_saques = 0;
    printf("----------------------------------------------------------------------\n");
    printf("Relatorio 'Valores Sacados'\n");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        if (clientes_ativo[i] == 1 && clientes_saques_contador[i] > 0)
        {
            clientes_com_saques++;
            double total_cliente = 0.0;
            printf("conta-corrente %s cpf %s\n", clientes_conta_corrente[i], clientes_cpf[i]);
            for (int j = 0; j < clientes_saques_contador[i]; j++)
            {
                total_cliente += clientes_historico_saques_valores[i][j];
                printf("                      R$ %.2f reais\n ", clientes_historico_saques_valores[i][j]);
            }
            printf("                                        R$ total_1 %.2f reais\n", total_cliente);
            total_geral += total_cliente;
        }
    }
    if (clientes_com_saques > 0)
    {
        printf("----------------------------------------------------------------------\n");
        printf("R$ %.2f reais ", total_geral);
        numeroPorExtenso((int)total_geral);

        printf("----------------------------------------------------------------------\n");
    }
    else
    {
        printf("Nenhum saque registrado.\n");
    }
}

// objetivo: exibe o relatorio do valor do saldo existente de todos os clientes ativos
// parametros: nenhum
// retorno: nenhum
void valorSaldoExistente()
{
    printf("----------------------------------------------------------------------\n");
    printf("Relatorio 'Valor do saldo existente'\n");
    printf("----------------------------------------------------------------------\n");
    long long saldo_total = 0;
    for (int i = 0; i < NUM_CEDULAS; i++)
    {
        saldo_total += (long long)estoque_cedulas_atuais[i] * valor_cedulas[i];
    }
    if (saldo_total > 0)
    {
        printf("R$ %.0f,00 (", (double)saldo_total);
        numeroPorExtenso((int)saldo_total);
        printf("----------------------------------------------------------------------\n");
    }
    else
    {
        printf("Nenhum saldo disponivel na maquina.\n");
    }
}

// objetivo: exibe o relatorio da quantidade de cedulas existentes no caixa
// parametros: nenhum
// retorno: nenhum
void QtdCedulasExistentes()
{
    printf("----------------------------------------------------------------------\n");
    printf("Relatorio 'Quantidade de cedulas existentes\n");
    printf("----------------------------------------------------------------------\n");
    int tem_cedulas = 0;
    for (int i = 0; i < NUM_CEDULAS; i++)
    {
        if (estoque_cedulas_atuais[i] > 0)
        {
            tem_cedulas = 1;
            printf("Cedula %d quantidade ", valor_cedulas[i]);
            numeroPorExtenso(estoque_cedulas_atuais[i]);
        }
    }
    if (!tem_cedulas)
    {
        printf("Nenhuma cedula existente.\n");
    }
}

// objetivo: encontra o indice de um cliente ativo pela conta
// parametros: conta a conta a ser buscada
// retorno: indice se encontrado e ativo, -1 caso contrario
int encontrarClientePorConta(const char *conta)
{
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        if (clientes_ativo[i] == 1 && strcmp(clientes_conta_corrente[i], conta) == 0)
        {
            return i;
        }
    }
    return -1;
}

// objetivo: verifica se o CPF ou a conta ja existem entre clientes ativos
// parametros: cpf e conta a serem verificados
// retorno: 1 se duplicado, 0 caso contrario
int cliente_duplicado(const char *cpf, const char *conta)
{
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        if (clientes_ativo[i] == 1)
        {
            if (strcmp(cpf, clientes_cpf[i]) == 0 || strcmp(conta, clientes_conta_corrente[i]) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

// objetivo: realiza o processo de cadastro de um unico cliente
// parametros: nenhum
// retorno: nenhum
void realizarCadastroClienteInterno()
{
    char novo_cpf[TAM_CPF], nova_conta[TAM_CONTA];
    int indice_disponivel = -1;
    char resposta[TAM_RESPOSTA];
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        if (clientes_ativo[i] == 0)
        {
            indice_disponivel = i;
            break;
        }
    }
    if (indice_disponivel == -1)
    {
        printf("Erro: Nao ha slots disponiveis para novos clientes.\n");
        return;
    }
    int deve_gerar_dados = 1;
    while (1)
    {
        if (deve_gerar_dados)
        {
            gera_cpf_valido(novo_cpf);
            geraContaCorrente(nova_conta);
            if (cliente_duplicado(novo_cpf, nova_conta))
            {
                printf("CPF %s ou Conta %s ja existem. Gerando novos dados...\n", novo_cpf, nova_conta);
            }
            else
            {
                printf("Dados gerados e unicos:\n");
                printf("  CPF: %s\n", novo_cpf);
                printf("  Conta: %s\n", nova_conta);
                deve_gerar_dados = 0;
            }
        }
        printf("Deseja continuar? (SIM / NAO)\n");
        if (scanf(" %s", resposta) != 1)
        {
            printf("Entrada invalida! Digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        for (int i = 0; resposta[i]; i++)
            resposta[i] = tolower(resposta[i]);
        if (strcmp(resposta, "sim") == 0)
        {
            strcpy(clientes_cpf[indice_disponivel], novo_cpf);
            strcpy(clientes_conta_corrente[indice_disponivel], nova_conta);
            total_money_retirado_por_cliente[indice_disponivel] = 0.0;
            clientes_numero_saques_realizados[indice_disponivel] = 0;
            clientes_saques_contador[indice_disponivel] = 0;
            clientes_ativo[indice_disponivel] = 1;
            quantidade_clientes++;
            printf("Cliente cadastrado com sucesso na posicao %d!\n", indice_disponivel);
            break;
        }
        else if (strcmp(resposta, "nao") == 0)
        {
            printf("Cadastro cancelado...\n");
            break;
        }
        else
        {
            printf("Resposta invalida. Digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n')
                ;
        }
    }
}

// objetivo: inclui um novo cliente se possivel
// parametros: nenhum
// retorno: nenhum
void incluirClientes()
{
    char resposta[TAM_RESPOSTA];
    if (quantidade_clientes >= MAX_CLIENTES)
    {
        printf("Numero maximo de clientes atingido.\n");
        return;
    }
    int resposta_valida = 0;
    do
    {
        printf("\nGostaria de incluir um novo cliente? (SIM / NAO)\n");
        if (scanf(" %s", resposta) != 1)
        {
            printf("Entrada invalida! Digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        for (int i = 0; resposta[i]; i++)
            resposta[i] = tolower(resposta[i]);
        if (strcmp(resposta, "sim") == 0)
        {
            printf("Iniciando o processo de inclusao de cliente...\n");
            realizarCadastroClienteInterno();
            resposta_valida = 1;
        }
        else if (strcmp(resposta, "nao") == 0)
        {
            printf("Inclusao de cliente cancelada.\n");
            resposta_valida = 1;
            break;
        }
        else
        {
            printf("Resposta invalida. Digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n')
                ;
        }
    } while (!resposta_valida);
}

// objetivo: mostra a lista de clientes cadastrados
// parametros: nenhum
// retorno: nenhum
void mostrarClientes()
{
    printf("\n--- Lista de Clientes Cadastrados ---\n");
    int clientes_exibidos = 0;
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        if (clientes_ativo[i] == 1)
        {
            printf("Cliente na Posicao %d:\n", i);
            printf("CPF: %s\n", clientes_cpf[i]);
            printf("Conta: %s\n", clientes_conta_corrente[i]);
            printf("Saldo de saques: %.2f\n", total_money_retirado_por_cliente[i]);
            printf("Numero de Saques Realizados: %d\n", clientes_numero_saques_realizados[i]);
            printf("--------------------------\n");
            clientes_exibidos++;
        }
    }
    if (clientes_exibidos == 0)
    {
        printf("Nenhum cliente ativo para exibir.\n");
    }
}

// objetivo: altera dados de um cliente existente
// parametros: nenhum
// retorno: nenhum
// objetivo: altera dados de um cliente existente
// parametros: nenhum
// retorno: nenhum
void alterarClientes()
{
    char resposta[TAM_RESPOSTA]; // Resposta para "Gostaria de alterar conta e cpf?"
    char conta_alvo[TAM_CONTA];
    int indice_cliente = -1; // Inicializa com -1 para indicar que a conta ainda nao foi encontrada

    if (quantidade_clientes == 0)
    {
        printf("Nenhum cliente cadastrado para alterar.\n");
        return; // Nao ha clientes, entao sai da funcao
    }

    // --- 1. Validacao da resposta inicial "SIM / NAO" ---
    // Este loop garante que o usuario digite SIM ou NAO corretamente
    int confirmacao_inicial_valida = 0;
    do
    {
        printf("Gostaria de alterar conta e cpf? (SIM / NAO)\n");
        // Verifica se a leitura foi bem sucedida
        if (scanf(" %s", resposta) != 1)
        {
            printf("Entrada invalida! Por favor, digite 'SIM' ou 'NAO'.\n");
            // Limpa o buffer do teclado para evitar loop infinito de entrada invalida
            while (getchar() != '\n')
                ;
            continue; // Repete o loop para pedir a resposta inicial novamente
        }

        // Converte a resposta para minusculas para facilitar a comparacao
        for (int i = 0; resposta[i]; i++)
        {
            resposta[i] = tolower(resposta[i]);
        }

        if (strcmp(resposta, "sim") == 0)
        {
            confirmacao_inicial_valida = 1; // Resposta valida e positiva, permite sair do loop
        }
        else if (strcmp(resposta, "nao") == 0)
        {
            printf("Alteracao de cliente cancelada.\n");
            return; // Resposta valida e negativa, sai da funcao
        }
        else
        {
            printf("Resposta invalida. Por favor, digite 'SIM' ou 'NAO'.\n");
            // Se a resposta for invalida, 'confirmacao_inicial_valida' permanece 0, e o loop continua.
        }
    } while (!confirmacao_inicial_valida); // Continua ate uma resposta valida ser dada

    // --- 2. Loop para encontrar e validar a conta do cliente ---
    int conta_valida_e_elegivel_encontrada = 0;
    do
    {
        printf("Digite o numero da conta atual do cliente: ");
        // Verifica se a leitura da conta foi bem sucedida
        if (scanf(" %10s", conta_alvo) != 1)
        { // %10s para evitar buffer overflow, considerando TAM_CONTA 11
            printf("Entrada invalida! Por favor, digite a conta no formato 999.999-x.\n");
            while (getchar() != '\n')
                ;     // Limpa o buffer
            continue; // Repete o loop para pedir a conta novamente
        }
        while (getchar() != '\n')
            ; // Limpa o buffer apos a leitura (importante para string)

        indice_cliente = encontrarClientePorConta(conta_alvo);

        if (indice_cliente == -1) // Conta nao encontrada ou inativa
        {
            printf("Conta nao encontrada ou cliente inativo. Por favor, tente novamente.\n");
            // Nao retorna, o loop continua para que o usuario possa tentar outra conta.
        }
        else if (clientes_numero_saques_realizados[indice_cliente] > 0) // Cliente com saques
        {
            printf("Alteracao nao permitida: cliente ja realizou saques.\n");
            return; // Sai da funcao, pois este cliente nao pode ser alterado.
        }
        else // Conta encontrada e elegivel
        {
            conta_valida_e_elegivel_encontrada = 1; // Permite sair deste loop
        }
    } while (!conta_valida_e_elegivel_encontrada); // Continua ate encontrar uma conta valida e elegivel

    int resposta_alterar;
    char novo_cpf[TAM_CPF], nova_conta[TAM_CONTA];
    int duplicada;

    // --- 3. Loop para a escolha do que modificar (CPF ou Conta Corrente) ---
    int opcao_alteracao_valida = 0;
    do
    {
        printf("O que deseja modificar?\n");
        printf("1 - CPF\n");
        printf("2 - Conta Corrente\n");
        printf("Escolha uma opcao (1 ou 2): ");
        // Verifica se a leitura foi bem sucedida e se é um numero inteiro
        if (scanf("%d", &resposta_alterar) != 1)
        {
            printf("Entrada invalida! Por favor, digite 1 para CPF ou 2 para Conta Corrente.\n");
            while (getchar() != '\n')
                ;     // Limpa o buffer
            continue; // Repete o loop para pedir a opcao novamente
        }
        while (getchar() != '\n')
            ; // Limpa o buffer apos ler o int

        if (resposta_alterar == 1 || resposta_alterar == 2)
        {
            opcao_alteracao_valida = 1; // Opcao valida, permite sair do loop
        }
        else
        {
            printf("Opcao invalida! Por favor, digite 1 para CPF ou 2 para Conta Corrente.\n");
            // Se a opcao for invalida, 'opcao_alteracao_valida' permanece 0, e o loop continua.
        }

    } while (!opcao_alteracao_valida); // Continua ate uma opcao valida ser digitada

    // Uma opcao valida (1 ou 2) foi escolhida, agora executa a alteracao.
    switch (resposta_alterar)
    {
    case 1: // Alterar CPF
        do
        {
            gera_cpf_valido(novo_cpf); // Gera um novo CPF aleatorio
            duplicada = 0;
            // Verifica se o novo CPF gerado ja existe para outro cliente ativo
            for (int i = 0; i < MAX_CLIENTES; i++)
            {
                if (i != indice_cliente && clientes_ativo[i] == 1 && strcmp(clientes_cpf[i], novo_cpf) == 0)
                {
                    duplicada = 1; // CPF duplicado
                    break;
                }
            }
        } while (duplicada); // Continua gerando CPF ate que nao seja duplicado para nenhum outro cliente

        strcpy(clientes_cpf[indice_cliente], novo_cpf);
        printf("CPF alterado com sucesso!\n");
        printf("Conta Corrente: %s\n", clientes_conta_corrente[indice_cliente]);
        printf("Novo CPF: %s\n", clientes_cpf[indice_cliente]);
        break;

    case 2: // Alterar Conta Corrente
        do
        {
            geraContaCorrente(nova_conta); // Gera um novo numero de conta aleatorio
            duplicada = 0;
            // Verifica se o novo numero de conta gerado ja existe para outro cliente ativo
            for (int i = 0; i < MAX_CLIENTES; i++)
            {
                if (i != indice_cliente && clientes_ativo[i] == 1 && strcmp(clientes_conta_corrente[i], nova_conta) == 0)
                {
                    duplicada = 1; // Conta duplicada
                    break;
                }
            }
        } while (duplicada); // Continua gerando conta ate que nao seja duplicada para nenhum outro cliente

        strcpy(clientes_conta_corrente[indice_cliente], nova_conta);
        printf("Conta Corrente alterada com sucesso!\n");
        printf("Antiga Conta Corrente: %s\n", conta_alvo);
        printf("Nova Conta Corrente: %s\n", clientes_conta_corrente[indice_cliente]);
        break;
    }
}

// objetivo: exclui um cliente existente
// parametros: nenhum
// retorno: nenhum
void excluirClientes()
{
    int indice_cliente;
    if (quantidade_clientes == 0)
    {
        printf("Nenhum cliente cadastrado para exclusao.\n");
        return;
    }
    char conta_alvo[TAM_CONTA], resposta[TAM_RESPOSTA];
    printf("\n--- Excluir Cliente ---\n");
    do
    {
        printf("Digite o numero da conta corrente a ser excluida: ");
        scanf(" %s", conta_alvo);
        indice_cliente = encontrarClientePorConta(conta_alvo);
        if (indice_cliente == -1)
        {
            printf("Erro: Conta corrente nao encontrada ou ja inativa.\n");
            return;
        }
        else
        {
            break;
        }
    } while (1);
    if (clientes_saques_contador[indice_cliente] > 0)
    {
        printf("Erro: Nao eh possivel excluir cliente com saques realizados.\n");
        return;
    }

    int resposta_valida = 0;
    do
    {
        printf("Gostaria de excluir a conta %s (CPF: %s)? (SIM / NAO)\n", clientes_conta_corrente[indice_cliente], clientes_cpf[indice_cliente]);
        if (scanf(" %s", resposta) != 1)
        {
            printf("Entrada invalida! Digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        for (int i = 0; resposta[i]; i++)
            resposta[i] = tolower(resposta[i]);
        if (strcmp(resposta, "sim") == 0)
        {
            clientes_ativo[indice_cliente] = 0;
            quantidade_clientes--;
            printf("Conta excluida com sucesso.\n");
            resposta_valida = 1;
        }
        else if (strcmp(resposta, "nao") == 0)
        {
            printf("Conta nao excluida.\n");
            resposta_valida = 1;
        }
        else
        {
            printf("Resposta invalida. Digite 'sim' ou 'nao'.\n");
            while (getchar() != '\n')
                ;
        }
    } while (!resposta_valida);
}

// objetivo: gerencia o menu de opcoes do cliente
// parametros: opcao_cliente a opcao selecionada
// retorno: nenhum
void menuCliente(int opcao_cliente)
{
    switch (opcao_cliente)
    {
    case 1:
        incluirClientes();
        break;
    case 2:
        mostrarClientes();
        break;
    case 3:
        alterarClientes();
        break;
    case 4:
        excluirClientes();
        break;
    case 5:
        printf("Voltando...\n");
        break;
    default:
        printf("Opcao Invalida!\n");
        break;
    }
}

// objetivo: exibe o menu de opcoes do cliente
// parametros: nenhum
// retorno: nenhum
void exibirMenuCliente()
{
    int opcaoCliente;
    do
    {
        printf("\nMenu Cliente\n1 - Incluir\n2 - Mostrar\n3 - Alterar\n4 - Excluir\n5 - Voltar\nEscolha uma opcao: ");
        if (scanf("%d", &opcaoCliente) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        menuCliente(opcaoCliente);
    } while (opcaoCliente != 5);
}

// objetivo: gerencia o menu de opcoes de relatorios
// parametros: opcaoRelatorio a opcao selecionada
// retorno: nenhum
void MenuRelatorios(int opcaoRelatorio)
{
    switch (opcaoRelatorio)
    {
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
        printf("Opcao Invalida!\n");
        break;
    }
}

// objetivo: exibe o menu de relatorios
// parametros: nenhum
// retorno: nenhum
void ExibirMenuRelatorios()
{
    int opcaoRelatorio;
    do
    {
        printf("\nRelatorios\n1 - Valores sacados\n2 - Valor do saldo existente\n3 - Quantidade de cedulas existentes\n4 - Voltar ao Menu Principal\nEscolha uma opcao: ");
        if (scanf("%d", &opcaoRelatorio) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        MenuRelatorios(opcaoRelatorio);
    } while (opcaoRelatorio != 4);
}

// objetivo: gerencia o menu principal
// parametros: opcao_principal a opcao selecionada
// retorno: nenhum
void MENU_PRINCIPAL(int opcao_principal)
{
    switch (opcao_principal)
    {
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
        printf("Opcao Invalida!\n");
        break;
    }
}

// objetivo: exibe o menu principal
// parametros: nenhum
// retorno: nenhum
void exibirMenu()
{
    int opcao_printar;
    do
    {
        printf("\nMENU PRINCIPAL\n1 - Cliente\n2 - Saque\n3 - Relatorios\n4 - Finalizar\nEscolha uma opcao: ");
        if (scanf("%d", &opcao_printar) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        MENU_PRINCIPAL(opcao_printar);
    } while (opcao_printar != 4);
}

int main()
{
    srand(time(NULL));
    inicializar_historico_saque();
    printf("Bem-Vindo ao Sistema!\n");
    exibirMenu();
    printf("\nSistema Encerrado.\n");
    return 0;
}
