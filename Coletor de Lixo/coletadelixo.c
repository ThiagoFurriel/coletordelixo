#include "coletadelixo.h"
#include <string.h>

typedef struct BlocoMemoria {
    size_t tamanho;
    int contador_referencias;
    struct BlocoMemoria* proximo;
} BlocoMemoria;

static BlocoMemoria* lista_blocos = NULL;


static BlocoMemoria* get_bloco_memoria(void* payload) {
    if (payload == NULL) {
        return NULL;
    }
    return (BlocoMemoria*)((char*)payload - sizeof(BlocoMemoria));
}

static void liberar_bloco(BlocoMemoria* bloco_a_liberar) {
    BlocoMemoria* atual = lista_blocos;
    BlocoMemoria* anterior = NULL;

    while (atual != NULL && atual != bloco_a_liberar) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == bloco_a_liberar) {
        if (anterior == NULL) {

            lista_blocos = atual->proximo;
        } else {

            anterior->proximo = atual->proximo;
        }


        free(bloco_a_liberar);
        printf("Coletor de lixo: Bloco liberado em %p \n", bloco_a_liberar);
    }
}


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


static void incrementar_referencia(void* endereco) {
    BlocoMemoria* bloco = get_bloco_memoria(endereco);
    if (bloco != NULL) {
        bloco->contador_referencias++;
        printf("[INCREMENTO: %p, CR: %d]\n", endereco, bloco->contador_referencias);
    }
}



void* malloc2(size_t tamanho) {
    BlocoMemoria* novo_bloco = (BlocoMemoria*)malloc(sizeof(BlocoMemoria) + tamanho);

    if (novo_bloco == NULL) {
        fprintf(stderr, "Erro de alocacao em malloc2!\n");
        return NULL;
    }


    novo_bloco->tamanho = tamanho;
    novo_bloco->contador_referencias = 1;

    novo_bloco->proximo = lista_blocos;
    lista_blocos = novo_bloco;


    void* payload = (void*)((char*)novo_bloco + sizeof(BlocoMemoria));
    printf("[MALLOC2: Alocou %zu bytes em %p]\n", tamanho, payload);
    return payload;
}

void atrib2(void** endereco, void* endereco2) {

    if (*endereco != NULL) {
        decrementar_referencia(*endereco);
    }


    *endereco = endereco2;


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
