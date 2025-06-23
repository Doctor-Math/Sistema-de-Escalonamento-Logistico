#include "tipoCelula.hpp"

// Construtor da célula
TipoCelula::TipoCelula() {
    item.SetChave(-1);  // Inicializa o item com chave inválida (-1) como marcador de célula vazia ou não inicializada
    prox = nullptr;     // Inicializa o ponteiro para o próximo como nulo (fim da lista por padrão)
}

// Retorna o item armazenado na célula
TipoItem TipoCelula::getItem() {
    return item;
}