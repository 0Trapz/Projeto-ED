# 🚀 GUIA DE IMPLEMENTAÇÃO - Supermercado Checkout System

## Status Atual ✅

### ✅ JÁ IMPLEMENTADO (Uteis.c):
- `Aleatorio(min, max)` - Gerador de números aleatórios
- `LerInteiro(txt)` - Leitura de inteiros
- `ToMaiscula(char)` - Conversão para maiúscula
- `wait(milsegundos)` - Delay em ms
- `wait_segundos(segundos)` - Delay em segundos
- `TeclaPressionada()` - Verificação de input (stub)
- **Lista/Fila** - DONE em Uteis.c? (verificar se está implementada)

---

## 📋 ORDEM DE IMPLEMENTAÇÃO RECOMENDADA

### **PASSO 1: Estrutura Produto** ⭐
**Ficheiro**: `Pessoa.h` e `Pessoa.c`

```c
// Pessoa.h - ADICIONAR:
typedef struct {
    int id;
    char *nome;
    float preco;
    int tempo_atendimento;
} Produto;

Produto* CriarProduto(int id, char *nome, float preco, int tempo_atendimento);
void DestruirProduto(Produto *p);
```

**Por que primeiro**: É o bloco básico para tudo. Sem Produto, não conseguimos criar Pessoa com carrinho.

**Verificação**: Criar 3 produtos de teste, imprimir detalhes, destruir.

---

### **PASSO 2: Estrutura Pessoa (Cliente)** ⭐⭐
**Ficheiro**: `Pessoa.h` e `Pessoa.c`

```c
// Pessoa.h - ADICIONAR:
typedef struct Pessoa {
    int id;
    char *nome;
    Lista *produtos_carrinho;      // Lista de Produto*
    float custo_total;
    int tempo_compra_total;        // Soma de tempo_atendimento dos produtos
    int tempo_pagamento_total;     // Mesmo que tempo_compra
    time_t tempo_chegada;
    time_t tempo_entrada_caixa;
    int tempo_espera_real;
    int produto_oferecido_id;      // -1 se nenhum
    int caixa_id;                  // ID da caixa atual
    int status;                    // 0=compra, 1=fila, 2=pagamento, 3=saido
} Pessoa;

Pessoa* CriarPessoa(int id, char *nome);
void AdicionarProduto(Pessoa *p, Produto *prod);
void CalcularTempos(Pessoa *p);
void CalcularCusto(Pessoa *p);
void DestruirPessoa(Pessoa *p);
```

**Por que aqui**: Precisa de Lista (já existe) e Produto (acabámos de fazer).

**Verificação**: Criar Pessoa, adicionar 3 produtos, calcular tempos, destruir.

---

### **PASSO 3: Relogio (Simulação Temporal)** ⭐
**Ficheiro**: `Relogio.h` e `Relogio.c`

**O que adicionar**:
```c
// Relogio.h - COMPLETAR:
typedef struct {
    time_t Tinicio;
    int Velocidade;
    time_t tempo_simulado;  // NOVO: tempo actual da simulação
} Relogio;

// Relogio.c - IMPLEMENTAR:
Relogio *CriarRelogio(int _velocidade) {
    Relogio *R = (Relogio *)malloc(sizeof(Relogio));
    R->Tinicio = time(NULL);
    R->Velocidade = _velocidade;
    R->tempo_simulado = 0;
    return R;
}

time_t GetTempo(Relogio *R) {
    return R->tempo_simulado;
}

void IncrementarTempo(Relogio *R, int segundos) {
    R->tempo_simulado += segundos;
}
```

**Por que agora**: Precisamos de tempo para simular o fluxo de clientes.

**Verificação**: Criar relógio, incrementar 10 vezes, verificar tempo.

---

### **PASSO 4: Estrutura Caixa** ⭐⭐
**Ficheiro**: `Supermercado.h` (ou novo `Caixa.h/c`)

```c
// Supermercado.h - ADICIONAR:
typedef struct Caixa {
    int id;
    char *operador_nome;
    int operador_id;
    Lista *fila_clientes;          // Lista de Pessoa*
    int ativa;                     // 1=aberta, 0=fechada
    int clientes_atendidos;
    int produtos_vendidos_total;
    float revenue_total;
    Lista *historico_clientes;     // Para relatórios
    int tempo_cliente_atual;       // Tempo de atendimento do cliente actual
} Caixa;

Caixa* CriarCaixa(int id, char *operador_nome);
void AdicionarClienteFila(Caixa *c, Pessoa *p);
Pessoa* RemoverClienteFila(Caixa *c);
int ObterTamanhoFila(Caixa *c);
void DestruirCaixa(Caixa *c);
```

**Por que aqui**: Precisa de Lista (existe) e Pessoa (já feita).

**Verificação**: Criar caixa, adicionar 2 clientes, remover, verificar tamanho fila.

---

### **PASSO 5: Hash para Caixas** ⭐
**Ficheiro**: `Supermercado.c`

```c
// Implementar tabela hash simples com tamanho 20
typedef struct {
    int ID;
    Caixa *caixa;
} HashNode;

typedef struct {
    HashNode **nodes;
    int tamanho;
    int n_elementos;
} Hash;

Hash* CriarHashCaixas(int tamanho);
void InserirCaixa(Hash *h, Caixa *c);
Caixa* ObterCaixa(Hash *h, int id);
void RemoverCaixa(Hash *h, int id);
void DestruirHash(Hash *h);
```

**Por que aqui**: Acesso rápido O(1) a caixas. Precisa de Caixa.

**Verificação**: Criar hash, inserir 3 caixas, obter por ID, remover.

---

### **PASSO 6: Carregar Ficheiros de Configuração** ⭐⭐⭐
**Ficheiro**: `Supermercado.h` e `Supermercado.c`

**Adicionar a Supermercado struct**:
```c
typedef struct {
    char *NOME;
    Lista *clientes_comprando;     // NOVO
    Hash *caixas;                  // NOVO
    Lista *produtos_disponiveis;   // NOVO (de produtos.txt)
    char **funcionarios;           // NOVO (array de strings)
    int n_funcionarios;            // NOVO
    int max_espera;                // NOVO (de config.txt)
    int n_caixas;
    int tempo_atendimento_produto_max;
    int max_preco;
    int max_fila;
    int min_fila;
    int cadencia_entrada_clientes;
    // ... ofertas e estatísticas
    Relogio *Rolex;
} Supermercado;
```

**Funções a criar**:
```c
int CarregarConfiguracao(Supermercado *S, char *ficheiro);
int CarregarProdutos(Supermercado *S, char *ficheiro);
int CarregarFuncionarios(Supermercado *S, char *ficheiro);
Produto* SelecionarProdutoAleatorio(Supermercado *S);
char* SelecionarOperadorAleatorio(Supermercado *S);
```

**Por que agora**: Temos todas as estruturas. Precisamos de dados para começar a simular.

**Verificação**: 
- Criar config.txt com valores default
- Ler config.txt, imprimir valores
- Ler primeiros 5 produtos
- Ler 10 funcionários

---

### **PASSO 7: Geração de Clientes** ⭐⭐
**Ficheiro**: `Supermercado.c`

```c
void EntradaPessoaSupermercado(Supermercado *S) {
    // A cada iteração:
    // 1. Gerar número aleatório 0-100
    // 2. IF < CadenciaEntradaClientes THEN
    //    a) Criar novo Pessoa
    //    b) Gerar N aleatório 1-10 produtos
    //    c) Selecionar N produtos aleatórios
    //    d) CalcularTempos(pessoa)
    //    e) CalcularCusto(pessoa)
    //    f) pessoa.tempo_chegada = GetTempo(Rolex)
    //    g) AdicionarElemento(clientes_comprando, pessoa)
    //    h) Imprimir: "Cliente X entrou (N produtos, tempo=Ys)"
}
```

**Por que agora**: Temos config, produtos, relógio.

**Verificação**: Rodar 100 iterações, deve gerar ~30 clientes (se cadencia=30).

---

### **PASSO 8: Transição Compra → Caixa** ⭐⭐
**Ficheiro**: `Supermercado.c`

```c
void ClientesPelaNaFila(Supermercado *S) {
    // A cada iteração:
    // PARA CADA cliente em clientes_comprando:
    //   IF GetTempo(Rolex) - cliente.tempo_chegada >= cliente.tempo_compra THEN
    //     a) Encontrar caixa ativa com MENOR fila
    //     b) AdicionarClienteFila(caixa, cliente)
    //     c) cliente.tempo_entrada_caixa = GetTempo(Rolex)
    //     d) cliente.caixa_id = caixa.id
    //     e) cliente.status = 1 (em fila)
    //     f) RemoverElemento(clientes_comprando, cliente_index)
    //     g) Imprimir: "Cliente X foi para Caixa Y (fila=Z)"
}
```

**Por que aqui**: Temos clientes em compra, caixas, relógio.

**Verificação**: Gerar 5 clientes, aguardar seus tempos, devem mover para caixas.

---

### **PASSO 9: Atendimento em Caixa + Ofertas** ⭐⭐⭐
**Ficheiro**: `Supermercado.c`

```c
void ProcessarCaixas(Supermercado *S) {
    // Para cada caixa ativa:
    //   IF fila nao vazia THEN
    //     cliente = primeiro da fila
    //     tempo_cliente += 1 segundo (ou decremento proporcional)
    //     IF tempo_cliente >= cliente.tempo_pagamento THEN
    //       a) tempo_espera = GetTempo - tempo_entrada_caixa
    //       b) IF tempo_espera > MAX_ESPERA THEN
    //            - Oferecer produto aleatório
    //            - Incrementar estatísticas de ofertas
    //       c) RemoverClienteFila(caixa)
    //       d) caixa.clientes_atendidos++
    //       e) caixa.produtos_vendidos_total += cliente.N_produtos
    //       f) caixa.revenue_total += cliente.custo_total
    //       g) AdicionarElemento(historico_clientes, cliente)
    //       h) DestruirPessoa(cliente)
}
```

**Por que aqui**: Core da simulação.

**Verificação**: 3 clientes em caixa, verificar timings, ofertas se > MAX_ESPERA.

---

### **PASSO 10: Regras Automáticas (Abrir/Fechar)** ⭐
**Ficheiro**: `Supermercado.c`

```c
void VerificarAbrirCaixa(Supermercado *S) {
    // Calcular media_fila = total_clientes / caixas_abertas
    // IF media > MAX_FILA E caixas_abertas < N_CAIXAS THEN
    //   - AbrirCaixa(novo_id)
    //   - Atribuir operador
    //   - InserirCaixa(Hash)
}

void VerificarFecharCaixa(Supermercado *S) {
    // IF media < MIN_FILA E caixas_abertas > 1 THEN
    //   - Encontrar caixa com menor fila
    //   - Marcar ativa = 0
    //   - REDISTRIBUIR clientes para outras caixas
    //   - RemoverCaixa(Hash)
}
```

**Por que aqui**: Otimização do sistema.

**Verificação**: Forçar cenários, verificar aberturas/fechamentos.

---

### **PASSO 11: Menu Completo** ⭐
**Ficheiro**: `main.c`

```c
int Menu() {
    printf("\n=== MENU GERENCIA ===\n");
    printf("1 - Listar Clientes em Compra\n");
    printf("2 - Listar Clientes em Filas\n");
    printf("3 - Pesquisar Cliente\n");
    printf("4 - Ver Estatísticas Parciais\n");
    printf("5 - Abrir Caixa Manual\n");
    printf("6 - Fechar Caixa Manual\n");
    printf("7 - Relatório Final\n");
    printf("0 - Sair\n");
    return LerInteiro("Opção: ");
}

void ExecutaAccoesMenu(Supermercado *S) {
    int op = Menu();
    switch(op) {
        case 1: ListarClientesCompra(S); break;
        case 2: ListarClientesFilas(S); break;
        case 3: PesquisarCliente(S); break;
        // ... etc
        case 0: break;
    }
}
```

**Por que aqui**: Depois de tudo funcionar, adicionamos interação.

---

### **PASSO 12: Estatísticas e Relatórios** ⭐
**Ficheiro**: `Supermercado.c` e `main.c`

```c
void CalcularEstatisticas(Supermercado *S) {
    // Caixa mais clientes
    // Caixa mais produtos
    // Operador menos clientes
    // Tempo médio espera
    // Produtos oferecidos (qty + valor)
}

void GravarHistoricoCSV(Supermercado *S) {
    // Abrir historico.csv
    // Gravar cada evento com timestamp
}
```

**Por que último**: Relatórios finais, depois de tudo funcionar.

---

## 🎯 PRÓXIMA AÇÃO

**COMECE PELO PASSO 1**: Implementar Produto em `Pessoa.h/c`

Depois de cada passo, recompile com:
```bash
gcc -c Pessoa.c -o Pessoa.o
gcc -c Supermercado.c -o Supermercado.o
gcc -c Relogio.c -o Relogio.o
gcc -c Uteis.c -o Uteis.o
gcc -c main.c -o main.o
gcc Pessoa.o Supermercado.o Relogio.o Uteis.o main.o -o supermercado
./supermercado
```

---

## ✅ Checklist de Implementação

- [ ] PASSO 1: Produto (Pessoa.c/h)
- [ ] PASSO 2: Pessoa (Pessoa.c/h)
- [ ] PASSO 3: Relogio (Relogio.c/h)
- [ ] PASSO 4: Caixa (Supermercado.c/h)
- [ ] PASSO 5: Hash (Supermercado.c)
- [ ] PASSO 6: Carregar ficheiros (Supermercado.c)
- [ ] PASSO 7: Geração de clientes (Supermercado.c)
- [ ] PASSO 8: Transição compra→caixa (Supermercado.c)
- [ ] PASSO 9: Atendimento + Ofertas (Supermercado.c)
- [ ] PASSO 10: Regras automáticas (Supermercado.c)
- [ ] PASSO 11: Menu (main.c)
- [ ] PASSO 12: Estatísticas (Supermercado.c + main.c)
