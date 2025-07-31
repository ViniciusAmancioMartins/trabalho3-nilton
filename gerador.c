#include <stdio.h>
#include <stdlib.h>

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

    i=rand()%9;
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
        printf("\nMenu Cliente\n");
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

//objetivo:Gerenciar o menu de opções do cliente.
//         Permite ao usuário selecionar ações como incluir, mostrar, alterar e excluir clientes.
//         Continua exibindo o menu até que o usuário escolha a opção 'Voltar'.
//parametros: nenhum
//retorno:nenhum
void menuCliente (int opcao_cliente){
    switch(opcao_cliente){
    case 1:
        printf("Incluir Cliente\n");

        break;
    
    case 2:
        printf("Mostrar Clientes\n");

        break;
    
    case 3:
        printf("Alterar Cliente\n");

        break;

    case 4:
        printf("Excluir Cliente\n");

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
    printf("Bem-Vindo ao Sistema!\n");
    exibirMenu();

    printf("\nSistema Encerrado.\n");

    return 0;
}