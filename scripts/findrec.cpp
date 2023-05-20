#include "../headers/util.hpp"

int tabela_hash_funcao(int ID) { //retorna a posição da tabela hash
    return ID % tabela_hash_tam;
}

int main(int argc, char *argv[]){
    /*========================== ABRE O ARQUIVO DE DADOS ================================*/
    std::string nome_arquivo_entrada; 

    if (argc < 2) nome_arquivo_entrada = argv[0];
    else nome_arquivo_entrada = argv[1];

    ifstream entrada_hash(nome_arquivo_entrada, ios::binary);

    if (!entrada_hash.is_open()) {
        cout << "Erro ao abrir o arquivo binário." << endl;
        return 0;
    }

    Registro_Arquivo registro_busca;
    /*========================== REALIZA A BUSCA ================================*/
    int id_busca;
    cout << "Digite o ID que deseja buscar na tabela hash: ";
    cin >> id_busca;

    int indice_busca = tabela_hash_funcao(id_busca);

    entrada_hash.seekg(indice_busca * sizeof(Registro_Arquivo));

    bool encontrado = false;
    while (!entrada_hash.eof()) {
        entrada_hash.read(reinterpret_cast<char*>(&registro_busca), sizeof(Registro_Arquivo));
        if (registro_busca.ID == id_busca) {
            encontrado = true;
            break;
        }
    }
    /*========================== IMPRIME O VALOR RETORNADO DA BUSCA ================================*/
    if (encontrado) {
        cout << "Registro encontrado:" << endl;
        cout << "ID: " << registro_busca.ID << endl;
        cout << "Título: " << registro_busca.titulo << endl;
        cout << "Ano: " << registro_busca.ano << endl;
        cout << "Autores: " << registro_busca.autores << endl;
        cout << "Citações: " << registro_busca.citacoes << endl;
        cout << "Atualizações: " << registro_busca.atualizacoes << endl;
        cout << "Snippet: " << registro_busca.Snippet << endl;
    } else {
        cout << "Registro não encontrado." << endl;
    }

    entrada_hash.close();

    return 0;
}
