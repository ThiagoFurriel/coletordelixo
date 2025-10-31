#include "coletadelixo.h"
#include <string.h> // Para memcpy

// --- ESTRUTURA DE METADADOS ---
typedef struct BlocoMemoria {
    size_t tamanho;
    int contador_referencias;
    struct BlocoMemoria* proximo; // Para a lista encadeada
    // O bloco de dados (payload) come�a IMEDIATAMENTE ap�s esta estrutura.
} BlocoMemoria;

// --- LISTA ENCADEADA DE BLOCOS ALOCADOS ---
// O ponteiro 'lista_blocos' aponta para o primeiro bloco alocado
static BlocoMemoria* lista_blocos = NULL;


// --- FUN��ES AUXILIARES ---

/**
 * Retorna o endere�o do BlocoMemoria (os metadados) dado o endere�o do payload.
 */
static BlocoMemoria* get_bloco_memoria(void* payload) {
    if (payload == NULL) {
        return NULL;
    }
    // O payload � o que � retornado por malloc2.
    // O metadado BlocoMemoria est� logo ANTES do payload.
    return (BlocoMemoria*)((char*)payload - sizeof(BlocoMemoria));
}

/**
 * Remove o bloco da lista encadeada e o libera (chama free()).
 */
static void liberar_bloco(BlocoMemoria* bloco_a_liberar) {
    BlocoMemoria* atual = lista_blocos;
    BlocoMemoria* anterior = NULL;

    while (atual != NULL && atual != bloco_a_liberar) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == bloco_a_liberar) {
        if (anterior == NULL) {
            // � o primeiro bloco da lista
            lista_blocos = atual->proximo;
        } else {
            // Est� no meio ou fim
            anterior->proximo = atual->proximo;
        }

        // Finalmente, libera a mem�ria real. O ponteiro � o do BlocoMemoria.
        free(bloco_a_liberar);
        printf("<<< GC: Bloco liberado em %p >>>\n", bloco_a_liberar);
    }
}

/**
 * Fun��o principal de decremento com verifica��o de GC.
 */
static void decrementar_referencia(void* endereco) {
    BlocoMemoria* bloco = get_bloco_memoria(endereco);
    if (bloco != NULL) {
        bloco->contador_referencias--;
        printf("[DECREMENTO: %p, CR: %d]\n", endereco, bloco->contador_referencias);

        if (bloco->contador_referencias == 0) {
            liberar_bloco(bloco);
        }
    }
}

/**
 * Fun��o principal de incremento.
 */
static void incrementar_referencia(void* endereco) {
    BlocoMemoria* bloco = get_bloco_memoria(endereco);
    if (bloco != NULL) {
        bloco->contador_referencias++;
        printf("[INCREMENTO: %p, CR: %d]\n", endereco, bloco->contador_referencias);
    }
}

// --- IMPLEMENTA��O DAS FUN��ES P�BLICAS ---

void* malloc2(size_t tamanho) {
    // Aloca espa�o para os metadados (BlocoMemoria) + o tamanho solicitado (payload).
    BlocoMemoria* novo_bloco = (BlocoMemoria*)malloc(sizeof(BlocoMemoria) + tamanho);

    if (novo_bloco == NULL) {
        fprintf(stderr, "Erro de alocacao em malloc2!\n");
        return NULL;
    }

    // Inicializa os metadados
    novo_bloco->tamanho = tamanho;
    novo_bloco->contador_referencias = 1; // Inicialmente, o ponteiro de retorno aponta para ele

    // Adiciona o novo bloco no IN�CIO da lista encadeada (mais r�pido)
    novo_bloco->proximo = lista_blocos;
    lista_blocos = novo_bloco;

    // Retorna o endere�o do PAYLOAD (a �rea de dados, que vem depois da estrutura)
    void* payload = (void*)((char*)novo_bloco + sizeof(BlocoMemoria));
    printf("[MALLOC2: Alocou %zu bytes em %p]\n", tamanho, payload);
    return payload;
}

void atrib2(void** endereco, void* endereco2) {
    // 1. Decrementar o contador do bloco que *endereco apontava ANTES da atribui��o
    if (*endereco != NULL) {
        decrementar_referencia(*endereco);
    }

    // 2. Realizar a atribui��o de ponteiros
    *endereco = endereco2;

    // 3. Incrementar o contador do bloco que *endereco AGORA aponta
    if (endereco2 != NULL) {
        incrementar_referencia(endereco2);
    }
}

void dump() {
    printf("\n--- ESTADO ATUAL DA MEMORIA GERENCIADA ---\n");
    if (lista_blocos == NULL) {
        printf("Nenhum bloco de memoria alocado.\n");
        printf("----------------------------------------\n");
        return;
    }

    BlocoMemoria* atual = lista_blocos;
    while (atual != NULL) {
        void* payload = (void*)((char*)atual + sizeof(BlocoMemoria));
        printf("ENDERECO: %p | TAMANHO: %zu | CR: %d | VALOR: ",
               payload, atual->tamanho, atual->contador_referencias);

        // Tentativa de imprimir o valor (supondo tipos b�sicos)
        if (atual->tamanho == sizeof(int)) {
            printf("%d (int)\n", *(int*)payload);
        } else if (atual->tamanho == sizeof(char)) {
            printf("%c (char)\n", *(char*)payload);
        } else {
             printf("[Desconhecido]\n");
        }

        atual = atual->proximo;
    }
    printf("----------------------------------------\n");
}
