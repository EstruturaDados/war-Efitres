// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS: 
// (NIVEL NOVATO) 1 - Construção dos territórios seguindo os requisitos funcionais/não funcionais

// (NIVEL AVENTUREIRO) 2 - Implementar a funcionalidade de ataque entre territórios. Com base nos territórios já cadastrados, 
                        //o sistema deverá permitir que um jogador selecione um território como atacante, e outro como defensor.
                        // O ataque será resolvido por meio de uma simulação com dados aleatórios (como rolagem de dados),
                        // e o resultado alterará o controle e as tropas do território atacado. 
                        // Essa etapa deve utilizar ponteiros para manipular os dados dos territórios
                        // e a alocação dinâmica de memória para armazenar os territórios cadastrados, fornecendo maior
                        // flexibilidade ao sistema.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================================
// Estrutura que representa um território
// ============================================================================
typedef struct {
    char nome[30];  // Nome do território
    char cor[10];   // Cor do exército/dono do território
    int tropas;     // Número de tropas presentes
} Territorio;

// ============================================================================
// Limpa o buffer do teclado para evitar entradas inválidas ou restos de '\n'
// ============================================================================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================================================
// Função para cadastrar territórios dinamicamente
// Recebe um vetor de territórios e a quantidade total a ser cadastrada
// ============================================================================
void cadastrarTerritorios(Territorio* territorios, int totalTerritorios) {
    for (int i = 0; i < totalTerritorios; i++) {
        printf("\n--- Cadastro do território %d ---\n", i + 1);

        printf("Nome: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // Remove '\n'

        printf("Cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBuffer();
    }
}

// ============================================================================
// Função para exibir todos os territórios cadastrados
// ============================================================================
void exibirTerritorios(Territorio* territorios, int totalTerritorios) {
    printf("\n=== Mapa do Mundo Atual ===\n\n");
    for (int i = 0; i < totalTerritorios; i++) {
        printf("%d. %s - ( Exército: %s , Tropas: %d ) \n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("\n****** HORA DO ATAQUE !!! ******\n");
}

// ============================================================================
// Função que realiza um ataque entre dois territórios
// Recebe ponteiros para o atacante e defensor para modificar diretamente os dados
// ============================================================================
void atacar(Territorio* atacante, Territorio* defensor) {
    // Verifica se não é do mesmo exército
    if (strcmp((*atacante).cor, (*defensor).cor) == 0) {
        printf("\nNão é permitido atacar um território do mesmo exército!\n");
        return;
    }

    // Simula a rolagem de dados (1 a 6) para atacante e defensor
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nRolagem de dados:\n");
    printf("Atacante (%s): %d | Defensor (%s): %d\n",
           (*atacante).nome, dadoAtacante, (*defensor).nome, dadoDefensor);

    // Mensagens que serão exibidas após a batalha
    char mensagemResultado[100] = "";
    char mensagemConquista[100] = "";

    // Caso o atacante vença
    if (dadoAtacante > dadoDefensor) {
        (*defensor).tropas--; // Reduz 1 tropa do defensor
        sprintf(mensagemResultado, "VITORIA DO ATACANTE! O DEFENSOR perdeu 1 tropa.");

        // Verifica se o defensor ficou sem tropas
        if ((*defensor).tropas <= 0) {
            strcpy((*defensor).cor, (*atacante).cor); // Defensor conquistado
            (*defensor).tropas = 1; // Inicializa com 1 tropa
            sprintf(mensagemConquista, "CONQUISTA! O território %s foi dominado pelo %s.",
                    (*defensor).nome, (*atacante).nome);
        }

    } else { // Caso o defensor vença
        (*atacante).tropas--; // Atacante perde 1 tropa
        sprintf(mensagemResultado, "VITORIA DO DEFENSOR! O ATACANTE perdeu 1 tropa.");

        // Verifica se o atacante ficou sem tropas
        if ((*atacante).tropas <= 0) {
            strcpy((*atacante).cor, (*defensor).cor); // Atacante conquistado
            (*atacante).tropas = 1; // Inicializa com 1 tropa
            sprintf(mensagemConquista, "CONQUISTA! O território %s foi dominado pelo %s.",
                    (*atacante).nome, (*defensor).nome);
        }
    }

    // Exibe mensagens de resultado e conquista
    printf("\n%s\n", mensagemResultado);
    if (strlen(mensagemConquista) > 0) {
        printf("%s\n", mensagemConquista);
    }
}

// ============================================================================
// Função para liberar memória alocada dinamicamente
// ============================================================================
void liberarMemoria(Territorio* territorios) {
    free(territorios);
}

// ============================================================================
// Função principal
// ============================================================================
int main() {
    srand(time(NULL)); // Inicializa aleatoriedade

    int totalTerritorios;
    printf("=================================\n");
    printf("===WAR ESTRUTURADO - CADASTRO ===\n");
    printf("=================================\n\n");
    printf("Digite o número de territórios: ");
    scanf("%d", &totalTerritorios);
    limparBuffer();

    // Aloca dinamicamente o vetor de territórios
    Territorio* territorios = (Territorio*) calloc(totalTerritorios, sizeof(Territorio));
    if (!territorios) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(territorios, totalTerritorios);
    exibirTerritorios(territorios, totalTerritorios);

    // Loop de ataques
    char opcao;
    do {
        int indiceAtacante, indiceDefensor;

        // Solicita território atacante
        printf("\nEscolha o território atacante (1-%d ou 0 para sair): ", totalTerritorios);
        scanf("%d", &indiceAtacante);
        limparBuffer();
        if (indiceAtacante == 0) break;

        // Solicita território defensor
        printf("Escolha o território defensor (1-%d): ", totalTerritorios);
        scanf("%d", &indiceDefensor);
        limparBuffer();

        // Verifica escolha válida e realiza ataque
        if (indiceAtacante >= 1 && indiceAtacante <= totalTerritorios &&
            indiceDefensor >= 1 && indiceDefensor <= totalTerritorios) {
            atacar(&territorios[indiceAtacante - 1], &territorios[indiceDefensor - 1]);
            exibirTerritorios(territorios, totalTerritorios);
        } else {
            printf("\nEscolha inválida!\n");
        }

        // Pergunta se o jogador deseja realizar outro ataque
        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &opcao);
        limparBuffer();
    } while (opcao == 's' || opcao == 'S');

    // Libera memória e encerra o programa
    liberarMemoria(territorios);
    printf("\nFim do jogo!\n");

    return 0;
}
