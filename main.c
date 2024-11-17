#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct nod
{
    void *info;
    struct nod *prox;
    struct nod *ant;
} Nod;
typedef struct listad
{
    Nod *ini, *fim;
} Listad;

typedef struct pagina
{
    int folha;
    int qtdeChaves;
    struct pagina *pai;
    Listad *listaChaves;
    struct pagina *direita;
} Pagina;
typedef struct chave
{
    int valorChave;
    struct pagina *filho;
} Chave;
typedef struct arvoreb
{
    struct pagina *raiz;
    int ordem;
    int altura;
} Arvoreb;

Listad *cria_listad()
{
    Listad *L = (Listad *)malloc(sizeof(Listad));
    L->ini = L->fim = NULL;
    return L;
}

Nod *cria_nod(void *info)
{
    Nod *novo = (Nod *)malloc(sizeof(Nod));
    novo->info = info;
    novo->ant = novo->prox = NULL;
    return novo;
}

void insere_inicio_listad(void *info, Listad *L)
{
    Nod *novo = cria_nod(info);

    if (L->ini == NULL)
    {
        L->ini = L->fim = novo;
    }
    else
    {
        novo->prox = L->ini;
        L->ini->ant = novo;
        L->ini = novo;
    }
}

void insere_fim_listad(void *info, Listad *L)
{
    Nod *novo = cria_nod(info);

    if (L->ini == NULL)
    {
        L->ini = L->fim = novo;
    }
    else
    {
        novo->ant = L->fim;
        L->fim->prox = novo;
        L->fim = novo;
    }
}

Listad *libera_listad(Listad *L)
{
    Nod *aux;

    while (L->ini != NULL)
    {
        aux = L->ini;
        L->ini = L->ini->prox;
        free(aux);
    }
    free(L);
    return NULL;
}

Nod *remove_inicio_listad(Listad *L)
{
    Nod *aux = NULL;
    if (L != NULL && L->fim != NULL) // caso haja elemento
    {
        aux = L->ini;
        if (L->ini == L->fim)
        {
            L->ini = L->fim = NULL;
        }
        else
        {
            L->ini = L->ini->prox;
            L->ini->ant = NULL;
        }
    }
    return aux;
}

Nod *remove_fim_listad(Listad *L)
{
    Nod *aux;
    if (L->fim != NULL)
    {
        aux = L->fim;

        if (L->ini != L->fim)
        {
            L->fim = L->fim->ant;
            L->fim->prox = NULL;
        }

        else
        {
            L->ini = L->fim = NULL;
        }
        aux->prox = aux->ant = NULL;
    }
    return aux;
}

Listad *divide_listad(Listad *L, int qtde_elementos_primeira_lista)
{
    Listad *L2 = cria_listad();
    Nod *aux = L->ini;
    int i = 0;
    while (i < qtde_elementos_primeira_lista)
    {
        i++;
        aux = aux->prox;
    }
    L2->ini = aux;
    L2->fim = L->fim;
    L->fim = aux->ant;
    L2->ini->ant = NULL;
    L->fim->prox = NULL;
    return L2;
}

Arvoreb *cria_arvoreb(int ordem);
Pagina *cria_pagina();
Chave *cria_chave(int valor);
void em_ordem(Pagina *raiz);
void insere_arvoreb(Arvoreb *T, int valor);
Pagina *encontra_folha(Pagina *raiz, int valor);
Pagina *divide(Pagina *pagina_dividir);
void insere_chave_pagina(Pagina *folha, Chave *ch);
Chave *retira_ultima_chave(Pagina *folha);
Pagina *cria_nova_raiz(Pagina *folha, Pagina *nova_pagina, Chave *ch);
Chave *retira_primeiro(Pagina *folha);
Pagina *getFilho(Nod *aux);
void busca (Pagina *raiz,int num);

int main()
{
    int num, ordem, suss;
    struct arvoreb *arvore = NULL;
    scanf("%i", &ordem);
    arvore = cria_arvoreb(ordem);
    do
    {
        scanf("%i", &num);
        if (num != -1)
        {
            insere_arvoreb(arvore, num);
        }
    } while (num != -1);
    scanf("%i",&suss);
    //printf("\n");
    busca(arvore->raiz,suss);
    //printf("\n");

    return 0;
}

Arvoreb *cria_arvoreb(int ordem)
{
    Arvoreb *T = malloc(sizeof(Arvoreb));
    T->raiz = NULL;
    T->ordem = ordem;
    T->altura = 0;
    return T;
}

Pagina *cria_pagina()
{
    Pagina *p = malloc(sizeof(Pagina));
    p->folha = 1; // eh folha
    p->qtdeChaves = 0;
    p->pai = NULL;
    p->listaChaves = cria_listad();
    p->direita = NULL;

    return p;
}

Chave *cria_chave(int valor)
{
    Chave *ch = malloc(sizeof(Chave));
    ch->valorChave = valor;
    ch->filho = NULL;
    return ch;
}

int getChave(Nod *aux)
{
    return ((Chave *)aux->info)->valorChave;
}

Pagina *getFilho(Nod *aux)
{
    return ((Chave *)aux->info)->filho;
}

void insere_ordenado(Listad *L, Chave *ch)
{

    Nod *aux = L->ini;
    Nod *novo_no;
    if (L->ini == NULL)
    {
        insere_inicio_listad(ch, L);
    }
    else
    {
        if (ch->valorChave < getChave(L->ini))
            insere_inicio_listad(ch, L);
        else if (ch->valorChave > getChave(L->fim))
            insere_fim_listad(ch, L);
        else
        {
            while (ch->valorChave > getChave(aux))
                aux = aux->prox;
            novo_no = cria_nod(ch);
            novo_no->ant = aux->ant;
            novo_no->prox = aux;
            aux->ant = novo_no;
            novo_no->ant->prox = novo_no;
        }
    }
}

void insere_arvoreb(Arvoreb *T, int valor)
{
    Pagina *pag_aux = T->raiz;
    int tem_valor_para_inserir = 1;
    Chave *ch = cria_chave(valor);

    Pagina *folha = encontra_folha(pag_aux, valor);
    Pagina *nova_pagina;

    if (folha == NULL)
    {
        T->raiz = cria_pagina();
        insere_chave_pagina(T->raiz, ch);
    }
    else
    {
        while (tem_valor_para_inserir)
        {
            insere_chave_pagina(folha, ch); /// no futuro vai ser uma chave
            if (folha->qtdeChaves < T->ordem)
            {
                tem_valor_para_inserir = 0;
            }
            else // estorou a pagina
            {
                nova_pagina = divide(folha);
                ch = retira_ultima_chave(folha);
                if (T->raiz == folha)
                {
                    T->raiz = cria_nova_raiz(folha, nova_pagina, ch);
                    tem_valor_para_inserir = 0;
                }
                else
                    folha = folha->pai;
            }
        }
    }
}

Pagina *encontra_folha(Pagina *raiz, int valor)
{
    Pagina *aux_pagina = raiz;

    if (raiz == NULL)
        return NULL;
    else
    {
        while (aux_pagina->folha != 1)
        {
            Nod *aux_lista = aux_pagina->listaChaves->ini;
            while (aux_lista != NULL && valor > getChave(aux_lista))
            {
                aux_lista = aux_lista->prox;
            }
            if (aux_lista == NULL)
                aux_pagina = aux_pagina->direita;
            else
                aux_pagina = getFilho(aux_lista);
        }
        return aux_pagina;
    }
}

void insere_chave_pagina(Pagina *folha, Chave *ch)
{
    insere_ordenado(folha->listaChaves, ch);
    folha->qtdeChaves++;
}

Pagina *divide(Pagina *pagina_dividir)
{

    Pagina *nova_pagina = cria_pagina();
    // a função ceil arrenda para cima (biblioteca math.h)
    int qtde_pagina_dividir = ceil(pagina_dividir->qtdeChaves / 2.0);

    Listad *nova_lista = divide_listad(pagina_dividir->listaChaves,qtde_pagina_dividir);
    nova_pagina->listaChaves = nova_lista;
    nova_pagina->qtdeChaves =
        pagina_dividir->qtdeChaves - qtde_pagina_dividir;
    nova_pagina->pai = pagina_dividir->pai;
    nova_pagina->folha = pagina_dividir->folha;
    nova_pagina->direita = pagina_dividir->direita;

    pagina_dividir->qtdeChaves = qtde_pagina_dividir;

    Chave *ultima_chave = pagina_dividir->listaChaves->fim->info;

    if (pagina_dividir->pai != NULL)
    {

        // O filho da chave seguinte a ponta para o nova página
        Nod *aux = pagina_dividir->pai->listaChaves->ini;
        while (aux != NULL && // nesse caso o ponteiro do filho é o da direita
               getFilho(aux) != pagina_dividir)
            aux = aux->prox;
        if (aux != NULL)
            ((Chave *)aux->info)->filho = nova_pagina;
        else
            pagina_dividir->pai->direita = nova_pagina;
    }

    if (pagina_dividir->folha == 0)
    {
        Nod *aux = nova_pagina->listaChaves->ini;
        while (aux != NULL)
        {
            getFilho(aux)->pai = nova_pagina;
            aux = aux->prox;
        }
        nova_pagina->direita->pai = nova_pagina;
        pagina_dividir->direita = ultima_chave->filho;
    }
    // O filho da chave que subiu aponta para a página que já existia
    ultima_chave->filho = pagina_dividir;
    return nova_pagina;
}

void em_ordem(Pagina *raiz)
{
    Nod *aux = NULL;
    if (raiz != NULL)
    {
        aux = raiz->listaChaves->ini;
        while (aux != NULL)
        {
            em_ordem(((Chave *)aux->info)->filho);
            printf("%d ", ((Chave *)aux->info)->valorChave);
            aux = aux->prox;
        }
        em_ordem(raiz->direita);
    }
}

Chave *retira_ultima_chave(Pagina *folha)
{
    Nod *ultimo_no = remove_fim_listad(folha->listaChaves);
    Chave *ch = ultimo_no->info;
    free(ultimo_no);
    folha->qtdeChaves--;
    return ch;
}

Chave *retira_primeiro(Pagina *folha)
{
    Nod *primeiro = remove_inicio_listad(folha->listaChaves);
    Chave *ch = primeiro->info;
    free(primeiro);
    folha->qtdeChaves--;
    return ch;
}

Pagina *cria_nova_raiz(Pagina *folha, Pagina *nova_pagina, Chave *ch)
{
    Pagina *nova_raiz = cria_pagina();
    nova_raiz->folha = 0;
    insere_chave_pagina(nova_raiz, ch);
    ch->filho = folha;
    nova_raiz->direita = nova_pagina;
    nova_pagina->pai = nova_raiz;
    ch->filho->pai = nova_raiz;

    return nova_raiz;
}

void busca (Pagina *raiz,int num) {
    Pagina *folha = encontra_folha(raiz,num);    
    Nod *aux = folha->listaChaves->fim;

    if(getChave(aux) == num && getFilho(aux) == NULL) {
        printf("-1\n");
    }
    else {
        Pagina *filho = NULL;
        Nod *aux_2 = folha->listaChaves->ini;
        while(aux_2 != NULL && getChave(aux_2) != num) {
            aux_2 = aux_2->prox;
            filho = getFilho(aux_2);
        }        
        if(filho != NULL)
        {
            printf("%i\n",getChave(filho->listaChaves->ini));
        }
        else 
        {
            Pagina *pai = folha->pai;
            Nod *aux_pai = pai->listaChaves->ini;
            while(aux_pai != NULL && getChave(aux_pai) != num)
            {
                aux_pai = aux_pai->prox;
            }
            if(aux_pai->prox == NULL)
            {
                printf("%i\n",getChave(pai->direita->listaChaves->ini));
            }
            else
            {
                printf("%i",getChave(aux_pai->prox));
            }
        }
    }
}
//fiz junto com a Isa e Erica
/*void busca (Pagina *raiz,int num) {
    Pagina *folha = encontra_folha(raiz,num);
    Nod *aux = folha->listaChaves->fim;

    if(getChave(aux) == num) {
        Pagina *pai = folha->pai;
        Nod *prox_aux = pai->listaChaves->ini->prox;
        Pagina *prox_p = getFilho(prox_aux);
        Nod *num = prox_p->listaChaves->ini;
        printf("%i ",getChave(num));
    }
    else {
        Nod *aux_2 = folha->listaChaves->ini;
        while (aux_2 != NULL && getChave(aux_2) != num) {
            aux_2 = aux_2->prox;
        }
        printf("%i ",getChave(aux_2->prox));
    }
}*/


