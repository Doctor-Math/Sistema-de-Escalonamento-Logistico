#ifndef EVENTO_ESCALONADOR_HPP
#define EVENTO_ESCALONADOR_HPP

#include "iostream"
#include <string>

// ---------------------- Estrutura Evento ----------------------
// Representa um evento no sistema de simulação, usado pelo escalonador
struct Evento {
    int tempo;         // Tempo do evento
    int origem;        // Armazém de origem (se aplicável)
    int destino;       // Armazém de destino (se aplicável)
    int tipoEvento;    // Tipo do evento: 1 = pacote, 2 = transporte
    int idPacote;      // Identificador do pacote (se aplicável)
    std::string chaveEvento; // Chave única para desempate e ordenação

    // Construtor com valores padrão
    Evento(int t = 0, int tp = 1, int o = -1, int d = -1, int idPac = -1)
        : tempo(t), origem(o), destino(d), tipoEvento(tp), idPacote(idPac) {

        // Geração da chave do evento com base em tipo:
        // Tipo 1 (pacote): tempo + idPacote + tipo
        // Tipo 2 (transporte): tempo + origem + destino + tipo
        if (tipoEvento == 1) {
            chaveEvento = Formata(tempo, 6) + Formata(idPacote, 6) + Formata(tipoEvento, 1);
        } else if (tipoEvento == 2) {
            chaveEvento = Formata(tempo, 6) + Formata(origem, 3) +
                          Formata(destino, 3) + Formata(tipoEvento, 1);
        }
    }

    // Função auxiliar: preenche à esquerda com zeros até atingir 'largura'
    std::string Formata(int valor, int largura) const {
        std::string s = std::to_string(valor);
        int zeros = largura - (int)s.size();
        if (zeros < 0) zeros = 0;
        return std::string(zeros, '0') + s;
    }

    // Operadores de comparação usados para ordenar eventos no heap
    bool operator<(const Evento& outro) const {
        return chaveEvento < outro.chaveEvento;
    }

    bool operator>(const Evento& outro) const {
        return chaveEvento > outro.chaveEvento;
    }
};

// ---------------------- Classe Escalonador ----------------------
// Implementa uma fila de prioridade usando heap mínimo
class Escalonador {
public:
    Escalonador() {
        tamanho = 0;
        capacidade = 10000; // Capacidade inicial do heap
        eventos = new Evento*[capacidade]; // Vetor de ponteiros para eventos
    }

    ~Escalonador() {
        // Libera memória dos eventos e do vetor
        for (int i = 0; i < tamanho; i++) {
            delete eventos[i];
        }
        delete[] eventos;
    }

    // Insere um evento no heap, mantendo a propriedade de mínimo
    void InserirEvento(const Evento& e) {
        if (tamanho >= capacidade) {
            Redimensiona(); // Aumenta a capacidade se necessário
        }

        eventos[tamanho] = new Evento(e); // Cria nova cópia dinâmica do evento
        int i = tamanho;
        int p = GetAncestral(i);

        // Sobe o evento no heap até manter a ordem
        while (i > 0 && *eventos[i] < *eventos[p]) {
            TrocaEventos(eventos[i], eventos[p]);
            i = p;
            p = GetAncestral(i);
        }

        tamanho++;
    }

    // Retira e retorna o evento com maior prioridade (menor chave)
    Evento RetiraProximoEvento() {
        if (Vazio()) {
            throw std::runtime_error("Sem eventos para retirar!");
        }

        Evento resultado = *eventos[0]; // Copia o evento de retorno
        delete eventos[0];             // Libera memória do evento retirado

        tamanho--;
        eventos[0] = eventos[tamanho]; // Move o último para o topo
        eventos[tamanho] = nullptr;

        // Reorganiza o heap (heapify para baixo)
        int i = 0;
        while (true) {
            int esq = GetSucessorEsq(i);
            int dir = GetSucessorDir(i);
            int s;

            if (esq < tamanho && dir < tamanho) {
                s = (*eventos[dir] < *eventos[esq]) ? dir : esq;
            } else if (esq < tamanho) {
                s = esq;
            } else if (dir < tamanho) {
                s = dir;
            } else {
                break; // Nó não tem filhos
            }

            if (*eventos[i] > *eventos[s]) {
                TrocaEventos(eventos[i], eventos[s]);
                i = s;
            } else {
                break;
            }
        }

        return resultado;
    }

    // Verifica se o escalonador está vazio
    bool Vazio() {
        return tamanho == 0;
    }

private:
    // Funções auxiliares para navegação no heap binário
    int GetAncestral(int posicao) {
        return (posicao - 1) / 2;
    }

    int GetSucessorEsq(int posicao) {
        return 2 * posicao + 1;
    }

    int GetSucessorDir(int posicao) {
        return 2 * posicao + 2;
    }

    // Dobra a capacidade do vetor de eventos
    void Redimensiona() {
        int novaCapacidade = capacidade * 2;
        Evento** novoArray = new Evento*[novaCapacidade];

        for (int i = 0; i < tamanho; i++) {
            novoArray[i] = eventos[i];
        }

        delete[] eventos;
        eventos = novoArray;
        capacidade = novaCapacidade;
    }

    // Troca dois ponteiros de eventos no vetor
    void TrocaEventos(Evento*& a, Evento*& b) {
        Evento* temp = a;
        a = b;
        b = temp;
    }

    int tamanho;           // Número atual de eventos no heap
    int capacidade;        // Capacidade alocada do vetor
    Evento** eventos;      // Vetor de ponteiros para eventos
};

#endif