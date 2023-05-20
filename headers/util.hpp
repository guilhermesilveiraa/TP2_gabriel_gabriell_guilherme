#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdbool.h>
#include <locale>
#include <regex>

using namespace std;
/*=============================================TIPO REGISTRO E CRIAÇÃO DA TABELA HASH=============================================*/

typedef struct Registro_Arquivo{ //registro de arquivo
	int ID;
	char titulo[300];
	int ano;
	char autores[150];
	int citacoes;
	char atualizacoes[25];
	char Snippet[2048];
} Registro_Arquivo;

typedef struct Posicao_tabela_hash{ //Posição da tabela hash
	Registro_Arquivo *endereco_registro;
	struct Posicao_tabela_hash *prox;
} Posicao_tabela_hash;

typedef Posicao_tabela_hash **tabela_hash_t; //inicialização da tabela hash
int tabela_hash_tam = 1000;

tabela_hash_t tabela_hash = NULL;

/*============================================DEFINIÇÕES DAS FUNÇÕES============================================*/
tabela_hash_t tabela_hash_criar();
bool posicao_tabela_hash_adicionar(Registro_Arquivo *p_registro, Posicao_tabela_hash ** posicao_hash);
int tabela_hash_funcao(Registro_Arquivo *p_registro);
bool escreve_no_arquivo(int indice, Registro_Arquivo *p_registro,std::ofstream& saida_hash);
bool tabela_hash_adicionar(Registro_Arquivo *p_registro, tabela_hash_t tabela_hash);
void Posicao_tabela_hash_escreve_no_final(Posicao_tabela_hash *posicao, std::ofstream& saida_hash);
void tabela_hash_escreve_final(tabela_hash_t tabela_hash, std::ofstream& saida_hash);
vector<string> verifica_valores_nulos(vector<string> campos, int contador_principal);
vector<string> aplica_verificacao_campos_nulos(vector<string> campos, int posicao);
void le_arquivo(std::ifstream &arquivo);
