#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP

#include "tipoItem.hpp"
#include "tipoCelula.hpp"

// Fila encadeada simples de TipoItem usando ponteiros para células
class FilaEncadeada {
public:
    FilaEncadeada();            // Construtor: inicializa fila vazia
    virtual ~FilaEncadeada();   // Destrutor: libera memória

    int GetTamanho();           // Retorna o número de elementos na fila
    bool Vazia();               // Retorna true se a fila está vazia
    void Enfileira(TipoItem item); // Adiciona um item ao final da fila
    TipoItem Desenfileira();    // Remove e retorna o item do início da fila
    void Limpa();               // Remove todos os elementos da fila

protected:
    int tamanho;                // Quantidade atual de elementos na fila

private:
    TipoCelula* frente;         // Ponteiro para o primeiro elemento
    TipoCelula* tras;           // Ponteiro para o último elemento
};

// Lista encadeada simples com célula sentinela
class ListaEncadeada {
public:
    ListaEncadeada();           // Construtor: inicializa lista vazia
    ~ListaEncadeada();          // Destrutor: libera memória

    int GetTamanho();           // Retorna o número de elementos na lista
    bool Vazia();               // Retorna true se a lista está vazia

    TipoItem GetItem(int pos);              // Retorna item na posição pos
    void SetItem(TipoItem item, int pos);   // Altera item na posição pos

    void InsereInicio(TipoItem item);       // Insere item no início da lista
    void InsereFinal(TipoItem item);        // Insere item no final da lista
    void InserePosicao(TipoItem item, int pos); // Insere item na posição pos

    TipoItem RemoveInicio();                // Remove e retorna item do início
    TipoItem RemoveFinal();                 // Remove e retorna item do final
    TipoItem RemovePosicao(int pos);        // Remove e retorna item da posição pos

    TipoItem Pesquisa(TipoChave c);         // Pesquisa item por chave

    void Imprime();                         // Imprime todos os itens da lista
    void Limpa();                           // Remove todos os itens da lista

    // Retorna ponteiro para célula na posição 'pos'
    // Se antes == true, retorna célula anterior à posição
    TipoCelula* Posiciona(int pos, bool antes = false);

protected:
    int tamanho;                // Quantidade de elementos na lista

private:
    TipoCelula* primeiro;       // Ponteiro para a célula sentinela (antes do primeiro elemento)
    TipoCelula* ultimo;         // Ponteiro para a última célula da lista
};

// Pilha encadeada usando ponteiros para células
class PilhaEncadeada {
public:
    PilhaEncadeada();           // Construtor: inicializa pilha vazia
    ~PilhaEncadeada();          // Destrutor: libera memória

    void Empilha(TipoItem item);    // Adiciona item ao topo da pilha
    TipoItem Desempilha();         // Remove e retorna o item do topo da pilha
    bool Vazia() const;            // Retorna true se a pilha está vazia
    void Limpa();                  // Remove todos os elementos da pilha

protected:
    int tamanho;                // Quantidade atual de elementos na pilha

private:
    TipoCelula* topo;           // Ponteiro para o elemento no topo da pilha
};

#endif