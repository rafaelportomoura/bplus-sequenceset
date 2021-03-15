
#include <fstream>
using namespace std;

fstream le_entrada;


void leArquivo( sequenceset& meuSeqSet ) {
    char operacao;
    dado umDado;
    tipoChave umaChave;
    string area;
    le_entrada.open( "teste.input" );
    do {
        try {
            le_entrada >> operacao;
            switch ( operacao ) {
            case 'i': // inserir
                le_entrada.ignore();
                getline( le_entrada, umaChave );
                strcpy( umDado.nome, umaChave.c_str() );

                le_entrada >> umDado.modalidade;

                le_entrada >> umDado.nivel;

                le_entrada >> umDado.inicio;

                le_entrada >> umDado.termino;

                le_entrada.ignore();
                getline( le_entrada, area );
                strcpy( umDado.area, area.c_str() );

                //cout<<" n"<<umDado.nome<<" m"<<umDado.modalidade<<" n"<<umDado.nivel<<" i"<<umDado.inicio<<" t"<<umDado.termino<<" a"<<umDado.area<<endl;

                meuSeqSet.inserirDado( umDado );
                break;
            case 'b': // buscar
                le_entrada.ignore();
                getline( le_entrada, umaChave );
                umDado = meuSeqSet.buscar( umaChave );
                cout << "Busca: " << umDado.nome << "/" << umDado.area << endl;
                break;
            case 'p': // mostrar estrutura
                meuSeqSet.imprimir();
                break;
            case 'd': // mostrar estrutura
                meuSeqSet.depurar();
                break;
            case 's': // sair
                // será tratado no while
                break;
            default:
                cout << "Opcao invalida!" << endl;
            }
        }
        catch ( runtime_error& e ) {
            cerr << e.what() << endl;
        }
    } while ( operacao != 's' );
    le_entrada.close();
}