# 🎯 PLANO PARA TI - O QUE FAZER AGORA

## ✅ VERIFICAÇÃO: ESTÁ TUDO BEM LIGADO?

### Verificação de Includes:
- ✅ Pessoa.h inclui Produto.h
- ✅ Supermercado.h inclui Pessoa.h e Relogio.h
- ✅ Produto.h define Lista (struct Node e struct Lista)
- ⚠️ **PROBLEMA**: Supermercado.h tem `ptCAIXA` (linha 36) mas Caixa.h não existe!

### Erros Encontrados em Supermercado.h:
1. **Linha 33**: `char nome[Supermercado + 1];` → Deve ser `char nome[256];` (constante?)
2. **Linha 36**: `ptCAIXA caixas;` → Tipo não definido (Caixa.h não existe)
3. **Linha 38**: `int totalPRo` → Linha incompleta/truncada
4. **Linha 49**: `#endif // SUPERMERCADO_H_INCLUDEDgg` → Lixo no final

---

## 📋 STATUS ATUAL

| Componente | Status | Responsável | O que falta |
|-----------|--------|-------------|-----------|
| Produto | ✅ PRONTO | - | - |
| Pessoa | ✅ PRONTO | - | Integração com Supermercado |
| Relogio | ✅ PRONTO | - | Integração com Supermercado |
| **Supermercado** | 🔴 ERROS | Colega | Corrigir struct, criar Caixa.h |
| **Caixa** | ⚪ TODO | ← **TU AQUI** | Criar Caixa.c/h |
| Menu/Relatórios | ⚪ TODO | - | Depois de tudo funcionar |

---

## 🚀 O QUE TU PODES FAZER AGORA

### **OPÇÃO 1: Criar Caixa.h/c** ⭐ (RECOMENDADO)

Isto é **bloqueante** porque Supermercado precisa disso.

```c
// Caixa.h
#ifndef CAIXA_H_INCLUDED
#define CAIXA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Pessoa.h"

// Nodo da fila de clientes numa caixa
typedef struct NodoCaixa {
    Pessoa* cliente;
    int tempoAtendimentoDecorrido;  // Tempo já passado no atendimento
    struct NodoCaixa* prox;
} NodoCaixa;

// Caixa de atendimento
typedef struct {
    int id;
    char operador[256];         // Nome do operador
    int operadorID;             // ID do operador (de funcionarios.txt)
    NodoCaixa* fila;            // Fila de clientes
    int ativa;                  // 1 = aberta, 0 = fechada
    int clientesAtendidos;      // Contador de clientes processados
    int produtosVendidos;       // Total de produtos vendidos
    float revenue;              // Receita total
    NodoCaixa* historico;       // Histórico de clientes atendidos
} Caixa;

// FUNÇÕES
Caixa* CriarCaixa(int id, const char *operador);
void DestruirCaixa(Caixa *c);
void AdicionarClienteFila(Caixa *c, Pessoa *cliente);
Pessoa* RemoverPrimeiroDaFila(Caixa *c);
int TamanhoDaFila(Caixa *c);
void MostrarCaixa(Caixa *c);

#endif // CAIXA_H_INCLUDED
```

Depois implementar em Caixa.c:
- `CriarCaixa()` - Alocar + inicializar
- `DestruirCaixa()` - Liberar (fila + histórico)
- `AdicionarClienteFila()` - Adicionar cliente à fila
- `RemoverPrimeiroDaFila()` - Remover e retornar primeiro cliente
- `TamanhoDaFila()` - Contar clientes na fila
- `MostrarCaixa()` - Debug/print

---

### **OPÇÃO 2: Corrigir Supermercado.h** ⭐ (ANTES de Caixa)

Há erros que precisam ser corrigidos:

```c
// Linha 33: Corrigir nome
char nome[256];  // Ao invés de: char nome[Supermercado + 1];

// Linha 36: Comentar até Caixa.h estar pronto
// ptCAIXA caixas;

// Linhas 37-38: Completar struct
int totalClientesAtendidos;
float totalReceita;
```

---

### **OPÇÃO 3: Criar funções de carregamento de dados** ⭐⭐

Em Supermercado.c:
- `CarregarConfiguracao(Supermercado *s, const char *ficheiro)` - Ler config.txt
- `CarregarProdutos(Supermercado *s, const char *ficheiro)` - Ler produtos.txt
- `CarregarFuncionarios(Supermercado *s, const char *ficheiro)` - Ler funcionarios.txt

Isto é **independente** do Caixa e podes fazer enquanto o colega trabalha.

---

## 🎯 MINHA RECOMENDAÇÃO

**Ordem de Ações:**

1. **Corrigir Supermercado.h** (5 min) - Fix simples dos erros
2. **Criar Caixa.h/c** (30-45 min) - Estrutura e funções básicas
3. **Criar funções de carregamento** (30-45 min) - CarregarConfiguracao, CarregarProdutos, CarregarFuncionarios

Isto deixa tudo **pronto para a simulação core** que é o passo seguinte.

---

## 🔧 Ficheiros a Editar/Criar

| Ficheiro | Ação | Prioridade |
|----------|------|-----------|
| Supermercado.h | **Corrigir** (linhas 33, 36, 38, 49) | 🔴 ALTA |
| Caixa.h | **Criar** (novo) | 🔴 ALTA |
| Caixa.c | **Criar** (novo) | 🔴 ALTA |
| Supermercado.c | **Expandir** (funções carregamento) | 🟡 MÉDIA |

---

## ✅ Quando tudo estiver pronto:

Próximas fases (para depois):
- Gerar cliente aleatório
- Transição compra → caixa
- Atendimento em caixa
- Regras de abrir/fechar caixas
- Menu e relatórios

---

**Quer que eu copie o código COMPLETO de Caixa.h/c no chat para corrigir?** ✅
