#include <iostream>
using namespace std;


class Noh {
    friend class ArvoreBMais;
    private:
    bool folha;
    unsigned num; //  numero de itens armazenados no noh
    dado itens[CAP_PACOTE];
    Noh* filhos[MAXFILHOS];

    public:
    Noh();
    ~Noh();
};

Noh::Noh() {
    num = 0;
    folha = false;
    for ( unsigned i = 0; i < MAXFILHOS; i++ ) {
        filhos[i] = nullptr;
    }

}

Noh :: ~Noh() {
    for ( unsigned i = 0; i < num + 1; i++ ) {
        delete filhos[i];
    }
}

class ArvoreBMais {
    private:
    friend ostream& operator<<( ostream& output, ArvoreBMais& arvore );
    Noh* Raiz;
    void percorreEmOrdemAux( Noh* atual, int nivel );
    // busca recursiva
    Noh* buscaAux( Noh* RaizSub, string chave );
    // funcoes auxiliares para insercao de um dado d no Noh umNoh
    Noh* insereAux( Noh* umNoh, const dado& umItem, dado& itemPromovido );
    Noh* divideNoh( Noh* umNoh, dado& itemPromovido );
    void insereEmNohFolhaNaoCheio( Noh* umNoh, dado umItem );
    void insereEmNohIntermediarioNaoCheio( Noh* umNoh, Noh* novoNoh, dado& itemPromovido );
    void imprimirAux( Noh* RaizSub, int nivel );

    public:
    ArvoreBMais();
    ~ArvoreBMais();
    void insere( const dado& umItem );
    // Busca um item na Arvore a partur da chave
    dado Busca( string chave );
    void percorreEmOrdem();
    void imprimir();
};

ArvoreBMais::ArvoreBMais() {
    Raiz = nullptr;
}

ArvoreBMais :: ~ArvoreBMais() {
    delete Raiz;
}

void ArvoreBMais::insere( const dado& umItem ) {
    // se a arvore tiver vazia , aloca noh folha para a raiz
    // e insere o objeto na posicao inicial
    if ( Raiz == nullptr ) {
        Raiz = new Noh;
        Raiz->folha = true;
        Raiz->itens[0] = umItem;
        Raiz->num = 1;
    }
    else {
           // ja tem algo na raiz
           // para prencher item de filho que foi dividido
        dado itemPromovido;
        //Noh gerado em cado de divisao
        Noh* novoNoh;
        // chama metodo auxiliar recursivo
        novoNoh = insereAux( Raiz, umItem, itemPromovido );
        // Verifica se houve divisao na raiz
        if ( novoNoh ) {
            // se novoNoh nao e nula , houve divisao
            // cria nova raiz apontando cim a antiga raiz e novoNoh com filhos
            Noh* antigaRaiz = Raiz;
            Raiz = new Noh;
            Raiz->itens[0] = itemPromovido;
            Raiz->num = 1;
            Raiz->filhos[0] = antigaRaiz;
            Raiz->filhos[1] = novoNoh;
        }
    }
}


Noh* ArvoreBMais::insereAux( Noh* umNoh, const dado& umItem, dado& itemPromovido ) {
    // Caso umNoh seja Folha , encontre o local para inserir novo item
    if ( umNoh->folha ) {
        //Verifica se umNoh nao esta cheio
        if ( umNoh->num < CAP_PACOTE ) {
            // nao esta cheio , basta inserir
            insereEmNohFolhaNaoCheio( umNoh, umItem );
            return nullptr;
        }
        else {
                   // umNoh esta cheio , precisa dividir
            Noh* novoNoh;
            novoNoh = divideNoh( umNoh, itemPromovido );
            // verifica quem vai receber o item , se umNoh ou novoNoh
            if ( strcmp( umItem.nome, novoNoh->itens[0].nome ) <= 0 ) {
                // item fica em umNoh
                insereEmNohFolhaNaoCheio( umNoh, umItem );
            }
            else {
                           // item fica em novoNoh
                insereEmNohFolhaNaoCheio( novoNoh, umItem );
            }
            return novoNoh;
        }
    }
    else {
           // noh nao e folha
           // encontra filho que ira receber novo item
        int i = umNoh->num - 1;

        while ( i >= 0 && strcmp( umNoh->itens[i].nome, umItem.nome ) > 0 ) {
            --i;
        }
        Noh* nohAux;
        nohAux = insereAux( umNoh->filhos[i + 1], umItem, itemPromovido );
        // verifica se nao houve estouro no filho
        if ( nohAux ) {
            // se novoNoh nao e nulo , houve divisao
            // antes de inserir o item promovido,
            // verifica se nao deve dividir o noh atual
            // para isso armazena itemPromovido em variavel auxiliar
            dado itemAux = itemPromovido;
            if ( umNoh->num < CAP_PACOTE ) {
                // umNoh nao esta cheio , so arrumar o estouro do filho)
                insereEmNohIntermediarioNaoCheio( umNoh, nohAux, itemAux );
                return nullptr;
            }
            else {
                           // umNOh esta cheio , divide antes de arrumar o estouro do filho
                Noh* novoNoh;
                novoNoh = divideNoh( umNoh, itemPromovido );
                // verifica quem vai receber novo noh e item promovido , umNoh ou novoNoh
                if ( strcmp( itemAux.nome, umNoh->itens[POS_MEIO].nome ) <= 0 ) {
                    // noh e item ficam em umNoh
                    insereEmNohIntermediarioNaoCheio( umNoh, nohAux, itemAux );
                }
                else {
                                   // noh e item ficam em novoNoh
                    insereEmNohIntermediarioNaoCheio( novoNoh, nohAux, itemAux );
                }
                return novoNoh;
            }
        } // caso novoNoh seja nulo , nao precisa fazer nada
    }
    return nullptr; // apenas evitar warning
}

Noh* ArvoreBMais::divideNoh( Noh* umNoh, dado& itemPromovido ) {


    Noh* novoNoh = new Noh;
    novoNoh->folha = umNoh->folha;

    // se o noh nao e folha , divide os filhos
    if ( !umNoh->folha ) {
        for ( unsigned i = 0; i < POS_MEIO + 1; i++ ) {
            novoNoh->filhos[i] = umNoh->filhos[POS_MEIO + 1 + i];
        }
    }

    for ( unsigned i = 0; i < POS_MEIO + 1; i++ ) {
        novoNoh->itens[i] = umNoh->itens[POS_MEIO + 1 + i];
        novoNoh->num++;
        umNoh->num--;
    }

    itemPromovido = novoNoh->itens[0];


    return novoNoh;
}


void ArvoreBMais::insereEmNohFolhaNaoCheio( Noh* umNoh, dado umItem ) {
    int i = umNoh->num - 1;

    while ( i >= 0 && strcmp( umNoh->itens[i].nome, umItem.nome ) > 0 ) {
        umNoh->itens[i + 1] = umNoh->itens[i];
        i--;
    }

    // insere novo item no local encontrado
    umNoh->itens[i + 1] = umItem;
    umNoh->num++;
}

void ArvoreBMais::insereEmNohIntermediarioNaoCheio( Noh* umNoh, Noh* novoNoh, dado& itemPromovido ) {
    int i = umNoh->num - 1;

    while ( i >= 0 && strcmp( umNoh->itens[i].nome, itemPromovido.nome ) > 0 ) {
        // move item uma posicao a direita
        umNoh->itens[i + 1] = umNoh->itens[i];
        // move o filho a direita de item uma posicao a direita
        umNoh->filhos[i + 2] = umNoh->filhos[i + 1];
        i--;
    }
    // insere novo item no local encontrada
    umNoh->itens[i + 1] = itemPromovido;
    // insere novo noh (uma posicao a frente no vetor de filhos
    umNoh->filhos[i + 2] = novoNoh;
    umNoh->num++;
}


void ArvoreBMais::percorreEmOrdem() {
    if ( Raiz == nullptr ) {
        throw runtime_error( "Erro no percorrimento: arvore vazia!" );
    }
    percorreEmOrdemAux( Raiz, 0 );
    cout << endl;
}

void  ArvoreBMais::percorreEmOrdemAux( Noh* umNoh, int nivel ) {
    unsigned i;
    if ( umNoh != nullptr ) {
        for ( i = 0; i < umNoh->num; i++ ) {
            // se noh nao e folha , imprima os dados do filho i
            // antes de imprimir o item i
            if ( !umNoh->folha ) {
                percorreEmOrdemAux( umNoh->filhos[i], nivel + 1 );
            }
            cout << umNoh->itens[i].nome << '/' << nivel << ' ';
        }

        // imprima os dados do ultimo filho
        if ( !umNoh->folha ) {
            percorreEmOrdemAux( umNoh->filhos[i], nivel + 1 );
        }
    }

}


dado ArvoreBMais::Busca( string chave ) {
    Noh* buscado = buscaAux( Raiz, chave );
    int tamanhoBuscado = buscado->num;
    dado* itemBuscado = new dado[tamanhoBuscado];

    if ( Raiz == nullptr ) {
        cerr << "Arvore Vazia !" << endl;
    }
    else {
        for ( unsigned i = 0; i < buscado->num; i++ ) {
            strcpy( itemBuscado[i].nome, buscado->itens[i].nome );
        }
        for ( unsigned i = 0; i < buscado->num; i++ ) {
            if ( strcmp( itemBuscado[i].nome, chave.c_str() ) == 0 ) {
                return itemBuscado[i];
            }
        }
    }
    return itemBuscado[buscado->num];
}

Noh* ArvoreBMais::buscaAux( Noh* RaizSub, string chave ) {
    unsigned i = 0;
    //percorre RaizSub ate achar um item com chave maior ou igual a procurada
    while ( ( i < RaizSub->num ) && ( strcmp( RaizSub->itens[i].nome, chave.c_str() ) <= 0 ) )
        i++;
        // returna a posicao anterior (desfazendo o ultimo incremento)
    i--;

    // se e igual , achou o elemento

    if ( strcmp( RaizSub->itens[i].nome, chave.c_str() ) == 0 ) {
        return RaizSub;
    }
    else {
           // se noh nao e folha , enta nao tem o elemento buscado
        if ( RaizSub->folha ) {
            throw runtime_error( "Erro na busca: elemento nao encontrado!" );
        }
        //nao e folha , desce em noh filho a esquerda do item da chave
        //maior que a procurada
        else {
            return buscaAux( RaizSub, chave );
        }
    }
}

ostream& operator<<( ostream& output, ArvoreBMais& arvore ) {
    // arvore.percorreEmOrdemAux(arvore.raiz,0);
    arvore.imprimir();
    return output;
}

// imprime formatado seguindo o padrao tree uma avl
void ArvoreBMais::imprimir() {
    if ( Raiz == nullptr ) {
        throw runtime_error( "Erro na impressao: arvore vazia!" );
    }
    imprimirAux( Raiz, 0 );
    cout << endl;

}

void ArvoreBMais::imprimirAux( Noh* RaizSub, int nivel ) {
    unsigned i;
    if ( RaizSub != nullptr ) {

        for ( i = 0; i < RaizSub->num; i++ ) {
            // se noh nao e folha , imprima os dados do filho i
            // antes de imprimir o item i
            if ( !RaizSub->folha ) {
                imprimirAux( RaizSub->filhos[i], nivel + 1 );
            }
            cout << endl;
            cout << "Nivel da Arvore : " << "[ " << nivel << " ]" << endl
                << RaizSub->itens[i]
                << endl;
        }

        if ( !RaizSub->folha ) {
            imprimirAux( RaizSub->filhos[i], nivel + 1 );

        }
    }
}

ArvoreBMais minhaArvore;

