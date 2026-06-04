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
    c->tempoAtendimentoDecorrido = 0;
    c->ativa = 1;
    c->motivoFecho[0] = '\0'; 
    c->clientesAtendidos = 0;
    c->produtosVendidos = 0;
    c->revenue = 0.0f;
    
    return c;
}

// Destruir caixa
void DestruirCaixa(Caixa *c) {
    if (!c) return;
    // Liberta APENAS nodos da fila
    // Clientes continuam vivos nas estruturas da simulacao/historico
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
Pessoa* RemoverClienteFila(Caixa *c) {
    if (!c || !c->fila || !c->fila->inicio) return NULL;

    NoCaixa *primeiro = c->fila->inicio;
    Pessoa *cliente = primeiro->cliente;
    c->fila->inicio = primeiro->prox;
    if (c->fila->inicio == NULL) {
        c->fila->fim = NULL;
    }
    c->fila->tamanho--;
    free(primeiro);
    return cliente;
}

// Procurar cliente na fila por ID (retorna ponteiro para Pessoa ou NULL)
Pessoa* ProcurarClienteFila(Caixa *c, const char *id, int *posicao) {

    NoCaixa *atual;
    int pos = 1;

    if(c ==NULL || c-> fila ==NULL || id == NULL)return NULL;

    atual = c->fila->inicio;

    while(atual != NULL){
        if (atual->cliente != NULL && strcmp(atual->cliente->id, id) ==0){
            if (posicao != NULL){
                *posicao = pos;
            }
            return atual->cliente;
        }
        atual = atual->prox;
        pos++;
    }
    return NULL;

}

// Remover cliente específico da fila por ID (muda estado 1→0 e retorna ponteiro para Pessoa ou NULL)
Pessoa* RemoverClienteFilaPorID(Caixa *c, const char *id) {


    NoCaixa *atual;
    NoCaixa *anterior = NULL;
    Pessoa *cliente;

    if(c == NULL || c->fila == NULL || id == NULL) return NULL;
    
    atual = c->fila->inicio;

    while (atual != NULL) {
        if (atual->cliente != NULL && strcmp(atual->cliente->id, id) == 0) {
            
            if (anterior == NULL) {
                c->fila->inicio = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            if (atual == c->fila->fim) {
                c->fila->fim = anterior; // Atualiza fim se necessário
            }
            c->fila->tamanho--;
            
            cliente = atual->cliente; 
            free(atual);
            return cliente;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return NULL; 
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
        c->tempoAtendimentoDecorrido = 0;
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
    c->tempoAtendimentoDecorrido++;
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
    c->tempoAtendimentoDecorrido = 0;
    return cliente;
}

// Obter cliente em atendimento
Pessoa* ObterClienteEmAtendimento(Caixa *c) {
    if (!c) return NULL;
    return c->emAtendimento;
}

// Encontrar caixa com menor fila (ativa) - retorna ponteiro para Caixa ou NULL
Caixa* CaixaComMenorFila(Caixa **caixas, int totalCaixas) {
    Caixa *melhor = NULL;
    int i;

    if (!caixas || totalCaixas <= 0) return NULL;

    for (i = 0; i < totalCaixas; i++) {
        Caixa *atual = caixas[i];
        if (!atual || !atual->ativa) continue;
        if (!melhor || TamanhoDaFila(atual) < TamanhoDaFila(melhor)) {
            melhor = atual;
        }
    }

    return melhor;
}

// Abrir próxima caixa inativa - retorna ponteiro para Caixa ou NULL
Caixa* AbrirProximaCaixa(Caixa **caixas, int totalCaixas) {
    int i;

    if (!caixas || totalCaixas <= 0) return NULL;

    for (i = 0; i < totalCaixas; i++) {
        if (caixas[i] && caixas[i]->ativa == 0 && caixas[i]->motivoFecho[0] == '\0') {
            caixas[i]->ativa = 1;
            return caixas[i];
        }
    }

    return NULL;
}

// Processar caixa (incrementa tempo de atendimento e finaliza cliente se tempo atingido)
int ProcessarCaixa(Caixa *c, Pessoa **clienteFinalizado) {
    if (clienteFinalizado) {
        *clienteFinalizado = NULL;
    }

    if (!c || !c->ativa) return 0;

    if (!ObterClienteEmAtendimento(c)) {
        IniciarAtendimentoProximoCliente(c);
    }

    if (ObterClienteEmAtendimento(c)) {
        IncrementarTempoAtendimento(c);
        if (c->emAtendimento && c->tempoAtendimentoDecorrido >= (int)c->emAtendimento->tempoCaixa) {
            Pessoa *cliente = FinalizarAtendimentoCliente(c);
            if (clienteFinalizado) {
                *clienteFinalizado = cliente;
            }
            return 1;
        }
    }

    return 0;
}
