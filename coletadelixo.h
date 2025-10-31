#ifndef COLETADELIXO_H
#define COLETADELIXO_H

#include <stdlib.h> // Para size_t
#include <stdio.h>  // Para printf em dump()

// Protótipos das funções da biblioteca
// ----------------------------------------------------

/**
 * Aloca dinamicamente um bloco de memória de 'tamanho' bytes e inicializa
 * a contagem de referências para 1.
 * Retorna o endereço do bloco de dados (o payload).
 */
void* malloc2(size_t tamanho);

/**
 * Realiza uma atribuição segura de ponteiros: 'endereco' = 'endereco2'.
 * Esta função atualiza as contagens de referências apropriadamente:
 * 1. Decrementa o contador do bloco que 'endereco' apontava anteriormente.
 * 2. Incrementa o contador do bloco para onde 'endereco2' aponta (se for não-NULL).
 * 3. Se um bloco tiver o contador zerado, ele é liberado (garbage collection).
 */
void atrib2(void** endereco, void* endereco2);

/**
 * Função de debug para imprimir o estado atual da memória gerenciada.
 */
void dump();

#endif
