#include <climits> 
#include <fstream> 
#include <iostream> 
#include <sstream> 
using namespace std; 
int MAX = 3; 
  
// BP Noh 
class Noh { 
    bool ehFolha; 
    int *chave, tam; 
    Noh** ptr; 
    friend class ArvoreBP; 
  
public: 
    Noh(); 
}; 
  
// BP tree 
class ArvoreBP { 
    Noh* raiz; 
    void insereInterno(int, Noh*, Noh*); 
    Noh* buscaPai(Noh*, Noh*); 
  
public: 
    ArvoreBP(); 
    void buscar(int); 
    void inserir(int); 
    void imprimir(Noh*); 
    Noh* obtemRaiz(); 
}; 
  
// COnstrutor do Noh 
Noh::Noh() 
{ 
    chave = new int[MAX]; 
    ptr = new Noh*[MAX + 1]; 
} 
  
// IInicializa arvore
ArvoreBP::ArvoreBP() 
{ 
    raiz = NULL; 
} 
  
// Buscar elementos
void ArvoreBP::buscar(int x) 
{ 
  
    // Se a arvore estiver vazia...
    if (raiz == NULL) { 
        cout << "Arvore vazia\n"; 
    } 
  
    // Percorrimento para encontrar valor
    else { 
  
        Noh* aux = raiz; 
  
        // Ate o noh folha....
        while (aux->ehFolha == false) { 
  
            for (int i = 0; i < aux->tam; i++) { 
  
                // Se nao encontrar
                if (x < aux->chave[i]) { 
                    aux = aux->ptr[i]; 
                    break; 
                } 
  
                // Se alcancar o fim do noh aux
                if (i == aux->tam - 1) { 
                    aux = aux->ptr[i + 1]; 
                    break; 
                } 
            } 
        } 

        // Percorre o aux (indice, ponteiro) em busca do noh
        for (int i = 0; i < aux->tam; i++) { 
  
            // Se achar, retorna
            if (aux->chave[i] == x) { 
                cout << "Achou\n" << i; //i posicao
                return; 
            } 
        } 
  
        // Se nao, elemento n existe na arvore
        cout << "Nao achou\n"; 
    } 
} 
  
// FUncao para insercao na arvore b+
void ArvoreBP::inserir(int x) 
{ 
  
    // Se raiz for NULA retorna um novo noh.
    if (raiz == NULL) { 
        raiz = new Noh; 
        raiz->chave[0] = x; 
        raiz->ehFolha = true; 
        raiz->tam = 1; 
    } 
  
    // Percorre a arvore
    else { 
        Noh* aux = raiz; 
        Noh* pai; 
  
        // Ate encontrar noh folha
        while (aux->ehFolha == false) { 
  
            pai = aux; 
  
            for (int i = 0; i < aux->tam; i++) { 

                // Se encontrar a posicao em que precisamos inserir... 
                if (x < aux->chave[i]) { 
                    aux = aux->ptr[i]; 
                    break; //usei break memo kkkk se ta no stackoverflow eh vdd
                } 
  
                // If reaches the end 
                if (i == aux->tam - 1) { 
                    aux = aux->ptr[i + 1]; 
                    break; //usei break memo kkkk se ta no stackoverflow eh vdd
                } 
            } 
        } 
  
        if (aux->tam < MAX) { 
            int i = 0; 
            while (x > aux->chave[i] and i < aux->tam) { 
                i++; 
            } 
  
            for (int j = aux->tam; j > i; j--) { 
                aux->chave[j] = aux->chave[j - 1]; 
            } 
  
            aux->chave[i] = x; 
            aux->tam++; 
  
            aux->ptr[aux->tam] = aux->ptr[aux->tam - 1]; 
            aux->ptr[aux->tam - 1] = NULL; 
        } 
  
        else { 
  
            // Cria um novo noh folha 
            Noh* novaFolha = new Noh; 
  
            int nohAux[MAX + 1]; 
  
            // Atualiza o ponteiro (aux) para o novo noh aux 
            for (int i = 0; i < MAX; i++) { 
                nohAux[i] = aux->chave[i]; 
            } 
            int i = 0, j; 
  
            // Percorre para buscar onde sera inserido o novo noh
            while (x > nohAux[i] and i < MAX) { 
                i++; 
            } 
  
            // Atualiza o novo noh auxiliar com o no antecessor
            for (int j = MAX + 1; j > i; j--) { 
                nohAux[j] = nohAux[j - 1]; 
            } 
  
            nohAux[i] = x; 
            novaFolha->ehFolha = true; 
  
            aux->tam = (MAX + 1) / 2; 
            novaFolha->tam = MAX + 1 - (MAX + 1) / 2; 
  
            aux->ptr[aux->tam] = novaFolha; 
  
            novaFolha->ptr[novaFolha->tam] = aux->ptr[MAX]; 
  
            aux->ptr[MAX] = NULL; 
  
            // Atualiza a chave do noh auxiliar a sua anterior 
            for (i = 0; i < aux->tam; i++) { 
                aux->chave[i] = nohAux[i]; 
            } 
  
            // Atualiza a chave do novo noh folha para a do auxiliar
            for (i = 0, j = aux->tam; i < novaFolha->tam; i++, j++) { 
                novaFolha->chave[i] = nohAux[j]; 
            } 
  
            // Se auxiliar for raiz do noh
            if (aux == raiz) { 
  
                // Cria um novo noh
                Noh* novaRaiz = new Noh; 
  
                // Atualiza os campos do noh da arvore b
                novaRaiz->chave[0] = novaFolha->chave[0]; 
                novaRaiz->ptr[0] = aux; 
                novaRaiz->ptr[1] = novaFolha; 
                novaRaiz->ehFolha = false; 
                novaRaiz->tam = 1; 
                raiz = novaRaiz; 
            } 
            else { 
  
                // Chamada recursiva para inserir internamente (?)
                insereInterno(novaFolha->chave[0], 
                               pai, 
                               novaFolha); 
            } 
        } 
    } 
} 
  
// Insercao interna
void ArvoreBP::insereInterno(int x, Noh* aux, Noh* filho) 
{ 
    // se nao estourar...
    if (aux->tam < MAX) { 
        int i = 0; 
  
        // Traverse the filho Noh 
        // Percorre o no filho para encotnrar o aux do noh atual 
        while (x > aux->chave[i] and i < aux->tam) { 
            i++; 
        } 
  
        // Percorre o noh auxiliar e atualiza as chaves atuais com as chaves do no anterior 
        for (int j = aux->tam; j > i; j--) { 
            aux->chave[j] = aux->chave[j - 1]; 
        } 
  
        // Percorre o no auxiliar e atualiza o ponteiro (ptr) atual para o no ptr anterior 
        for (int j = aux->tam + 1; j > i + 1; j--) { 
            aux->ptr[j] = aux->ptr[j - 1]; 
        } 
        aux->chave[i] = x; 
        aux->tam++; 
        aux->ptr[i + 1] = filho; 
    } 
  
    // Se estourar capacidade, partimos o noh
    else { 
  
        // Novo intervalo de noh
        Noh* novoInterno = new Noh; 
        int chaveAux[MAX + 1]; 
        Noh* PtrAux[MAX + 2]; 
  
        // Inserir a lista de chave atual do noh aux para o ptraux
        for (int i = 0; i < MAX; i++) { 
            chaveAux[i] = aux->chave[i]; 
        } 
  
        // Insere a lista atul do ptr de nohAux para PtrAux 
        for (int i = 0; i < MAX + 1; i++) { 
            PtrAux[i] = aux->ptr[i]; 
        } 
  
        int i = 0, j; 
  
        // Percorre para buscar onde o novo noh sera inserido 
        while (x > chaveAux[i] and i < MAX) { 
            i++; 
        } 
  
        // Percorre o Noh chaveAux e atualiza a chave atual pela anterior 
        for (int j = MAX + 1; j > i; j--) {
            chaveAux[j] = chaveAux[j - 1]; 
        } 
  
        chaveAux[i] = x; 
  
        // Percorre o noh chaveAux e atualiza o ptr atual para o noh ptr anterior 
        for (int j = MAX + 2; j > i + 1; j--) { 
            PtrAux[j] = PtrAux[j - 1]; 
        } 
  
        PtrAux[i + 1] = filho; 
        novoInterno->ehFolha = false; 
  
        aux->tam = (MAX + 1) / 2; 
  
        novoInterno->tam = MAX - (MAX + 1) / 2; 
  
        // Inserir novo Noh como um nohInterno 
        for (i = 0, j = aux->tam + 1; i < novoInterno->tam; i++, j++) { 
            novoInterno->chave[i] = chaveAux[j]; 
        } 
  
        for (i = 0, j = aux->tam + 1; i < novoInterno->tam + 1; i++, j++) { 
            novoInterno->ptr[i] = PtrAux[j]; 
        } 
  
        // Se aux eh noh raiz
        if (aux == raiz) { 
  
            // cria uma nova raiz
            Noh* novaRaiz = new Noh; 
  
            // atualiza o valor da chave
            novaRaiz->chave[0] 
                = aux->chave[aux->tam]; 
  
            // atualiza o resto dos campos da arvore
            novaRaiz->ptr[0] = aux; 
            novaRaiz->ptr[1] = novoInterno; 
            novaRaiz->ehFolha = false; 
            novaRaiz->tam = 1; 
            raiz = novaRaiz; 
        } 
  
        else { 
              // chamada recursiva para inserir o dado
            insereInterno(aux->chave[aux->tam], 
                           buscaPai(raiz, 
                                      aux), 
                           novoInterno); 
        } 
    } 
} 
  
// Busca no pai
Noh* ArvoreBP::buscaPai(Noh* aux, Noh* filho) 
{ 
    Noh* pai; 
  
    // Ser aux atinge o fim da arvore
    if (aux->ehFolha or (aux->ptr[0])->ehFolha) { 
        return NULL; 
    } 
  
    // Percorrre o noh atual com todos os filhos 
    for (int i = 0; i < aux->tam + 1; i++) { 
  
        // Atualiza o pai com o noh filho 
        if (aux->ptr[i] == filho) { 
            pai = aux; 
            return pai; 
        } 
  
        // Se n percorre recursivamente para encotnrar o noh filho
        else { 
            pai = buscaPai(aux->ptr[i],filho); 
  
            // Se encontra o pai, entao retorna o noh
            if (pai != NULL) 
                return pai; 
        } 
    } 
  
    // retorna pai
    return pai; 
} 
  
// Fobter a raiz 
Noh* ArvoreBP::obtemRaiz() 
{ 
    return raiz; 
} 
  
// funcao principal nao feita adaptar chave valor
int main() 
{ 
    ArvoreBP Noh; 
  
    // Exemplo de insercao
    Noh.inserir(6); 
    Noh.inserir(16); 
    Noh.inserir(42); 

  
    // Exempl ode busca
    Noh.buscar(16); 
  
    return 0; 
} 