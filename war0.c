// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[30];
    char corExercito[20];
    int numTropas;
};


// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:

void inicializarTerritorios(struct Territorio* territorio);
// Funções de interface com o usuário:
void exibirMenuPrincipal();
// Funções de lógica principal do jogo:

// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand((unsigned int)time(NULL));
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    struct Territorio* mapa = (struct Territorio*)calloc(5, sizeof(struct Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o mapa.\n");
        return 1;
    }
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    exibirMenuPrincipal();
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(struct Territorio* territorio) {
    printf("\nCadastro de territorios\n");
    for (int i = 0; i < 5; i++) {
        printf("Territorio %d\n", i + 1);
        printf("Nome: ");
        fgets(territorio[i].nome, sizeof(territorio[i].nome), stdin);
        territorio[i].nome[strcspn(territorio[i].nome, "\n")] = 0; // Remove newline

        printf("Cor do exercito: ");
        fgets(territorio[i].corExercito, sizeof(territorio[i].corExercito), stdin);
        territorio[i].corExercito[strcspn(territorio[i].corExercito, "\n")] = 0; // Remove newline

        printf("Numero de tropas: ");
        scanf("%d", &territorio[i].numTropas);
        getchar(); // limpa buffer
    }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
  printf("\n--- MENU PRINCIPAL ---\n");
  printf("1 - Iniciar cadastro de territorios\n");
  printf("2 - Exibir mapa atual\n");
  printf("3 - Iniciar fase de ataque\n");
  printf("4 - Sortear missão\n");

  scanf("Escolha uma opcao: %d", &opcao);
  switch (opcao) {
    case 1:
      inicializarTerritorios(mapa);
      break;
    case 2:
      exibirMapa(mapa);
      break;
    case 3:
      faseDeAtaque(mapa);
      break;
    case 4:
      int idMissao = sortearMissao();
      exibirMissao(idMissao);
      break;
    default:
      printf("Opcao invalida!\n");
      break;
  }
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const struct Territorio* territorio) {
    printf("\n=== MAPA ATUAL DOS TERRITORIOS ===\n");
    printf("%-5s | %-15s | %-15s | %-10s\n", "ID", "NOME", "COR EXERCITO", "TROPAS");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < 5; i++) {
        printf("%-5d | %-15s | %-15s | %-10d\n",
          i, territorio[i].nome, territorio[i].corExercito, territorio[i].numTropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int idMissao) {
    printf("\n=== MISSÃO DO JOGADOR ===\n");
    switch (idMissao) {
        case 1:
            printf("Destruir o exército vermelho!\n");
            break;
        case 2:
            printf("Conquistar 3 territórios inimigos!\n");
            break;
        case 3:
            printf("Conquistar 5 territórios inimigos!\n");
            break;
        default:
            printf("Missão desconhecida!\n");
            break;
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
faseDeAtaque();
// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    /* retorna um valor aleatorio entre 1 e 3 */
    return (rand() % 3) + 1;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
