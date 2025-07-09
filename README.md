# 📦 Sistema de Escalonamento Logístico - Armazéns de Hanói
![Feito em C++](https://img.shields.io/badge/C%2B%2B-Feito%20em-blue)

![Badge Concluído](https://img.shields.io/badge/Status-Conclu%C3%ADdo-brightgreen)

Simulação de um sistema logístico de entregas de pacotes em uma rede de armazéns interconectados, com foco em transporte, rearmazenamento e entrega eficiente de pacotes utilizando eventos discretos e Programação Orientada a Objetos (POO).

## 📝 Sobre o Projeto

Este projeto foi desenvolvido como parte do **Trabalho Prático 2** da disciplina de **Estruturas de Dados** (DCC/UFMG). A proposta consiste em modelar um sistema logístico inspirado em cenários reais, utilizando uma abordagem orientada a objetos e simulação por eventos discretos.

A rede logística é representada por um grafo não direcionado. Os pacotes seguem rotas calculadas por busca em largura (BFS), e a movimentação ocorre por eventos escalonados no tempo.

---

## 📁 Estrutura do Projeto

- `include/` – Arquivos de cabeçalho
- `src/` – Implementações das classes
- `bin/` – Executável final: `tp2.out`
- `obj/` – Arquivos objeto da compilação
- `Makefile` – Script de compilação automatizada

---

## 🔧 Principais Classes e Estruturas

### Estruturas principais
- `Pacote`: representa o pacote, com ID único, rota, posição atual, estatísticas e métodos como `BuscaMenorCaminho()` e `atualizaRota()`.
- `Armazem`: representa um armazém com ID e conexões para outros armazéns.
- `ArmazemVizinho`: modela a ligação entre armazéns, com pilhas LIFO de pacotes.
- `Sistema`: gerencia toda a rede logística.
- `Escalonador`: um min-heap de eventos que controla a ordem temporal.
- `Evento`: encapsula eventos do tipo 1 (chegada) e tipo 2 (transporte).

### Estruturas auxiliares
- `FilaEncadeada`: usada na busca em largura.
- `PilhaEncadeada`: organiza pacotes nos armazéns.
- `ListaEncadeada`: armazena rotas dos pacotes.

---

## ▶️ Como Compilar e Executar

### 💻 Compilar
No terminal, dentro do diretório raiz do projeto:

```bash
make
```

O executável será gerado em `bin/tp2.out`.

### 🚀 Executar
```bash
./bin/tp2.out entrada.txt
```

> Substitua `entrada.txt` pelo nome do arquivo com os dados da simulação.

---

## ⏱️ Análise de Complexidade

- **Escalonador**: inserção e remoção em `O(log n)`
- **BuscaMenorCaminho()**: `O(n + m)` para `n` armazéns e `m` conexões
- **Função principal**: no pior caso, `O(pn(p+C) + n²)` para `p` pacotes e `C` capacidade de transporte

---

## 🛡️ Estratégias de Robustez

- ✅ Validação de argumentos e arquivos de entrada
- ✅ Verificação da leitura dos dados
- ✅ Checagem de IDs de armazéns (limites válidos)
- ✅ Lançamento de exceções (`std::runtime_error`) em inconsistências internas
- ✅ Liberação adequada de memória em todos os casos

---

## 📊 Resultados Experimentais

- O tempo de simulação cresce quadraticamente com o número de armazéns (`n`) e pacotes (`p`).
- A contenção de pacotes diminui com:
  - Alta **capacidade de transporte**
  - Pequenos **intervalos entre transportes**
  - Latência **maior** que o intervalo de transporte
- Rearmazenamentos são otimizados ao equilibrar os parâmetros do sistema.

---

## 📚 Referências

- [Simulação de eventos discretos — Wikipédia](https://pt.wikipedia.org/wiki/Simula%C3%A7%C3%A3o_de_eventos_discretos)
- Especificação do Trabalho Prático 2 - DCC/ICEx/UFMG
- Slides da disciplina DCC205 — Prof. Anisio Lacerda, Wagner Meira Jr., Washington Cunha

---

## 👤 Autor

**Matheus Soares dos Santos de Freitas**  
📧 masanfreitas@outlook.com  
🎓 Universidade Federal de Minas Gerais (UFMG)  
