#include "iostream"
#include "tipoItem.hpp"

// Construtor padrão: inicializa a chave com -1, indicando item inválido ou não inicializado
TipoItem::TipoItem() {
    chave = -1;
}

// Construtor com parâmetro: inicializa a chave com o valor fornecido
TipoItem::TipoItem(TipoChave c) {
    chave = c;
}

// Método setter: define o valor da chave
void TipoItem::SetChave(TipoChave c) {
    chave = c;
}

// Método getter: retorna o valor da chave
TipoChave TipoItem::GetChave() {
    return chave;
}

// Imprime a chave na saída padrão
void TipoItem::Imprime() {
    std::cout << chave;
}