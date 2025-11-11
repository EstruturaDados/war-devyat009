#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ====================
// Estrutura dos territórios
// ====================
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ====================
// Protótipos das funções
// ====================
void cadastrarTerritorios(Territorio *mapa, int n);
void exibirTerritorios(const Territorio *mapa, int n);
void atacar(Territorio *atacante, Territorio *defensor);
int escolherTerritorio(const Territorio *mapa, int n, const char *tipo);
void liberarMemoria(Territorio *mapa, char *missao);
void verificarMissao(const Territorio *mapa, int n, const char *missao, int *cumprida);

// ====================
// Função principal
// ====================
int main() {
    srand(time(NULL)); // Garante aleatoriedade

    int n;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);
    getchar(); // limpa buffer

    // Alocação dinâmica para os territórios
    Territorio *mapa = (Territorio *)calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para os territorios!\n");
        return 1;
    }

    // Alocação dinâmica para a missão do jogador
    char *missao = (char *)calloc(100, sizeof(char));
    if (missao == NULL) {
        printf("Erro ao alocar memoria para a missao!\n");
        free(mapa);
        return 1;
    }

    // Cadastra os territórios
    cadastrarTerritorios(mapa, n);

    // Define uma missão simples para o jogador
    strcpy(missao, "Conquistar pelo menos um territorio inimigo!");
    printf("\n=== MISSAO DO JOGADOR ===\n%s\n\n", missao);

    int cumprida = 0; // variável de controle da missão
    char opcao;

    do {
        printf("\n--- MENU ---\n");
        printf("1 - Exibir territorios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);
        getchar();

        switch (opcao) {
            case '1':
                exibirTerritorios(mapa, n);
                break;

            case '2': {
                int idxAtacante = escolherTerritorio(mapa, n, "atacante");
                int idxDefensor = escolherTerritorio(mapa, n, "defensor");

                if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
                    printf("\n❌ Nao e permitido atacar territorios do mesmo exercito!\n");
                    break;
                }

                atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
                exibirTerritorios(mapa, n);

                // Verificação silenciosa da missão
                verificarMissao(mapa, n, missao, &cumprida);
                if (cumprida) {
                    printf("\n🎉 MISSAO CUMPRIDA! Voce conquistou um territorio inimigo!\n");
                    opcao = '0'; // encerra o jogo
                }
                break;
            }

            case '0':
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != '0');

    liberarMemoria(mapa, missao);
    return 0;
}

// ====================
// Funções auxiliares
// ====================

// Cadastra os territórios dinamicamente
void cadastrarTerritorios(Territorio *mapa, int n) {
    printf("\n=== Cadastro de Territorios ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }
}

// Exibe os territórios (passagem por valor, leitura apenas)
void exibirTerritorios(const Territorio *mapa, int n) {
    printf("\n=== ESTADO DOS TERRITORIOS ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d) %-15s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula o ataque (passagem por referência, altera os dados)
void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n=== Ataque: %s (%s) -> %s (%s) ===\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    if (atacante->tropas < 2) {
        printf("❌ O atacante precisa de pelo menos 2 tropas!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("🎲 Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("✅ O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);       // muda o dono do território
        defensor->tropas = atacante->tropas / 2;    // metade das tropas do atacante
        atacante->tropas -= defensor->tropas / 2;   // atacante perde algumas tropas
    } else {
        printf("❌ O defensor resistiu! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// Escolhe o território atacante/defensor
int escolherTerritorio(const Territorio *mapa, int n, const char *tipo) {
    exibirTerritorios(mapa, n);
    int idx;
    printf("\nEscolha o territorio %s (indice): ", tipo);
    scanf("%d", &idx);
    getchar();

    if (idx < 0 || idx >= n) {
        printf("Indice invalido! Usando o primeiro territorio por padrao.\n");
        return 0;
    }
    return idx;
}

// Verifica silenciosamente se a missão foi cumprida
void verificarMissao(const Territorio *mapa, int n, const char *missao, int *cumprida) {
    if (*cumprida) return; // já cumprida, nada a fazer

    // Verifica se há pelo menos dois territórios com a mesma cor (conquistado)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(mapa[i].cor, mapa[j].cor) == 0) {
                *cumprida = 1;
                return;
            }
        }
    }
}

// Libera toda a memória alocada dinamicamente
void liberarMemoria(Territorio *mapa, char *missao) {
    free(mapa);
    free(missao);
    printf("\n🧹 Memoria liberada com sucesso!\n");
}