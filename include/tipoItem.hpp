#ifndef TIPOITEM_HPP
#define TIPOITEM_HPP

typedef int TipoChave; // TipoChave é um inteiro

// Classe que representa um item genérico com uma chave identificadora
class TipoItem {
public:
    TipoItem();                    // Construtor padrão: chave não inicializada
    TipoItem(TipoChave c);         // Construtor que inicializa a chave com 'c'
    
    void SetChave(TipoChave c);    // Define/atualiza o valor da chave
    TipoChave GetChave();          // Retorna o valor da chave

    void Imprime();                // Imprime a chave (identificador do item)

private:
    TipoChave chave;               // Atributo que representa a chave do item (por exemplo, o ID de um pacote)
};

#endif