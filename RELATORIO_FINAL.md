# Relatório Final do Projeto: Gestão de Caixas de Supermercado

## Objetivo
Desenvolver um sistema para simular e gerir o atendimento em caixas de um supermercado, controlando filas de clientes, abertura/fecho de caixas, tempo de espera, produtos oferecidos e estatísticas gerais.

## Funcionamento Geral
- O supermercado possui múltiplas caixas, cada uma com sua fila de clientes.
- Clientes são gerados aleatoriamente, recebem um conjunto de produtos e passam por etapas: entrada, compra, fila, pagamento e saída.
- O tempo de atendimento depende do número de produtos.
- Se o tempo de espera de um cliente ultrapassar o máximo permitido, um produto pode ser oferecido para reduzir o tempo.
- O sistema abre ou fecha caixas automaticamente conforme a média de espera nas filas.
- Estatísticas de atendimento, produtos vendidos e perdas por ofertas são registradas.

## Estrutura do Projeto
- **main.c**: Inicializa o sistema, carrega dados, exibe menus e coordena a execução.
- **Supermercado.h/c**: Estrutura central, armazena o estado global, configurações, caixas, estatísticas e relógio. Gerencia abertura/fecho de caixas e distribuição de clientes.
- **Caixa.h/c**: Representa cada caixa, controla fila de clientes, status (ativa/inativa), estatísticas e atendimento.
- **Pessoa.h/c**: Representa um cliente, com identificação, produtos, tempos de compra, espera e pagamento.
- **Produto.h/c**: Representa produtos disponíveis, com nome, preço e tempos associados.
- **Relogio.h/c**: Controla o tempo da simulação, permite avançar e consultar o tempo atual.
- **Uteis.h/c**: Funções auxiliares (listas, filas, aleatoriedade, etc).

## Fluxo de um Cliente
1. **Entrada**: Cliente gerado aleatoriamente, recebe produtos aleatórios.
2. **Compra**: Tempo de compra é a soma dos tempos de atendimento dos produtos.
3. **Fila**: Cliente escolhe a caixa com menor fila e aguarda sua vez.
4. **Pagamento**: Tempo de pagamento é a soma dos tempos dos produtos.
5. **Oferta**: Se tempo de espera > MAX_ESPERA, um produto é oferecido ao cliente.
6. **Saída**: Cliente deixa o supermercado, estatísticas são atualizadas.

## Módulos e Funções Principais
- **Produto**: Criar, destruir, copiar, obter produto mais barato.
- **Pessoa**: Criar, destruir, calcular totais, adicionar/remover produtos, gerenciar lista de clientes ativos.
- **Caixa**: Criar, destruir, adicionar/remover clientes, obter tamanho da fila.
- **Supermercado**: Inicializar, abrir/fechar caixas, distribuir clientes, registrar estatísticas.
- **Relógio**: Inicializar, avançar tempo, obter tempo atual.
- **Uteis**: Listas genéricas, funções de aleatoriedade, delays, etc.

## Destaques da Implementação
- Estruturas de dados dinâmicas para filas e listas de clientes/produtos.
- Gerenciamento automático de abertura/fecho de caixas baseado em estatísticas.
- Oferta automática de produto ao cliente que espera demais.
- Registro detalhado de estatísticas de atendimento e perdas.

## Observações Finais
O projeto foi desenvolvido em C, com separação modular dos componentes principais. O código está documentado e preparado para simulação realista do fluxo de clientes em um supermercado, permitindo fácil expansão para novas regras ou estatísticas.

---

**Autores:** [Seu Nome Aqui]
**Data:** Abril de 2026