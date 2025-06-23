#ifndef CELULA_HPP
#define CELULA_HPP

#include "tipoItem.hpp"

// Classe que representa uma célula de estrutura encadeada (lista, pilha ou fila)
class TipoCelula {
public:
    TipoCelula();             // Construtor padrão: inicializa ponteiros e item
    TipoItem getItem();       // Retorna o item armazenado na célula

private:
    TipoCelula* prox;         // Ponteiro para a próxima célula da estrutura
    TipoItem item;            // Item armazenado na célula (por exemplo, um pacote com chave/id)

    // As classes abaixo podem acessar membros privados desta classe
    friend class PilhaEncadeada;
    friend class FilaEncadeada;
    friend class ListaEncadeada;
};

#endif