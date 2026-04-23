# Documento Unificado do Projeto
## Sistema de Gestao de Caixas de Supermercado

**Data:** Abril de 2026  
**Objetivo:** Consolidar plano atualizado, proximos passos e relatorio final num unico documento coerente com as ideias do projeto.

---

## 1. Visao Geral do Projeto
O projeto simula a operacao de caixas de um supermercado em C, incluindo:
- entrada de clientes;
- selecao e compra de produtos;
- formacao e gestao de filas por caixa;
- atendimento e pagamento;
- regras automaticas de abertura/fecho de caixas;
- ofertas de produtos em casos de espera excessiva;
- recolha de estatisticas operacionais.

O foco e representar um fluxo realista de atendimento e permitir evolucao do sistema com novas regras de negocio.

---

## 2. Objetivo Funcional
Desenvolver um sistema que permita:
- controlar filas e estado das caixas;
- monitorizar tempos de espera e atendimento;
- aplicar regras automaticas para otimizar o atendimento;
- registar indicadores como clientes atendidos, produtos vendidos, receita e perdas por ofertas.

---

## 3. Estrutura Modular do Projeto
- **main.c**: inicializacao, carregamento de dados, menu e ciclo principal.
- **Supermercado.h/c**: estado global da simulacao, configuracoes, caixas, clientes e estatisticas.
- **Caixa.h/c**: estrutura e operacoes de cada caixa (fila, atendimento, historico e estado).
- **Pessoa.h/c**: representacao de cliente (identificacao, carrinho, custos e tempos).
- **Produto.h/c**: produtos e operacoes associadas.
- **Relogio.h/c**: tempo da simulacao.
- **Uteis.h/c**: funcoes auxiliares e estruturas de suporte.

---

## 4. Estado Atual Consolidado
### Implementado
- Estrutura base de **Produto**.
- Estrutura base de **Relogio**.
- Parte da arquitetura modular ja definida (headers e separacao por ficheiros).

### Em progresso
- Integracao completa de **Pessoa** com o fluxo do supermercado.
- Definicao final e integracao de **Caixa** com **Supermercado**.
- Ajustes de coerencia em campos/structs de **Supermercado**.

### Pendente
- Carregamento integral de ficheiros de dados (`config.txt`, `produtos.txt`, `funcionarios.txt`).
- Simulacao completa do ciclo de cliente.
- Regras automaticas de abrir/fechar caixa.
- Relatorios finais e exportacao de historico.

---

## 5. Fluxo Funcional Previsto (Cliente)
1. **Entrada**: cliente e gerado com perfil e produtos aleatorios.
2. **Compra**: acumulacao de tempo/custo dos produtos no carrinho.
3. **Fila**: cliente vai para a caixa adequada (ex.: menor fila).
4. **Pagamento**: processamento por caixa com base nos produtos.
5. **Oferta (regra de espera)**: se ultrapassar o tempo maximo, oferece-se produto elegivel.
6. **Saida**: cliente conclui atendimento e estatisticas sao atualizadas.

---

## 6. Regras de Negocio Planeadas
- **Abertura automatica de caixa** quando filas excedem limite (`MAX_FILA`) ou tempo medio de espera.
- **Fecho automatico de caixa** quando procura baixa (`MIN_FILA`) e sem impacto operacional.
- **Gestao de ofertas** para mitigar insatisfacao em esperas elevadas.
- **Atualizacao continua de indicadores** para analise de desempenho.

---

## 7. Proximos Passos Prioritarios
### Prioridade 1 (bloqueante)
- Fechar implementacao de `Pessoa.h/c` totalmente alinhada com o fluxo.
- Criar e estabilizar `Caixa.h/c` (criacao, fila, remocao, destruicao, metricas).

### Prioridade 2
- Corrigir e completar `Supermercado.h/c` para integrar caixas e estatisticas sem inconsistencias.
- Implementar carregamento de configuracao, produtos e funcionarios.

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

---

## 10. Conclusao
O projeto esta bem direcionado e com base modular adequada. A consolidacao das fases tecnicas em torno de `Pessoa`, `Caixa` e `Supermercado` e o passo determinante para concluir a simulacao completa. Com os proximos passos priorizados acima, o sistema fica pronto para demonstracao, analise de desempenho e entrega final.
