typedef struct TNoRB {
    int chave;
    int cor;
    struct TNoRB *pai;
    struct TNoRB *esq;
    struct TNoRB *dir;
} TNoRB;

void insert(TNoRB **arvore, TNoRB *pPai, TNoRB **pMain, int k);

TNoRB *gFather(TNoRB *arvore);

TNoRB *uncle(TNoRB *arvore);

void rRotation(TNoRB **no);

void lRotation(TNoRB **no);

void fixRB(TNoRB **arvore, TNoRB **pMain);

void inOrder(TNoRB *raiz);

void dRRotation(TNoRB **no);

void dLRotation(TNoRB **no);

void remove(TNoRB **arvore, int k);

void nBlackIBlackFRed(TNoRB *aRemover, TNoRB **arvore);

void nBlackIBlackFBlack(TNoRB *aRemover, TNoRB **arvore);

void nBlackIRed(TNoRB *aRemover, TNoRB **arvore);

TNoRB *searchNode(TNoRB *arvore, int k);

TNoRB *brother(TNoRB *pai, TNoRB *noAtual);

int lChild(TNoRB *arvore);

TNoRB **gLeft(TNoRB **pGLeft);
