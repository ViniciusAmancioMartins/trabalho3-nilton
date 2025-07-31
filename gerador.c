#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
#define TAM_CPF 15
#define TAM_CONTA 11

// --- Para o Historico de Saque (se for por cliente) ---
#define MAX_SAQUES_POR_CLIENTE 10 // Maximo de saques que um cliente pode ter registrado
#define MAX_TRANSACAO_INFO 50    // Tamanho da string para descrever cada transacao.

// --- Para o Estoque de Cedulas do Caixa ---
#define NUM_CEDULAS 8
int valor_cedulas[NUM_CEDULAS] = {1, 2, 5, 10, 20, 50, 250, 450}; // Valores das cedulas
int estoque_cedulas_atuais[NUM_CEDULAS] = {12800, 6400, 3200, 1600, 800, 400, 200, 100}; // Quantidade inicial 

// --- Matrizes para os Dados dos Clientes (Globais) ---
// Cada linha 'i' de cada matriz corresponde ao cliente 'i'
char clientes_cpf[MAX_CLIENTES][TAM_CPF];
char clientes_conta_corrente[MAX_CLIENTES][TAM_CONTA];
double clientes_saldo[MAX_CLIENTES];
int clientes_numero_saques_realizados[MAX_CLIENTES]; // Contador de saques para cada cliente
int clientes_ativo[MAX_CLIENTES]; // 1 para ativo, 0 para inativo (exclusao logica)

// Variável para manter o controle do número atual de clientes ativos
int quantidade_clientes = 0;


// --- Matrizes para o Historico de Saques POR CLIENTE ---
// Historico detalhado por cliente
// 1. Conta corrente       cpf         valor

// Cada cliente_historico_saque[i][j] possui o valor do j saque do cliente i
// E clientes_saques_count[i] possui quantos saques o cliente i já realizou
double clientes_historico_saques_valores[MAX_CLIENTES][MAX_SAQUES_POR_CLIENTE]; //historico dos valores dos saques por clientes
int clientes_saques_contador[MAX_CLIENTES]; // Quantos saques cada cliente realizou (usado como contador de indices)

char clientes_historico_saques_desc[MAX_CLIENTES][MAX_SAQUES_POR_CLIENTE][MAX_TRANSACAO_INFO];


void inicializar_historico_saque (){
    for (int i = 0; i < MAX_CLIENTES; i++){
        clientes_saques_contador[i] = 0;
        for (int j = 0; j < MAX_CLIENTES; i++){
            clientes_historico_saques_valores[i][j] = 0.0;
        }
    }
}



//objetivo:escolhe dentre as letras do alfabeto ('a'..'z') uma letra aleatoriamente
//parametros: nenhum
//retorno:a letra do alfabeto
char geraAlfabeto() {
    int i;
    char letras[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

    i=rand()%26;
    return(letras[i]-32);
}

//objetivo:escolhe dentre os numeros ('0'..'9') uma numero aleatoriamente
//parametros: nenhum
//retorno:o numero
char geraNumero() {
    int i;
    char numeros[] = { '0','1','2','3','4','5','6','7','8','9'};

    i=rand()%9 + 1;
    return(numeros[i]);
}

//objetivo:gera aleatoriamente um numero de conta corrente no formato 999.999-X
//parametros: c onde armazera a conta gerada
//retorno:nenhum
void geraContaCorrente(char c[]) {

   //implemente aqui

}
//objetivo:verifica se um cpf no formato 999.999.999-99 e valido
//parametros: cpf a ser verificado
//retorno: 1 se cpf e valido ou 0 se cpf nao e valido
int verifica_cpf_valido(char cpf[]) {
   int valido=1;

   //implemente aqui

   return(valido);
}

//objetivo:insere pontuacoes '.' e '- ' em um cpf
//parametros: cpf_origem o cpf recebido no format 99999999999
//            cpf_destino o cpf com as pontuacoes inseridas no formato 999.999.999-99
void insere_pontuacao_cpf(char cpf_origem[], char cpf_destino[]) {

   //implemente aqui

}

//objetivo:calcula o primeiro digito verificador de um cpf no formato 999999999
//parametros: cpf o cpf sem os digitos verificadores
//retorno: o calculo do primeiro digito verificador
int obtem_primeiro_digito_verificador(char cpf[]) {
   int digito;

   //implemente aqui

   return(digito);
}

//objetivo:calcula o segundo digito verificador de um cpf no formato 999999999
//parametros: cpf sem os digitos verificadores
//retorno: o calculo do segundo digito verificador
int obtem_segundo_digito_verificador(char cpf[]) {
   int digito;

   //implemente aqui

   return(digito);
}

//objetivo:gera aleatoriamente um cpf valido no formato 999.999.999-99
//parametros: cpf onde sera armazenado o cpf valido
//retorno: nenhum
void gera_cpf_valido(char cpf[]) {

   //implemente aqui

}




// INICIO FUNCOES RELATORIOS
void valorSacado(){}
void valorSaldoExistente(){}
void QtdCedulasExistentes(){}

//AINDA EM DESENVOLVIMENTO
void ExibirMenuSaque(){
    int opcaoSaque;

    printf("\n--- Realizar Saque ---\n");
    printf("Digite o numero da conta corrente (formato 999.999-x): ");
}
//objetivo:Gerenciar o menu de opções do relatorio
//         Permite ao usuário selecionar ações como valor sacado, saldo existente etc
//         Continua exibindo o menu até que o usuário escolha a opção 'Voltar'.
//parametros: nenhum
//retorno:nenhum
void MenuRelatorios (int opcaoRelatorio){
    switch(opcaoRelatorio){
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

//objetivo:Exibir o menu de Relatorios
//parametros: nenhum
//retorno:nenhum
void ExibirMenuRelatorios(){
    int opcaoRelatorio;
    do {
        printf("\n Relatorios\n");
        printf("1 - Valores sacados\n");
        printf("2 - Valor do saldo existente\n");
        printf("3 - Quantidade de cedulas existentes\n");
        printf("4 - Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcaoRelatorio) != 1) {
            while (getchar() != '\n'); // Limpa o buffer
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        MenuRelatorios(opcaoRelatorio);
    } while(opcaoRelatorio != 4);
}
//FIM FUNCOES RELATORIOS




void incluirClientes (){

}
void mostrarClientes(){}
void alterarClientes(){}
void excluirClientes(){}

//objetivo:Gerenciar o menu de opções do cliente.
//         Permite ao usuário selecionar ações como incluir, mostrar, alterar e excluir clientes.
//         Continua exibindo o menu até que o usuário escolha a opção 'Voltar'.
//parametros: nenhum
//retorno:nenhum
void menuCliente (int opcao_cliente){
    switch(opcao_cliente){
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



//objetivo:Exibir o menu de opções do cliente.
//parametros: nenhum
//retorno:nenhum
void exibirMenuCliente(){
    int opcaoCliente;
    do {
        printf("\nMenu Cliente\n");
        printf("1 - Incluir\n");
        printf("2 - Mostrar\n");
        printf("3 - Alterar\n");
        printf("4 - Excluir\n");
        printf("5 - Voltar\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcaoCliente) != 1) {
            while (getchar() != '\n'); // Limpa o buffer
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        menuCliente(opcaoCliente);
    } while(opcaoCliente != 5);
}








//objetivo:Gerenciar o menu de opções do menu principal
//         Permite ao usuário gerenciar os outros menus
//         Continua exibindo o menu até que o usuário escolha a opção 'finalizar'.
//parametros: nenhum
//retorno:nenhum
void MENU_PRINCIPAL(int opcao_principal) {
    switch (opcao_principal){
    case 1:
        exibirMenuCliente();

        break;


    case 2:
        ExibirMenuSaque(); //DESENVOLVER - JOTA
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

//objetivo:Exibir o menu de opções do menu principal
//parametros: nenhum
//retorno:nenhum
void exibirMenu (){
    int opcao_printar;
    do {
        printf("\nMENU PRINCIPAL\n");
        printf("1 - Cliente\n");
        printf("2 - Saque\n");
        printf("3 - Relatorios\n");
        printf("4 - Finalizar\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao_printar) != 1) {
            while (getchar() != '\n'); // Limpa o buffer
            printf("Entrada invalida! Tente novamente.\n");
            continue;
        }
        MENU_PRINCIPAL(opcao_printar);
    } while (opcao_printar != 4);
}






int main (){
    
    srand(time(NULL));
    printf("Bem-Vindo ao Sistema!\n");
    exibirMenu();

    printf("\nSistema Encerrado.\n");





    return 0;
}