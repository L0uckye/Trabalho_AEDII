/*Eu realmente tentei, várias vezes, fazer o codigo funcionar em sua totalidade de forma modularizada e organizada, infelizmente tive
vários imprevistos nos últimos tempos, minha cabeça estava cheia, além de ter formatado o PC e por isso ter vários problemas com a 
configuração da build do VS code pra C/C++. Por isso não estou confiante em mostrar o código para a sala na apresentação.*/

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <ctime>
#include <random>
#include <fstream>
#include <sstream>
using namespace std;

// struct Paciente(Estrutura que guarda as informações do paciente)
struct Paciente {
    std::string nome;
    std::string ID;
    std::string telefone;
    int idade;
    int prioridade;
    int especialidade;
    float peso;
    float altura;
    std::string medicamentos;
    std::string sintomas;
    bool retornando;
    int retornos;
    std::string Medico_do_retorno;
    int faltas;

    // Construtor que inicializa os membros a partir de uma string CSV
    Paciente(const std::string& linhaCSV)
        : nome(""), ID(""), telefone(""), idade(0), prioridade(0),
          especialidade(0), peso(0.0f), altura(0.0f), medicamentos(""),
          sintomas(""), retornando(false), retornos(1), Medico_do_retorno(""), faltas(0) {

        std::stringstream ss(linhaCSV);
        std::string token;

        try {
            // Extrair cada valor separado por vírgula e atribuir aos campos
            if (std::getline(ss, token, ',')) nome = token;
            if (std::getline(ss, token, ',')) ID = token;
            if (std::getline(ss, token, ',')) telefone = token;
            if (std::getline(ss, token, ',')) idade = std::stoi(token);
            if (std::getline(ss, token, ',')) prioridade = std::stoi(token);
            if (std::getline(ss, token, ',')) especialidade = std::stoi(token);
            if (std::getline(ss, token, ',')) peso = std::stof(token);
            if (std::getline(ss, token, ',')) altura = std::stof(token);
            if (std::getline(ss, token, ',')) medicamentos = token;
            if (std::getline(ss, token, ',')) sintomas = token;
        } catch (const exception& e) {
            // Se houver um erro de conversão, você pode lidar com ele aqui
            // por exemplo, imprimir uma mensagem de erro ou atribuir valores padrão
        }
    }

    // Construtor padrão
    Paciente() 
        : nome(""), ID(""), telefone(""), idade(0), prioridade(0),
          especialidade(0), peso(0.0f), altura(0.0f), medicamentos(""),
          sintomas(""), retornando(false), retornos(1), Medico_do_retorno(""), faltas(0) {}
};

// struct No de Fila(NoF)
struct NoF{
    Paciente paciente;
    NoF* proximo;
    // Construtor do NoF
    NoF(Paciente& pacienteNo): paciente(pacienteNo), proximo(nullptr){}
};

// struct Medico(Estrutura que guarda as informações do médico)
struct Medico{
    string nome;
    int horas_trabalhadas;
    int especialidade;
    bool atendendo;
    NoF* fila_paciente;

    // Construtor do Medico
    Medico(const string linhaCSV): atendendo(false), fila_paciente(nullptr), horas_trabalhadas(0){
    stringstream ss(linhaCSV);
    string token;
    
    // mesmo processo feito para pegar as informações do paciente, só que adfaptado para os médicos
        try{
            if(getline(ss, token, ',')) nome = token; // nome do médico
            if(getline(ss, token, ',')) especialidade = stoi(token); // especialidade do médico
        }catch(const exception& e){
            // Se houver um erro de conversão, você pode lidar com ele aqui
            // por exemplo, imprimir uma mensagem de erro ou atribuir valores padrão
        }
    }

    // Construtor padrão de Medico
    Medico():nome(""), horas_trabalhadas(0),especialidade(0),atendendo(false),fila_paciente(nullptr){}

    // Metodo que coloca os pacientes na fila de pacientes do medico
    void inserir_Fila(Paciente& pacienteNo){
    NoF* NovoNo = new NoF(pacienteNo);
    if(fila_paciente == nullptr){
        fila_paciente = NovoNo;
        atendendo = not atendendo;
    }else{
        NoF* atual = fila_paciente;
        while(atual->proximo != nullptr){
            atual = atual->proximo;
        }
        atual->proximo = NovoNo;
        }
    }   

    // Metodo para verificar qual o tamanho da fila de pacientes do medico
    int verifica_Fila(){
    int indice = 0;
    if(fila_paciente == nullptr){
        return indice;
    }else{
        NoF* atual = fila_paciente;
        while(atual != nullptr){
            indice++;
            atual = atual->proximo;
        }
        return indice;
        }
    }

    // Metodo para retirar um paciente da fila e passar para o próximo se existir
    void retirar_Paciente(){
    if(atendendo == false){
        if(fila_paciente != nullptr){
            horas_trabalhadas++;
            if(fila_paciente->proximo != nullptr){
                NoF* temp = fila_paciente;
                fila_paciente = fila_paciente->proximo;
                atendendo = not atendendo;
                delete temp;
            }else
                fila_paciente = fila_paciente->proximo;
            }
        }
    }
};

// struct Sala(Estrutura que guarda as informações da Sala, como ID, médico nela, etc)
struct Sala{
    string ID;
    Medico medico;
    string temp = {"temp, 0"};

    // Construtor da Sala
    Sala(string novo_ID){
    ID = novo_ID;
    Medico tempM(temp);
    medico = tempM;
    }
};

// struct No de Lista(NoL)
struct NoL{
    Medico medico;
    NoL* proximo;

    // Construtor do NoL
    NoL(Medico& medicoNo): medico(medicoNo), proximo(nullptr){} 
};

// struct Vetor_Salas(Onde ficam guardadas as salas do hospital)
struct Vetor_Salas{
    vector<Sala> Salas;
    vector<Medico> Medicos_reserva;

    // Metodo para inserir as salas no vetor de salas
    void Inserir_Sala(Sala& salaNova){
    Salas.push_back(salaNova);
    }

    // Metodo para receber medicos que estão atendendo(Com algum paciente na fila)
    void Receber_Medico(vector<Medico>& Medicos){
    if(!Medicos_reserva.empty()){
        int x = 0;
        while(x < Salas.size()){
            if(Medicos_reserva[x].atendendo){
                Salas[x].medico = Medicos_reserva[x];
                Medicos_reserva[x].retirar_Paciente();
                x++;
            }
            Medicos_reserva.erase(Medicos_reserva.begin(), Medicos_reserva.begin() + x);
        }
    }
    int i = 0;
    while(i < Salas.size() && Salas[i].medico.atendendo){
        i++;
    }
    int j = 0;
    while(i < Salas.size()){
        Salas[i].medico = Medicos[j];
        i++;
        j++;
    }
    while(j < Medicos.size()){
        Medicos_reserva.push_back(Medicos[j]);
        j++;
    }
    }

    // Metodo para retornar as salas com todas as informações e liberar o vetor de salas
    vector<Sala> Consulta_Terminada(){
        vector<Sala> Medicos_trabalharam;
        for(int j = 0; j<Salas.size(); j++){
            if(Salas[j].medico.atendendo){
                Medicos_trabalharam.push_back(Salas[j]);
                Salas[j].medico.atendendo = false;
                Salas[j].medico.retirar_Paciente();
            }
        }
        return Medicos_trabalharam;
    }

    // Metodo não utilizado(Sem uma utilidade até o ponto atual do código)
    vector<Sala> getSalas_Usando(){
    vector<Sala> Medicos_consultando;
    for(int i = 0; i < Salas.size(); i++){
        if(Salas[i].medico.atendendo){
            Medicos_consultando.push_back(Salas[i]);
            }
        }
    return Medicos_consultando;
    }
};

// struct Heapmax_Pacientes(Fila de prioridade dos pacientes)
struct Heapmax_pacientes{
    vector<Paciente> Heap_pacientes;
    int pacientes_por_dia;

    // Metodo para subir elemento de acordo com sua prioridade
    void Heap_subir(int indice){
    while(indice > 0){
        int pai = (indice-1)/2;
        if(Heap_pacientes[indice].prioridade <= Heap_pacientes[pai].prioridade) 
            break;
        swap(Heap_pacientes[indice], Heap_pacientes[pai]);
        indice = pai;
        }
    }

    // Metodo para descer elemento depois de uma remoção
    void Heap_descer(int indice){
    int filho_esquerdo, filho_direito, maior;
    while(indice < Heap_pacientes.size()){
        filho_esquerdo = (2 * indice)+1;
        filho_direito = filho_esquerdo + 1;
        maior = indice;
        
        if(filho_esquerdo < Heap_pacientes.size() && Heap_pacientes[filho_esquerdo].prioridade > Heap_pacientes[maior].prioridade){
            maior = filho_esquerdo;
        }if(filho_direito < Heap_pacientes.size() && Heap_pacientes[filho_direito].prioridade > Heap_pacientes[maior].prioridade){
            maior = filho_direito;
        }if(maior == indice){
            break;
        }
        swap(Heap_pacientes[indice], Heap_pacientes[maior]);
        indice = maior;
        }
    }

    // Metodo que retorna uma expectativa de dias para todos os pacientes serem atendidos, com resto de pacientes
    pair<int, int> espectativa_dias_exigidos(){
    return {Heap_pacientes.size() / pacientes_por_dia, Heap_pacientes.size() % pacientes_por_dia};
    }

    // Metodo de Busca de pelo menos um paciente que esteja na primeira consulta 
    int primeira_consulta(int menor, int maior) {
        // Valida os limites
        if (menor < 0 || maior > static_cast<int>(Heap_pacientes.size()) || menor >= maior) {
            throw std::invalid_argument("Intervalo inválido fornecido para primeira_consulta.");
        }

        // Itera de trás para frente
        for (int i = maior - 1; i >= menor; --i) {
            if (!Heap_pacientes[i].retornando) {
                return i; // Retorna o índice do primeiro paciente encontrado
            }
        }

        // Retorna -1 se nenhum paciente for encontrado
        return -1;
    }

    // Metodo para inserir o paciente e organizar o heap a partir de sua prioridade
    void Inserir_paciente(Paciente& pacienteN){
    if(pacienteN.retornos == 0 || pacienteN.retornos == 1){
        Heap_pacientes.push_back(pacienteN);
        Heap_subir(Heap_pacientes.size()-1);
        }
    }

    // Metodo para remover o paciente e reorganizar o heap a partir de sua remoção
    Paciente Remover_Paciente(){
    if(Heap_pacientes.empty()){
        throw std::out_of_range("Heap de pacientes está vazio!");
    }else{
        Paciente maior_prioridade = Heap_pacientes[0];
        Heap_pacientes[0] = Heap_pacientes.back();
        Heap_pacientes.pop_back();
        Heap_descer(0);
        return maior_prioridade;
        }
    }
};

// struct Tabela_Especialidades(Tabela Hash de especialidades, guarda o nome das especialidades também)
struct Tabela_Especialidades{
    vector<string> Nome_Especialidade;
    int T_Especialidades;
    vector<NoL*> Especialidades;

    // Construtor da Tabela_Especialidades
    Tabela_Especialidades(int tamanho): T_Especialidades(tamanho), Nome_Especialidade(){
    Especialidades.resize(tamanho);
    }
    
    // Metodo que retorna número de especialidades(não usado)
    int Numero_Especialidades(){
    return Nome_Especialidade.size();
    }

    // Metodo que retorna qual especialidade e indice da tabela hash deve ser acessado
    int funcao_Hash(int N_espec){
    return N_espec % Nome_Especialidade.size();
    }

    // Metodo que retorna o número de Medicos atendendo
    int Quant_Medicos_aten(){
    int indice = Nome_Especialidade.size();
    int quantidade;
    for(int i = 0; i < indice; i++){
        NoL* atual = Especialidades[i];
        while(atual != nullptr){
            if(atual->medico.atendendo){
                quantidade++;
                atual = atual->proximo;
            }else
                atual = atual->proximo;
        }
    }
    return quantidade;
    }

    // Metodo que retorna o número de pacientes que faltam ser atendidos(aguardando na fila)
    int Pacientes_faltando() {
    int N_pacientes_faltando = 0;

    // Verifica se há especialidades
    if (Especialidades.empty() || Nome_Especialidade.empty()) {
        std::cerr << "Erro: Especialidades ou Nome_Especialidade está vazio." << std::endl;
        return 0;
    }

    // Certifica-se de que há correspondência entre Nome_Especialidade e Especialidades
    if (Especialidades.size() != Nome_Especialidade.size()) {
        std::cerr << "Erro: Tamanho de Nome_Especialidade e Especialidades não correspondem." << std::endl;
        return 0;
    }

    // Percorre todas as especialidades
    for (size_t i = 0; i < Nome_Especialidade.size(); i++) {
        NoL* atual = Especialidades[i];

        // Itera sobre a lista de médicos na especialidade
        while (atual != nullptr) {
            NoF* paciente = atual->medico.fila_paciente;

            // Conta os pacientes na fila do médico
            while (paciente != nullptr) {
                N_pacientes_faltando++;
                paciente = paciente->proximo;
            }

            atual = atual->proximo;
        }
    }

    return N_pacientes_faltando;
    }

    // Metodo para guardar o nome das especialidades no vetor Nome_Especialidades
    void Guardar_Especialidade(string Especialidades){
    Nome_Especialidade.push_back(Especialidades);
    }

    // Metodo para guardar os medicos em cada especialidade
    void inserir_Medico(Medico& Especialista){
    int index = funcao_Hash(Especialista.especialidade);
    NoL* NovoNo = new NoL(Especialista);
    if(Especialidades[index]==nullptr){
        Especialidades[index] = NovoNo;
    }else{
        NoL* atual = Especialidades[index];
        while(atual->proximo != nullptr){
            atual = atual->proximo;
            }
        atual->proximo = NovoNo;
        }
    }

    // Metodo que libera os medicos que terminaram de atender
    void hora_Atendida(vector<Medico>& Atendimento_terminado){
    for(int i = 0; i< Atendimento_terminado.size(); i++){
        int index = funcao_Hash(Atendimento_terminado[i].especialidade);
        NoL* atual = Especialidades[index];
        while(atual != nullptr){
            if(atual->medico.nome == Atendimento_terminado[i].nome){
                atual->medico.atendendo = not atual->medico.atendendo;
                atual->medico.retirar_Paciente();
                break;
            }else
                atual = atual->proximo;
            }
        }
    }  

    // Metodo que retorna o vetor de medicos que estão atendendo(com pelo menos 1 paciente na fila)
    vector<Medico> procurar_Medico_atendendo(){
    vector<Medico> Medicos_aten;
    for(int i=0; i<Especialidades.size();i++){
        NoL* atual = Especialidades[i];
        while(atual != nullptr){
            if(atual->medico.atendendo){
                string medico = atual->medico.nome + ',' + to_string(atual->medico.especialidade);
                Medico temp_Medico(medico);
                Medicos_aten.push_back(medico);
                Medicos_aten.back().inserir_Fila(atual->medico.fila_paciente->paciente);
                atual = atual->proximo;
            }else
                atual = atual->proximo;
            }
        }
    return Medicos_aten;
    }

    // Metodo que retorna os medicos da especialidade( não usado)
    vector<Medico> Medicos_da_Especialidade(int i){
    NoL* atual = Especialidades[i];
    vector<Medico> vetor_Especial;
    while(atual != nullptr){
        string medico = atual->medico.nome + to_string(atual->medico.especialidade);
        Medico temp_Medico(medico);
        vetor_Especial.push_back(temp_Medico);
        vetor_Especial.back().inserir_Fila(atual->medico.fila_paciente->paciente);
        atual = atual->proximo;
        }
    return vetor_Especial;
    }

    // Metodo que verifica se tem algum medico livre em certa especialidade N_Espec
    bool procurar_Medico_Livre(int N_Espec){
    int index = funcao_Hash(N_Espec);
    NoL* atual = Especialidades[index];
    while(atual != nullptr){
        if(atual->medico.atendendo){
            atual = atual->proximo;
        }else
            return 1;
        }
    return 0;
    }

    // Metodo que insere um paciente em uma fila de um medico de acordo com a especialidade
    void Escolher_Medico(Paciente& pacienteNo) {
    int index = funcao_Hash(pacienteNo.especialidade);
    NoL* atual = Especialidades[index];
    NoL* anterior = nullptr;

    if (pacienteNo.retornando) {
        // Paciente retornando
        while (atual != nullptr) {
            if (atual->medico.nome == pacienteNo.Medico_do_retorno) {
                atual->medico.inserir_Fila(pacienteNo);
                return;
            }
            atual = atual->proximo;
        }
        throw std::runtime_error("Médico associado ao retorno não encontrado.");
    } else {
        // Paciente novo
        if (procurar_Medico_Livre(pacienteNo.especialidade)) {
            // Inserir no primeiro médico disponível
            while (atual != nullptr) {
                if (!atual->medico.atendendo) {
                    atual->medico.inserir_Fila(pacienteNo);
                    return;
                }
                atual = atual->proximo;
            }
        } else {
            // Equilibrar entre médicos
            while (atual != nullptr) {
                if (anterior != nullptr) {
                    int Q_ant = anterior->medico.verifica_Fila();
                    int Q_atu = atual->medico.verifica_Fila();

                    if (Q_ant <= Q_atu) {
                        anterior->medico.inserir_Fila(pacienteNo);
                        return;
                    } else {
                        atual->medico.inserir_Fila(pacienteNo);
                        return;
                    }
                }
                anterior = atual;
                atual = atual->proximo;
            }

            // Caso nenhum médico seja encontrado, insira no último
            if (anterior != nullptr) {
                anterior->medico.inserir_Fila(pacienteNo);
            } else {
                throw std::runtime_error("Nenhum médico disponível.");
                }
            }
        }
    }

    // Metodo para traduzir a especialidade de int para os nomes guardados no Nome_Especialidades
    string Traduzir_Especialidade(int indice){
    int index = funcao_Hash(indice);
    return Nome_Especialidade[index];
    }

    // Metodo que libera a memória usada para a alocação dinâmica na tabela_Hash
    void Deletar(){
    for(int i = 0; i<Especialidades.size(); i++){
        NoL* atual = Especialidades[i];
        while(atual != nullptr){
            NoL* temp = atual;
            atual = atual->proximo;
            delete temp;
            }
        }
    }   
};

// Classe de Operacoes
class Operacao{
    public:

    //Metodo para iniciar o gerador aleatório
    static mt19937& Iniciar_random() {
    static mt19937 gen(random_device{}());
    return gen;
    }

    // Metodo que retorna o numero usado para calcular se um paciente vai faltar
    static int Chance_Faltar() {
    uniform_int_distribution<> distrib(1, 100);
    return distrib(Iniciar_random());
    }

    // Metodo que retorna uma Tabela_Especialidades, para fazer acesso de uma variavel global para uma local
    static Tabela_Especialidades Receber_Especialidades(Tabela_Especialidades& Especialidades){
    return Especialidades;
    }

    // Metodo para calcular oa pacientes por dia do Heapmax_pacientes
    static void Calcular_Pacientes_Por_dia(Vetor_Salas& Salas_Hospital, Heapmax_pacientes& Heapmax, int horas_por_dia){
    Heapmax.pacientes_por_dia = Salas_Hospital.Salas.size() * horas_por_dia;
    }

    // Metodo para reinserir um paciente depois de sua primeira consulta
    static void Paciente_Retornando(Heapmax_pacientes& Heapmax, Sala& sala_ocupada) {
    if (!sala_ocupada.medico.fila_paciente) {
        throw std::runtime_error("Fila de paciente nula.");
    }

    sala_ocupada.medico.fila_paciente->paciente.retornando = !static_cast<bool>(sala_ocupada.medico.fila_paciente->paciente.retornando);
    sala_ocupada.medico.fila_paciente->paciente.retornos--;
    if (sala_ocupada.medico.fila_paciente->paciente.retornando && sala_ocupada.medico.fila_paciente->paciente.retornos>=0) {
        pair<int, int> resultado = Heapmax.espectativa_dias_exigidos();
        int total_pacientes = static_cast<int>(Heapmax.Heap_pacientes.size());
        if(Heapmax.Heap_pacientes.empty()){
            Heapmax.Inserir_paciente(sala_ocupada.medico.fila_paciente->paciente);
        }else{
            if (resultado.first >= 30) {
            int ultimo_paciente = std::min(Heapmax.pacientes_por_dia * 30, total_pacientes);
            int paciente_consulta1 = Heapmax.primeira_consulta(0, ultimo_paciente);

            if (paciente_consulta1 >= 0 && paciente_consulta1 < total_pacientes) {
                sala_ocupada.medico.fila_paciente->paciente.prioridade = Heapmax.Heap_pacientes[paciente_consulta1].prioridade + 1;
                sala_ocupada.medico.fila_paciente->paciente.Medico_do_retorno = sala_ocupada.medico.nome;
                Heapmax.Inserir_paciente(sala_ocupada.medico.fila_paciente->paciente);
            } else {
                int extra_consulta1 = Heapmax.primeira_consulta(ultimo_paciente, total_pacientes);
                if (extra_consulta1 >= ultimo_paciente && extra_consulta1 < total_pacientes) {
                    sala_ocupada.medico.fila_paciente->paciente.prioridade = Heapmax.Heap_pacientes[extra_consulta1].prioridade + 1;
                    sala_ocupada.medico.fila_paciente->paciente.Medico_do_retorno = sala_ocupada.medico.nome;
                    Heapmax.Inserir_paciente(sala_ocupada.medico.fila_paciente->paciente);
                } else {
                    sala_ocupada.medico.fila_paciente->paciente.prioridade = 1;
                    sala_ocupada.medico.fila_paciente->paciente.Medico_do_retorno = sala_ocupada.medico.nome;
                    Heapmax.Inserir_paciente(sala_ocupada.medico.fila_paciente->paciente);
                }
            }
        } else {
            int ultimo_consulta1 = Heapmax.primeira_consulta(0, total_pacientes);
            if (ultimo_consulta1 >= 0 && ultimo_consulta1 < total_pacientes) {
                sala_ocupada.medico.fila_paciente->paciente.prioridade = Heapmax.Heap_pacientes[ultimo_consulta1].prioridade + 1;
                sala_ocupada.medico.fila_paciente->paciente.Medico_do_retorno = sala_ocupada.medico.nome;
                Heapmax.Inserir_paciente(sala_ocupada.medico.fila_paciente->paciente);
            } else {
                sala_ocupada.medico.fila_paciente->paciente.prioridade = 1;
                sala_ocupada.medico.fila_paciente->paciente.Medico_do_retorno = sala_ocupada.medico.nome;
                Heapmax.Inserir_paciente(sala_ocupada.medico.fila_paciente->paciente);
            }
        }
        }
    }
}

    // Metodo para fazer as operações necessárias em caso de falta do paciente
    static Paciente Paciente_Faltou(Heapmax_pacientes& Heapmax) {
    // Incrementa o número de faltas do paciente no topo do heap
    Heapmax.Heap_pacientes[0].faltas += 1;
    Paciente faltou = Heapmax.Heap_pacientes[0];

    if (faltou.faltas < 2) {
        // Primeira falta: troca com o paciente do próximo dia
        pair<int, int> resultado = Heapmax.espectativa_dias_exigidos();
        int proximo_paciente_idx;

        if (resultado.first > 0) {
            // Determina o índice do paciente do próximo dia
            if (resultado.first == 1 && resultado.second == 0) {
                proximo_paciente_idx = Heapmax.pacientes_por_dia - 1;
            } else {
                proximo_paciente_idx = Heapmax.pacientes_por_dia;
            }
        } else {
            // Último paciente do heap
            proximo_paciente_idx = Heapmax.Heap_pacientes.size() - 1;
        }

        // Troca de posição
        swap(Heapmax.Heap_pacientes[0].prioridade, Heapmax.Heap_pacientes[proximo_paciente_idx].prioridade);
        swap(Heapmax.Heap_pacientes[0], Heapmax.Heap_pacientes[proximo_paciente_idx]);
    } else {
        // Segunda falta: remove o paciente do heap
        Heapmax.Remover_Paciente();
    }

    // Retorna o paciente que faltou
    return faltou;
    }

    // Metodo que retorna as salas(com todas as informações) e libera o vetor de salas para novos médicos entrarem
    static vector<Sala> Alocar_Salas_Para_Medicos(Vetor_Salas& Salas_Hospital, vector<Medico>& Medicos_atendendo){
    Salas_Hospital.Receber_Medico(Medicos_atendendo);
    vector<Sala> Consultas_Terminadas = Salas_Hospital.Consulta_Terminada();
    return Consultas_Terminadas;
    }

    // Metodo para printar as consultas que aconteceram a partir do Alocar_Salas_Para_Medicos
    static void Printar_Consultados(vector<Sala>& Salas_Usadas, Tabela_Especialidades& Vetor_especialidades,
                                   ofstream& arquivo, int dia, int hora){
    arquivo << "Consultas no dia " << to_string(dia) << " na " << to_string(hora) << "ª hora:" << "\n";
    arquivo << "Sala___________|Médico_________|Especialidade__|Paciente_______|ID_do_Paciente__|Retorno_______" << "\n\n";
    for(int i = 0; i<Salas_Usadas.size(); i++){
        string retorno;
        if(!Salas_Usadas[i].medico.fila_paciente->paciente.retornando){
            retorno = "1ª Consulta";
        }else{retorno = "Retorno";}
        string linhaCSV = Salas_Usadas[i].ID + '|' +
                          Salas_Usadas[i].medico.nome + '|' + 
                          Vetor_especialidades.Traduzir_Especialidade(Salas_Usadas[i].medico.especialidade) + '|' +
                          Salas_Usadas[i].medico.fila_paciente->paciente.nome + '|' +
                          Salas_Usadas[i].medico.fila_paciente->paciente.ID + '|' +
                          retorno;
        arquivo << linhaCSV << "\n";
    }
    arquivo << "\n";
    }

    // Metodo para printar as faltas que aconteceram
    static void Printar_Falta(Paciente& pacienteNo, ofstream& arquivo){
    if(pacienteNo.faltas < 2){
        arquivo << "paciente de ID:" << pacienteNo.ID << " faltou 1 vez e consulta postergada" << "\n";
    }else
        arquivo << "paciente de ID:" << pacienteNo.ID << " perdeu direito a agendamentos" << "\n";
    }

    // Metodo para Registrar paciente que está a retornar a partir do vetor de salas( não usado)
    static void Registrar_Paciente_Retornando(Heapmax_pacientes& Heapmax, vector<Sala> Pacientes_a_retornar){
    for(int i = 0; i<Pacientes_a_retornar.size(); i++){
        Paciente_Retornando(Heapmax, Pacientes_a_retornar[i]);
        }
    }

    // Metodo que libera os medicos que estavam nas salas
    static void Liberar_Medicos(vector<Sala>& Salas_Usadas, Tabela_Especialidades& Vetor_Especialidades){
    vector<Medico> Medicos;
    for(int i = 0; i<Salas_Usadas.size(); i++){ 
        Medicos.push_back(Salas_Usadas[i].medico);
        }
    Vetor_Especialidades.hora_Atendida(Medicos);
    }

    // Metodo para retornar o número de medicos atendendo(não usado)
    static int N_Medicos_atendendo(vector<Medico>& Medicos_atendendo, Vetor_Salas& Salas_Hospital){
    int N_Medicos_atendendo = Medicos_atendendo.size();
    if(N_Medicos_atendendo < Salas_Hospital.Salas.size()){
        return Salas_Hospital.Salas.size() - N_Medicos_atendendo;
    }else
        return -1;
    }

    // Metodo que verifica a alocação de pacientes(não usado)
    static bool Verifica_alocacao(Heapmax_pacientes& Heapmax, Vetor_Salas& Salas_Hospital){
    vector<Sala> Salas_atendendo = Salas_Hospital.getSalas_Usando();
    if(Heapmax.Heap_pacientes.size() > 0 || Salas_atendendo.size() == Salas_Hospital.Salas.size()){
        return true;
    }else
        return false;
}

    // Metodo para printar quantas horas cada médico trabalhou
    static void Printar_Horas_Trabalhadas_Medicos(Tabela_Especialidades& Vetor_Especialidades, ofstream& arquivo){
    arquivo << "Medico__|Horas_Trabalhadas__\n";
    for(int i = 0; i<Vetor_Especialidades.Nome_Especialidade.size() ; i++){
        NoL* atual = Vetor_Especialidades.Especialidades[i];
        while(atual != nullptr){
            arquivo << atual->medico.nome << " |" << atual->medico.horas_trabalhadas << " h\n";
            atual = atual->proximo;
            }
        }
    }
};

// Função para gerenciar as faltas dos pacientes( não usado)
void GerenciarFaltas(mt19937& gen, Heapmax_pacientes& fila_de_prioridade, ofstream& arquivo){
    int chance = Operacao::Chance_Faltar();
    if(chance % 20 == 0){
        Paciente paciente_faltou = Operacao::Paciente_Faltou(fila_de_prioridade);
        Operacao::Printar_Falta(paciente_faltou, arquivo);
    }
}

// Função para realizar o atendimento e imprimir os resultados( não usado)
void RealizarAtendimento(Vetor_Salas& salas_hospital,Heapmax_pacientes& fila_de_prioridade, vector<Medico>& atendimentos, Tabela_Especialidades& Especialidades_Medicos, ofstream& arquivo, int& dias, int& horas){
    vector<Sala> salas_usadas = Operacao::Alocar_Salas_Para_Medicos(salas_hospital, atendimentos);
    Operacao::Liberar_Medicos(salas_usadas, Especialidades_Medicos);
    Operacao::Printar_Consultados(salas_usadas, Especialidades_Medicos, arquivo, dias, horas);
    for(int i = 0; i < salas_usadas.size(); i++){
        Operacao::Paciente_Retornando(fila_de_prioridade, salas_usadas[i]);
    }
}

// Codigo principal
int main(){
    // Inicia todas as estruturas e variáveis globais importantes
    ifstream file("dados.csv");
    Heapmax_pacientes Fila_de_Prioridade;
    Vetor_Salas Salas_Hospital;
    Tabela_Especialidades Especialidades_Medicos(1);
    //Utilizei 8 horas de trabalho diário, caso queria mudar é só modificar abaixo
    int horas_funcionamento_diario = 8;
    bool inf_lidas = false;
    vector<string> Informacoes; 
    
    // verifica se está tudo okay com o file
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo" << std::endl;
        return 1;
    }

    // lê o file seprando ele em 2 partes, uma vai para o vetor de informações, a outra é guardado no Heap de pacientes
    string linhaCSV;
    while(getline(file,linhaCSV)){
        string token;
        string value;
    
        if(!inf_lidas){
            if(linhaCSV.empty()){
                inf_lidas = not inf_lidas;
                continue;
            }else{
            // Parte que guarda informações no vetor Informacoes
            stringstream ss(linhaCSV);
            getline(ss, token, ','); value = token;
            Informacoes.push_back(value);
            }
        }else{
            // Parte que guarda pacientes no Hepa de pacientes
            string med = linhaCSV;
            Paciente nova_paciente(med);
            Fila_de_Prioridade.Inserir_paciente(nova_paciente);
        }
    }

    // inicia varáveis importantes para a leitura do vetor de informações
    int indice = 0;
    int N_pac_iniciais, N_salas, limite_salas;
    int N_espec, limite_espec;
    int N_med, limite_med;
    int especialidade = 0;
    // while que lê o vetor de informações
    while (indice < Informacoes.size()) {
        if (indice == 0) {
            // Número de pacientes iniciais
            N_pac_iniciais = stoi(Informacoes[indice]);
            indice++;
        } else if (indice == 1) {
            // Número de salas e limite
            N_salas = stoi(Informacoes[indice]);
            limite_salas = indice + N_salas;
            indice++;
        } else if (indice > 1 && indice <= limite_salas) {
            // Guardar salas no vetor de salas
            Sala nova_sala(Informacoes[indice]);
            Salas_Hospital.Inserir_Sala(nova_sala);
            indice++;
        } else if (indice == limite_salas + 1) {
            // Número de especialidades e limite
            N_espec = stoi(Informacoes[indice]);
            limite_espec = indice + N_espec;
            Tabela_Especialidades temp(stoi(Informacoes[indice]));
            // Tabela Hash de especialidades atualizada para o tamanho real que ela deveria ter
            Especialidades_Medicos = Operacao::Receber_Especialidades(temp);
            indice++;
        } else if (indice > limite_salas + 1 && indice <= limite_espec) {
            // Adicionar especialidades ao vetor Nome_Especialidades
            Especialidades_Medicos.Guardar_Especialidade(Informacoes[indice]);
            indice++;
        } else if (indice == limite_espec + 1) {
            // Número de médicos e limite
            N_med = stoi(Informacoes[indice]);
            limite_med = indice + N_med;
            indice++;
        } else if (indice > limite_espec + 1 && indice < Informacoes.size()) {
            // Adicionar médicos
            if (indice == limite_med + 1) {
                N_med = stoi(Informacoes[indice]);
                limite_med = indice + N_med;
                especialidade++;
                indice++;
            } else {
                // Armazenar os medicos na Tabela Hash
                string medico = Informacoes[indice] + ',' + to_string(especialidade);
                Medico novo_Medico(medico);
                Especialidades_Medicos.inserir_Medico(novo_Medico);
                indice++;
            }
        }
    }
    
    // Criação do arquivo e vaiáveis dias e horas além do calculo de pacientes diários
    ofstream arquivo("Relatorio.csv");
    Operacao::Calcular_Pacientes_Por_dia(Salas_Hospital, Fila_de_Prioridade, horas_funcionamento_diario);
    int horas = 1;
    int dias = 1;
    // arquivo é aberto para as informações serem guardadas e registradas
    if(arquivo.is_open()){
        // Loop para percorrer os dias
        while(Fila_de_Prioridade.Heap_pacientes.size() > 0){
            vector<Medico> atendimentos = Especialidades_Medicos.procurar_Medico_atendendo();
            if(horas * Salas_Hospital.Salas.size() < Fila_de_Prioridade.pacientes_por_dia){
                if(atendimentos.size() < Salas_Hospital.Salas.size()){
                    int chance = Operacao::Chance_Faltar();
                    if(chance % 20 == 0){
                        Paciente faltante = Operacao::Paciente_Faltou(Fila_de_Prioridade);
                        Operacao::Printar_Falta(faltante, arquivo);
                    }else{
                        Paciente a_alocar = Fila_de_Prioridade.Remover_Paciente();
                        Especialidades_Medicos.Escolher_Medico(a_alocar);
                        atendimentos = Especialidades_Medicos.procurar_Medico_atendendo();
                    }
                }else{
                    vector<Sala> salas_usadas = Operacao::Alocar_Salas_Para_Medicos(Salas_Hospital, atendimentos);
                    Operacao::Printar_Consultados(salas_usadas, Especialidades_Medicos, arquivo, dias, horas);
                    Operacao::Liberar_Medicos(salas_usadas, Especialidades_Medicos);
                    for(int i = 0; i< salas_usadas.size(); i++){
                        Operacao::Paciente_Retornando(Fila_de_Prioridade, salas_usadas[i]);
                    }
                    horas++;
                }
            }else{
                if(atendimentos.size() < Salas_Hospital.Salas.size()){
                    int chance = Operacao::Chance_Faltar();
                    if(chance % 20 == 0){
                        Paciente faltante = Operacao::Paciente_Faltou(Fila_de_Prioridade);
                        Operacao::Printar_Falta(faltante, arquivo);
                    }else{
                        Paciente a_alocar = Fila_de_Prioridade.Remover_Paciente();
                        Especialidades_Medicos.Escolher_Medico(a_alocar);
                        atendimentos = Especialidades_Medicos.procurar_Medico_atendendo();
                    }
                }else{
                    vector<Sala> salas_usadas = Operacao::Alocar_Salas_Para_Medicos(Salas_Hospital, atendimentos);
                    Operacao::Printar_Consultados(salas_usadas, Especialidades_Medicos, arquivo, dias, horas);
                    Operacao::Liberar_Medicos(salas_usadas, Especialidades_Medicos);
                    for(int i = 0; i< salas_usadas.size(); i++){
                        Operacao::Paciente_Retornando(Fila_de_Prioridade, salas_usadas[i]);
                    }
                    horas = 1;
                    dias++;
                }
            }
        }
        // Parte que não consegui finalizar de forma que funcionasse, adicionando-a dependendo as vezes pode causar problema
        /*while (Especialidades_Medicos.Pacientes_faltando() > 0 || Fila_de_Prioridade.Heap_pacientes.size() > 0) {
        // Médicos disponíveis para atender
        vector<Medico> atendimentos = Especialidades_Medicos.procurar_Medico_atendendo();

        // Verifica se há capacidade para atender pacientes
        if (horas * Salas_Hospital.Salas.size() < Fila_de_Prioridade.pacientes_por_dia) {
            if (!Fila_de_Prioridade.Heap_pacientes.empty()) {
                // Checa chance de faltar
                int chance = Operacao::Chance_Faltar();
                if (chance % 20 == 0) {
                    Paciente faltante = Operacao::Paciente_Faltou(Fila_de_Prioridade);
                    Operacao::Printar_Falta(faltante, arquivo);
                } else {
                    // Aloca paciente ao médico
                    Paciente a_alocar = Fila_de_Prioridade.Remover_Paciente();
                    Especialidades_Medicos.Escolher_Medico(a_alocar);
                    atendimentos = Especialidades_Medicos.procurar_Medico_atendendo();
                }
            }
        } else {
            if (!Fila_de_Prioridade.Heap_pacientes.empty()) {
                // Checa chance de faltar
                int chance = Operacao::Chance_Faltar();
                if (chance % 20 == 0) {
                    Paciente faltante = Operacao::Paciente_Faltou(Fila_de_Prioridade);
                    Operacao::Printar_Falta(faltante, arquivo);
                } else {
                    // Aloca paciente ao médico
                    Paciente a_alocar = Fila_de_Prioridade.Remover_Paciente();
                    Especialidades_Medicos.Escolher_Medico(a_alocar);
                    atendimentos = Especialidades_Medicos.procurar_Medico_atendendo();
                }
            } else {
                // Aloca salas para os médicos e registra consultas
                vector<Sala> salas_usadas = Operacao::Alocar_Salas_Para_Medicos(Salas_Hospital, atendimentos);
                Operacao::Printar_Consultados(salas_usadas, Especialidades_Medicos, arquivo, dias, horas);
                Operacao::Liberar_Medicos(salas_usadas, Especialidades_Medicos);

                // Processa pacientes retornando para a fila
                for (Sala& sala : salas_usadas) {
                    Operacao::Paciente_Retornando(Fila_de_Prioridade, sala);
                }

                // Atualiza tempo (horas e dias)
                if (horas * Salas_Hospital.Salas.size() >= Fila_de_Prioridade.pacientes_por_dia) {
                    horas = 1;
                    dias++;
                } else {
                    horas++;
                }
            }
        }
    }*/

    // Limpeza e fechamento do arquivo e registro de horas trabalhadas pelos médicos 
        Operacao::Printar_Horas_Trabalhadas_Medicos(Especialidades_Medicos, arquivo);
        Especialidades_Medicos.Deletar();
        arquivo.close();
        cout << "Relatorio.csv criado com sucesso" << endl;
    }else{
        cerr << "Erro ao abrir arquivo" << endl;
    }
    return 0;
}
