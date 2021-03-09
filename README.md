# B+ e Sequence Set
Repositório usado para hospedar o trabalho da disciplina de Estrutura de Dados da UFLA, sobre árvore B+ e Sequence Set

[Árvores B](https://sites.google.com/ufla.br/arvores-b)  
[Exemplo de manipulação de uma árvore B+](https://campusvirtual.ufla.br/presencial/pluginfile.php/685380/mod_resource/content/2/%C3%81rvore%20B%2B-Exemplo.pdf)

Enunciado da atividade: [Atividade de Implementação - Árvore B+](https://docs.google.com/document/d/1O3eG7bul8HfqKo_c62wB9xS0w_FxLoHRhMA752czwik/edit)

[Download](https://campusvirtual.ufla.br/presencial/mod/resource/view.php?id=469487) do Sequence Set 

Link para entrega da [atividade](https://campusvirtual.ufla.br/presencial/mod/assign/view.php?id=469555)

## Sorteio: 
Grupo  17 - Rafael Porto Vieira de Moura, Rodrigo Faria Melo Freitas, Tales Ribeiro Magalhães  
Estrutura: Bolsistas Produtividade do CNPq na UFLA - Ordem: mínimo de chaves 3, máximo de chaves 6  


**Bolsistas Produtividade do CNPq na UFLA** - campos: Nome, Modalidade, Nível, Início, Término, Área.  
Campo chave da tabela: Nome.   

Estrutura a ser utilizada:   
```c++
struct dado {  
    char nome[50];  // utilizar este campo como chave
    char modalidade[5];
    char nivel[5];
    char inicio[15];
    char termino[15];
    char area[60];
};
```  
[Exemplos de dados](https://dados.ufla.br/dataset/bolsistas-de-produtividade):  
Adelir Aparecida Saczk,PQ,2,01/03/2018,28/02/2021,Química  
Adriano Teodoro Bruzi,PQ,2,01/03/2018,28/02/2021,Agronomia  
Alberto Colombo,PQ,2,01/03/2018,28/02/2021,Engenharia Agrícola   


## Operações 

O arquivo de entradas, veja arquivo teste.input no código fornecido, deverá conter um conjunto de operações válidas na implementação realizada. O arquivo de entradas deverá conter ao menos:  
* 100 operações de inserção;  
* 20 operações de busca;  
* 1 operação de impressão;  
* 1 operação de depuração.  

Observação: os dados inseridos não precisam ser reais ou mesmo terem significado prático, podendo ser utilizados valores como “aaaa”, “abab”, etc. nos campos da estrutura.   
