# Documento Unificado do Projeto
## Sistema de Gestao de Caixas de Supermercado

**Data:** Abril de 2026  
**Objetivo:** Consolidar plano atualizado, proximos passos e relatorio final num unico documento coerente com as ideias do projeto.

---

## 1. Visao Geral do Projeto
O projeto simula a operacao de um supermercado em C. A ideia e usar funcoes auxiliares em `Uteis` e `Relogio` para evitar repetir codigo e concentrar a logica principal em `Pessoa`, `Produto`, `Caixa` e `Supermercado`.

O fluxo geral inclui:
- carregamento do universo de clientes para um array;
- entrada aleatoria de clientes a partir desse universo;
- sorteio de produtos para cada cliente;
- calculo do gasto e dos tempos de compra;
- transicao para a fila de uma caixa;
- abertura e fecho automaticos de caixas;
- atendimento, pagamento e saida do cliente;
- passagem do cliente para a lista historico;
- recolha de estatisticas operacionais.

O foco e representar um fluxo realista de atendimento e permitir evolucao do sistema com novas regras de negocio.

---

## 2. Objetivo Funcional
Desenvolver um sistema que permita:
- carregar todos os clientes para um universo em memoria;
- escolher aleatoriamente um cliente quando entra no supermercado;
- gerar aleatoriamente o numero de produtos comprados por esse cliente;
- calcular o total a pagar e o tempo de compra a partir desses produtos;
- gerir o estado do cliente nas listas de compras, fila, caixa e historico;
- controlar filas e estado das caixas;
- monitorizar tempos de espera e atendimento;
- aplicar regras automaticas para abrir e fechar caixas;
- registar indicadores como clientes atendidos, produtos vendidos, receita e perdas por ofertas.

---

## 3. Estrutura Modular do Projeto
- **main.c**: inicializacao, carregamento de dados, menu e ciclo principal.
- **Supermercado.h/c**: coordena o fluxo global, os universos de clientes/produtos, as listas de ativos e historico, as caixas e as estatisticas.
- **Caixa.h/c**: estrutura e operacoes de cada caixa, incluindo fila, estado, abertura e fecho.
- **Pessoa.h/c**: representacao do cliente, incluindo numero de produtos, gasto, tempos e estado.
- **Produto.h/c**: universo de produtos, sorteio aleatorio e calculos de valores/tempos.
- **Relogio.h/c**: tempo da simulacao.
- **Uteis.h/c**: funcoes auxiliares usadas varias vezes no projeto.

---

## 4. Estado Atual Consolidado
### Implementado
- Estrutura base de **Produto** e respetivas funcoes principais.
- Estrutura base de **Relogio**.
- Parte da arquitetura modular ja definida (headers e separacao por ficheiros).
- Base de **Pessoa** com universo de clientes e criacao de cliente ativo.

### Em progresso
- Integracao completa de **Pessoa** com o fluxo do supermercado.
- Definicao final e integracao de **Caixa** com **Supermercado**.
- Ajustes de coerencia em campos/structs de **Supermercado**.
- Ligacao entre cliente ativo, fila da caixa e lista historico.

### Pendente
- Carregamento integral de ficheiros de dados (`config.txt`, `produtos.txt`, `funcionarios.txt`).
- Simulacao completa do ciclo de cliente.
- Regras automaticas de abrir/fechar caixa.
- Relatorios finais e exportacao de historico.

### Analise do codigo atual
- O modulo de **Produto** esta perto do objetivo funcional: carrega produtos, sorteia produtos aleatorios e calcula totais.
- O modulo de **Pessoa** ja usa o numero de produtos sorteado e guarda gasto/tempos, mas ainda nao guarda explicitamente a lista dos produtos do cliente.
- O modulo de **Supermercado** ainda nao esta bem alinhado com o fluxo descrito: existem inconsistencias de nomes, partes incompletas e logica ainda embrionaria.
- O modulo de **Caixa** existe no projeto, mas precisa de ser ligado ao fluxo de entrada, espera, atendimento e saida.

---

## 5. Fluxo Funcional Previsto (Cliente)
1. **Entrada no supermercado**: o sistema escolhe um cliente do universo de clientes.
2. **Geracao de compras**: o numero de produtos a comprar e sorteado aleatoriamente.
3. **Sorteio de produtos**: sao escolhidos X produtos aleatorios do universo de produtos.
4. **Calculo inicial**: soma-se o gasto total e o tempo de compra dos produtos sorteados.
5. **Fila da caixa**: o cliente vai para uma caixa e o estado passa para espera/atendimento.
6. **Pagamento**: o tempo da caixa e calculado com base nos produtos do cliente.
7. **Saida**: o cliente abandona o supermercado, o estado passa para out e o cliente vai para o historico.

---

## 6. Regras de Negocio Planeadas
- **Uteis** e **Relogio** sao modulos auxiliares para evitar repetir codigo e centralizar funcoes usadas varias vezes.
- O **universo de clientes** deve ser carregado para memoria antes da simulacao.
- Sempre que um cliente entra, deve ser escolhido aleatoriamente desse universo.
- O numero de produtos por cliente deve ser aleatorio e esse valor define gasto e tempos.
- **Abertura automatica de caixa** quando filas excedem limite (`MAX_FILA`) ou tempo medio de espera.
- **Fecho automatico de caixa** quando procura baixa (`MIN_FILA`) e sem impacto operacional.
- **Gestao de ofertas** para mitigar insatisfacao em esperas elevadas.
- **Atualizacao continua de indicadores** para analise de desempenho.

---

## 7. Proximos Passos Prioritarios
### Prioridade 1 (bloqueante)
- Corrigir `Supermercado.h/c` para ficar coerente com o fluxo real do projeto.
- Fechar implementacao de `Pessoa.h/c` alinhando cliente, historico e produtos sorteados.
- Criar e estabilizar `Caixa.h/c` (criacao, fila, remocao, destruicao, metricas).

### Prioridade 2
- Ligar o carregamento de configuracao, produtos e funcionarios.
- Garantir que a entrada de clientes atualiza os estados corretos.
- Garantir que a saida de clientes remove da lista de ativos e passa para historico.

### Prioridade 3
- Implementar simulacao core:
  - entrada ciclica de clientes;
  - transicao compra -> fila;
  - processamento de atendimento por caixa;
  - aplicacao da regra de oferta.

### Prioridade 4
- Expandir menu principal e consultas (listar, pesquisar, estatisticas).
- Gerar relatorio final detalhado e historico (ex.: CSV).

---

## 8. Entregaveis Esperados
- Sistema funcional de simulacao de caixas em C.
- Arquitetura modular consistente e extensivel.
- Relatorio tecnico com:
  - descricao da implementacao;
  - resultados de simulacao;
  - estatisticas principais;
  - conclusoes e melhorias futuras.

---

## 9. Riscos e Atencoes
- Dependencia entre modulos (`Pessoa` <-> `Caixa` <-> `Supermercado`) pode criar bloqueios se interfaces mudarem sem alinhamento.
- Necessidade de validar bem alocacao/libertacao de memoria para evitar leaks.
- Importante manter consistencia dos dados carregados dos ficheiros externos.
- O ficheiro `Supermercado.c` atual tem erros estruturais e de sintaxe que impedem uma simulacao fiavel.
- O ciclo completo do cliente ainda nao esta fechado no codigo atual, por isso a documentacao do fluxo deve ser seguida como referencia.

---

## 10. Conclusao
O projeto esta bem direcionado e com base modular adequada. A consolidacao das fases tecnicas em torno de `Pessoa`, `Caixa` e `Supermercado` e o passo determinante para concluir a simulacao completa. Com os proximos passos priorizados acima, o sistema fica pronto para demonstracao, analise de desempenho e entrega final.
