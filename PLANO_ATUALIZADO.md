# 🚀 PLANO ATUALIZADO - Status da Implementação

## ✅ JÁ IMPLEMENTADO (3/25)

1. **Lista/Fila** ✅ (Produto.h)
2. **Produto** ✅ (Produto.c/h completo - 60 linhas)
3. **Relógio** ✅ (Relogio.c/h completo - com horas/minutos/segundos)

---

## 🔴 EM PROGRESSO (3/25)

1. **Pessoa** - Pessoa.h/c (vazio, só tem `#include`)
2. **Caixa** - Precisa de struct + funções
3. **Hash** - Para caixas (acesso rápido)

---

## ⚪ PENDENTE (19/25)

### FASE 2: Dados e Inicialização
- [ ] Ler Configuracao.txt
- [ ] Carregar produtos.txt
- [ ] Carregar funcionarios.txt
- [ ] Carregar Dados.txt (opcional - estado inicial)

### FASE 3: Simulação Core
- [ ] Gerar Cliente Aleatoriamente
- [ ] Fluxo Completo do Cliente
- [ ] Transição Compra → Caixa
- [ ] Atendimento em Caixa
- [ ] Sistema de Ofertas

### FASE 4: Regras Automáticas
- [ ] Abrir Caixa Automática (MAX_FILA)
- [ ] Fechar Caixa Automática (MIN_FILA)

### FASE 5: Menu e Relatórios
- [ ] Menu Principal Expandido
- [ ] Pesquisar Cliente
- [ ] Calcular Estatísticas
- [ ] Histórico em CSV
- [ ] Calcular Memória

---

## 🎯 PRÓXIMOS PASSOS (Ordem Recomendada)

### **PASSO 1: COMPLETAR PESSOA.c/h** ⭐ (URGENTE)
**Status**: Quase vazio (2 linhas)

Adicionar:
```c
// Pessoa.h
typedef struct {
    int id;
    char *nome;
    Lista *produtos_carrinho;
    float custo_total;
    int tempo_compra_total;
    float tempo_pagamento_total;
    time_t tempo_chegada;
    time_t tempo_entrada_caixa;
    int tempo_espera_real;
    int produto_oferecido_id;
    int caixa_id;
    int status;
} Pessoa;

Pessoa* CriarPessoa(int id, char *nome);
void AdicionarProdutoPessoa(Pessoa *p, Produto *prod);
void CalcularTemposPessoa(Pessoa *p);
void CalcularCustoPessoa(Pessoa *p);
Produto* ObterProdutoMaisBaratoPessoa(Pessoa *p);
void DestruirPessoa(Pessoa *p);
```

---

### **PASSO 2: CRIAR CAIXA.h/c** ⭐
```c
typedef struct {
    int id;
    char *operador_nome;
    int operador_id;
    Lista *fila_clientes;
    int ativa;
    int clientes_atendidos;
    int produtos_vendidos_total;
    float revenue_total;
    Lista *historico_clientes;
    int tempo_cliente_atual;
} Caixa;

Caixa* CriarCaixa(int id, char *operador_nome);
void AdicionarClienteFila(Caixa *c, Pessoa *p);
Pessoa* RemoverClienteFila(Caixa *c);
int ObterTamanhoFila(Caixa *c);
void DestruirCaixa(Caixa *c);
```

---

### **PASSO 3: IMPLEMENTAR HASH PARA CAIXAS** ⭐
Em Supermercado.c:
- `CriarHashCaixas(int tamanho)`
- `InserirCaixa(Hash *h, Caixa *c)`
- `ObterCaixa(Hash *h, int id)`
- `RemoverCaixa(Hash *h, int id)`
- `ListarTodasCaixas(Hash *h)`

---

### **PASSO 4: EXPANDIR SUPERMERCADO struct** ⭐⭐
```c
typedef struct {
    char *NOME;
    Lista *clientes_comprando;
    Hash *caixas;
    Lista *produtos_disponiveis;
    char **funcionarios;
    int n_funcionarios;
    int max_espera;
    int n_caixas;
    int tempo_atendimento_produto_max;
    int max_preco;
    int max_fila;
    int min_fila;
    int cadencia_entrada_clientes;
    int total_produtos_oferecidos;
    float valor_oferecido_total;
    Relogio *Rolex;
} Supermercado;
```

---

### **PASSO 5: CARREGAR DADOS DOS FICHEIROS** ⭐⭐⭐
- `CarregarConfiguracao(S, "config.txt")`
- `CarregarProdutos(S, "produtos.txt")`
- `CarregarFuncionarios(S, "funcionarios.txt")`

---

### **PASSO 6: GERAÇÃO E FLUXO DE CLIENTES** ⭐⭐⭐
- `EntradaPessoaSupermercado(S)` - Gerar cliente aleatório
- `ClientesPelaNaFila(S)` - Transição compra → caixa
- `ProcessarCaixas(S)` - Atendimento + ofertas

---

### **PASSO 7: REGRAS AUTOMÁTICAS**
- `VerificarAbrirCaixa(S)`
- `VerificarFecharCaixa(S)`

---

### **PASSO 8: MENU E RELATÓRIOS**
- Expandir menu (listar, pesquisar, stats)
- Relatório final
- CSV com histórico

---

## 📊 Resumo de Ficheiros

| Ficheiro | Status | Linhas | O que falta |
|----------|--------|--------|-----------|
| Produto.h/c | ✅ DONE | 60 | - |
| Relogio.h/c | ✅ DONE | 66 | - |
| Pessoa.h/c | 🔴 IN_PROGRESS | 2 | Tudo |
| Caixa.h/c | ⚪ TODO | 0 | Criar |
| Supermercado.h/c | 🔴 IN_PROGRESS | 73 | Expandir struct, funções de carregamento |
| main.c | ⚪ TODO | 47 | Menu completo, loop |
| config.txt | ✅ EXISTS | 7 | - |

---

## 🚀 Próxima Ação

**PRIORIDADE 1**: Completar **Pessoa.h/c** (é bloqueante para tudo o resto)

Quer que eu copie o código completo de Pessoa para corrigir? ✅
