//Projeto Prático - ANP 2022/1
//Gilson dos Santos Júnior
//Iann Santos Ribeiro Zorkot
//João Marcos Machado

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <cmath>

using namespace std;

//registro que armazena os elementos do arquivo 
struct twitter {
	int id;
	int status_id;
	char data[40];
	char texto[100];
	int fav;
	int rt;
	char u_name[60];
	char s_name[60];
	int seguidores;
	int amigos;
	char u_data[100];
	char localizacao[60];
	char source[60];
};

//função que retorna o tamanho de um elemento no registro, usada para avançar entre pontos
int sizeofTwitter () {
	
	int tamID = sizeof(int);
	int tamStatus = sizeof(int);
	int tamData = sizeof(char)*40;
	int tamTexto = sizeof(char)*100;
	int tamFav = sizeof(int);
	int tamRt = sizeof(int);
	int tamU_name = sizeof(char)*60;
	int tamS_name = sizeof(char)*60;
	int tamSeguidores = sizeof(int);
	int tamAmigos = sizeof(int);
	int tamU_data = sizeof(char)*100;
	int tamLocalização = sizeof(char)*60;
	int tamSource = sizeof(char)*60;
	
	return tamID + tamStatus + tamData + tamTexto + tamFav + tamRt + tamU_name + tamS_name + tamSeguidores + tamAmigos + tamU_data + tamU_name + tamLocalização + tamSource;
}

//cabecalho que contém o numero de elementos do arquivo
struct cabecalho {
	
	int numElementos = 0;
};

//retorna o tamanho do cabecalho
int sizeofCabecalho () {
	
	int numElementos = sizeof(int);
	
	return numElementos;
}

//move o ponteiro pro inicio do arquivo e cria o cabecalho
void escreverCabecalho(cabecalho novoCabecalho, fstream &arquivoBin) {
	
	arquivoBin.seekp(arquivoBin.beg);
	arquivoBin.write((const char *) (&novoCabecalho), sizeofCabecalho());	
}

cabecalho *lerCabecalho(fstream &arquivoBin) {

	cabecalho *cabecalhoLido = new cabecalho;
	arquivoBin.seekg(arquivoBin.beg);
	arquivoBin.read((char *) cabecalhoLido, sizeofCabecalho());
	return cabecalhoLido;
}

//pega a posicao relativa fornecida e retorna a posicao absoluta calculada
int acharPosicao(int posicao) {
	
	int posiAbsoluta = (posicao*sizeofTwitter()) + sizeofCabecalho(); 
	return posiAbsoluta;
} 

//le o atleta do arquivo csv e retorna um atleta no registro
twitter* lerTwitterCSV(ifstream &arquivo) {
	if (!arquivo.good()) {
        return nullptr;
    }
	twitter* novoTwitter = new twitter;
	string linha;
	string atributo;
		
	getline(arquivo, linha);	
    stringstream  lineStream(linha);
        
    getline(lineStream, atributo, ','); 
    novoTwitter->id = atoi(atributo.c_str());

	getline(lineStream, atributo, ','); 
    novoTwitter->status_id = atoi(atributo.c_str());
    
    getline(lineStream, atributo, ',');
    strcpy(novoTwitter->data,atributo.c_str());

	getline(lineStream, atributo, ',');
    strcpy(novoTwitter->texto,atributo.c_str());

	getline(lineStream, atributo, ','); 
    novoTwitter->fav = atoi(atributo.c_str());

	getline(lineStream, atributo, ','); 
    novoTwitter->rt = atoi(atributo.c_str());
    
    getline(lineStream, atributo, ',');
    strcpy(novoTwitter->u_name,atributo.c_str());

	getline(lineStream, atributo, ',');
    strcpy(novoTwitter->s_name,atributo.c_str());
    
    getline(lineStream, atributo, ','); 
    novoTwitter->seguidores = atoi(atributo.c_str());
    
    getline(lineStream, atributo, ',');
    novoTwitter->amigos = atoi(atributo.c_str());
    
    getline(lineStream, atributo, ',');
    strcpy(novoTwitter->u_data,atributo.c_str());

	getline(lineStream, atributo, ',');
    strcpy(novoTwitter->localizacao,atributo.c_str());
    
    getline(lineStream, atributo, ',');
    strcpy(novoTwitter->source,atributo.c_str());
    
	return novoTwitter;
}

//retorna um registro armazenado na posicao fornecida
twitter* lerTwitterBIN(fstream &arquivoBIN, int posicao) {

	twitter* novoTwitter = new twitter;
    
    posicao = acharPosicao(posicao);
    
	arquivoBIN.seekg(posicao);

	if (!arquivoBIN.good()) {
        return nullptr;
    }

    arquivoBIN.read((char*) novoTwitter, sizeofTwitter());
	
	arquivoBIN.seekg(arquivoBIN.beg);

	return novoTwitter;
    
}

//altera um atleta no arquivo sem atualizar o cabecalho
void escreverTwitterBIN(twitter* novoTwitter, fstream &arquivo, int posicao) {
	
	posicao = acharPosicao(posicao);
	
	arquivo.seekp(posicao);
	arquivo.write((const char *) (novoTwitter), sizeofTwitter());
	
	arquivo.seekp(arquivo.beg);
}

//atualiza um atleta no arquivo binario
void adicionarTwitterBIN(fstream &arquivoBin, twitter* novoTwitter, int posicao) {

	cabecalho* cabecalhoLido;
	cabecalhoLido = lerCabecalho(arquivoBin);
	
	while (posicao < cabecalhoLido->numElementos) {
		twitter* velhoTwitter = lerTwitterBIN(arquivoBin, posicao);
		escreverTwitterBIN(novoTwitter, arquivoBin, posicao);
		posicao++;
		delete novoTwitter;
		novoTwitter = velhoTwitter;
	}
	
	escreverTwitterBIN(novoTwitter, arquivoBin,cabecalhoLido->numElementos);
	cabecalhoLido->numElementos ++;
	escreverCabecalho(*cabecalhoLido, arquivoBin);
	delete cabecalhoLido;
}

//imprime todos os registros entre duas posicoes fornecidas
void visualizarRegistro(fstream &arquivoBin,int inicio, int final) {
	cabecalho* cabecalhoLido;
	cabecalhoLido = lerCabecalho(arquivoBin);
	if (cabecalhoLido->numElementos <= final){
		final = cabecalhoLido->numElementos - 1;
	}
	for (int i = inicio; i < final; i++){
		lerTwitterBIN(arquivoBin, i);
		twitter *novoTwitter = lerTwitterBIN(arquivoBin, i);
		cout << novoTwitter->id << " ";
		cout << novoTwitter->status_id << " ";
		cout << novoTwitter->data << " ";
		cout << novoTwitter->texto << " ";
		cout << novoTwitter->fav << " ";
		cout << novoTwitter->rt << " ";
		cout << novoTwitter->u_name << " ";
		cout << novoTwitter->s_name << " ";
		cout << novoTwitter->seguidores << " ";
		cout << novoTwitter->amigos << " ";
		cout << novoTwitter->u_data << " ";
		cout << novoTwitter->localizacao << " ";
		cout << novoTwitter->source << endl;	
		delete novoTwitter;
	}
}

//funcao pra editar um registro existente
void editarRegistro(fstream &arquivoBin, twitter *novoTwitter, int posicao) {
	escreverTwitterBIN(novoTwitter, arquivoBin, posicao);
}

//troca dois registros entre posicoes
void trocarRegistros(fstream &arquivoBin, int posicaoPrimeiro, int posicaoSegundo) {
	twitter* primeiroTwitter = lerTwitterBIN(arquivoBin, posicaoPrimeiro);
	twitter* segundoTwitter = lerTwitterBIN(arquivoBin, posicaoSegundo);
	escreverTwitterBIN(primeiroTwitter, arquivoBin, posicaoSegundo);
	escreverTwitterBIN(segundoTwitter, arquivoBin, posicaoPrimeiro);
}

//imprime todos os registros contidos no arquivo
void imprimir(fstream &arquivoBin){
	cabecalho* cabecalhoLido;
	cabecalhoLido = lerCabecalho(arquivoBin);
	visualizarRegistro(arquivoBin, 0, cabecalhoLido->numElementos - 1);
	delete cabecalhoLido;
}

int main() {
	
	ifstream arquivo("./Log4ShellTweets_dataset.csv");
	
	string linha;
		
	getline(arquivo, linha);
	
	fstream arquivoBin(
		"./Log4ShellTweets_dataset.bin",
		ios::binary|ios::in|ios::out|ios::ate
	);
	
	if (!arquivoBin.good()) {
		arquivoBin.close();
		ofstream ArquivoBin("./Log4ShellTweets_dataset.bin",ios::binary);
		arquivoBin.close();
		arquivoBin.open("./Log4ShellTweets_dataset.bin",ios::binary|ios::out|ios::ate);
    }

	cabecalho novoCabecalho;
	cout << novoCabecalho.numElementos << endl;
	
	escreverCabecalho(novoCabecalho, arquivoBin);
	
	while(arquivo.peek() != EOF) {
		twitter* novoTwitter;
		novoTwitter = lerTwitterCSV(arquivo);
		escreverTwitterBIN(novoTwitter, arquivoBin, novoCabecalho.numElementos);
		novoCabecalho.numElementos++;
		escreverCabecalho(novoCabecalho, arquivoBin);

		delete novoTwitter;
	}

	char indicador;

	do { 
		cout << "[A] - Adicionar um atleta na posicao" << endl;
		cout << "[B] - Visualizar os registros entre posicoes" << endl;
		cout << "[C] - Alterar dados do registro entre posicoes" << endl;
		cout << "[D] - Trocar dois registros de posicao " << endl;
		cout << "[E] - Imprimir todos os registros" << endl;
		cout << "[F] - Ordenar registros por Nome" << endl;
		cout << "[G] - Ordenar registros por ID" << endl;
		cout << "[X] - Fechar " << endl;

		cin >> indicador;

		if (indicador == 'a' or indicador == 'A') {
			//adicionar atleta na posição
			twitter* novoTwitter = new twitter;
			
			int posicao;
			cout << "Digite a posicao que deseja inserir" << endl;
			cin >> posicao;
			cout << "Digite o novo ID" << endl;
			cin >> novoTwitter->id;
			cout << "Digite um novo ID de status" << endl;
			cin >> novoTwitter->status_id;
			cout << "Digite uma nova data de status" << endl;
			cin >> novoTwitter->data;
			cout << "Digite o seu texto" << endl;
			cin >> novoTwitter->texto;
			cout << "Digite a contagem de favoritos" << endl;
			cin >> novoTwitter->fav;
			cout << "Digite a contagem de ReTweets" << endl;
			cin >> novoTwitter->rt;
			cout << "Digite o nome de usuario" << endl;
			cin >> novoTwitter->u_name;
			cout << "Digite o nome da conta" << endl;
			cin >> novoTwitter->s_name;
			cout << "Digite o numero de seguidores" << endl;
			cin >> novoTwitter->seguidores;
			cout << "Digite a data de usuario" << endl;
			cin >> novoTwitter->u_data;
			cout << "Digite a localizacao" << endl;
			cin >> novoTwitter->localizacao;
			cout << "Digite a fonte" << endl;
			cin >> novoTwitter->source;

			adicionarTwitterBIN(arquivoBin, novoTwitter, posicao);
			delete novoTwitter;
		}
		else if (indicador == 'b' or indicador == 'B') {
			//visualizar registro entre posições
			int inicio, fim;
			cin >> inicio >> fim;

			if (fim > inicio){
				visualizarRegistro(arquivoBin, inicio, fim);
			}
			else {
				cout << "Pedido Ilegal!";
			}
		}
		else if (indicador == 'c' or indicador == 'C') {
			//alterar os dados do registro em posição específica
			twitter* novoTwitter = new twitter;
			
			int posicao;
			cout << "Digite a posicao que deseja inserir" << endl;
			cin >> posicao;
			cout << "Digite o novo ID" << endl;
			cin >> novoTwitter->id;
			cout << "Digite um novo ID de status" << endl;
			cin >> novoTwitter->status_id;
			cout << "Digite uma nova data de status" << endl;
			cin >> novoTwitter->data;
			cout << "Digite o seu texto" << endl;
			cin >> novoTwitter->texto;
			cout << "Digite a contagem de favoritos" << endl;
			cin >> novoTwitter->fav;
			cout << "Digite a contagem de ReTweets" << endl;
			cin >> novoTwitter->rt;
			cout << "Digite o nome de usuario" << endl;
			cin >> novoTwitter->u_name;
			cout << "Digite o nome da conta" << endl;
			cin >> novoTwitter->s_name;
			cout << "Digite o numero de seguidores" << endl;
			cin >> novoTwitter->seguidores;
			cout << "Digite a data de usuario" << endl;
			cin >> novoTwitter->u_data;
			cout << "Digite a localizacao" << endl;
			cin >> novoTwitter->localizacao;
			cout << "Digite a fonte" << endl;
			cin >> novoTwitter->source;
			
			editarRegistro(arquivoBin, novoTwitter, posicao);
			visualizarRegistro(arquivoBin, posicao, posicao);
			delete novoTwitter;
		}
		else if (indicador == 'd' or indicador == 'D') {
			//trocar dois registros de posicao
			int posicao1, posicao2;
			cin >> posicao1 >> posicao2;

			trocarRegistros(arquivoBin, posicao1, posicao2);
		}
		else if (indicador == 'e' or indicador == 'E') {
			//imprimir todos os registros no arquivo
			imprimir(arquivoBin);
		}
		else if (indicador == 'f' or indicador == 'F') {
			//ordenar arquivo por Nome
			cout << "AREA EM CONSTRUCAO" << endl;   
		}
		else if (indicador == 'g' or indicador == 'G') {
			//ordenar arquivo por ID
			cout << "AREA EM CONSTRUCAO" << endl;
		}
		
	} while (indicador != 'x' or indicador != 'X');
	
	arquivo.close();
	arquivoBin.close();
	
    return 0;
}
