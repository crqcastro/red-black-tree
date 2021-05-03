#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "rbt.h"

#define BLACK 0
#define RED 1

void insert(TNoRB **arvore, TNoRB *pPai, TNoRB **pMain, int k) {
    assert(arvore);

    if (*arvore == NULL) {
        *arvore = (TNoRB*)malloc(sizeof(TNoRB));
        if (*arvore == NULL) return;

        (*arvore)->chave = k;
        (*arvore)->cor = RED;

        (*arvore)->esq = NULL;
        (*arvore)->dir = NULL;
        (*arvore)->pai = pPai;
    } else {

        if (k < (*arvore)->chave) {
            insert(&((*arvore)->esq), *arvore, pMain, k);
            fixRB(&((*arvore)->esq), pMain);
        } else if (k > (*arvore)->chave) {
            insert(&((*arvore)->dir), *arvore, pMain, k);
            fixRB(&((*arvore)->dir), pMain);
        }
    }
    
    if (*arvore == *pMain)
            (*arvore)->cor = BLACK;

}

TNoRB *gFather(TNoRB *arvore) {
    if ((arvore != NULL) && (arvore->pai != NULL))
            return (arvore->pai)->pai;
    else
        return NULL;
}

TNoRB *uncle(TNoRB *arvore) {
    TNoRB *nGFather = gFather(arvore);
    if (nGFather == NULL) return NULL;

    if (arvore->pai == nGFather->esq)
        return nGFather->dir;
    else
        return nGFather->esq;

}

void rRotation(TNoRB **no) {
    assert(no);

    if (*no == NULL) return;

    TNoRB *aux = *no;
    *no = aux->esq;
    aux->esq = (*no)->dir;
    (*no)->dir = aux;

    (*no)->pai = aux->pai;
    if(aux->dir != NULL)
        (aux->dir)->pai = aux;

    aux->pai = *no;

    aux->cor = RED;
    (*no)->cor = BLACK;

}

void lRotation(TNoRB **no) {
    assert(no);

    if (*no == NULL) return;

    TNoRB *aux = *no;
    *no = aux->dir;
    aux->dir = (*no)->esq;
    (*no)->esq = aux;

    (*no)->pai = aux->pai;
    if (aux->esq != NULL)
        (aux->esq)->pai = aux;

    aux->pai = *no;
 
    aux->cor = RED;
    (*no)->cor = BLACK;
}

void fixRB(TNoRB **arvore, TNoRB **pMain) {
    assert(arvore);

    TNoRB *nGFather = NULL;
    TNoRB *nUncle = NULL;

    if((*arvore)->pai != NULL) {
        if((*arvore)->pai->cor == BLACK) return;

        if((*arvore)->cor == RED) {
            nUncle = uncle(*arvore); 
            if (nUncle != NULL && nUncle->cor == RED) {
                nGFather = gFather(*arvore);
                (*arvore)->pai->cor = BLACK;
                nUncle->cor = BLACK;
                if (nGFather->pai != NULL) {
                    nGFather->cor = RED;
                } 
            } else {
                nGFather = gFather(*arvore);
                if (nGFather != NULL) {
                    if ((*arvore)->pai == nGFather->esq) { // filho esquerdo
                        if ((*arvore) == (nGFather->esq)->esq) {
                            if (nGFather->pai != NULL) {
                                if ((nGFather->pai)->esq == nGFather)
                                    rRotation(&((nGFather->pai)->esq));
                                else
                                    rRotation(&((nGFather->pai)->dir));
                            } else {
                                rRotation(pMain);
                            }


                       } else {
                           if (nGFather->pai != NULL) {
                               if ((nGFather->pai)->esq == nGFather) {
                                    dRRotation(&((nGFather->pai)->esq));
                               } else
                                   dRRotation(&((nGFather->pai)->dir));
                           } else { 
                                dRRotation(pMain);
                           }
                      }
                   } else { 
                       if ((*arvore) == ((nGFather->dir)->dir)) {
                           if (nGFather->pai != NULL) {
                               if (((nGFather->pai)->esq) == nGFather) {
                                    lRotation(&((nGFather->pai)->esq));
                               } else
                                   lRotation(&((nGFather->pai)->dir));
                           } else {
                                lRotation(pMain);
                           }

                       } else {
                           if (nGFather->pai != NULL) {
                               if((nGFather->pai)->esq == nGFather) {
                                    dLRotation(&((nGFather->pai)->esq));
                               } else {
                                   dLRotation(&((nGFather->pai)->dir));
                               }
                           } else {
                                    dLRotation(pMain);
                           }
                       }
                }
                }
            }
        }
    }
}
    

void inOrder(TNoRB *raiz) {
    if (raiz == NULL) return;

    inOrder(raiz->esq);
    printf("Mora: %p\nChave: %d, Cor: %d\nPai: %p\nEsq: %p\nDir: %p\n\n", raiz, raiz->chave, raiz->cor, raiz->pai, raiz->esq, raiz->dir);
    inOrder(raiz->dir);
}

void dLRotation(TNoRB **no) {
    assert(no);

    rRotation(&((*no)->dir));
    lRotation(no);

}

void dRRotation(TNoRB **no) {
    assert(no);

    lRotation(&((*no)->esq));
    rRotation(no);
}

void remove(TNoRB **arvore, int k){
    assert(arvore);

    if((*arvore) == NULL) return;

    TNoRB *aRemover = *arvore;

    aRemover = searchNode(aRemover, k);

    if(aRemover == NULL) return;

    if(aRemover->dir == NULL && aRemover->esq == NULL){
        if(aRemover->pai == NULL){
            free(aRemover);
            *arvore = NULL;
            return;
        } else {
            if(aRemover->cor == RED && aRemover->dir == NULL && aRemover->esq == NULL){
                if(lChild(aRemover) == 1){
                    free(aRemover);
                    aRemover->pai->esq = NULL;
                } else {
                    free(aRemover);
                    aRemover->pai->dir = NULL;
                }
                return;
            } else {
                TNoRB *nbrother = brother(aRemover->pai,aRemover);
                    if(nbrother == NULL) return;

                if(aRemover->cor == BLACK && nbrother->cor == BLACK){
                    if(nbrother->dir == NULL && nbrother->esq == NULL){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    } else if(nbrother->esq->cor == BLACK && nbrother->dir->cor == BLACK){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    }
                    else if(nbrother->dir->cor == BLACK && nbrother->esq == NULL){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    }
                    else if(nbrother->esq->cor == BLACK && nbrother->dir == NULL){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    }
                } else if(aRemover->cor == BLACK && nbrother->cor == BLACK && (nbrother->esq->cor == RED || nbrother->dir->cor == RED)){
                    if(nbrother->esq != NULL){
                        nBlackIBlackFRed(aRemover, arvore);
                        return;
                    } else if (nbrother->dir != NULL){
                        nBlackIBlackFRed(aRemover, arvore);
                        return;
                    }
                } else if(aRemover->cor == BLACK && nbrother->cor == RED){
                    nBlackIRed(aRemover, arvore);
                }
            }

            return;
        }
    } else if (aRemover->dir == NULL || aRemover->esq == NULL){
        if(aRemover->dir != NULL){
            aRemover->chave = aRemover->dir->chave;
            free(aRemover->dir);
            aRemover->dir = NULL;
        } else {
            aRemover->chave = aRemover->esq->chave;
            free(aRemover->esq);
            aRemover->esq = NULL;
        }
    } else {
        TNoRB **nSubstituto = gLeft(&(aRemover->esq));
        TNoRB *auxSubstituto = *nSubstituto;

        aRemover->chave = (*nSubstituto)->chave;
        (*nSubstituto)->pai->esq = (*nSubstituto)->esq;


        if(aRemover->cor == RED){
            if(aRemover->esq == NULL && aRemover == aRemover->pai->esq && (aRemover->dir->dir != NULL || aRemover->dir->esq != NULL)){
                if(aRemover->dir->dir != NULL){
                    rRotation(&(aRemover->esq));
                    lRotation(&aRemover);
                }
                if(aRemover->dir->esq != NULL){
                    rRotation(&(aRemover->dir));
                    lRotation(&aRemover);
                }
            } else if(aRemover->esq == NULL && aRemover == aRemover->pai->dir && (aRemover->esq->dir != NULL || aRemover->esq->esq != NULL)){
                if(aRemover->esq->dir != NULL){
                    lRotation(&(aRemover->esq));
                    rRotation(&aRemover);
                }
                if(aRemover->esq->esq != NULL){
                    lRotation(&(aRemover->esq));
                    rRotation(&aRemover);
                }
            }

            aRemover->cor = BLACK;
            if(aRemover->esq != NULL)
                aRemover->esq->cor = RED;
            if(aRemover->dir != NULL)
                aRemover->dir->cor = RED;
        }
        free(auxSubstituto);
    }
}

void nBlackIBlackFRed(TNoRB *aRemover, TNoRB **arvore){
    assert(arvore);

    TNoRB *paiARemover = aRemover->pai;

    if(lChild(aRemover) == 1){
        free(aRemover);
        aRemover->pai->esq = NULL;
    } else {
        free(aRemover);
        aRemover->pai->dir = NULL;
    }

    if(paiARemover->esq == NULL){
        if(paiARemover->pai == NULL){
            if((paiARemover->dir)->dir == NULL){
                rRotation(&((*arvore)->dir));
            }
            lRotation(arvore);
        } 
        else {
            if((paiARemover->dir)->dir == NULL){
                rRotation(&(paiARemover->dir));
            }
            lRotation(&paiARemover);
        }
    } 
    else {
        if(paiARemover->pai == NULL){
            if((paiARemover->esq)->esq == NULL){
                lRotation(&((*arvore)->esq));
            }
            rRotation(arvore);
        } else {
            if((paiARemover->dir)->dir == NULL){
                lRotation(&(paiARemover->esq));
            }
            rRotation(&paiARemover);
        }
    }
    return;
}

void nBlackIBlackFBlack(TNoRB *aRemover, TNoRB **arvore){
    assert(arvore);

    TNoRB *paiARemover = aRemover->pai;
    TNoRB *nbrother = brother(paiARemover, aRemover);

    nBlackIRed(aRemover, arvore);

    paiARemover->cor = BLACK;
    nbrother->cor = RED;
}

void nBlackIRed(TNoRB *aRemover, TNoRB **arvore){
    assert(arvore);

    TNoRB *paiARemover = aRemover->pai;

    nBlackIBlackFRed(aRemover,arvore);

    paiARemover->cor = BLACK;
    if(paiARemover->esq != NULL)
        (paiARemover->esq)->cor = RED;
    if(paiARemover->dir != NULL)
        (paiARemover->dir)->cor = RED;
}

TNoRB *searchNode(TNoRB *arvore, int k){
    if (arvore == NULL) return NULL;

    if (arvore->chave == k) return arvore;

    if (k < arvore->chave)
        return searchNode(arvore->esq, k);
    else
        return searchNode(arvore->dir, k);
}

TNoRB *brother(TNoRB *pai, TNoRB *noAtual){
    if(pai == NULL) return NULL;

    if(pai->dir->chave == noAtual->chave)
        return pai->esq;
    else
        return pai->dir;
}

int lChild(TNoRB *arvore){
    if(arvore->pai->esq == arvore)
        return 1;
    else
        return 0;
}

TNoRB **gLeft(TNoRB **pGLeft){
    assert(pGLeft);
    
    if (*pGLeft == NULL) return NULL;

    if ((*pGLeft)->dir == NULL)
        return pGLeft;
    else
        return gLeft(&((*pGLeft)->dir));
}
