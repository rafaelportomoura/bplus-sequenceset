#include <iostream>
using namespace std;

// B+ funcinando funcionado 
// constantes para verificacao
const unsigned MAXITENS = 4;
const unsigned MINITENS = 2;
const unsigned MAXFILHOS = 5;
// filhos = 10
// minimo de itens = 4

struct Pokemon {
  unsigned id;
  string nome; // utilizar este campo como chave
  string tipo;
  unsigned total;
  unsigned ataque;
  unsigned defesa;
  unsigned at_esp;
  unsigned def_esp;
  unsigned velocidade;
};


ostream& operator<<(ostream& saida, const Pokemon& Vet) {
	saida << "(" << Vet.id << "," << Vet.nome << "," << Vet.tipo << "," << Vet.total << "," << Vet.ataque <<","<< Vet.defesa <<
			 "," << Vet.at_esp << "," << Vet.def_esp << "," << Vet.velocidade << "," << ")";
	return saida;
}

istream& operator>>(istream& entrada, Pokemon& Vet) {
	entrada >> Vet.id >> Vet.nome >> Vet.tipo >> Vet.total >> Vet.ataque >> Vet.defesa >> 
			   Vet.at_esp >> Vet.def_esp >> Vet.velocidade;
	return entrada;
}

//posicao do elemente meio ( o que sobe para pagina pai em caso de estouro)
const unsigned MEIO = 1;
// meio = 4

typedef Pokemon Dado;

class Noh{
	friend class ArvoreBMais;
	private:
		bool folha;
		unsigned num; //  numero de itens armazenados no noh
		Pokemon itens[MAXITENS];
		Noh* filhos[MAXFILHOS];
		
	public:
		Noh();
		~Noh();
};

Noh :: Noh(){
	num = 0;
	folha = false;
	for (unsigned i = 0; i < MAXFILHOS; i++){
		filhos[i] = nullptr;
	}
	
}
	
Noh :: ~Noh(){
	for (unsigned i = 0; i < num + 1; i++){
		delete filhos[i];
	}
}

class ArvoreBMais{
	private:
		friend ostream& operator<<(ostream& output, ArvoreBMais& arvore);
		Noh* Raiz;
		void percorreEmOrdemAux(Noh* atual , int nivel);
		// busca recursiva
		Noh* buscaAux(Noh* RaizSub , unsigned chave);
		// funcoes auxiliares para insercao de um Dado d no Noh umNoh
		Noh* insereAux(Noh* umNoh , const Pokemon& umItem , Pokemon& itemPromovido);
		Noh* divideNoh(Noh* umNoh , const Pokemon& umItem , Pokemon& itemPromovido);
		void insereEmNohFolhaNaoCheio(Noh* umNoh , Pokemon umItem);
		void insereEmNohIntermediarioNaoCheio(Noh* umNoh , Noh* novoNoh , Pokemon& itemPromovido);
		void imprimirAux(Noh* RaizSub , int nivel);
		
	public:
		ArvoreBMais();
		~ArvoreBMais();
		void insere(const Pokemon& umItem);
		// Busca um item na Arvore a partur da chave
		Dado Busca(unsigned chave);
		void percorreEmOrdem();
		void imprimir();
};

ArvoreBMais :: ArvoreBMais(){
	Raiz = nullptr;
}

ArvoreBMais :: ~ArvoreBMais(){
	delete Raiz;
}

void ArvoreBMais :: insere(const Pokemon& umItem){
	// se a arvore tiver vazia , aloca noh folha para a raiz
	// e insere o objeto na posicao inicial
	if(Raiz == nullptr){
		Raiz = new Noh;
		Raiz->folha = true;
		Raiz->itens[0] = umItem;
		Raiz->num = 1;
	}else{
		// ja tem algo na raiz
		// para prencher item de filho que foi dividido
		Dado itemPromovido;
		//Noh gerado em cado de divisao
		Noh* novoNoh;
		// chama metodo auxiliar recursivo
		novoNoh = insereAux(Raiz , umItem , itemPromovido);
		// Verifica se houve divisao na raiz
		if(novoNoh){
			// se novoNoh nao e nula , houve divisao
			// cria nova raiz apontando cim a antiga raiz e novoNoh com filhos
			cout << "!!! Divisao da Raiz!!!" << endl;
			Noh* antigaRaiz = Raiz;
			Raiz = new Noh;
			Raiz->itens[0] = itemPromovido;
			Raiz->num = 1;
			Raiz->filhos[0] = antigaRaiz;
			Raiz->filhos[1] = novoNoh;
		}
	}
}


Noh* ArvoreBMais :: insereAux(Noh* umNoh ,const Pokemon& umItem , Dado& itemPromovido){
	// Caso umNoh seja Folha , encontre o local para inserir novo item
	if(umNoh->folha){
		//Verifica se umNoh nao esta cheio
		if(umNoh->num < MAXITENS){
			// nao esta cheio , basta inserir
			insereEmNohFolhaNaoCheio(umNoh , umItem);
			return nullptr;
		}else{
			// umNoh esta cheio , precisa dividir
			Noh* novoNoh;
			novoNoh = divideNoh(umNoh , umItem , itemPromovido);
			// verifica quem vai receber o item , se umNoh ou novoNoh
			if(umItem.id <= novoNoh->itens[0].id){
				// item fica em umNoh
				insereEmNohFolhaNaoCheio(umNoh , umItem);
			}else{
				// item fica em novoNoh
				insereEmNohFolhaNaoCheio(novoNoh , umItem);
			}
			return novoNoh;
		}
	}else{
		// noh nao e folha
		// encontra filho que ira receber novo item
		int i = umNoh->num-1;
		
		while(i >= 0 and umNoh->itens[i].id > umItem.id){
			--i;
		}
		Noh* nohAux;
		nohAux = insereAux(umNoh->filhos[i+1] , umItem , itemPromovido);
		// verifica se nao houve estouro no filho
		if(nohAux){
			// se novoNoh nao e nulo , houve divisao
			// antes de inserir o item promovido,
			// verifica se nao deve dividir o noh atual
			// para isso armazena itemPromovido em variavel auxiliar
			Dado itemAux = itemPromovido;
			if(umNoh->num < MAXITENS){
				// umNoh nao esta cheio , so arrumar o estouro do filho)
				insereEmNohIntermediarioNaoCheio(umNoh , nohAux , itemAux);
				return nullptr;
			}else{
				// umNOh esta cheio , divide antes de arrumar o estouro do filho
				Noh* novoNoh;
				novoNoh = divideNoh(umNoh , umItem , itemPromovido);
				// verifica quem vai receber novo noh e item promovido , umNoh ou novoNoh
				if(itemAux.id <= umNoh->itens[MEIO].id){
					// noh e item ficam em umNoh
					insereEmNohIntermediarioNaoCheio(umNoh , nohAux , itemAux);
				}else{
					// noh e item ficam em novoNoh
					insereEmNohIntermediarioNaoCheio(novoNoh , nohAux , itemAux);
				}
				return novoNoh;
			}
		} // caso novoNoh seja nulo , nao precisa fazer nada
	}
	return nullptr; // apenas evitar warning
}

Noh* ArvoreBMais :: divideNoh(Noh* umNoh ,const Pokemon& umItem , Pokemon& itemPromovido){
	cout << "!!Divisao de Noh!!" << endl;
	

	Noh* novoNoh = new Noh;
	novoNoh->folha = umNoh->folha;
	
	// se o noh nao e folha , divide os filhos
	if(not umNoh->folha){
		for (unsigned i = 0; i < MEIO + 1; i++){
			novoNoh->filhos[i] = umNoh->filhos[MEIO + 1 + i];
		}
	}

	for (unsigned i = 0; i < MEIO + 1; i++){
		novoNoh->itens[i] = umNoh->itens[MEIO + 1 + i];
		novoNoh->num++;
		umNoh->num--;
	}
	
	itemPromovido = novoNoh->itens[0];
	
	
	return novoNoh;
}


void ArvoreBMais :: insereEmNohFolhaNaoCheio(Noh* umNoh , Pokemon umItem){
	int i = umNoh->num-1;
	
	while(i >= 0 and umNoh->itens[i].id > umItem.id){
		umNoh->itens[i + 1] = umNoh->itens[i];
		i--;
	}
	
	// insere novo item no local encontrado
	umNoh->itens[i + 1] = umItem;
	umNoh->num++;
}

void ArvoreBMais :: insereEmNohIntermediarioNaoCheio(Noh* umNoh , Noh* novoNoh , Pokemon& itemPromovido){
	int i = umNoh->num-1;
	
	while (i >= 0 and umNoh->itens[i].id > itemPromovido.id){
		// move item uma posicao a direita
		umNoh->itens[i+1] = umNoh->itens[i];
		// move o filho a direita de item uma posicao a direita
		umNoh->filhos[i+2] = umNoh->filhos[i+1];
		i--;
	}
	// insere novo item no local encontrada
	umNoh->itens[i+1] = itemPromovido;
	// insere novo noh (uma posicao a frente no vetor de filhos
	umNoh->filhos[i+2] = novoNoh;
	umNoh->num++;
}


void ArvoreBMais :: percorreEmOrdem(){
	percorreEmOrdemAux(Raiz , 0);
	cout << endl;
}

void  ArvoreBMais :: percorreEmOrdemAux(Noh* umNoh , int nivel){
	unsigned i; 
	if(umNoh == nullptr){
		throw runtime_error("Erro na busca: elemento nao encontrado!") ;
	}
	
	for (i = 0; i < umNoh->num; i++){
		// se noh nao e folha , imprima os dados do filho i
		// antes de imprimir o item i
		if(not umNoh->folha){
			percorreEmOrdemAux(umNoh->filhos[i] , nivel + 1);
		}
		cout << umNoh->itens[i].id << '/' << nivel << ' ';
	}
	
	// imprima os dados do ultimo filho
	if(not umNoh->folha){
		percorreEmOrdemAux(umNoh->filhos[i] , nivel+1);
	}
}


Dado ArvoreBMais :: Busca(unsigned chave){
	Noh* buscado = buscaAux(Raiz , chave);
    Dado itemBuscado[buscado->num];
	
    if(Raiz == nullptr){
        cerr << "Arvore Vazia !" << endl;
    }else{
        for (unsigned i=0; i < buscado->num; i++){
            itemBuscado[i].id = buscado->itens[i].id;
        }
        for (unsigned i=0; i < buscado->num; i++){
            if (itemBuscado[i].id == chave){
                return itemBuscado[i];
            }
        }
    }
    return itemBuscado[buscado->num];
}

Noh* ArvoreBMais :: buscaAux(Noh* RaizSub , unsigned chave){
    unsigned i = 0;
    //percorre RaizSub ate achar um item com chave maior ou igual a procurada
    while((i < RaizSub->num)  and (RaizSub->itens[i].id <= chave))
        i++;
        // returna a posicao anterior (desfazendo o ultimo incremento)
    i--;

    // se e igual , achou o elemento

    if(RaizSub->itens[i].id == chave){
        return RaizSub;
    }else{
        // se noh nao e folha , enta nao tem o elemento buscado
        if(RaizSub->folha){
			throw runtime_error("Erro na busca: elemento nao encontrado!");
        }
        //nao e folha , desce em noh filho a esquerda do item da chave
        //maior que a procurada
        else{
            return buscaAux(RaizSub, chave);
        }
    }
}

ostream& operator<<(ostream& output, ArvoreBMais& arvore) {
	// arvore.percorreEmOrdemAux(arvore.raiz,0);
	arvore.imprimir();
	return output;
}

// imprime formatado seguindo o padrao tree uma avl
void ArvoreBMais :: imprimir(){
	imprimirAux(Raiz, 0);
	cout << endl;

}

void ArvoreBMais :: imprimirAux(Noh* RaizSub , int nivel){
	unsigned i; 
	
	if(RaizSub == nullptr){
		throw runtime_error("Erro na busca: elemento nao encontrado!") ;
	}
	
	for (i = 0; i < RaizSub->num; i++){
		// se noh nao e folha , imprima os dados do filho i
		// antes de imprimir o item i
		if(not RaizSub->folha){
				imprimirAux(RaizSub->filhos[i] , nivel + 1);
			}
			cout << endl;
			cout << "Nivel da Arvore : " <<"[ "<< nivel << " ]" << endl <<"Id : " << RaizSub->itens[i].id << endl
				 << "Nome : " << RaizSub->itens[i].nome << endl
				 << "Tipo : " << RaizSub->itens[i].tipo << endl
				 << "Total : "<< RaizSub->itens[i].total << endl 
				 << "Ataaque : " << RaizSub->itens[i].ataque << endl 
				 << "Defesa : "<< RaizSub->itens[i].defesa << endl
				 << "At esp : " << RaizSub->itens[i].at_esp << endl 
				 << "At def : "<< RaizSub->itens[i].def_esp << endl
				 << "Velocidade : " << RaizSub->itens[i].velocidade 
				 << endl;
	}
	
	if(not RaizSub->folha){
		imprimirAux(RaizSub->filhos[i] , nivel+1);
		
	}
	// imprima os dados do ultimo filho
}

int main(){
	ArvoreBMais minhaArvore;
	Pokemon umPokemon;
	char operacao;
	unsigned id; // chave da tabela 
		
	do{
		try{
			cin >> operacao;
			
			switch (operacao){
				case 'i':{
					cin >> umPokemon;
					minhaArvore.insere(umPokemon);
					break;
				}
				case 'p':{
					minhaArvore.percorreEmOrdem();
					
					break;
				}
				case 'b':{
					cin >> id;
					umPokemon = minhaArvore.Busca(id); 
					cout << umPokemon << endl;
					
					break;
				}
				case 'd':{
					cout << minhaArvore;
					
					break;
				}
				case 's':{
					break;
				}
				default:{
					cout << "Invalido\n";
				}
			}
		}
		catch (exception& e){
			cout << "Erro " << endl;
		}
	} while(operacao != 's');
	
	return 0;
}	


