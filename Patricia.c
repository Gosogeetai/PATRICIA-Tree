#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//eu defino o alfabeto como numero, pra poder comparar letra com letra e assim saber qual a "menor" letra
#define a 1
#define b 2
#define c 3
#define d 4
#define e 5
#define f 6
#define g 7
#define h 8
#define i 9
#define j 10
#define k 11
#define l 12
#define m 13
#define n 14
#define o 15
#define p 16
#define q 17
#define r 18
#define s 19
#define t 20
#define u 21
#define v 22
#define w 23
#define x 24
#define y 25
#define z 26
typedef struct TPatricia{ // estrutura da arvore patricia
	int qualposicao; //um int que guarda a posição de diferença
	char palavra[20]; //um char que guarda a palavra, mas em casos que não são folha, essa palavra so serve de prefixo mesmo
	struct TPatricia* filhoEsq; //ponteiro pro filho esquerdo
	struct TPatricia* filhoDir;// e outro pro direito
}TPatricia;

TPatricia* CriaNo(char palavra[]){ //essa função cria um no de arvore patricia com a palavra como argumento
	TPatricia* novo = malloc(sizeof(TPatricia)); //aloco espaço
	strcpy(novo->palavra,palavra); //copio a string pro no
	novo->qualposicao = 99; //esse numero so serve pra indicar que a posicao é nula, pois em folhas não existem diferenças
	novo->filhoEsq = NULL; //seto os filhos como nulo
	novo->filhoDir = NULL;
	return novo; //e retorno o no
}

int Busca(TPatricia* raiz,char palavra[],int acumulador){ // função que busca uma palavra na arvore, tem como argumento  um no,a palavra em si, e um acumulador que serve pra função saber a partir de qual char comparar
	int existe = 0;// flag que indica se existe ou não a palavra, 0 não existe, 1 existe
	if(raiz!=NULL){ //se a raiz for nula com certeza nao existe entao so busco se for != null
		int qual = raiz->qualposicao;		 //usei uma variavel pra guardar qual posicao devo procurar
		if(raiz->filhoEsq == raiz->filhoDir){ //como as palavras so estao nas folhas, eu so comparo se for um no folha
			if(strcmp(raiz->palavra,palavra) == 0){ // se forem iguais
				return 1; //retorno 1 que indica que existe
			}
		}
		else if(palavra[qual+acumulador]-96 > raiz->palavra[qual+acumulador]-96){ // se não eu recurso pro filho esquerdo ou direito, dependendo do valor do char que foi definido la em cima no alfabeto
			existe = Busca(raiz->filhoDir,palavra,acumulador+raiz->qualposicao);
		}
		else if(palavra[qual+acumulador]-96 <= raiz->palavra[qual+acumulador]-96){//
			existe = Busca(raiz->filhoEsq,palavra,acumulador+raiz->qualposicao);
		}
	}
	return existe; //e retorno a flag
}
TPatricia* Decrementa(TPatricia* raiz,int decrementador){//essa função ela serve pra decrementar os valores da qualposicao de todos os nós e filhos de um nó, porém ele so decrementa nós não-folhas
	if(raiz!=NULL){// se a raiz for null retorna null
		raiz->filhoEsq = Decrementa(raiz->filhoEsq,decrementador);//recurso pro filho da esquerda
		if(raiz->filhoEsq != NULL || raiz->filhoDir !=NULL){ // se for não folha
    		raiz->qualposicao = raiz->qualposicao - decrementador; //eu decremento a qualposicao
		}
		raiz->filhoDir = Decrementa(raiz->filhoDir,decrementador);//e recurso pro filho da direito
	}
	return raiz; //e retorno o no modificado
}
TPatricia* Incrementa(TPatricia* raiz,int incrementador){//essa função ela serve pra incrementar os valores da qualposicao de todos os nós e filhos de um nó, porém ele só incrementa nós não-folhas
	if(raiz!=NULL){// se a raiz for null retorna null
		raiz->filhoEsq = Incrementa(raiz->filhoEsq,incrementador);//recurso pro filho da esquerda
		if(raiz->filhoEsq != NULL || raiz->filhoDir !=NULL){ // se for não folha
    		raiz->qualposicao = raiz->qualposicao + incrementador; //eu incremento a qualposicao
		}
		raiz->filhoDir = Incrementa(raiz->filhoDir,incrementador);//e recurso pro filho da direito
	}
	return raiz; //e retorno o no modificado
}
TPatricia* BuscaPai(TPatricia* no,char palavra[],int acumulador){ // essa função busca o pai do no folha a ser removido, usa os mesmos principios da busca normal
	TPatricia* caminhador = NULL;
	if(no!=NULL){
		if(no->filhoEsq == no->filhoDir){
			return no;
		}
		else{
			TPatricia* ponteiro = no->filhoEsq;
			if(ponteiro->filhoEsq == ponteiro->filhoDir){
				if(strcmp(no->filhoEsq->palavra,palavra) == 0){
					return no;
				}
			}
			ponteiro = no->filhoDir;
			if(ponteiro->filhoEsq == ponteiro->filhoDir){
				if(strcmp(no->filhoDir->palavra,palavra) == 0){
					return no;
				}
			}
			if(palavra[no->qualposicao+acumulador]-96 > no->palavra[no->qualposicao+acumulador]-96){
				caminhador = BuscaPai(no->filhoDir,palavra,no->qualposicao+acumulador);
			}
			else{
				caminhador = BuscaPai(no->filhoEsq,palavra,no->qualposicao+acumulador);
			}
		}
	}
	return caminhador;
}
TPatricia* Remove(TPatricia* no,char palavra[]){ //função que remove uma palavra da arvore
	int busca = Busca(no,palavra,0); //primeiro vejo se a palavra existe na arvore
	if(busca == 0){ // se não existe eu retorno o no e printo que não existe
		printf("Palavra nao existe na arvore\n");
		return no;
	}
	else{ //se não eu acho o no que corresponde ao pai do elemento a ser removido
		TPatricia* caminhador = BuscaPai(no,palavra,0); //que é feito nessa função
		if(caminhador->filhoEsq == caminhador->filhoDir){ //se a função me retorna uma folha, então so existe um elemento na arvore
			free(caminhador); // dou free e retorno null
			return NULL;
		}
		else{
			if(strcmp(caminhador->filhoEsq->palavra,palavra)==0){ //comparo pra ver se a palavra ta na folha esquerda
				caminhador->filhoDir = Incrementa(caminhador->filhoDir,caminhador->qualposicao); //se tiver eu incremento todos os nos da outra folha com a posicao do no pai
				free(caminhador->filhoEsq); //dou free
				memmove(caminhador,caminhador->filhoDir,128); //e movo o filho direito pro pai
				return no; //retorno o no
			}
			else{ //se não faço a mesma coisa so que espelhado
				caminhador->filhoEsq = Incrementa(caminhador->filhoEsq,caminhador->qualposicao);
				free(caminhador->filhoDir);
				memmove(caminhador,caminhador->filhoEsq,128);
				return no;
			}
		}
	}
	return no;
}
int Diferente(char palavra[],char palavranova[],int limite){ //essa função retorna a posição em que uma palavra é diferente da outra, o limite é o qualposicao da raiz que ta comparando a palavra com
	int contador = 0 ; //começa com 0
	while(palavranova[contador] == palavra[contador] && contador<limite){ //comparo char com char, e vai rodando o while enquanto for igual e enquanto o contador for menor que o limite
			contador++;
		}
	return contador; //e retorno esse int da diferença
}
TPatricia* InsereFolha(TPatricia* no,char palavra[],int qual){ //essa função insere uma palavra numa folha, esse "qual" é a posição que é diferente
	if(palavra[qual]-96 > no->palavra[qual]-96){ //comparo as letras pra saber qual deve ficar no novo nó a ser criado
		TPatricia* novo1 = CriaNo(no->palavra); //crio uma nova folha com  a palavra que tava
		no->filhoEsq = novo1; //e assigno devidamente os filhos
		TPatricia* novo2 = CriaNo(palavra); //mesma coisa aqui
		no->filhoDir= novo2;
	}
	else{ // mesma coisa so que de modo espelhado
		TPatricia* novo1 = CriaNo(no->palavra);
		TPatricia* novo2 = CriaNo(palavra);
		no->filhoEsq = novo2;
		no->filhoDir= novo1;
		strcpy(no->palavra,palavra);
	}
	no->qualposicao = qual; //e atualizo a posição do no
	return no;
}
TPatricia* InsereInter(TPatricia* no,char palavra[],int qual){ //Insere uma palavra em um no intermediario
	TPatricia* novo = CriaNo(palavra); //primeiro crio o no com a palavra
	TPatricia* novaraiz = CriaNo("foo"); //crio um no placeholder por enquanto
	if(palavra[qual]-96>no->palavra[qual]-96){ //verifico qual a menor letra, se a menor for a palavra que ta no nó atual
		strcpy(novaraiz->palavra,no->palavra); //se a menor letra for do no que to vendo entao copio a string pra nova raiz
		novaraiz->filhoEsq = no; //e assigno os filhos
		novaraiz->filhoDir = novo;
	}
	else{
		strcpy(novaraiz->palavra,palavra); //se não faço a mesma coisa so que de modo espelhado
		novaraiz->filhoEsq = novo;
		novaraiz->filhoDir = no;
	}
	novaraiz->filhoEsq = Decrementa(novaraiz->filhoEsq,qual); // decremento todos os filhos da esquerda
	novaraiz->filhoDir = Decrementa(novaraiz->filhoDir,qual); //e os da direita
	novaraiz->qualposicao = qual; // e atualizo a posicao
	return novaraiz; //e retorno o novo no
}
TPatricia* InsereSec(TPatricia* no,char palavra[]){ //função que junta as outras inserções
	int cont = Diferente(no->palavra,palavra,no->qualposicao); //primeiro conto pra ver onde ta a diferença
	if(cont == no->qualposicao){ //se o contador chegou no final do prefixo do no entao preciso me deslocar de no
		if(palavra[cont]>no->palavra[cont]){ // se a letra for maior que a letra do no, entao recurso pra direita
			no->filhoDir = InsereSec(no->filhoDir,palavra);
		}
		else if(palavra[cont]==no->palavra[cont]){ //se for IGUAL eu recurso pra esquerda
			no->filhoEsq = InsereSec(no->filhoEsq,palavra);
		}
		else if(palavra[cont]< no->palavra[cont]){ //se for menor eu crio um novo no pra ser o novo pai desse no
			no = InsereInter(no,palavra,cont);
		}
	}
	//se o contador deu menor
	else if(no->filhoEsq!=NULL  || no->filhoDir !=NULL){ //verifico se esse no é folha ou intermediario
		no = InsereInter(no,palavra,cont); //se for intermediario uso a devida função
	}
	else if(no->filhoEsq==NULL  && no->filhoDir ==NULL){
		no = InsereFolha(no,palavra,cont); // mesma coisa com a folha
	}
	return no; //e retorno o no modificado
}
TPatricia* Insere(TPatricia* raiz,char palavra[]){ //essa função usa a insereSec mas também insere na raiz
	if(raiz==NULL){ // se a raiz for nula entao crio novo no com a palavra e retorno ela
		TPatricia* novo = CriaNo(palavra);
		return novo;
	}
	else{// se não eu chamo a InsereSec
		raiz = InsereSec(raiz,palavra);
	}
	return raiz;
}
int main(){
	TPatricia* raiz = NULL; //raiz da arvore
	int busca; //inteiro da busca
	char palavra[20]; //a palavra que vou ler
	int opera = 0; //e a operação
	while(opera!=4){
		printf("Selecione a operacao\n");
		printf("1)Insere\n2)Busca\n3)Remove\n4)Finalizar\n");
		scanf("%d",&opera);
		if(opera== 1){
			printf("Digite a palavra\n");
			scanf("%s",&palavra);
			busca = Busca(raiz,palavra,0);
			if(busca==1){
				printf("%s ja existe na arvore\n",palavra);
			}
			else{
			raiz = Insere(raiz,palavra);
			}
		}
		else if(opera == 2){
			printf("Digite a palavra\n");
			scanf("%s",&palavra);
			busca = Busca(raiz,palavra,0);
			if(busca == 1){
				printf("%s existe na arvore\n",palavra);
			}
			else{
				printf("%s não existe na arvore\n",palavra);
			}
		}
		else if(opera == 3){
			printf("Digite a palavra\n");
			scanf("%s",&palavra);
			raiz = Remove(raiz,palavra);
		}
	}
}
