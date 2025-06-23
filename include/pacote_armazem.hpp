#ifndef PACOTE_ARMAZEM_HPP
#define PACOTE_ARMAZEM_HPP

#include <string>
#include "sistema.hpp"
#include "estruturas.hpp"

// ------------------------- Classe Pacote ----------------------------

// Representa um pacote que circula entre os armazéns
class Pacote {

private:
    int idPac;                 // ID único do pacote
    int tempoChegada;         // Tempo em que o pacote chegou ao próximo armazém
    int tempoArmazenado;      // Tempo total que o pacote ficou armazenado
    int tempoTransito;        // Tempo total que o pacote ficou em trânsito
    int idArmazemInicial;     // ID do armazém de origem
    int idArmazemFinal;       // ID do armazém de destino
    std::string estadoAtual;  // Estado atual do pacote (ex: armazenado, em trânsito)

    // Controle da rota
    int posicaoNaRota;        // Posição atual do pacote na rota
    int idProximoArmazem;     // ID do próximo armazém no trajeto

public:
    Pacote* prox;             // Ponteiro para o próximo pacote (usado em estruturas encadeadas)
    ListaEncadeada* rotaArmazens; // Rota do pacote armazenada como lista encadeada de armazéns

    // Construtores e destrutor
    Pacote();   // Construtor padrão
    Pacote(int idPac, int tempoChegada, int idArmazemInicial, int idArmazemFinal); // Construtor com parâmetros
    ~Pacote();  // Destrutor

    // Métodos principais

    // Gera a rota do pacote entre armazéns com base no menor caminho
    void BuscaMenorCaminho(Sistema* s);

    // Atualiza o estado textual do pacote (ex: "armazenado", "em trânsito", etc.)
    void atualizaEstado(int idEstado);

    // Atualiza o tempo total que o pacote ficou armazenado (com base no relógio)
    void atualizaTempoArmazenado(int tempoRelogio) { 
        tempoArmazenado += (tempoRelogio - tempoChegada); 
    }

    // Atualiza o tempo total em trânsito (com base na latência do transporte)
    void atualizaTempoTransito(int latenciaTransporte) { 
        tempoTransito += latenciaTransporte; 
    }

    // Atualiza a posição do pacote na rota, retorna false se fim da rota
    bool atualizaRota();

    // Getters (acessam informações do pacote)
    int getIdPac() const { return idPac; }
    int getTempoChegada() const { return tempoChegada; }
    int getIdArmazemInicial() const { return idArmazemInicial; }
    int getIdArmazemFinal() const { return idArmazemFinal; }
    std::string getEstadoAtual() const { return estadoAtual; }
    int getPosicaoNaRota() const { return posicaoNaRota; }
    int getIdProximoArmazem() const { return idProximoArmazem; }

    // Setters (modificam atributos do pacote)
    void setTempoChegada(int tempo) { tempoChegada = tempo; }
};

// ------------------------- Estruturas e Classe Armazém ----------------------------

class Armazem; // Declaração antecipada (necessária para ponteiros)

// Estrutura que representa um vizinho de um armazém
struct ArmazemVizinho {
    Armazem* representante;        // Ponteiro para o armazém vizinho
    ArmazemVizinho* prox;         // Próximo vizinho na lista
    PilhaEncadeada secao;         // Pilha de pacotes destinados a este vizinho

    // Construtor da estrutura ArmazemVizinho
    ArmazemVizinho(Armazem* representante = nullptr, ArmazemVizinho* prox = nullptr)
        : representante(representante), prox(prox) {}
};

// Representa um armazém, com seus vizinhos conectados
class Armazem {

private:
    int idArm; // ID do armazém

public:
    ArmazemVizinho* listaVizinhos; // Lista encadeada dos vizinhos (com rota direta)

    Armazem();     // Construtor
    ~Armazem();    // Destrutor

    void setIndice(int indice);  // Define o ID do armazém
    int getIndice() const;       // Retorna o ID do armazém
};

#endif
