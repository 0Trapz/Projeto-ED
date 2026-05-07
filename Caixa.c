#include "Caixa.h"

// Funçao de criação de caixa
Caixa* CriarCaixa(int id, const char *operador, int operadorID) {
    if (!operador) {
        printf("ERRO: Operador inválido\n");
        return NULL;
    }
    
    Caixa *c = (Caixa *)malloc(sizeof(Caixa));
    if (!c) {
        printf("ERRO: Falha ao alocar memória para Caixa\n");
        return NULL;
    }
    
    c->id = id;
    
    c->operador = (char *)malloc(strlen(operador) + 1);
    if (!c->operador) {
        printf("ERRO: Falha ao alocar memória para nome do operador\n");
        free(c);
        return NULL;
    }
    strcpy(c->operador, operador);
    
    c->operadorID = operadorID;
    
    // Aloca a fila
    c->fila = (FilaCaixa *)malloc(sizeof(FilaCaixa));
    if (!c->fila) {
        printf("ERRO: Falha ao alocar memória para FilaCaixa\n");
        free(c->operador);
        free(c);
        return NULL;
    }
    
    c->fila->inicio = NULL;
    c->fila->fim = NULL;
    c->fila->tamanho = 0;
    
    c->emAtendimento = NULL;
    c->ativa = 1;
    c->clientesAtendidos = 0;
    c->produtosVendidos = 0;
    c->revenue = 0.0f;
    
    return c;
}

// Destruir caixa
void DestruirCaixa(Caixa *c) {
    if (!c) return;
    // Liberta APENAS nodos da fila
    // Pessoas continuam vivas em UniversoClientes
    if (c->fila) {
        NoCaixa *atual = c->fila->inicio;
        while (atual) {
            NoCaixa *temp = atual;
            atual = atual->prox;
            free(temp);  // Liberta nodo, NÃO a pessoa
        }
        free(c->fila);
    }
    if (c->operador) {
        free(c->operador);
    }
    c->emAtendimento = NULL;
    free(c);
}

// Adicionar cliente à fila (muda estado 0→1)
void AdicionarClienteFila(Caixa *c, Pessoa *cliente) {
    if (!c || !cliente || !c->fila) return;
    // Muda estado de 0 (compra) para 1 (na fila)
    cliente->estado = 1;
    // Cria novo nodo
    NoCaixa *novo = (NoCaixa *)malloc(sizeof(NoCaixa));
    if (!novo) {
        printf("ERRO: Falha ao alocar memória para nodo da fila\n");
        return;
    }
    novo->cliente = cliente;  // REFERÊNCIA (não cópia)
    novo->prox = NULL;
    // ENQUEUE: adiciona no fim
    if (c->fila->fim == NULL) {  // Fila vazia
        c->fila->inicio = novo;
        c->fila->fim = novo;
    } else {
        c->fila->fim->prox = novo;
        c->fila->fim = novo;
    }
    c->fila->tamanho++;
}

// Remover primeiro cliente da fila (muda estado 1→2)
Pessoa* IniciarAtendimentoProximoCliente(Caixa *c) {
    if (!c || !c->fila || !c->fila->inicio) return NULL;
    // DEQUEUE: remove do início
    NoCaixa *primeiro = c->fila->inicio;
    Pessoa *cliente = primeiro->cliente;
    c->fila->inicio = primeiro->prox;
    // Se fila ficou vazia, atualiza fim também
    if (c->fila->inicio == NULL) {
        c->fila->fim = NULL;
    }
    c->fila->tamanho--;
    if (cliente) {
        // Muda estado de 1 (fila) para 2 (em atendimento)
        cliente->estado = 2;
        c->emAtendimento = cliente;
    }
    // Liberta nodo (pessoa continua viva!)
    free(primeiro);
    return cliente;
}

// Obter tamanho da fila
int TamanhoDaFila(Caixa *c) {
    if (!c || !c->fila) return 0;
    return c->fila->tamanho;
}

// Incrementar tempo de atendimento
void IncrementarTempoAtendimento(Caixa *c) {
    if (!c || !c->emAtendimento) return;
    // Incrementa tempo decorrido do cliente em atendimento
}

// Finalizar atendimento (muda estado 2→3)
Pessoa* FinalizarAtendimentoCliente(Caixa *c) {
    if (!c || !c->emAtendimento) return NULL;
    Pessoa *cliente = c->emAtendimento;
    if (cliente) {
        // Muda estado de 2 (atendimento) para 3 (saído)
        cliente->estado = 3;
        // Atualiza estatísticas da caixa
        c->clientesAtendidos++;
        c->produtosVendidos += cliente->numProdutos;
        c->revenue += cliente->totalGasto;
    }
    // Limpa referência (pessoa continua viva!)
    c->emAtendimento = NULL;
    return cliente;
}

// Obter cliente em atendimento
Pessoa* ObterClienteEmAtendimento(Caixa *c) {
    if (!c) return NULL;
    return c->emAtendimento;
}