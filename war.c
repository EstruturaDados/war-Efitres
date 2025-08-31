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
// (NIVEL MESTRE) 3 - implementar a funcionalidade de missões estratégicas individuais para cada jogador,
                    // que deverá receber, no início do jogo, uma missão sorteada de forma automática entre diversas
                    // descrições pré-definidas, armazenadas em um vetor de strings. Essa missão será consultada durante
                    // o jogo para verificar se a condição de vitória foi atingida. A nova camada de estratégia exige
                    // organização modular do código, uso de ponteiros, passagem de parâmetros por valor e referência
                    // e gerenciamento adequado da memória.

#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc, free e exit
#include <string.h> // Para strlen, strcpy, strcspn
#include <time.h>   // Para srand e rand

// ================================
// Constantes
// ================================
#define MAX_TERRITORIOS 50
#define MAX_MISSOES 5
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MENSAGEM 100
#define TOTAL_JOGADORES 2

// ================================
// Estruturas
// ================================

// Estrutura para armazenar dados de um território
typedef struct {
    char nome[MAX_NOME];  // Nome do território
    char cor[MAX_COR];    // Cor do exército/dono
    int tropas;           // Número de tropas presentes
} Territorio;

// Estrutura para armazenar dados de um jogador
typedef struct {
    char* descricao;      // Missão do jogador, alocada dinamicamente
} Jogador;

// ================================
// Protótipos de funções
// ================================
void limparBuffer();
int lerTotalTerritorios();
Territorio* criarTerritorios(int totalTerritorios);
void exibirTerritorios(Territorio* territorios, int totalTerritorios);
void exibirMissoes(Jogador* jogadores, int totalJogadores, Territorio* territorios, int totalTerritorios);
void atacar(Territorio* atacante, Territorio* defensor);
Jogador* criarJogadores(int totalJogadores);
void criarMissoesParaJogadores(Jogador* jogadores, int totalJogadores);
int verificarMissao(Jogador jogador, Territorio* territorios, int totalTerritorios);
void jogar(Territorio* territorios, int totalTerritorios, Jogador* jogadores, int totalJogadores);
void liberarMemoria(Territorio* territorios, Jogador* jogadores, int totalJogadores);

// ================================
// Função principal
// ================================
int main() {
    srand(time(NULL)); // Inicializa a semente aleatória para rand()

    // Lê a quantidade de territórios do usuário
    int totalTerritorios = lerTotalTerritorios();

    // Cria dinamicamente o vetor de territórios
    Territorio* territorios = criarTerritorios(totalTerritorios);

    // Cria jogadores dinamicamente
    Jogador* jogadores = criarJogadores(TOTAL_JOGADORES);

    // Atribui missões aleatórias para cada jogador
    criarMissoesParaJogadores(jogadores, TOTAL_JOGADORES);

    // Exibe o mapa inicial
    exibirTerritorios(territorios, totalTerritorios);

    // Exibe status das missões
    exibirMissoes(jogadores, TOTAL_JOGADORES, territorios, totalTerritorios);

    // Loop de ataques e verificação de missões
    jogar(territorios, totalTerritorios, jogadores, TOTAL_JOGADORES);

    // Libera toda a memória alocada dinamicamente
    liberarMemoria(territorios, jogadores, TOTAL_JOGADORES);

    printf("\nFim do jogo!\n");
    return 0;
}

// ================================
// Implementação das funções
// ================================

// Limpa o buffer do teclado para evitar problemas com fgets ou scanf
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Lê do usuário o número de territórios
int lerTotalTerritorios() {
    int total;
    printf("============================================\n");
    printf("========== WAR ESTRUTURADO - MESTRE ========\n");
    printf("============================================\n\n");
    printf("Digite o numero de territorios: ");
    scanf("%d", &total);
    limparBuffer(); 
    return total;
}

// ================================
// Função que cria territórios dinamicamente
// ================================
Territorio* criarTerritorios(int totalTerritorios) {
    // calloc aloca memória e já inicializa com 0
    Territorio* territorios = (Territorio*) calloc(totalTerritorios, sizeof(Territorio));
    if (!territorios) {
        // Se calloc falhar, exit encerra o programa imediatamente
        printf("Erro: Falha ao alocar memoria para territorios.\n");
        exit(1);
    }

    Territorio* t = territorios; // ponteiro auxiliar para percorrer o vetor

    for (int i = 0; i < totalTerritorios; i++, t++) {
        printf("\n--- Cadastro do território %d ---\n", i + 1);

        printf("Nome: ");
        fgets((*t).nome, MAX_NOME, stdin);
        // remove o \n do final, substituindo por \0
        (*t).nome[strcspn((*t).nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        fgets((*t).cor, MAX_COR, stdin);
        (*t).cor[strcspn((*t).cor, "\n")] = '\0';

        printf("Numero de tropas: ");
        scanf("%d", &(*t).tropas);
        limparBuffer();
    }

    return territorios; // retorna ponteiro inicial
}

// ================================
// Exibe territórios
// ================================
void exibirTerritorios(Territorio* territorios, int totalTerritorios) {
    printf("\n=== Mapa do Mundo Atual ===\n\n");
    Territorio* t = territorios;
    for (int i = 0; i < totalTerritorios; i++, t++) {
        printf("%d. %s - (Exercito: %s, Tropas: %d)\n",
               i + 1, (*t).nome, (*t).cor, (*t).tropas);
    }
    printf("\n****** HORA DO ATAQUE !!! ******\n");
}

// ================================
// Exibe missões dos jogadores
// ================================
void exibirMissoes(Jogador* jogadores, int totalJogadores, Territorio* territorios, int totalTerritorios) {
    printf("\n=== Status das Missoes ===\n");
    Jogador* j = jogadores;
    for (int i = 0; i < totalJogadores; i++, j++) {
        int completa = verificarMissao(*j, territorios, totalTerritorios);
        printf("Missao do jogador %d: %s - [%s]\n",
               i + 1, (*j).descricao, completa ? "COMPLETA" : "INCOMPLETA");
    }
}

// ================================
// Função de ataque entre territórios
// ================================
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp((*atacante).cor, (*defensor).cor) == 0) {
        printf("\nNao é permitido atacar um territorio do mesmo exercito!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nRolagem de dados:\n");
    printf("Atacante (%s): %d | Defensor (%s): %d\n",
           (*atacante).nome, dadoAtacante, (*defensor).nome, dadoDefensor);

    char mensagemResultado[MAX_MENSAGEM] = "";
    char mensagemConquista[MAX_MENSAGEM] = "";

    if (dadoAtacante > dadoDefensor) {
        (*defensor).tropas--;
        sprintf(mensagemResultado, "VITORIA DO ATACANTE! O DEFENSOR perdeu 1 tropa.");
        if ((*defensor).tropas <= 0) {
            // strcpy copia string de um lugar para outro
            strcpy((*defensor).cor, (*atacante).cor);
            (*defensor).tropas = 1;
            sprintf(mensagemConquista, "CONQUISTA! O território %s foi dominado pelo %s.",
                    (*defensor).nome, (*atacante).nome);
        }
    } else {
        (*atacante).tropas--;
        sprintf(mensagemResultado, "VITORIA DO DEFENSOR! O ATACANTE perdeu 1 tropa.");
        if ((*atacante).tropas <= 0) {
            strcpy((*atacante).cor, (*defensor).cor);
            (*atacante).tropas = 1;
            sprintf(mensagemConquista, "CONQUISTA! O território %s foi dominado pelo %s.",
                    (*atacante).nome, (*defensor).nome);
        }
    }

    printf("\n%s\n", mensagemResultado);
    if (strlen(mensagemConquista) > 0) {
        printf("%s\n", mensagemConquista);
    }
}

// ================================
// Cria jogadores dinamicamente
// ================================
Jogador* criarJogadores(int totalJogadores) {
    // malloc apenas aloca memória sem inicializar
    Jogador* jogadores = (Jogador*) malloc(totalJogadores * sizeof(Jogador));
    if (!jogadores) exit(1); // termina se malloc falhar
    return jogadores;
}

// ================================
// Cria missões para jogadores
// ================================
void criarMissoesParaJogadores(Jogador* jogadores, int totalJogadores) {
    char* missoes[MAX_MISSOES] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Dominar territorios pares do mapa",
        "Acumular 10 tropas em um territorio",
        "Possuir territorios de todas as cores"
    };

    Jogador* j = jogadores;
    for (int i = 0; i < totalJogadores; i++, j++) {
        int idx = rand() % MAX_MISSOES;
        // malloc para armazenar string dinamicamente
        (*j).descricao = (char*) malloc(strlen(missoes[idx]) + 1);
        strcpy((*j).descricao, missoes[idx]); // copia a string
    }
}

// ================================
// Função de verificação de missão
// ================================
int verificarMissao(Jogador jogador, Territorio* territorios, int totalTerritorios) {
    // Missão 1: Eliminar todas as tropas da cor vermelha
    if (strcmp(jogador.descricao, "Eliminar todas as tropas da cor vermelha") == 0) {
        for (int i = 0; i < totalTerritorios; i++)
            if (strcmp((territorios + i)->cor, "vermelha") == 0)
                return 0;
        return 1;
    }

    // Missão 2: Conquistar 3 territórios seguidos (mesma cor não neutra)
    if (strcmp(jogador.descricao, "Conquistar 3 territorios seguidos") == 0) {
        int count = 1;
        for (int i = 1; i < totalTerritorios; i++) {
            if (strcmp((territorios + i)->cor, (territorios + i - 1)->cor) == 0 &&
                strcmp((territorios + i)->cor, "neutro") != 0) {
                count++;
            } else {
                count = 1;
            }
            if (count >= 3) return 1;
        }
        return 0;
    }

    // Missão 3: Dominar territórios pares do mapa (mesma cor dos anteriores)
    if (strcmp(jogador.descricao, "Dominar territorios pares do mapa") == 0) {
        for (int i = 1; i < totalTerritorios; i += 2) {
            if (strcmp((territorios + i)->cor, (territorios + i - 1)->cor) != 0)
                return 0;
        }
        return 1;
    }

    // Missão 4: Acumular 10 tropas em um território
    if (strcmp(jogador.descricao, "Acumular 10 tropas em um territorio") == 0) {
        for (int i = 0; i < totalTerritorios; i++)
            if ((territorios + i)->tropas >= 10)
                return 1;
        return 0;
    }

    // Missão 5: Possuir territórios de todas as cores
    if (strcmp(jogador.descricao, "Possuir territorios de todas as cores") == 0) {
        int coresEncontradas[5] = {0}; // vermelha, azul, verde, amarela, neutro
        for (int i = 0; i < totalTerritorios; i++) {
            char* cor = (territorios + i)->cor;
            if (strcmp(cor, "vermelha") == 0) coresEncontradas[0] = 1;
            else if (strcmp(cor, "azul") == 0) coresEncontradas[1] = 1;
            else if (strcmp(cor, "verde") == 0) coresEncontradas[2] = 1;
            else if (strcmp(cor, "amarela") == 0) coresEncontradas[3] = 1;
            else if (strcmp(cor, "neutro") == 0) coresEncontradas[4] = 1;
        }
        for (int i = 0; i < 5; i++)
            if (!coresEncontradas[i]) return 0;
        return 1;
    }

    return 0; // nenhuma missão cumprida
}

// ================================
// Loop de ataques
// ================================
void jogar(Territorio* territorios, int totalTerritorios, Jogador* jogadores, int totalJogadores) {
    char opcao;
    do {
        int indiceAtacante, indiceDefensor;

        printf("\nEscolha o territorio atacante (1-%d ou 0 para sair): ", totalTerritorios);
        scanf("%d", &indiceAtacante);
        limparBuffer();
        if (indiceAtacante == 0) break;

        printf("Escolha o territorio defensor (1-%d): ", totalTerritorios);
        scanf("%d", &indiceDefensor);
        limparBuffer();

        if (indiceAtacante >= 1 && indiceAtacante <= totalTerritorios &&
            indiceDefensor >= 1 && indiceDefensor <= totalTerritorios) {
            atacar(territorios + (indiceAtacante - 1), territorios + (indiceDefensor - 1));
        }
        exibirTerritorios(territorios, totalTerritorios);

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &opcao);
        limparBuffer();
    } while (opcao == 's' || opcao == 'S');
}

// ================================
// Libera memória
// ================================
void liberarMemoria(Territorio* territorios, Jogador* jogadores, int totalJogadores) {
    free(territorios);
    Jogador* j = jogadores;
    for (int i = 0; i < totalJogadores; i++, j++) free((*j).descricao);
    free(jogadores);
}
