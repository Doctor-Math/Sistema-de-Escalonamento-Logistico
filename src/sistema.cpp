#include "sistema.hpp"
#include "pacote_armazem.hpp"

// Construtor
Sistema::Sistema(int cap) : capacidade(cap), quantidade(0) {
    armazens = new Armazem*[capacidade];
    for (int i = 0; i < capacidade; i++) {
        armazens[i] = nullptr;
    }
}

// Destrutor
Sistema::~Sistema() {
    for (int i = 0; i < quantidade; i++) {
        delete armazens[i]; // Armazem se encarrega de deletar seus vizinhos
    }
    delete[] armazens;
}

// Redimensiona a estrutura para o dobro da capacidade atual
void Sistema::redimensionar() {
    int novaCapacidade = capacidade * 2;
    Armazem** novo = new Armazem*[novaCapacidade];

    for (int i = 0; i < quantidade; i++) {
        novo[i] = armazens[i];
    }

    // Inicializa os novos espaços como nullptr
    for (int i = quantidade; i < novaCapacidade; i++) {
        novo[i] = nullptr;
    }

    delete[] armazens;
    armazens = novo;
    capacidade = novaCapacidade;
}

// Adiciona um novo armazém ao sistema
int Sistema::AdicionarArmazem() {
    if (quantidade == capacidade) {
        redimensionar();
    }

    Armazem* a = new Armazem();
    a->setIndice(quantidade);
    a->listaVizinhos = nullptr;

    armazens[quantidade] = a;

    return quantidade++;
}

// Adiciona uma aresta entre os armazéns v e w (ligação bidirecional)
void Sistema::AdicionarAresta(int v, int w) {
    // Verifica se os índices são válidos
    if (v < 0 || w < 0 || v >= quantidade || w >= quantidade ||
        armazens[v] == nullptr || armazens[w] == nullptr) {
        return;
    }

    // Se já existe aresta, não adiciona novamente
    if (ExisteAresta(v, w)) {
        return;
    }

    // Adiciona aresta v->w
    ArmazemVizinho* novoVizinho = new ArmazemVizinho();
    novoVizinho->representante = armazens[w];
    novoVizinho->prox = armazens[v]->listaVizinhos;
    armazens[v]->listaVizinhos = novoVizinho;

    // Adiciona aresta w->v
    novoVizinho = new ArmazemVizinho();
    novoVizinho->representante = armazens[v];
    novoVizinho->prox = armazens[w]->listaVizinhos;
    armazens[w]->listaVizinhos = novoVizinho;
}

// Verifica se existe uma aresta entre os armazéns v e w
bool Sistema::ExisteAresta(int v, int w) const {
    if (v < 0 || w < 0 || v >= quantidade || w >= quantidade ||
        armazens[v] == nullptr || armazens[w] == nullptr) {
        return false;
    }

    ArmazemVizinho* atual = armazens[v]->listaVizinhos;
    while (atual) {
        if (atual->representante == armazens[w]) {
            return true; // Já existe aresta entre v e w
        }
        atual = atual->prox;
    }
    return false; // Aresta não existe
}

// Retorna a capacidade atual do sistema
int Sistema::GetCapacidade() const {
    return capacidade;
}