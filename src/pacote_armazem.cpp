#include "iostream"
#include "pacote_armazem.hpp"
#include "sistema.hpp"

// -------------------- Métodos da classe Pacote --------------------------

Pacote::Pacote(){
    // Inicializa os atributos com valores padrão
    idPac = 0;
    tempoChegada = 0;
    tempoArmazenado = 0;
    tempoTransito = 0;
    idArmazemInicial = 0;
    idArmazemFinal = 0;

    atualizaEstado(1); // Estado inicial: "Não foi postado"

    rotaArmazens = new ListaEncadeada(); // Inicializa a rota como uma lista vazia
    posicaoNaRota = -1; // Ainda não foi postado, posição indefinida
}

Pacote::Pacote(int idPac, int tempoChegada, int idArmazemInicial, int idArmazemFinal)
    : idPac(idPac), tempoChegada(tempoChegada),
      idArmazemInicial(idArmazemInicial), idArmazemFinal(idArmazemFinal),
      rotaArmazens(new ListaEncadeada()) {
    
    tempoArmazenado = 0;
    tempoTransito = 0;

    atualizaEstado(1); // Estado inicial: "Não foi postado"
    posicaoNaRota = -1; // Ainda não foi postado, posição indefinida
}

Pacote::~Pacote() {
    // Libera a memória alocada para a rota
    delete rotaArmazens;
    rotaArmazens = nullptr;
}

void Pacote::BuscaMenorCaminho(Sistema* rede) {
    int numeroArmazens = rede->GetCapacidade();

    // Fila para busca em largura (BFS)
    FilaEncadeada* visitar = new FilaEncadeada();

    // Vetor para armazenar o "pai" de cada nó visitado
    int parentesco[numeroArmazens];

    // Inicializa todos os armazéns como não visitados
    for (int i = 0; i < numeroArmazens; i++) {
        parentesco[i] = -1;
    }

    visitar->Enfileira(idArmazemInicial);  // Começa pelo armazém de origem
    parentesco[idArmazemInicial] = idArmazemInicial; // Marca como visitado

    bool encontrou = false;

    // Algoritmo BFS (Busca em Largura)
    while (!visitar->Vazia()) {
        int atual = visitar->Desenfileira().GetChave(); // Armazém atual

        if (atual == idArmazemFinal) {
            encontrou = true; // Destino encontrado
            break;
        }

        if (rede->armazens[atual] == nullptr) {
            std::cerr << "ERRO: Armazém com índice inválido (" << atual << ")" << std::endl;
            delete visitar;
            return;
        }

        // Percorre os vizinhos do armazém atual
        ArmazemVizinho* vizinho = rede->armazens[atual]->listaVizinhos;
        while (vizinho != nullptr) {
            int idVizinho = vizinho->representante->getIndice();

            // Se ainda não foi visitado
            if (parentesco[idVizinho] == -1) {
                visitar->Enfileira(idVizinho);
                parentesco[idVizinho] = atual; // Marca quem é o pai
            }

            vizinho = vizinho->prox;
        }
    }

    delete visitar;

    if (!encontrou) {
        throw std::runtime_error("ERRO: Não foi possível encontrar caminho entre os armazéns.");
    }

    // Reconstrói o caminho da rota, do destino até a origem
    int atual = idArmazemFinal;
    while (atual != idArmazemInicial) {
        rotaArmazens->InsereInicio(atual); // Constrói a rota na ordem correta
        atual = parentesco[atual];         // Vai voltando pelos "pais"
    }

    idProximoArmazem = idArmazemInicial; // Próximo passo: origem (o início da rota)
}

void Pacote::atualizaEstado(int idEstado) {
    // Atualiza o estado textual do pacote, conforme seu código
    switch (idEstado) {
        case 1:
            estadoAtual = "Não foi postado";
            break;
        case 2:
            estadoAtual = "Chegada escalonada a um armazém (postagem ou transporte)";
            break;
        case 3:
            estadoAtual = "Armazenado na seção associada ao próximo destino de um armazém";
            break;
        case 4:
            estadoAtual = "Removido da seção para transporte";
            break;
        case 5:
            estadoAtual = "Entregue";
            break;
        default:
            throw "ERRO: Estado inválido!";
    }
}

bool Pacote::atualizaRota() {
    bool chegou = false;

    // Se houver mais de um armazém na rota
    if (rotaArmazens->GetTamanho() > 1) {
        posicaoNaRota = idProximoArmazem; // Atualiza posição atual
        idProximoArmazem = rotaArmazens->RemoveInicio().GetChave(); // Avança para o próximo armazém
    }
    else if (rotaArmazens->GetTamanho() == 1) {
        posicaoNaRota = idProximoArmazem;
        idProximoArmazem = rotaArmazens->RemoveInicio().GetChave(); // Último salto da rota
    }
    else {
        chegou = true; // Rota vazia: chegou ao destino
    }

    return chegou;
}

// -------------------- Métodos da classe Armazém --------------------------

Armazem::Armazem() : idArm(-1), listaVizinhos(nullptr) {} // Inicializa sem índice definido e sem vizinhos

Armazem::~Armazem() {
    // Libera a memória dos vizinhos do armazém
    ArmazemVizinho* atual = listaVizinhos;
    while (atual != nullptr) {
        ArmazemVizinho* temp = atual;
        atual = atual->prox;
        delete temp;
    }
}

void Armazem::setIndice(int indice) {
    idArm = indice;
}

int Armazem::getIndice() const {
    return idArm;
}