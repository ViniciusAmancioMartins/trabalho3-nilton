#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//ALTERAÇÕES FEITAS 31-07: modularizei a maior parte do codigo, e tambem iniciei a geracao de CPF, falta apenas fazer a parte de validacao e tals.
//alterações efita 03/07: finalizei a 




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
#define TAM_CPF 15         // "999.999.999-99\0" (14 chars + 1 \0)
#define TAM_CONTA 11    // "999.999-X\0" (9 chars + 1 \0)
#define TAM_RESPOSTA 5 // Para "SIM\0" ou "NAO\0"

// --- Para o Historico de Saque (se for por cliente) ---
#define MAX_SAQUES_POR_CLIENTE 10 // Maximo de saques que um cliente pode ter registrado
#define MAX_TRANSACAO_INFO 50     // Tamanho da string para descrever cada transacao.

// --- Para o Estoque de Cedulas do Caixa ---
#define NUM_CEDULAS 8
int valor_cedulas[NUM_CEDULAS] = {1, 2, 5, 10, 20, 50, 250, 450};                        // Valores das cedulas
int estoque_cedulas_atuais[NUM_CEDULAS] = {12800, 6400, 3200, 1600, 800, 400, 200, 100}; // Quantidade inicial

// --- Matrizes para os Dados dos Clientes (Globais) ---
// Cada linha 'i' de cada matriz corresponde ao cliente 'i'

char clientes_cpf[MAX_CLIENTES][TAM_CPF];              // clientes_cpf[0][057.957321-41]
char clientes_conta_corrente[MAX_CLIENTES][TAM_CONTA]; // clientes_conta[0][XXX.XXXX-J]
double clientes_saldo[MAX_CLIENTES];                   // clientes_saldo[0] = 200 reais
int clientes_numero_saques_realizados[MAX_CLIENTES];   // Contador de saques para cada cliente || //clientes_numeros_saques_realizados[0] = nº saques
int clientes_ativo[MAX_CLIENTES];                      // 1 para ativo, 0 para inativo (exclusao logica) || clientes_ativo[0] = 0

// se (clientes_ativo[i] = 0) entao
// clientes_ativo[i] = vai receber novo cliente

// Variável para manter o controle do número atual de clientes ativos
int quantidade_clientes = 0;

// --- Matrizes para o Historico de Saques POR CLIENTE ---
// Historico detalhado por cliente
// 1. Conta corrente       cpf         valor

// Cada cliente_historico_saque[i][j] possui o valor do j saque do cliente i
// E clientes_saques_count[i] possui quantos saques o cliente i já realizou
double clientes_historico_saques_valores[MAX_CLIENTES][MAX_SAQUES_POR_CLIENTE]; // historico dos valores dos saques por clientes
int clientes_saques_contador[MAX_CLIENTES];                                     // Quantos saques cada cliente realizou (usado como contador de indices)

char clientes_historico_saques_desc[MAX_CLIENTES][MAX_SAQUES_POR_CLIENTE][MAX_TRANSACAO_INFO];

void inicializar_historico_saque()
{
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        clientes_saques_contador[i] = 0;
        for (int j = 0; j < MAX_CLIENTES; j++)
        {
            clientes_historico_saques_valores[i][j] = 0.0;
        }
    }
}

// objetivo:escolhe dentre as letras do alfabeto ('a'..'z') uma letra aleatoriamente
// parametros: nenhum
// retorno:a letra do alfabeto
char geraAlfabeto()
{
    int i;
    char letras[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    i = rand() % 26;
    return (letras[i] - 32);
}

// objetivo:escolhe dentre os numeros ('0'..'9') uma numero aleatoriamente
// parametros: nenhum
// retorno:o numero
char geraNumero()
{
    int i;
    char numeros[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    i = rand() % 10;
    return (numeros[i]);
}

// objetivo:gera aleatoriamente um numero de conta corrente no formato 999.999-X
// parametros: c onde armazera a conta gerada
// retorno:nenhum
void geraContaCorrente(char c[])
{

    // implemente aqui
}


// objetivo:insere pontuacoes '.' e '- ' em um cpf
// parametros: cpf_origem o cpf recebido no format 99999999999
//             cpf_destino o cpf com as pontuacoes inseridas no formato 999.999.999-99
void insere_pontuacao_cpf(char cpf_origem[], char cpf_destino[])
{
    int iOrigem = 0;    //indice para origem
    int jDestino = 0;    //indice para destino
    
    while (iOrigem < 11){
        cpf_destino[jDestino] = cpf_origem[iOrigem];
        iOrigem++;
        jDestino++;

        if (iOrigem == 3 || iOrigem == 6){
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
int obtem_primeiro_digito_verificador(char cpf[])
{
    int digito;
    int soma = 0;
    int peso = 10;

    for (int i = 0; i < 9; i++)
    {
        int numero_cpf_separado = cpf[i] - '0'; // transformo string em inteiro
        soma += numero_cpf_separado * peso;     // multiplico cada numero do cpf pelo peso de cada iteracao de 10 ate 1, e acrescento a soma
        peso--;                                 // decremento o peso
    }

    int resto = soma % 11;
    if (resto < 2)
    {
        digito = 0;
    }
    else
    {
        digito = 11 - resto;
    }

    return (digito);
}

// objetivo:calcula o segundo digito verificador de um cpf no formato 999999999
// parametros: cpf sem os digitos verificadores
// retorno: o calculo do segundo digito verificador
int obtem_segundo_digito_verificador(char cpf[])
{
    int digito;
    int soma = 0;
    int peso = 11;

    for (int i = 0; i < 10; i++)
    {
        int numero_cpf_separado = cpf[i] - '0'; // transformo string em inteiro
        soma += numero_cpf_separado * peso;     // multiplico cada numero do cpf pelo peso de cada iteracao de 10 ate 1, e acrescento a soma
        peso--;                                 // decremento o peso
    }

    int resto = soma % 11;
    if (resto < 2)
    {
        digito = 0;
    }
    else
    {
        digito = 11 - resto;
    }

    return (digito);
}

// objetivo:gera aleatoriamente um cpf valido no formato 999.999.999-99
// parametros: cpf onde sera armazenado o cpf valido
// retorno: nenhum
void gera_cpf_valido(char cpf[])
{
    char cpf_temp_9_digitos[10];  //    Primeiros 9 digitos do cpf + '/0'
    char cpf_temp_10_digitos[11]; //    Para os 10 digitos do cpf (9 + 1ª verificador) + '/0'
    char cpf_temp_11_digitos[12]; //    Para os 11 digitos do cpf (10 + 2ª verificador) + '/0'
    int primeiro_verificador;
    int segundo_verificador;

    for (int i = 0; i < 9; i++)
    {
        cpf_temp_9_digitos[i] = (rand() % 10) + '0'; //'0' buffer e tals
    }
    cpf_temp_9_digitos[9] = '\0'; // garante que eh uma string valida para as funcoes.

    primeiro_verificador = obtem_primeiro_digito_verificador(cpf_temp_9_digitos);
    strncpy(cpf_temp_10_digitos, cpf_temp_9_digitos, 9);         // copia o cpf na variavel temporaria de 9 digitos para a de 10 digitos
    cpf_temp_10_digitos[9] = (char)(primeiro_verificador + '0'); // Adiciona o primeiro verificador como caractere na posicao que deve estar.
    cpf_temp_10_digitos[10] = '\0';
    
    segundo_verificador = obtem_segundo_digito_verificador(cpf_temp_10_digitos);
    strncpy(cpf_temp_11_digitos, cpf_temp_10_digitos, 10);
    cpf_temp_11_digitos[10] = (char)(segundo_verificador + '0');
    cpf_temp_11_digitos[11] = '\0';

    //chamar funcao para inserir pontuacao:
    insere_pontuacao_cpf(cpf_temp_11_digitos, cpf);
}

// objetivo:verifica se um cpf no formato 999.999.999-99 e valido
// parametros: cpf a ser verificado
// retorno: 1 se cpf e valido ou 0 se cpf nao e valido
int verifica_cpf_valido(char cpf[])
{
    int valido = 1; //0 para invalido
    char cpf_apenas_digito[12];       //vetor temporario pra os digitos do cpf + '\0'
    int i = 0;
    int j = 0;

    while(cpf[i] != '\0'){                      //enquanto o cpf != \0 'buffer', eu continuo verificando se esta tudo certo cada numero      
        if (cpf[i] >= '0' && cpf[i] <= '9'){    //caso esteja, passo para uma variavel temp. para usar ela na verificacao
            cpf_apenas_digito[j] = cpf[i];
            j++;
        }
        i++;
    }
    cpf_apenas_digito[j] = '\0'; // garanto que a string terminou com o \0

    if (strlen (cpf_apenas_digito) != 11){  //utilizo a funcao strlen ver o tamanho do cpf, caso seja diferente de 11, esta errado
        valido = 0;
    }

    //FAZER A VERIFICACAO SE TODOS OS NUMEROS SAO IGUAIS
    if (valido == 1){
        int numerosIguais = 1; //assumo que todos os numeros do cpf sao iguais

        for (int k = 0; k < 11; k++){
            if(cpf_apenas_digito[k] != cpf_apenas_digito[0]){ //caso um numero seja diferente, eu pauso a verificacao
                numerosIguais = 0;
                break;
            }
        }

        if (numerosIguais){
            valido = 0; //todos os numeros iguais eh cpf invalido
        }
    }

    if (valido == 1){
        int primeiro_dv_fornecido = cpf_apenas_digito[9] - '0';
        int segundo_dv_fornecido = cpf_apenas_digito[10] - '0'; //'0' serve para definir o char em numero e efetuar a conta

        char cpf9_digitos_para_calcular[10];            //variavel criada para fazer as operacoes de verificacao
        strcpy(cpf9_digitos_para_calcular, cpf_apenas_digito);      //copiar o cpf e colocar na variavel acima5
        cpf9_digitos_para_calcular[9] = '\0';

        int primeiroDVcalcular = obtem_primeiro_digito_verificador(cpf9_digitos_para_calcular);

        char cpf10_digitos_para_calcular[11];
        strcpy(cpf10_digitos_para_calcular, cpf_apenas_digito);
        cpf10_digitos_para_calcular[9] = (char)(primeiro_dv_fornecido + '0');
        cpf10_digitos_para_calcular[10] = '\0';

        int segundoDVcalcular = obtem_segundo_digito_verificador(cpf10_digitos_para_calcular);

        if (primeiro_dv_fornecido == primeiroDVcalcular && segundo_dv_fornecido == segundoDVcalcular){
            valido = 1; //valido
        } else {
            valido = 0; //invalido
        }
    }

    return (valido);
}


// INICIO FUNCOES RELATORIOS
void valorSacado() {}
void valorSaldoExistente() {}
void QtdCedulasExistentes() {}

// AINDA EM DESENVOLVIMENTO
void ExibirMenuSaque()
{
    inicializar_historico_saque();
    int opcaoSaque;

    printf("\n--- Realizar Saque ---\n");
    printf("Digite o numero da conta corrente (formato 999.999-x): ");
}

// objetivo:Gerenciar o menu de opções do relatorio
//          Permite ao usuário selecionar ações como valor sacado, saldo existente etc
//          Continua exibindo o menu até que o usuário escolha a opção 'Voltar'.
// parametros: nenhum
// retorno:nenhum
void MenuRelatorios(int opcaoRelatorio)
{
    switch (opcaoRelatorio)
    {
    case 1:
        printf("Valor sacado");

        break;
    case 2:
        printf("Valor do saldo existente");
        break;

    case 3:
        printf("Quantidade de cedulas existentes");

        break;

    case 4:
        printf("Voltando ao menu principal...");
        break;

    default:
        printf("Opcao Invalida!\n");
        break;
    }
}

// objetivo:Exibir o menu de Relatorios
// parametros: nenhum
// retorno:nenhum
void ExibirMenuRelatorios()
{
    int opcaoRelatorio;
    do
    {
        printf("\n Relatorios\n");
        printf("1 - Valores sacados\n");
        printf("2 - Valor do saldo existente\n");
        printf("3 - Quantidade de cedulas existentes\n");
        printf("4 - Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcaoRelatorio) != 1)
        {
            while (getchar() != '\n')
                ; // Limpa o buffer
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        MenuRelatorios(opcaoRelatorio);
    } while (opcaoRelatorio != 4);
}
// FIM FUNCOES RELATORIOS

// --- FUNÇÕES AUXILIARES PARA CLIENTES ---

// Função auxiliar para encontrar o índice de um cliente ativo pela conta.
// Retorna o índice se o cliente for encontrado e ativo, -1 caso contrário.
int encontrarClientePorConta(const char *conta)
{
    for (int i = 0; i < MAX_CLIENTES; i++) // Percorre todo o array de clientes
    {
        // Verifica se o cliente está ativo E se a conta corrente corresponde
        if (clientes_ativo[i] == 1 && strcmp(clientes_conta_corrente[i], conta) == 0)
        {
            return i; // Cliente encontrado e ativo no índice 'i'
        }
    }
    return -1; // Cliente não encontrado ou inativo
}

// Função auxiliar para verificar se o CPF ou a conta já existem entre clientes ATIVOS.
// Retorna 1 se for duplicado, 0 caso contrário.
int cliente_duplicado(const char* cpf, const char* conta)
{
    for (int i = 0; i < MAX_CLIENTES; i++) // Percorre todo o array de clientes
    {
        // APENAS verifica clientes ATIVOS
        if (clientes_ativo[i] == 1) // Adicionado: verifica se o cliente está ativo
        {
            if (strcmp(cpf, clientes_cpf[i]) == 0 || strcmp(conta, clientes_conta_corrente[i]) == 0)
            {
                return 1; // Duplicado (CPF ou Conta já existe em cliente ativo)
            }
        }
    }
    return 0; // Não duplicado
}

// Função interna para realizar o processo de cadastro de um único cliente
// É chamada por incluirClientes
void realizarCadastroClienteInterno() { //desenvolver
}


void incluirClientes()// caso o cliente deseja incluir o cadastro chama a funao realizar cadastro cliente
{
    char resposta[TAM_RESPOSTA];

    if (quantidade_clientes >= MAX_CLIENTES) // Primeira verificação de capacidade
    {
        printf("Numero maximo de clientes atingido.\n");
        return;
    }

    printf("Gostaria de incluir um novo cliente? (SIM / NAO)\n");
    scanf(" %s", resposta); // Espaço antes de %s para consumir qualquer newline pendente

    // Converte a resposta para minúsculas
    for (int i = 0; resposta[i]; i++)
    {
        resposta[i] = tolower(resposta[i]);
    }

    if (strcmp(resposta, "sim") == 0)
    {
        realizarCadastroClienteInterno(); // Chama a função que faz o trabalho real de cadastro
    }
    else if (strcmp(resposta, "nao") == 0)
    {
        printf("Cadastro nao realizado.\n");
    }
    else
    {
        printf("Resposta invalida. Por favor, digite 'sim' ou 'nao'.\n");
    }
}

void mostrarClientes() {
    printf("\n--- Lista de Clientes Cadastrados ---\n");

    int clientes_exibidos = 0;
    // Percorre todo o array para encontrar clientes ATIVOS
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        if (clientes_ativo[i] == 1) // APENAS mostra clientes ATIVOS
        {
            printf("Cliente na Posicao %d:\n", i); // Mostrar a posição real do array
            printf("CPF: %s\n", clientes_cpf[i]);
            printf("Conta: %s\n", clientes_conta_corrente[i]);
            printf("Saldo: %.2f\n", clientes_saldo[i]);
            // Adicione outras informações se desejar, como número de saques
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
void alterarClientes() {}
void excluirClientes() {
    char resposta[TAM_RESPOSTA];
    char conta_alvo[TAM_CONTA];

    if (quantidade_clientes == 0)
    {
        printf("Nenhum cliente cadastrado para exclusao.\n");
        return;
    }

    printf("\n--- Excluir Cliente ---\n");
    printf("Digite o numero da conta corrente a ser excluida: ");
    scanf(" %s", conta_alvo);

    int indice_cliente = encontrarClientePorConta(conta_alvo);

    if (indice_cliente == -1)
    {
        printf("Erro: Conta corrente nao encontrada ou ja inativa.\n");
        return;
    }

    // REQUISITO: A exclusão SOMENTE será permitida se não há nenhum saque realizado
    if (clientes_saques_contador[indice_cliente] > 0) {
        printf("Erro: Nao eh possivel excluir cliente com saques realizados.\n");
        return;
    }

    printf("Gostaria de excluir a conta %s (CPF: %s)? (SIM / NAO)\n", clientes_conta_corrente[indice_cliente], clientes_cpf[indice_cliente]);
    scanf(" %s", resposta);

    for (int i = 0; resposta[i]; i++)
    {
        resposta[i] = tolower(resposta[i]);
    }

    if (strcmp(resposta, "sim") == 0)
    {
        // Lógica para a exclusão lógica
        clientes_ativo[indice_cliente] = 0; // Marca como inativo
        printf("Conta excluida com sucesso.\n");
    }
    else if (strcmp(resposta, "nao") == 0)
    {
        printf("Conta nao excluida.\n");
    }
    else
    {
        printf("Resposta invalida. Por favor, digite 'sim' ou 'nao'.\n");
    }
}

// objetivo:Gerenciar o menu de opções do cliente.
//          Permite ao usuário selecionar ações como incluir, mostrar, alterar e excluir clientes.
//          Continua exibindo o menu até que o usuário escolha a opção 'Voltar'.
// parametros: nenhum
// retorno:nenhum
void menuCliente(int opcao_cliente)
{
    switch (opcao_cliente)
    {
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

        printf("Opcao Invalida!\n");
        break;
    }
}

// objetivo:Exibir o menu de opções do cliente.
// parametros: nenhum
// retorno:nenhum
void exibirMenuCliente()
{
    int opcaoCliente;
    do
    {
        printf("\nMenu Cliente\n");
        printf("1 - Incluir\n");
        printf("2 - Mostrar\n");
        printf("3 - Alterar\n");
        printf("4 - Excluir\n");
        printf("5 - Voltar\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcaoCliente) != 1)
        {
            while (getchar() != '\n')
                ; // Limpa o buffer
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        menuCliente(opcaoCliente);
    } while (opcaoCliente != 5);
}







// objetivo:Gerenciar o menu de opções do menu principal
//          Permite ao usuário gerenciar os outros menus
//          Continua exibindo o menu até que o usuário escolha a opção 'finalizar'.
// parametros: nenhum
// retorno:nenhum
void MENU_PRINCIPAL(int opcao_principal)
{
    switch (opcao_principal)
    {
    case 1:
        exibirMenuCliente();

        break;

    case 2:
        ExibirMenuSaque(); // DESENVOLVER - JOTA
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

// objetivo:Exibir o menu de opções do menu principal
// parametros: nenhum
// retorno:nenhum
void exibirMenu()
{
    int opcao_printar;
    do
    {
        printf("\nMENU PRINCIPAL\n");
        printf("1 - Cliente\n");
        printf("2 - Saque\n");
        printf("3 - Relatorios\n");
        printf("4 - Finalizar\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao_printar) != 1)
        {
            while (getchar() != '\n')
                ; // Limpa o buffer
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        MENU_PRINCIPAL(opcao_printar);
    } while (opcao_printar != 4);
}

int main()
{
    srand(time(NULL));
    printf("Bem-Vindo ao Sistema!\n");

   char meu_cpf_gerado[TAM_CPF];
 for (int i = 0; i < 5; i++) {
    gera_cpf_valido(meu_cpf_gerado);
    printf("CPF Gerado %d: %s\n", i + 1, meu_cpf_gerado);
}


    exibirMenu();



    printf("\nSistema Encerrado.\n");
    return 0;
}