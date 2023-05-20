#include "../headers/util.hpp"

/*=============================================FUNÇÕES TABELA HASH=============================================*/

tabela_hash_t tabela_hash_criar() { //função para criar a tabela hash
    tabela_hash = (tabela_hash_t)malloc(sizeof(Posicao_tabela_hash *) * (tabela_hash_tam));
    int c = 0;
    while (c < tabela_hash_tam) {
        tabela_hash[c] = NULL;
        c++;
    }
    return tabela_hash;
}

bool posicao_tabela_hash_adicionar(Registro_Arquivo *p_registro, Posicao_tabela_hash **posicao_hash) { //insere na posição da tabela hash
    Posicao_tabela_hash *item_novo = (Posicao_tabela_hash *)malloc(sizeof(Posicao_tabela_hash)); // Aloca o novo item
    if (item_novo == NULL)
        return false; // Falta memória?
    item_novo->endereco_registro = p_registro; // Seta o novo item
    item_novo->prox = *posicao_hash;           // O próximo do novo item será a lista
    *posicao_hash = item_novo;                  // Aqui, estamos mudando o ponteiro da lista
    return true;
}

int tabela_hash_funcao(Registro_Arquivo *p_registro) { //função hash que retorna o índice - posicao a ser inserida na tabela
    return (p_registro->ID % tabela_hash_tam);
}

bool escreve_no_arquivo(int indice, Registro_Arquivo *p_registro, std::ofstream& saida_hash) { //escreve no arquivo de dados os valores extraídos
    Posicao_tabela_hash *auxiliar = tabela_hash[indice];
    if (auxiliar) {
        while (auxiliar->endereco_registro != NULL) {
            if (auxiliar->endereco_registro->ID == p_registro->ID) {
                saida_hash.write(reinterpret_cast<const char *>(auxiliar->endereco_registro), sizeof(Registro_Arquivo));
                return true;
            }
            auxiliar = auxiliar->prox;
        }
    }
    return false;
}

bool tabela_hash_adicionar(Registro_Arquivo *p_registro, tabela_hash_t tabela_hash) { //adiciona na tabela
    int indice = tabela_hash_funcao(p_registro);
    if (posicao_tabela_hash_adicionar(p_registro, &tabela_hash[indice])) {
        return true;
    } else
        return false;
}

void Posicao_tabela_hash_escreve_no_final(Posicao_tabela_hash *posicao, std::ofstream& saida_hash) { //escreve no arquivo de saida que contém a tabela hash
    if (posicao) {
        Posicao_tabela_hash *aux = posicao;
        saida_hash.write(reinterpret_cast<const char *>(aux->endereco_registro), sizeof(Registro_Arquivo));
        while (aux->prox != NULL) {
            aux = aux->prox;
            saida_hash.write(reinterpret_cast<const char *>(aux->endereco_registro), sizeof(Registro_Arquivo));
        }
    }
}

void tabela_hash_escreve_final(tabela_hash_t tabela_hash,std::ofstream& saida_hash) { //função principal que escreve na tabela hash, chama a função acima
    int c = 0;
    while (c < tabela_hash_tam) {
        Posicao_tabela_hash_escreve_no_final(tabela_hash[c], saida_hash);
        c++;
    }
}

/*=============================================FUNÇÕES DA LEITURA E EXTRAÇÃO DE DADOS DO ARQUIVO CSV=============================================*/

vector<string> verifica_valores_nulos(vector<string> campos, int contador_principal) { //verifica se existe valores vazio e aplica nulo nos campos
    vector<string> campos_saida;
    int contador = 0;
    while (contador < 7) {
        if (contador == 0) {
            string str_aux = to_string(contador_principal);
            campos_saida.push_back(str_aux);
        }
        if (contador == 1 || contador == 3) {
            campos_saida.push_back("0");
        } else {
            campos_saida.push_back("NULL");
        }
        contador++;
    }
    return campos_saida;
}

vector<string> aplica_verificacao_campos_nulos(vector<string> campos, int posicao) { //função principal para verificar os campos nulos
    if (campos.size() < 7) {
        campos = verifica_valores_nulos(campos, posicao);
    }
    return campos;
}

void le_arquivo(std::ifstream &arquivo) { //lê o arquivo de entrada, armazena em um buffer auxiliar e salva em elementos
    std::string buffer_linha;
    vector<string> campos;

    std::regex padrao("\"((?:[^\"]|\"\")*)\"");
    std::smatch matches;

    Registro_Arquivo *p_registro;
    int posicao = 0;

    while (!arquivo.eof()) { //extrai os dados do arquivo
        getline(arquivo, buffer_linha);
        cout << "===================================================================\n";
        std::sregex_iterator iter(buffer_linha.begin(), buffer_linha.end(), padrao);
        std::sregex_iterator end;

        while (iter != end) {
            std::string campo = (*iter)[1].str();
            campo = std::regex_replace(campo, std::regex("\"\""), "\""); // Substitui as ocorrências de "" por "

            campos.push_back(campo);
            ++iter;
        }
        //cout << "tamanho campos " << campos.size() << "\n";
        cout << "LENDO ARQUIVO ID:" << campos[0] << "\n";
        /*
        int i=0;
        for (const std::string& campo : campos) {
            std::cout << "campos[" << i << "]:" << campo << std::endl;
            i++;
        }*/
        campos = aplica_verificacao_campos_nulos(campos, posicao);

        p_registro = (Registro_Arquivo *) malloc(sizeof(Registro_Arquivo));

        p_registro->ID = stoi(campos[0]); //set id -----------------------------------------INT
        strncpy((*p_registro).titulo, campos[1].c_str(), 300 - 1); //set titulo
        p_registro->ano = stoi(campos[2]); //set ano  -----------------------------------------INT
        strncpy((*p_registro).autores, campos[3].c_str(), 150 - 1); //set autores
        p_registro->citacoes = stoi(campos[4]); //set citacoes  -----------------------------------------INT
        strncpy((*p_registro).atualizacoes, campos[5].c_str(), 25 - 1); //set atualizações
        strncpy((*p_registro).Snippet, campos[6].c_str(), 1024 - 1); // set snippet

        tabela_hash_adicionar(p_registro, tabela_hash);
        //cout << "chegou ao final!!!\n";
        campos.clear();
        posicao++;
    }
}

int main(int argc, char *argv[]) { //main
    /*=========================================ABERTURA DE ARQUIVOS===========================================*/
    std::string nome_arquivo_entrada;

    if (argc < 2) nome_arquivo_entrada = argv[0];
    else nome_arquivo_entrada = argv[1];

    std::ifstream arquivo(nome_arquivo_entrada);
    std::ofstream saida_hash("arquivo_dados.bin", std::ios::out | std::ios::trunc);

    tabela_hash = tabela_hash_criar();
    le_arquivo(arquivo);
    cout << "arquivo lido com sucesso!" << "\n";
    cout << "salvando tabela hash no arquivo_dados" << "\n";
    tabela_hash_escreve_final(tabela_hash,saida_hash);
    saida_hash.close();
    arquivo.close();
}
