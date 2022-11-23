#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>//BIBLIOTECA BOOLEANA
#include <time.h>//BIBLIOTECA PARA USAR FUN��O ALEATORIA
#include <ctype.h>

//------------------------VARIAVEIS GLOBAIS -----------------
unsigned int sortPalavra;

//------------------------ ESTRUTURAS GLOBAIS ---------------
typedef struct registroPalavras{//STRUCT GLOBAL ARMAZENAMENTO DE PALVRAS E ID
	unsigned int idDica;
	char palavra[100];
}registro,temporarios;

typedef struct estrutDicas{//STRUCT GLOBAL ARMAZENAR DICA
	char dica[100];
}dica;

	
// -----------------------CHAMANDO AS FUN��ES --------------------------
FILE* acessarArquivo(char nome[],char modo[]);
FILE* fecharArquivo(FILE *arquivo);
void addPalavra();
void addDica();
void sorteioPalavra();
void voltarMenu();
void init();
void menu();
void mostraBoneco(int membros);
void resultado(char palavra[100], bool result);
void iniciarJogo();
bool verificaLetra(char letra, char palavraEdit[100], char *esconde, char palavra[100]);
void creditos();


int main(){
	init();
	menu();	

	return 0;
}

//------------------FUN��ES-------------------

//FUN��O PARA ACESSAR O ARQUIVO
FILE* acessarArquivo(char nome[], char modo[]){
	FILE *arquivo;
	arquivo= fopen(nome,modo);
	if(arquivo==NULL){//VERIFICA��O DE ERRO
		printf("/nERROOO !!\nArquivo nao foi ABERTO!\n");	
	}
	return arquivo;
}

//FUN��O PARA FECHAR O ARQUIVO
FILE* fecharArquivo(FILE *arquivo){
	if(arquivo==NULL){//VERIFICA��O DE ERRO
		printf("/nERROOO !!\nArquivo nao foi FECHADO!\n");	
	}
	fclose(arquivo);
	return arquivo;
}

//FUN��O PARA ADDICIONAR A PALAVRA
void addPalavra(){
	char resposta;
	bool erro;
	FILE *temp;
	//---chamando estruturas---
	registro recolhe; 
	temporarios temporario;
	dica guarda,registroTemp;
	
	do{//loop para para confirmar a palavra e armazenala
		system("cls");
		printf("\nDIGITE UMA PALAVRA: ");
		fflush(stdin);
		scanf("%99[^\n]",recolhe.palavra);
		printf("DIGITE A DICA: ");
		fflush(stdin);
		scanf("%99[^\n]",guarda.dica);
		printf("\nDESEJA USAR ESSA PALAVRA ? <S/N>");
		fflush(stdin);
		resposta=getchar();
	}while(resposta!='S' && resposta!='s');
	
	strupr(recolhe.palavra);//passar os caracteres das strings para maiusculas
	strupr(guarda.dica);
	temp=acessarArquivo("dicas.fc","r");//abrir o arquivo dica.fc
	erro=true;
	recolhe.idDica=0;//declara�ao valor idDica
	
	while(!feof(temp)){//Checar se a dicas � existente no arquivo
		fread(&registroTemp,sizeof(registroTemp),1,temp);
		if(strcmp(guarda.dica,registroTemp.dica)==0){//compara��o das duas strings
			erro=false;
			break;
		}
		recolhe.idDica++;//add +1 no id da Dica
	}
	temp=fecharArquivo(temp);
	
	if(erro==true){//VERIFICA ERRO DE DICA N�O EXISTENTE
		printf("\nERROO!... DICA NAO EXISTENTE");
		printf("\nAPERTE ENTER PARA VOLTAR AO MENU.\n");
		fflush(stdin);
		resposta=getchar();
		return;
	}
	temp=acessarArquivo("palavras.fc","r");//VERIFICAR ERRO SE PALAVRA JA EXISTE
	erro=false;
	while(!feof(temp)){//enquanto !n�o chegar no final do arquivo
		fread(&temporario,sizeof(temporario),1,temp);//ler arquivo
		if(strcmp(recolhe.palavra,temporario.palavra)==0){//comparando as palavras
			erro=true;
			break;
		}
	}
	if(erro==true){
		printf("\nERROO!... ESSA PALAVRA JA EXISTE!");
		printf("\nAPERTE ENTER PARA VOLTAR AO MENU.\n");
		fflush(stdin);
		resposta=getchar();
		return;
	}	
	temp=fecharArquivo(temp);
	
	temp=acessarArquivo("palavras.fc","a");//ARQUIVO ADD PALAVRA
	if(fwrite(&recolhe,sizeof(recolhe),1,temp)!=1){//adiciona a palavra e verifica erro caso add mais de uma roda o if
		printf("\nNAO FOI POSSIVEL ADICIONAR PALAVRA");
		printf("\nAPERTE ENTER PARA VOLTAR AO MENU.\n");
		fflush(stdin);
		resposta=getchar();
		return;	
	}
	temp=fecharArquivo(temp);//fecha arquivo 
	voltarMenu();//direciona para fun�ao do menu
}

//FUN��O PARA RECOLHER A DICA DO USUARIO
void addDica(){
	dica recolhe, temp;
	char resposta;
	bool erro=false;
	do{//loop para armazenar dica e confirmar resposta 
		system("cls");
		printf("\nDIGITE UMA DICA:");
		fflush(stdin);
		scanf("%s[^\n]",&recolhe.dica);
		printf("\nDESEJA USAR ESSA DICA ? <S/N>");
		fflush(stdin);
		resposta=getchar();
	}while(resposta!='S' && resposta!='s');
	//verifica��o se tem alguma palavra igual
	FILE *arquivo;
	strupr(recolhe.dica);//pega a dica e transforma os caracteres em maiusculas
	arquivo=acessarArquivo("dicas.fc","r");//abre arquivo dicas.fc em modo leitura
	while(!feof(arquivo)){//loop verificar se dica � existente
		fread(&temp,sizeof(temp),1,arquivo);//ler os dados do arquivo
		if(strcmp(recolhe.dica,temp.dica)==0){//verificando se as dica ja � existente
			printf("\nERROO!... ESSA DICA JA EXISTE!");
			erro=true;
			break;
		}
	}
	arquivo=fecharArquivo(arquivo);
	if(erro==false){//verifica se palavras forem diferentes add em acessarArquivo
		arquivo=acessarArquivo("dicas.fc","a");//abir arquivo dicas.fc modo adicinar
		fwrite(&recolhe,sizeof(recolhe),1,arquivo);// add dados no arquivo dicas.fc
		arquivo=fecharArquivo(arquivo);
	}
	voltarMenu();
}

//FUN��O PARA SORTEIO DAS PALAVRAS
void sorteioPalavra(){
	srand(time(NULL));
	unsigned int quantidadePalavras=0;//variavel para contar a quantidade de palavras do arquivo
	FILE *arquivo;
	registro temp;
	arquivo=acessarArquivo("palavras.fc","r");
	while(!feof(arquivo)){//ler quantidade de palvras e add +1 conforme o la�o percorre cada uma
		fread(&temp,sizeof(temp),1,arquivo);//ler dados 
		quantidadePalavras++;
	}
	quantidadePalavras-=1;
	arquivo=fecharArquivo(arquivo);
	sortPalavra=rand()%quantidadePalavras;//sorteia palavra 
}

//FUN��O INICIALIZAR OS ARQUIVOS NO CORPO DO PROJETO
void init(){
	FILE *temp;
	
	//INICIALIZANDO ARQUIVO DICA.FC
	temp=acessarArquivo("dicas.fc","a");
	temp=fecharArquivo(temp);
	
	//INICIALIZANDO ARQUIVO PALAVRAS.FC
	temp=acessarArquivo("palavras.fc","a");
	temp=fecharArquivo(temp);
}

//FUN��O PARA VOLTAR AO MENU
void voltarMenu(){
	printf("\n\t\t\tAPERTE ENTER PARA VOLTAR AO MENU.\n");
	fflush(stdin);
	getchar();
}

//FUN��O DO MENU
void menu(){
	char escolha;
	do{
		system("cls");
		printf("\n\n\n");
		printf("\t\t\t\t#######################################################\n");
		printf("\t\t\t\t##***************************************************##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*                JOGO DA FORCA                    *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##***************************************************##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*            [1] - INICIAR JOGO.                  *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*            [2] - CADASTRAR PALAVRA.             *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*            [3] - CADASTRAR DICA.                *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*            [4] - CREDITOS.                      *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*            [5] - SAIR.                          *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##*                                                 *##\n");
		printf("\t\t\t\t##***************************************************##\n");
		printf("\t\t\t\t#######################################################\n");
		
		printf("\n\n\t\t\t\tDIGITE SUA ESCOLHA: ");
		fflush(stdin);
		scanf("%c",&escolha);
		
		if(escolha=='1'){
			iniciarJogo();
		}
		else if(escolha=='2'){
			addPalavra();
		}
		else if(escolha=='3'){
			addDica();
		}
		else if(escolha=='4'){
			creditos();
		}else{
			break;
		}
		
	}while(escolha!='5'); 	
}

//FUN��O PARA INICIAR JOGO
void iniciarJogo(){
	registro recolhe;
	dica guarda;
	char esconde[100];
	char palavraEdit[100];
	char letra;
	char letDigi[50]={'\0'};
	int i;
	int j;
	int quantidadeErro=0;
	int tamanho=0;
	bool fim=false;
	bool result;
	FILE *arquivo;
	sorteioPalavra(); 
	arquivo=acessarArquivo("palavras.fc","r");
	fseek(arquivo,sizeof(recolhe)*sortPalavra,SEEK_SET);//ler a palavra sorteada da variavel global sortPalavra
	fread(&recolhe,sizeof(recolhe),1,arquivo);
	arquivo=fecharArquivo(arquivo);
	
	arquivo=acessarArquivo("dicas.fc","r");
	fseek(arquivo,sizeof(guarda)*recolhe.idDica,SEEK_SET);
	fread(&guarda,sizeof(guarda),1,arquivo);
	arquivo=fecharArquivo(arquivo);
	
	for(i=0;i<strlen(recolhe.palavra);i++){
		if(recolhe.palavra[i]=='�' || recolhe.palavra[i]=='�' || recolhe.palavra[i]=='�'){
			palavraEdit[i]='A';
		}
		else if(recolhe.palavra[i]=='�' || recolhe.palavra[i]=='�'){
			palavraEdit[i]='E';
		}
		else if(recolhe.palavra[i]=='�' || recolhe.palavra[i]=='�'){
			palavraEdit[i]='I';
		}
		else if(recolhe.palavra[i]=='�' || recolhe.palavra[i]=='�' || recolhe.palavra[i]=='�'){
			palavraEdit[i]='O';
		}
			else if(recolhe.palavra[i]=='�' || recolhe.palavra[i]=='�'){
			palavraEdit[i]='U';
		}else{
			palavraEdit[i]=recolhe.palavra[i];
		}
	}
	for(i=0;i<strlen(recolhe.palavra);i++){
		if(recolhe.palavra[i]==' '){
			esconde[i]=' ';
		}
		else if(recolhe.palavra[i]=='-'){
			esconde[i]='-';
		}else{
			esconde[i]='_';
		}
		if(recolhe.palavra[i]!=' ' && recolhe.palavra[i]!='-'){
			tamanho++;
		}
	}
	esconde[i]='\0';
	j=0;
	while(fim==false){
		system("cls");
		mostraBoneco(quantidadeErro);
		printf("\n\t\t\t");
		for(i=0;i<strlen(esconde);i++){
			printf(" %c ",esconde[i]);
		}
		printf("\n\n\n\tLETRAS DIGITADAS: ");
		for(i=0;i<strlen(letDigi);i++){
			printf(" %c ",letDigi[i]);
		}
		printf("\n\n\tDICA: %s\n",guarda.dica);
		printf("\n\tTAMANHO: %d",tamanho);
		printf("\n\n\tDIGITE UMA LETRA: ");
		fflush(stdin);
		scanf("%c",&letra);
		letra=toupper(letra);
		letDigi[j]=letra;
		letDigi[j+1]='\0';
		j++;
		
		if(verificaLetra(letra,palavraEdit,esconde,recolhe.palavra)==true){
			quantidadeErro++;
		}
		if(quantidadeErro>=6){
			fim=true;
			result=false;
		}
		else if(strcmp(recolhe.palavra,esconde)==0){
			result=true;
			fim=true;	
		}
	}
	resultado(recolhe.palavra,result);
}

//FUN��O PARA VERIFICAR A LETRA
bool verificaLetra(char letra,char palavraEdit[100], char *esconde, char palavra[100]){
	bool erro=true;
	int i;
	for(i=0;i<strlen(palavraEdit);i++){
		if(letra==palavraEdit[i]){
			erro=false;
			esconde[i]=palavra[i];
		}
	}
	return erro;
}

//FUN��O ANALIZAR RESULTADOS
void resultado(char palavra[100],bool result){
	system("cls");
	printf("------------------------------------------RESULTADO--------------------------------------------------");
	printf("\n\n\t\t\t\tPALAVRA: %s\n",palavra);
	if(result==true){
		printf("\n\t\t\t\tPARABENS!!!!...VOCE GANHOU!!\n\n\n\n");
	}else{
		printf("\n\t\t\t\tERROU!!!!...VOCE PERDEU!!\n\n\n\n");
	}
	voltarMenu();
}

//FUN��O DESENHO DO BONECO
void mostraBoneco(int membros){
	switch(membros){
		case 1:
			printf("\n\t�����������   ");
			printf("\n\t�         |   ");
			printf("\n\t�        ***  ");
			printf("\n\t�       *   * ");
			printf("\n\t�        ***  ");
			printf("\n\t�             ");
			printf("\n\t�             ");
			printf("\n\t�             ");
			printf("\n\t�             ");
			printf("\n\t�             ");
			printf("\n\t�");
			printf("\n\t�");
			break;
		case 2:
			printf("\n\t�����������   ");
			printf("\n\t�         |   ");
			printf("\n\t�        ***  ");
			printf("\n\t�       *   * ");
			printf("\n\t�        ***  ");
			printf("\n\t�         |   ");
			printf("\n\t�         |   ");
			printf("\n\t�         |   ");
			printf("\n\t�             ");
			printf("\n\t�             ");
			printf("\n\t�");
			printf("\n\t�");
			break;
		case 3:
			printf("\n\t�����������   ");
			printf("\n\t�         |   ");
			printf("\n\t�        ***  ");
			printf("\n\t�       *   * ");
			printf("\n\t�        ***  ");
			printf("\n\t�        /|   ");
			printf("\n\t�       / |   ");
			printf("\n\t�         |   ");
			printf("\n\t�             ");
			printf("\n\t�             ");
			printf("\n\t�");
			printf("\n\t�");
			break;
		case 4:
			printf("\n\t�����������  ");
			printf("\n\t�         |     ");
			printf("\n\t�        ***    ");
			printf("\n\t�       *   *   ");
			printf("\n\t�        ***    ");
			printf("\n\t�        /|\\  ");
			printf("\n\t�       / | \\ ");
			printf("\n\t�         |     ");
			printf("\n\t�               ");
			printf("\n\t�               ");
			printf("\n\t�               ");
			printf("\n\t�               ");
			break;
		case 5:
			printf("\n\t����������� ");
			printf("\n\t�         |      ");
			printf("\n\t�        ***     ");
			printf("\n\t�       *   *    ");
			printf("\n\t�        ***     ");
			printf("\n\t�        /|\\    ");
			printf("\n\t�       / | \\   ");
			printf("\n\t�         |      ");
			printf("\n\t�        /       ");
			printf("\n\t�       /        ");
			printf("\n\t�                ");
			printf("\n\t�                ");
			break;
		case 6:
			printf("\n\t�����������   ");
			printf("\n\t�         |      ");
			printf("\n\t�        ***     ");
			printf("\n\t�       *   *    ");
			printf("\n\t�        ***     ");
			printf("\n\t�        /|\\     ");
			printf("\n\t�       / | \\   ");
			printf("\n\t�         |      ");
			printf("\n\t�        / \\     ");
			printf("\n\t�       /   \\    ");
			printf("\n\t�                ");
			printf("\n\t�                ");
			break;
		default:
			printf("\n\t����������� ");
			printf("\n\t�         | ");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			printf("\n\t�");
			break;
	}
}

//FUN��O CREDITOS
void creditos(){
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\t\t\t\t\t\tDESENVOLVEDORES: ");
	printf("\n\t\t\t\t\t\tAyrton M. Rodrigues.");
	printf("\n\t\t\t\t\t\tJose Vieira Stable.\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	system("pause");
}
