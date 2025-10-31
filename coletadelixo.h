#ifndef COLETADELIXO_H
#define COLETADELIXO_H

#include <stdlib.h> // Para size_t
#include <stdio.h>  // Para printf em dump()

// Prot�tipos das fun��es da biblioteca
// ----------------------------------------------------

/**
 * Aloca dinamicamente um bloco de mem�ria de 'tamanho' bytes e inicializa
 * a contagem de refer�ncias para 1.
 * Retorna o endere�o do bloco de dados (o payload).
 */
void* malloc2(size_t tamanho);

/**
 * Realiza uma atribui��o segura de ponteiros: 'endereco' = 'endereco2'.
 * Esta fun��o atualiza as contagens de refer�ncias apropriadamente:
 * 1. Decrementa o contador do bloco que 'endereco' apontava anteriormente.
 * 2. Incrementa o contador do bloco para onde 'endereco2' aponta (se for n�o-NULL).
 * 3. Se um bloco tiver o contador zerado, ele � liberado (garbage collection).
 */
void atrib2(void** endereco, void* endereco2);

/**
 * Fun��o de debug para imprimir o estado atual da mem�ria gerenciada.
 */
void dump();

#endif
