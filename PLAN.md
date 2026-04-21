## Plan: Supermercado Checkout System - Correct Implementation

**TL;DR**: Sistema de simulação de supermercado com **fluxo de clientes real**: 
1. Cliente entra (aleatório) → 2. Gera N produtos aleatórios → 3. Passa tempo_compra na loja → 4. Va para fila de caixa (espera) → 5. Paga (tempo_pagamento) → 6. Sai. Se tempo_espera > MAX_ESPERA: oferece produto. Carrega config/dados de ficheiros, aplica regras de abrir/fechar caixas, gera estatísticas e histórico.

---

## FLUXO REAL DE UM CLIENTE

```
ENTRADA → COMPRA (tempo) → FILA (espera) → PAGAMENTO (tempo) → SAÍDA
  ↓
Se tempo_espera > MAX_ESPERA: OFERTA de 1 produto
```

**Detalhes**:
- **ENTRADA**: Cliente gerado aleatoriamente (probabilidade = CadenciaEntradaClientes%)
- **GERAÇÃO PRODUTOS**: N aleatório (1-10), seleciona N produtos aleatórios de produtos.txt
- **COMPRA**: Espera `tempo_compra = Σ tempo_atendimento_produto` segundos
- **FILA**: Escolhe caixa com MENOR fila, espera clientes à frente
- **PAGAMENTO**: `tempo_pagamento = Σ tempo_atendimento_produto` (na caixa)
- **VERIFICAÇÃO OFERTA**: Se `tempo_espera > MAX_ESPERA` → oferece 1 produto aleatório
- **SAÍDA**: Remove cliente, registar estatísticas

---

## PHASE 1: Estruturas de Dados Base

### 1.1 - Produto (Pessoa.h/c)
Campos: ID, nome, preco, tempo_atendimento
Funções: CriarProduto, DestruirProduto

### 1.2 - Pessoa (Pessoa.h/c)
Campos: ID, nome, produtos_carrinho (Lista), custo_total, tempo_compra, tempo_pagamento, 
         tempo_chegada, tempo_entrada_caixa, tempo_espera_real, produto_oferecido_id, caixa_id
Funções: CriarPessoa, AdicionarProduto, CalcularTempos, CalcularCusto, DestruirPessoa

### 1.3 - Caixa (Supermercado.h/c)
Campos: ID, operador_nome, operador_id, fila_clientes (Lista), ativa, clientes_atendidos, 
         produtos_vendidos_total, revenue_total, historico_clientes (Lista), tempo_cliente_atual
Funções: CriarCaixa, AdicionarClienteFila, RemoverClienteFila, ObterTamanhoFila, DestruirCaixa

### 1.4 - Lista/Fila Genérica (Uteis.c)
Operações: CriarLista, AdicionarElemento, RemoverPrimeiro, RemoverElemento, 
           ObterElemento, ObterTamanho, DestruirLista, ListarElementos

### 1.5 - Hash para Caixas (Supermercado.c)
Funções: CriarHashCaixas, InserirCaixa, ObterCaixa, RemoverCaixa, ListarTodasCaixas, DestruirHash

### 1.6 - Relógio (Relogio.c/h)
Campos: tempo_inicio_real, velocidade, tempo_simulado
Funções: CriarRelogio, GetTempo, IncrementarTempo, DestruirRelogio

---

## PHASE 2: Carregamento de Dados

### 2.1 - Ler config.txt
Parâmetros: MAX_ESPERA, N_CAIXAS, TEMPO_ATENDIMENTO_PRODUTO, MAX_PRECO, MAX_FILA, MIN_FILA
Armazenar em Supermercado struct

### 2.2 - Carregar produtos.txt
Ler ~625KB, popular Lista de Produto
Criar função: SelecionarProdutoAleatorio()

### 2.3 - Carregar funcionarios.txt
Ler operadores, armazenar array de nomes
Função: SelecionarOperadorAleatorio()

### 2.4 - (Opcional) Carregar Dados.txt
Estado inicial das caixas e clientes em fila

---

## PHASE 3: Simulação Temporal - Core

### 3.1 - Geração de Clientes (EntradaPessoaSupermercado)
A cada iteração: com probabilidade CadenciaEntradaClientes%, gerar novo cliente
- Gerar N aleatório (1-10)
- Selecionar N produtos aleatórios
- Calcular tempo_compra e custo_total
- Adicionar a clientes_comprando

### 3.2 - Transição Compra→Caixa (ClientesPelaNaFila)
Quando tempo_chegada + tempo_compra ≤ tempo_atual:
- Escolher caixa com MENOR fila
- Adicionar cliente à fila
- Registar tempo_entrada_caixa

### 3.3 - Atendimento em Caixa (ProcessarCaixas)
Para cada caixa ativa:
- Descontar tempo de atendimento
- Quando termina: calcular tempo_espera, verificar OFERTA
- Remover cliente, registar estatísticas

### 3.4 - Abrir Caixa Automática (VerificarAbrirCaixa)
Se media_fila > MAX_FILA e n_caixas_abertas < N_CAIXAS:
- Abrir nova caixa com operador aleatório

### 3.5 - Fechar Caixa Automática (VerificarFecharCaixa)
Se media_fila < MIN_FILA e n_caixas_abertas > 1:
- Fechar caixa com menor fila
- REDISTRIBUIR clientes para outras caixas

---

## PHASE 4: Menu e Interação

### 4.1 - Menu Expandido
Opções: Listar clientes compra, Listar clientes filas, Pesquisar, Stats, Abrir/Fechar, Relatório, Sair

### 4.2 - Pesquisar Cliente
Input: nome → Output: localização ou "já saiu"

### 4.3 - Abrir Caixa Manual
Menu para gerente abrir caixa

### 4.4 - Estatísticas Parciais
Display: clientes, caixas abertas, tempo simulado, ofertas

---

## PHASE 5: Estatísticas e Relatórios

### 5.1 - Calcular Estatísticas
Caixa mais clientes, mais produtos, operador menos clientes, tempo médio espera, ofertas, etc.

### 5.2 - Histórico em CSV
Gravar histórico.csv com todas ações (timestamps, operações)

### 5.3 - Memória Utilizada
Calcular sizeof total de estruturas

### 5.4 - Memória Desperdiçada
Detectar fragmentação em listas/hash

---

## Ficheiros a Modificar

| Ficheiro | Ação |
|----------|------|
| `Pessoa.h/c` | Implementar Produto + Pessoa |
| `Supermercado.h/c` | Caixa, Hash, Carregamento, Lógica |
| `Relogio.h/c` | Completar GetTempo, IncrementarTempo |
| `Uteis.c` | Lista, funções memória |
| `main.c` | Menu, loop, relatórios |
| `config.txt` | CRIAR |
| `historico.csv` | GERAR |

---

## Ordem Implementação

1. Lista genérica (Uteis.c)
2. Produto (Pessoa.h/c)
3. Pessoa (Pessoa.h/c)
4. Caixa (Supermercado.h/c)
5. Hash (Supermercado.c)
6. Relógio (Relogio.c)
7. Carregar config + produtos + funcionários
8. Geração de clientes
9. Transição compra → caixa
10. Atendimento em caixa + ofertas
11. Regras de abrir/fechar
12. Menu + Estatísticas + CSV

---

## Status: PRONTO PARA IMPLEMENTAÇÃO ✅
