#ifndef SIST_HPP
#define SIST_HPP

class Armazem; // Declaração prévia para evitar inclusão circular

class Sistema {
    
    private:
    int capacidade;         // Capacidade máxima atual do vetor
    int quantidade;         // Quantidade atual de armazéns

    void redimensionar();   // Dobra a capacidade do vetor se necessário
    
    public:
    Sistema(int cap);       // Construtor
    ~Sistema();             // Destrutor

    Armazem** armazens;     // Vetor de ponteiros para armazéns
    int AdicionarArmazem();                 // Adiciona um novo armazém e retorna seu índice
    void AdicionarAresta(int v, int w);     // Adiciona uma ligação bidirecional entre dois armazéns
    bool ExisteAresta(int u, int v) const;  // Verifica se há ligação entre dois armazéns
    int GetCapacidade() const;              // Retorna a capacidade atual do sistema

    friend class Pacote;  
};

#endif