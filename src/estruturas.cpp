#include <stdexcept>
#include <iostream>
#include "estruturas.hpp"

// -------------------- Métodos da classe FilaEncadeada --------------------------

FilaEncadeada::FilaEncadeada() {
    frente = new TipoCelula();  // Cria célula sentinela (não contém item válido)
    tras = frente;              // No início, frente e trás apontam para a sentinela
    tamanho = 0;
}

FilaEncadeada::~FilaEncadeada() {
    Limpa();        // Remove todos os elementos da fila
    delete frente;  // Libera a célula sentinela
}

int FilaEncadeada::GetTamanho() {
    return tamanho;
}

bool FilaEncadeada::Vazia() {
    return tamanho == 0;
}

void FilaEncadeada::Enfileira(TipoItem item) {
    TipoCelula* nova = new TipoCelula();
    nova->item = item;
    nova->prox = nullptr;   // Nova célula será o fim da fila
    tras->prox = nova;      // A antiga última célula aponta para a nova
    tras = nova;            // Atualiza o ponteiro 'tras'
    tamanho++;
}

TipoItem FilaEncadeada::Desenfileira() {
    if (Vazia()) {
        throw std::runtime_error("Fila está vazia!");
    }

    TipoCelula* p = frente->prox;  // Primeira célula válida
    TipoItem aux = p->item;        // Armazena o item para retorno

    frente->prox = p->prox;        // Remove a célula da fila

    if (tras == p) {               // Se era o único elemento, atualiza 'tras'
        tras = frente;
    }

    delete p;                      // Libera memória
    tamanho--;

    return aux;
}

void FilaEncadeada::Limpa() {
    while (!Vazia()) {
        Desenfileira(); // Reutiliza o método para limpar todas as células
    }
}

// -------------------- Métodos da classe ListaEncadeada --------------------------

ListaEncadeada::ListaEncadeada() {
    primeiro = new TipoCelula();  // Célula sentinela
    ultimo = primeiro;            // Lista vazia: sentinela é o último também
    tamanho = 0;
}

ListaEncadeada::~ListaEncadeada() {
    Limpa();         // Remove todos os elementos
    delete primeiro; // Libera a sentinela
}

int ListaEncadeada::GetTamanho() {
    return tamanho;
}

bool ListaEncadeada::Vazia() {
    return tamanho == 0;
}

// Retorna ponteiro para a célula da posição desejada
TipoCelula* ListaEncadeada::Posiciona(int pos, bool antes) {
    if (pos <= 0 || pos > tamanho) {
        throw std::out_of_range("ERRO: Posição inválida!");
    }

    TipoCelula* p = primeiro;
    int i = 0;

    // Percorre até a célula anterior ou a própria posição
    while (i < (antes ? pos - 1 : pos)) {
        p = p->prox;
        i++;
    }
    return p;
}

TipoItem ListaEncadeada::GetItem(int pos) {
    TipoCelula* p = Posiciona(pos);
    return p->item;
}

void ListaEncadeada::SetItem(TipoItem item, int pos) {
    TipoCelula* p = Posiciona(pos);
    p->item = item;
}

void ListaEncadeada::InsereInicio(TipoItem item) {
    TipoCelula* nova = new TipoCelula();
    nova->item = item;
    nova->prox = primeiro->prox;
    primeiro->prox = nova;
    tamanho++;

    if (nova->prox == nullptr) {
        ultimo = nova;
    }
}

void ListaEncadeada::InsereFinal(TipoItem item) {
    TipoCelula* nova = new TipoCelula();
    nova->item = item;
    nova->prox = nullptr;
    ultimo->prox = nova;
    ultimo = nova;
    tamanho++;
}

void ListaEncadeada::InserePosicao(TipoItem item, int pos) {
    TipoCelula* p = Posiciona(pos, true); // Posição anterior
    TipoCelula* nova = new TipoCelula();
    nova->item = item;
    nova->prox = p->prox;
    p->prox = nova;
    tamanho++;

    if (nova->prox == nullptr) {
        ultimo = nova;
    }
}

TipoItem ListaEncadeada::RemoveInicio() {
    if (Vazia()) {
        throw std::runtime_error("ERRO: Lista vazia!");
    }

    TipoCelula* p = primeiro->prox;
    TipoItem aux = p->item;

    primeiro->prox = p->prox;
    tamanho--;

    if (primeiro->prox == nullptr) {
        ultimo = primeiro;
    }

    delete p;
    return aux;
}

TipoItem ListaEncadeada::RemoveFinal() {
    if (Vazia()) {
        throw std::runtime_error("ERRO: Lista vazia!");
    }

    TipoCelula* p = Posiciona(tamanho, true);
    TipoItem aux = ultimo->item;
    delete ultimo;
    ultimo = p;
    ultimo->prox = nullptr;
    tamanho--;

    return aux;
}

TipoItem ListaEncadeada::RemovePosicao(int pos) {
    if (Vazia()) {
        throw std::runtime_error("ERRO: Lista vazia!");
    }

    TipoCelula* p = Posiciona(pos, true);
    TipoCelula* q = p->prox;
    TipoItem aux = q->item;

    p->prox = q->prox;
    tamanho--;

    if (p->prox == nullptr) {
        ultimo = p;
    }

    delete q;
    return aux;
}

TipoItem ListaEncadeada::Pesquisa(TipoChave c) {
    if (Vazia()) {
        throw std::runtime_error("ERRO: Lista vazia!");
    }

    TipoCelula* p = primeiro->prox;

    while (p != nullptr) {
        if (p->item.GetChave() == c) {
            return p->item;
        }
        p = p->prox;
    }

    TipoItem aux;
    aux.SetChave(-1);  // Chave inválida indicando "não encontrado"
    return aux;
}

void ListaEncadeada::Imprime() {
    TipoCelula* p = primeiro->prox;
    while (p != nullptr) {
        p->item.Imprime();
        std::cout << " ";
        p = p->prox;
    }
    std::cout << std::endl;
}

void ListaEncadeada::Limpa() {
    TipoCelula* p = primeiro->prox;
    while (p != nullptr) {
        TipoCelula* temp = p;
        p = p->prox;
        delete temp;
    }
    primeiro->prox = nullptr;
    ultimo = primeiro;
    tamanho = 0;
}

// -------------------- Métodos da classe PilhaEncadeada --------------------------

PilhaEncadeada::PilhaEncadeada() : topo(nullptr) {
    tamanho = 0;
}

PilhaEncadeada::~PilhaEncadeada() {
    Limpa();  // Remove todos os elementos
}

void PilhaEncadeada::Empilha(TipoItem item) {
    TipoCelula* nova = new TipoCelula();
    nova->item = item;
    nova->prox = topo;
    topo = nova;
    tamanho++;
}

TipoItem PilhaEncadeada::Desempilha() {
    if (Vazia()) {
        throw "A pilha está vazia!";
    }

    TipoItem aux = topo->item;
    TipoCelula* p = topo;
    topo = topo->prox;
    delete p;
    tamanho--;

    return aux;
}

bool PilhaEncadeada::Vazia() const {
    return tamanho == 0;
}

void PilhaEncadeada::Limpa() {
    while (!Vazia()) {
        Desempilha();
    }
}


