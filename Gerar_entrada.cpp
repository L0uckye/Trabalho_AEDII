#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
using namespace std;

// Função para gerar um número inteiro aleatório dentro de um intervalo
int gerarNumeroAleatorio(int min, int max, mt19937& gen) {
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Função para gerar um número de ponto flutuante aleatório dentro de um intervalo
float gerarFloatAleatorio(float min, float max, mt19937& gen) {
    uniform_real_distribution<> distrib(min, max);
    return distrib(gen);
}

string to_string_with_precision(float value, int precision) {
    ostringstream out;
    out.precision(precision);
    out << fixed << value;
    return out.str();
}

// Função para gerar um nome aleatório
string gerarNomeAleatorio(mt19937& gen) {
    vector<string> nomes = {"João", "Maria", "Pedro", "Ana", "Carlos", "Beatriz", "Lucas", "Fernanda", "Ricardo", "Sofia"};
    return nomes[gerarNumeroAleatorio(0, nomes.size() - 1, gen)];
}

//Função para gerar um telefone aleatório
string gerarTelefoneAleatorio(mt19937& gen){
    long parte1 = gerarNumeroAleatorio(1000, 9999, gen);
    long parte2 = gerarNumeroAleatorio(1000, 9999, gen);
    return "(88) 9"+to_string(parte1)+"-"+to_string(parte2);
}

// Função para gerar uma especialidade Aleatória dentre as disponíveis
int gerarEspecialidadeAleatoria(mt19937& gen, vector<string> Especialidades){
    return gerarNumeroAleatorio(0, Especialidades.size() - 1, gen);
}

// Função para gerar um sintoma aleatório
string gerarSintomaAleatorio(mt19937& gen) {
    vector<string> sintomas = {"dor de cabeça", "febre", "tosse", "dor abdominal", "náusea", "cansaço", "dor muscular"};
    return sintomas[gerarNumeroAleatorio(0, sintomas.size() - 1, gen)];
}

// Função para gerar uma medicação aleatória
string gerarMedicacaoAleatoria(mt19937& gen) {
    vector<string> medicacoes = {"Paracetamol", "Ibuprofeno", "Dipirona", "Omeprazol", "Amoxicilina", "Antialérgico"};
    return medicacoes[gerarNumeroAleatorio(0, medicacoes.size() - 1, gen)];
}

// Função para gerar as informações de um paciente
string gerarPacienteAleatorio(mt19937& gen, int prioridade, vector<string> Especialidades) { 
    string nome = gerarNomeAleatorio(gen);
    int id = gerarNumeroAleatorio(10000, 99999, gen);
    string telefone = gerarTelefoneAleatorio(gen);
    int idade = gerarNumeroAleatorio(0, 100, gen);
    int prioridade_especifc = gerarNumeroAleatorio(1, prioridade, gen);
    int especialidade = gerarEspecialidadeAleatoria(gen, Especialidades);
    float peso = gerarFloatAleatorio(30.0, 120.0, gen);
    float altura = gerarFloatAleatorio(1.2, 2.0, gen);
    string medicacao = gerarMedicacaoAleatoria(gen);
    string sintomas = gerarSintomaAleatorio(gen);
    string speso = to_string_with_precision(peso, 2);
    string saltura = to_string_with_precision(altura, 2);
    // Criando a linha em formato CSV
    string linhaCSV = nome +','+ to_string(id) +','+ telefone +','+ to_string(idade) +','+ to_string(prioridade_especifc) +','+
                            to_string(especialidade) +','+ speso +','+ saltura +','+ medicacao +','+ sintomas;
    return linhaCSV;
}

int main() {
    ofstream arquivo("dados.csv");
    //Editar prioridade_max em caso de querer adionar mais pacientes
    int prioridade_max = 400;
    //Editar o vetor Salas caso queria aumentar ou diminuir as salas
    vector<string> Salas = {"SA101","SA102","SA103", "SA201", "SA202", "SA203", "SA301", "SA302", "SA303"};
    //Editar o vetor Especialidades caso queria aumentar ou diminuir especialidades
    vector<string> Especialidades = {"Psicologia","Clinico Geral","Cardiologista","Otorrino", "Psiquiatra", "Endócrino"};
    //Se aumentar ou diminuir o vetor Especialidades, é necessário que o vetor abaixo tenha o mesmo tamanho
    vector<int> Quant_Medicos_por_especialidade = {3,2,2,1,2,3};
    mt19937 gen(time(0));

    if (arquivo.is_open()) {
        arquivo << to_string(prioridade_max) << '\n';
        arquivo << to_string(Salas.size()) << '\n';
        for(int a = 0; a < Salas.size(); a++){
            arquivo << Salas[a] << '\n';
        }
        arquivo << to_string(Especialidades.size()) << '\n';
        for(int b = 0; b < Especialidades.size(); b++){
            arquivo << Especialidades[b] << '\n';
        }
        int med = 0;
        for(int c = 0; c < Quant_Medicos_por_especialidade.size(); c++){
            arquivo << to_string(Quant_Medicos_por_especialidade[c]) << '\n';
            int y = 0;
            while(y < Quant_Medicos_por_especialidade[c]){
                y++;
                med++;
                arquivo << "Médico" << to_string(med) << '\n';
            }
        }
        arquivo << '\n';
        // Gerar e escrever 50(por enquanto) pacientes aleatórios no arquivo
        for (int i = 0; i < prioridade_max; ++i){
            arquivo << gerarPacienteAleatorio(gen, prioridade_max, Especialidades) << "\n";
            continue;
        }
        arquivo.close();
        cout << "Arquivo 'dados.csv' gerado com sucesso!" << endl;
    } else {
        cerr << "Erro ao abrir o arquivo!" << endl;
    }

    return 0;
}
