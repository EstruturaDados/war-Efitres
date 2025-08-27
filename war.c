// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS: 
// 1 - Construção dos territórios seguindo os requisitos funcionais/não funcionais

#include <stdio.h>   // Biblioteca padrão de entrada e saída
#include <string.h>  // Biblioteca de manipulação de strings

// ================================================
//            CADASTRO DE TERRITÓRIOS
// Objetivo: Criar e exibir informações de territórios
// ================================================

// Constantes para tamanho das strings e quantidade de territórios
#define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR 10

// Definição da struct Territorio
// Cada território terá um nome, cor do exército e quantidade de tropas
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

// Função que limpa todo o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
}

int main() {
    // Vetor de 5 territórios
    struct Territorio territ[MAX_TERRITORIOS];
    int i;

    printf("=== CADASTRANDO OS 5 TERRITÓRIOS INICIAIS DO MUNDO ===\n\n");

    // Laço de entrada de dados
    for(i = 0; i < MAX_TERRITORIOS; i++) {
        printf("-- Cadastrando Território -- %d -- \n", i + 1);

        // Entrada do nome do território
        printf("Digite o nome do Território: ");
        fgets(territ[i].nome, TAM_NOME, stdin);
        
        // Remove o \n que o fgets deixa
        territ[i].nome[strcspn(territ[i].nome, "\n")] = '\0';

        // Entrada da cor do exército
        printf("Digite a cor do Exército: ");
        fgets(territ[i].cor, TAM_COR, stdin);
        territ[i].cor[strcspn(territ[i].cor, "\n")] = '\0';

        // Entrada da quantidade de tropas
        printf("Digite a quantidade de Tropas: ");
        scanf("%d", &territ[i].tropas);
        limparBuffer();

        printf("\n"); // Espaço para melhor visualização

    }

    printf("Cadastro inicial finalizado com sucesso!\n\n");

    // Exibição dos territórios cadastrados
    printf("=== MAPA DO MUNDO - ESTADO ATUAL ===\n\n");
    for(i = 0; i < MAX_TERRITORIOS; i++) {
        printf("TERRITÓRIO %d\n", i + 1);
        printf(" * Nome: %s\n", territ[i].nome);
        printf(" * Dominado por: Exército %s\n", territ[i].cor);  // usando o campo cor como "dono"
        printf(" * Tropas: %d\n", territ[i].tropas);
        printf("\n"); // Espaço entre territórios
    }

    //OBSERVAÇÃO em relação a capturar entradas :
    // fgets → melhor para strings, limpa o \n com strcspn.
    // scanf → melhor para números, limpa o buffer com getchar().


    return 0;
}
