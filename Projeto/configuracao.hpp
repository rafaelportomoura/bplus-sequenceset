/* Sequence Set - Arquivo de Configuração
 * (tipo de elemento armazenado, tamanho do pacote, etc.)
 *
 * by Joukim, Fevereiro de 2021 - Estrutura de Dados (GCC216)
 * Características desta implementação - fevereiro de 2021:
 * -> usa chave/valor como dado de exemplo, mas que pode
 * -> ser trocado com facilidade, usando typedef
 * ->
 * -> tamanho máximo e mínimo do pacote definido por
 * -> constantes (CAP_PACOTE e MIN_PACOTE)
 */

#include <string.h>

using namespace std;

struct elemento {
    char nome[50];  // utilizar este campo como chave
    char modalidade[5];
    char nivel[5];
    char inicio[15];
    char termino[15];
    char area[60];
};






typedef elemento dado; // a arvore guarda informacoes do tipo Dado
typedef string tipoChave; // tipo da chave usada na comparação

// capacidade máxima e mínima do pacote
const unsigned CAP_PACOTE = 6;
const unsigned MIN_PACOTE = 3;

// posição do elemento do meio do pacote
const unsigned POS_MEIO = 2;

// posição inválida no disco
const int POS_INVALIDA = -1;


const unsigned MAXFILHOS = 7;



ostream& operator<<( ostream& saida, const dado& umDado ) {
    saida << "(" << umDado.nome << "," << umDado.modalidade << "," << umDado.nivel << "," << umDado.inicio << "," << umDado.termino <<
        "," << umDado.area << ")";
    return saida;
}

istream& operator>>( istream& entrada, dado& umDado ) {
    entrada >> umDado.nome >> umDado.modalidade >> umDado.nivel >> umDado.inicio >> umDado.termino >>
        umDado.area;
    return entrada;
}


