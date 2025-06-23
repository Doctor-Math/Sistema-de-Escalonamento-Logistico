#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include "sistema.hpp"
#include "escalonador.hpp"
#include "estruturas.hpp"
#include "pacote_armazem.hpp"

int main(int argc, char* argv[]) {
    // Verifica se o nome do arquivo foi passado como argumento
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo.wkl>" << std::endl;
        return 1;
    }

    // Tenta abrir o arquivo fornecido
    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return 1;
    }

    // Lê os parâmetros iniciais da simulação
    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao, numeroArmazens;
    if (!(infile >> capacidadeTransporte >> latenciaTransporte >> intervaloTransportes >> custoRemocao >> numeroArmazens)) {
        std::cerr << "Erro ao ler parâmetros iniciais." << std::endl;
        return 1;
    }

    // Inicializa a rede com a quantidade de armazéns
    Sistema* rede = new Sistema(numeroArmazens);
    for (int i = 0; i < numeroArmazens; i++) {
        rede->AdicionarArmazem();  // Adiciona armazéns à rede
    }

    Escalonador escalonador;  // Fila de eventos

    // Lê a matriz de adjacência para criar conexões entre armazéns
    for (int i = 0; i < numeroArmazens; i++) {
        for (int j = 0; j < numeroArmazens; j++) {
            int ligacao;
            if (!(infile >> ligacao)) {
                std::cerr << "Erro ao ler matriz de adjacência." << std::endl;
                delete rede;
                return 1;
            }
            if (ligacao == 1) {
                rede->AdicionarAresta(i, j);  // Cria aresta entre os armazéns
                Evento eventoTransporte(0, 2, i, j, -1);  // Evento tipo 2 = transporte
                escalonador.InserirEvento(eventoTransporte);
            }
        }
    }

    // Leitura dos pacotes
    int numeroPacotes;
    if (!(infile >> numeroPacotes)) {
        std::cerr << "Erro ao ler número de pacotes." << std::endl;
        delete rede;
        return 1;
    }

    Pacote** pacotes = new Pacote*[numeroPacotes];
    int tempoChegadaPrimeiro = 0; // Para controle do tempo base

    for (int i = 0; i < numeroPacotes; i++) {
        int tempoChegada, idPac, idArmIni, idArmFinal;
        std::string temp, pacStr, orgStr, dstStr;

        // Leitura das informações do pacote
        if (!(infile >> tempoChegada >> pacStr >> idPac >> orgStr >> idArmIni >> dstStr >> idArmFinal)) {
            std::cerr << "Erro ao ler pacote #" << i << std::endl;
            delete rede;
            return 1;
        }

        // Validação dos IDs de armazéns
        if (idArmIni < 0 || idArmIni >= numeroArmazens || idArmFinal < 0 || idArmFinal >= numeroArmazens) {
            std::cerr << "ERRO: Id de armazém está além dos limites (pacote " << idPac << ")." << std::endl;
            delete rede;
            return 1;
        }

        if (i == 0) tempoChegadaPrimeiro = tempoChegada;

        // Cria o pacote e calcula sua rota
        Pacote* p = new Pacote(i, tempoChegada, idArmIni, idArmFinal);
        p->BuscaMenorCaminho(rede);

        // Cria o evento de chegada do pacote
        Evento eventoPacote(p->getTempoChegada(), 1, -1, -1, i); // tipo 1 = chegada
        escalonador.InserirEvento(eventoPacote);
        p->atualizaEstado(2); // Estado inicial: a caminho do primeiro armazém

        pacotes[i] = p;
    }

    int pacotesEntregues = 0;
    int relogio = 0;

    // Laço principal de execução dos eventos
    while (!escalonador.Vazio() && (pacotesEntregues < numeroPacotes)) {
        Evento atual = escalonador.RetiraProximoEvento();
        relogio = atual.tempo;

        // Evento de transporte
        if (atual.tipoEvento == 2) {
            ArmazemVizinho* armAtual = rede->armazens[atual.origem]->listaVizinhos;

            while (armAtual != nullptr) {
                if (armAtual->representante->getIndice() == atual.destino) {
                    PilhaEncadeada pacotesRemovidos;
                    int tempoPacote = relogio + tempoChegadaPrimeiro;

                    // Remove os pacotes da seção (do mais novo para o mais antigo)
                    while (!armAtual->secao.Vazia()) {
                        TipoItem idPacoteRemovido = armAtual->secao.Desempilha();
                        tempoPacote += custoRemocao;

                        int j = 0;
                        while (j < numeroPacotes && pacotes[j]->getIdPac() != idPacoteRemovido.GetChave()) j++;
                        if (j == numeroPacotes) throw std::runtime_error("Pacote não encontrado!");

                        pacotes[j]->atualizaEstado(4); // Estado: removido para transporte

                        std::cout << std::setfill('0') << std::setw(7) << tempoPacote
                        << " pacote " << std::setw(3) << idPacoteRemovido.GetChave()
                        << " removido de " << std::setw(3) << atual.origem
                        << " na secao " << std::setw(3) << atual.destino << std::endl;

                        pacotesRemovidos.Empilha(idPacoteRemovido);
                    }

                    // Transporta até o limite de capacidade
                    int quantidadeTransportada = 0;
                    while (quantidadeTransportada < capacidadeTransporte && !pacotesRemovidos.Vazia()) {
                        TipoItem idPacote = pacotesRemovidos.Desempilha();
                        int j = 0;
                        while (j < numeroPacotes && pacotes[j]->getIdPac() != idPacote.GetChave()) j++;
                        if (j == numeroPacotes) throw std::runtime_error("Pacote não encontrado!");

                        pacotes[j]->atualizaTempoArmazenado(tempoPacote);
                        pacotes[j]->setTempoChegada(relogio + latenciaTransporte);
                        pacotes[j]->atualizaEstado(2); // Estado: em trânsito

                        std::cout << std::setfill('0') << std::setw(7) << tempoPacote
                        << " pacote " << std::setw(3) << idPacote.GetChave()
                        << " em transito de " << std::setw(3) << atual.origem
                        << " para " << std::setw(3) << atual.destino << std::endl;

                        escalonador.InserirEvento(
                            Evento(tempoPacote + latenciaTransporte, 1, -1, -1, idPacote.GetChave())
                        );

                        quantidadeTransportada++;
                    }

                    // Rearmazena o que não foi transportado
                    while (!pacotesRemovidos.Vazia()) {
                        TipoItem idPacoteRestante = pacotesRemovidos.Desempilha();
                        int j = 0;
                        while (j < numeroPacotes && pacotes[j]->getIdPac() != idPacoteRestante.GetChave()) j++;
                        if (j == numeroPacotes) throw std::runtime_error("Pacote não encontrado!");

                        pacotes[j]->atualizaEstado(3); // Estado: rearmazenado

                        std::cout << std::setfill('0') << std::setw(7) << tempoPacote
                        << " pacote " << std::setw(3) << idPacoteRestante.GetChave()
                        << " rearmazenado em " << std::setw(3) << atual.origem
                        << " na secao " << std::setw(3) << atual.destino << std::endl;

                        armAtual->secao.Empilha(idPacoteRestante);
                    }

                    break; // seção encontrada
                }
                armAtual = armAtual->prox;
            }

            // Agenda novo transporte para o futuro
            escalonador.InserirEvento(
                Evento(relogio + intervaloTransportes, 2, atual.origem, atual.destino, -1)
            );
        }

        // Evento de chegada de pacote
        if (atual.tipoEvento == 1) {
            int j = 0;
            while (j < numeroPacotes && pacotes[j]->getIdPac() != atual.idPacote) j++;
            if (j == numeroPacotes) throw std::runtime_error("Pacote não encontrado!");

            pacotes[j]->atualizaTempoTransito(latenciaTransporte);

            if (pacotes[j]->atualizaRota()) {
                pacotesEntregues++;
                pacotes[j]->atualizaEstado(5); // Estado: entregue

                std::cout << std::setfill('0') << std::setw(7) << relogio
                    << " pacote " << std::setw(3) << pacotes[j]->getIdPac()
                    << " entregue em " << std::setw(3) << pacotes[j]->getIdProximoArmazem() << std::endl;
            } else {
                // Armazena para o próximo salto da rota
                int prox = pacotes[j]->getIdProximoArmazem();
                ArmazemVizinho* armAtual = rede->armazens[pacotes[j]->getPosicaoNaRota()]->listaVizinhos;

                while (armAtual != nullptr) {
                    if (armAtual->representante->getIndice() == prox) {
                        armAtual->secao.Empilha(pacotes[j]->getIdPac());
                        pacotes[j]->atualizaEstado(3); // Estado: armazenado

                        std::cout << std::setfill('0') << std::setw(7) << relogio
                        << " pacote " << std::setw(3) << pacotes[j]->getIdPac()
                        << " armazenado em " << std::setw(3) << pacotes[j]->getPosicaoNaRota()
                        << " na secao " << std::setw(3) << prox << std::endl;
                        break;
                    }
                    armAtual = armAtual->prox;
                }
            }
        }
    }

    // Libera memória
    for (int i = 0; i < numeroPacotes; i++) delete pacotes[i];
    delete[] pacotes;
    delete rede;

    return 0;
}