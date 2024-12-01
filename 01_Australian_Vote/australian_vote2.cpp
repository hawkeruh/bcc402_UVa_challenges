#include <iostream> 
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void leituraInput(vector<string>& nome_candidato, vector<vector<int>>& rank_candidatos);
void criarMatriz(vector<vector<int>>& rank_candidatos);
bool depuracao(vector<string>& nome_candidato, vector<vector<int>>& rank_candidatos);
int encontrarMaior(vector<int>& total_votos, int& indice_candidato);
int encontrarMenor(vector<int>& total_votos, int& indice_candidato);

int main (){

    // leitura de dados
    vector<string> nome_candidato;
    vector<vector<int>> rank_candidatos;
        
    // ler todo input e armazena os dados por referencia no main
    leituraInput(nome_candidato, rank_candidatos);

    return 0;
}


/*
    funções: passar parametros vector como referencia, para alterar o vector dentro do main 
sem precisar retornar o vector na funcao.

*/


// uma função que faz a leitura do input e armazena por referencia
// verificar se a modificacao dos vector por referencia funciona sem usar &
void leituraInput(vector<string>& nome_candidato, vector<vector<int>>& rank_candidatos){

    int num_candidatos, voto, casos;
    string nome;

    // ler numero de casos e candidatos
    cin >> casos;

    for (int j =  0; j < casos; j++){
        
        cin >> num_candidatos;

        cin.ignore();

        // validar a entrada

        if (num_candidatos > 20){
            cout << "Operação inválida." << endl;
            return;
        }

        for (int i = 0; i < num_candidatos; i++){
    
            getline(cin, nome);
            
            if(!nome.empty()){
                // validar a entrada
                if (nome.length() > 80){
                    nome = nome.substr(0,80);
                }

                // inserir nome do candidato no vetor de nomes
                nome_candidato.push_back(nome);
            }

        }

        // ler todos os votos e armazenar
        // para evitar a leitura continua a cada fase e melhorar
        // desempenho do codigo

        criarMatriz(rank_candidatos);

        // bool retorna true se candidato maior 50%
        depuracao(nome_candidato, rank_candidatos);

        // limpar para proximo caso
        nome_candidato.clear();
        rank_candidatos.clear();

        cout << endl;
    }

}

// nao precisou de num_candidatos/quantas colunas
void criarMatriz(vector<vector<int>>& rank_candidatos){
    // uma linha = um voto
    // uma coluna = um candidato
    string linha_voto;
    vector<int> linha_int;
    int votos_limite = 0; // tem um limite de 1000 votos
    int rank;

    // while por que o total de votos/linhas é desconhecido
    // 1000 por que é a primeira condição de parada
    // while (rank_candidatos.size() < 1000){ // loop linhas

    while(getline(cin, linha_voto)){

        // leiura de uma linha = um voto
        //getline(cin, linha_voto);

        // primeira condicao de parada, se o numero de votos exceder 1000
        /* votos_limite++;
        if (votos_limite > 1000){
            break;
        }
        */

        // segunda condicao de parada, se linha vazia
        if (linha_voto.empty()){
            break;
        }


        // armazenar linha por linha, transformar em int
        stringstream ss(linha_voto); 

        // ler cada numero do ss e armazenar
        while (ss >> rank){ // loop colunas
            linha_int.push_back(rank);
        }

        // armazenar linha/voto na matriz 
        rank_candidatos.push_back(linha_int);

        // limpar linha para nao acumular na proxima iteraçao
        linha_int.clear();
    }
}

bool depuracao(vector<string>& nome_candidato, vector<vector<int>>& rank_candidatos){
    
    // CONTAGEM DE VOTOS E ARMAZENAR TOTAL

    while(true){ // condicao de parada: até entrar no if com o return, enquanto isso vai eliminando candidatos

        // um vetor com o total de votos para cada candidato, tamanho de colunas = numero de candidatos
        vector<int> total_votos(nome_candidato.size(),0);

        for (const auto& linha : rank_candidatos) { // para cada linha da matriz
            int menor = linha[0];
            int indice_candidato = 0;

            // loop encontra menor elemento (ex: 1 = primeira posicao no rank) e indice = coluna do candidato
            for (int i = 0; i < linha.size(); ++i) { // Começa do índice 1
                if (linha[i] < menor) {
                    menor = linha[i];
                    indice_candidato = i;
                }
            }

            // quando encontrado o candidato de preferencia, contabilizar
            // validar se posicao valida?? evitar erro
            total_votos[indice_candidato]++;
        }

        // VERIFICAR SE TOTAL DE VOTOS > 50% DOS VOTOS

        int indice_candidato = 0;

        // Percorrer a soma dos votos e achar o candidato com mais votos
        int maior = encontrarMaior(total_votos, indice_candidato);

        // rank_candidatos.size(); == NUMERO TOTAL DE VOTOS
        // maior = NUMERO TOTAL DE VOTOS DO CANDIDATO indice_candidato

        // menor que 50%, cortar candidato com menor quantidade de votos
        int menor = encontrarMenor(total_votos, indice_candidato);

        // cortar candidato da matriz com o ranks e dos nomes registrados
        for(auto& linha : rank_candidatos){
            linha.erase(linha.begin() + indice_candidato);
        }
        nome_candidato.erase(nome_candidato.begin() + indice_candidato);

        if (maior >= rank_candidatos.size()/2.0){ // achou candidato eleito, float caso numero de votos seja impar

            for (int j = 0; j < nome_candidato.size(); j++){ // caso empate maior >= (menor ou igual)
                cout << nome_candidato[j] << endl;
            }

            return true;

        }
    }
}

int encontrarMaior(vector<int>& total_votos, int& indice_candidato){
    
    int maior = total_votos[0]; // comecar do primeiro
    indice_candidato = 0;

    // Percorrer o vetor para encontrar o maior valor e seu índice
    for (int i = 0; i < total_votos.size(); ++i) { 
        if (total_votos[i] > maior) {
            maior = total_votos[i]; // Atualiza o maior valor
            indice_candidato = i;   // Atualiza o índice correspondente
        }
    }

    return maior;
}

int encontrarMenor(vector<int>& total_votos, int& indice_candidato){
    
    int menor = total_votos[0]; // comecar do primeiro
    indice_candidato = 0;

    // Percorrer o vetor para encontrar o maior valor e seu índice
    for (int i = 0; i < total_votos.size(); ++i) { 
        if (total_votos[i] < menor) {
            menor = total_votos[i]; // atualiza o menor valor
            indice_candidato = i;   // atualiza o índice correspondente
        }
    }

    return menor;
}
