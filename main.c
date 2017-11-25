#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ferramentas.h"


#define MAX_STRING 255
#define MAX_CAMINHO 150
typedef struct noDiretorioArquivo{

    char nome[MAX_STRING];
    int tipo; //1 = diretorio, 2 = arquivo, 3= .. (diretorio anterior e novo nivel)

    struct noDiretorioArquivo * proximo;
    struct noDiretorioArquivo * anterior;
    struct noDiretorioArquivo * subDiretorio;


}tpNoDiretorioArquivo;

typedef struct caminhoDiretorio{
    tpNoDiretorioArquivo * vetCaminhoDiretorio[MAX_CAMINHO];
    int posVet;
}tpCaminhoDiretorio;

tpNoDiretorioArquivo * criaNoDiretorioArquivo(char * nome, int tipo){

    tpNoDiretorioArquivo * novoNo;

    novoNo = (tpNoDiretorioArquivo * ) malloc(sizeof(tpNoDiretorioArquivo));

    strcpy(novoNo->nome, nome);
    novoNo->tipo = tipo;

    novoNo->proximo = NULL;
    novoNo->anterior = NULL;
    novoNo->subDiretorio = NULL;

    return novoNo;
}

void imprimeArquivoDiretorio(tpNoDiretorioArquivo * no){

    if(no==NULL){
        erro(4);
        return;
    }
    else{
        printf("\t");
        tipoDiretorioOuArquivo(no->tipo);
        printf("\t%s\n", no->nome);
    }
}

//Este nivel é como se fosse a cabeça do diretorio, que aponta para o diretorio anterior, e aponta para os proximos diretorios e/ou arquivos
tpNoDiretorioArquivo * criaNivelDiretorio(tpNoDiretorioArquivo *path){
    tpNoDiretorioArquivo * novoNivelDiretorio;
    novoNivelDiretorio = criaNoDiretorioArquivo("..", 3);
    path->subDiretorio = novoNivelDiretorio;
    novoNivelDiretorio->anterior = path;
    return novoNivelDiretorio;
}

tpNoDiretorioArquivo * criaDiretorio(char *nomeDiretorio, tpNoDiretorioArquivo * path){
    tpNoDiretorioArquivo * novoDiretorio;

    novoDiretorio = criaNoDiretorioArquivo(nomeDiretorio, 1);
    criaNivelDiretorio(novoDiretorio);
    novoDiretorio->subDiretorio->anterior = path; //nivel anterior
    path->subDiretorio = novoDiretorio;

    return novoDiretorio;
}

tpNoDiretorioArquivo * criaArquivo(char *nomeArquivo){
    tpNoDiretorioArquivo * novoArquivo;
    novoArquivo = criaNoDiretorioArquivo(nomeArquivo, 2);
    novoArquivo->subDiretorio = NULL;
    return novoArquivo;
}

void insereNoDiretorioArquivo(tpNoDiretorioArquivo * diretorioArquivoInserir, tpNoDiretorioArquivo *path){

    tpNoDiretorioArquivo *atual = path;

    while(atual!=NULL){

        if(atual->proximo==NULL){
            atual->proximo = diretorioArquivoInserir;
            diretorioArquivoInserir->anterior=atual;
            return;
        }
        atual=atual->proximo;
    }
}

void ls(tpNoDiretorioArquivo *path){
    tpNoDiretorioArquivo * atual = path;

    while(atual!=NULL){
        imprimeArquivoDiretorio(atual);
        atual=atual->proximo;
    }
}

tpNoDiretorioArquivo * cd(char *nomeDiretorioArquivo, tpNoDiretorioArquivo *path){
    tpNoDiretorioArquivo * atual = path;

    while((strcmp(atual->nome, nomeDiretorioArquivo)==0) || atual->proximo!=NULL){

        if((strcmp(nomeDiretorioArquivo, "/")==0)){
            erro(8);
            return path;
        }
        else if((strcmp(atual->nome, nomeDiretorioArquivo)==0)&&(atual->tipo == 2)){
            erro(5);
            return path;
        }
        else if((strcmp(atual->nome, nomeDiretorioArquivo)==0)&&(atual->tipo == 1)){
            return atual->subDiretorio;
        }
        else if((strcmp(atual->nome, nomeDiretorioArquivo)==0)&&(atual->tipo == 3)){
            return atual->anterior;
        }

        atual = atual->proximo;
    }
return path;
}

int verificaExistenciaDiretorioArquivo(tpNoDiretorioArquivo  * path, char *nome){ //0 se existe, 1 se não existe
    tpNoDiretorioArquivo * atual = path;

    for(;atual!=NULL;atual=atual->proximo){
        if(strcmp(atual->nome, nome)==0){
            return 0;
        }
    }
    return 1;
}

int rmDir(char *nomeDiretorioArquivo, tpNoDiretorioArquivo *path){

     tpNoDiretorioArquivo * atual = path;

    if(verificaExistenciaDiretorioArquivo(path, nomeDiretorioArquivo)==0){ //verifica existencia
            if(strcmp(nomeDiretorioArquivo, "..")==0 || strcmp(nomeDiretorioArquivo, "/")==0){
                erro(11);
                return 1;
            }
            else{ //remoção
                for(;atual!=NULL;atual=atual->proximo){
                    if(strcmp(atual->nome, nomeDiretorioArquivo)==0 && (atual->subDiretorio->proximo==NULL)){

                        if(atual->proximo==NULL){
                            atual->anterior->proximo = NULL;
                            free(atual);
                            return 2;
                        }
                        else{
                            atual->anterior->proximo = atual->proximo;
                            atual->proximo->anterior = atual->anterior;
                            free(atual);
                            return 2;
                        }
                    }
                    else if(strcmp(atual->nome, nomeDiretorioArquivo)==0 && atual->subDiretorio->proximo!=NULL){ //diretorios subsequentes
                        erro(12);
                        return 1;
                    }
                }
            }
    }
    else{
        return 1; //não encontrado
    }
return 3;
}

tpNoDiretorioArquivo * node(char *nome, tpNoDiretorioArquivo *path){

 tpNoDiretorioArquivo * atual = path;

    for(;atual!=NULL;atual=atual->proximo){
        if(strcmp(atual->nome, nome)==0){
            return atual;
        }
    }
    return NULL;


}

int rmFile(char *nomeArquivo, tpNoDiretorioArquivo *path){

     tpNoDiretorioArquivo * atual = path;

    if(verificaExistenciaDiretorioArquivo(path, nomeArquivo)==0){ //verifica existencia

                for(;atual!=NULL;atual=atual->proximo){
                    if(strcmp(atual->nome, nomeArquivo)==0 && atual->tipo==2){

                        if(atual->proximo==NULL){
                            atual->anterior->proximo = NULL;
                            free(atual);
                            return 2;
                        }
                        else{
                            atual->anterior->proximo = atual->proximo;
                            atual->proximo->anterior = atual->anterior;
                            free(atual);
                            return 2;
                        }
                    }
                }
    }

    else{
        return 1; //não encontrado
    }
return 3;
}

tpCaminhoDiretorio * iniciaCaminhoDiretorio(){
    tpCaminhoDiretorio * caminhoDiretorio;
    caminhoDiretorio = (tpCaminhoDiretorio *)malloc(sizeof(tpCaminhoDiretorio));
    caminhoDiretorio->posVet= -1;
    return caminhoDiretorio;
}

void insereCaminhoDiretorio(tpCaminhoDiretorio *caminhoDiretorio, tpNoDiretorioArquivo *path, int codIncDec){ //1 -> avançar diretorio, 2 -> retroceder diretorio
    if(codIncDec == 1){
        caminhoDiretorio->posVet++;
        caminhoDiretorio->vetCaminhoDiretorio[caminhoDiretorio->posVet] = path->anterior->proximo;
    }
    else if(codIncDec == 2){
        caminhoDiretorio->posVet--;
    }
}

void printaCaminhoDiretorio(tpCaminhoDiretorio *caminhoDiretorio){
     int i;
     printf("/:");
     for(i=0;i<=caminhoDiretorio->posVet;i++)
        printf("%s>", caminhoDiretorio->vetCaminhoDiretorio[i]->nome);
     printf(" >>");
}

int isArquivo(tpNoDiretorioArquivo  * path, char *nome){ //0 se existe, 1 se não existe
    tpNoDiretorioArquivo * atual = path;

    for(;atual!=NULL;atual=atual->proximo){
        if(strcmp(atual->nome, nome)==0 && atual->tipo == 2){
            return 0; //é arquivo
        }
    }
    return 1;
}

void comandosAcao(tpNoDiretorioArquivo *raiz, tpNoDiretorioArquivo *path){
    char comando[MAX_STRING];
    tpCaminhoDiretorio * caminhoDiretorio = iniciaCaminhoDiretorio();


    do{
        printaCaminhoDiretorio(caminhoDiretorio);
        scanf("%s", comando);

        if(strcmp(comando, "mkdir")==0){
            char nomeDiretorio[MAX_STRING];
            tpNoDiretorioArquivo * novoDiretorio;
            scanf("%s", nomeDiretorio);

            if(verificaExistenciaDiretorioArquivo(path, nomeDiretorio)==1){
                novoDiretorio = criaDiretorio(nomeDiretorio, path);
                insereNoDiretorioArquivo(novoDiretorio, path);
            }
            else if(verificaExistenciaDiretorioArquivo(path, nomeDiretorio)==0){
                erro(6);
            }
        }

        else if(strcmp(comando, "mkfile")==0){
            char nomeArquivo[MAX_STRING];
            tpNoDiretorioArquivo * novoArquivo;
            scanf("%s", nomeArquivo);
            if(verificaExistenciaDiretorioArquivo(path, nomeArquivo)==1){
                novoArquivo = criaArquivo(nomeArquivo);
                insereNoDiretorioArquivo(novoArquivo, path);
            }
            else if(verificaExistenciaDiretorioArquivo(path, nomeArquivo)==0)
            {
                erro(7);
            }
        }

        else if(strcmp(comando, "rmdir")==0){
            tpNoDiretorioArquivo * temp;
            char nomeDiretorio[MAX_STRING];
            scanf("%s", nomeDiretorio);

            temp = node(nomeDiretorio, path);
            if(temp->tipo==1){
                if(rmDir(nomeDiretorio, path)==1){
                    erro(10);
                }
            }
            else erro(13);

        }

        else if(strcmp(comando, "rm")==0){
            tpNoDiretorioArquivo * temp;
            char nomeArquivo[MAX_STRING];
            scanf("%s", nomeArquivo);

            temp = node(nomeArquivo, path);
            if(temp->tipo==2){
                 rmFile(nomeArquivo, path);
            }
            else erro(14);

        }

        else if(strcmp(comando, "cd")==0){
            char nomeDiretorioArquivo[MAX_STRING];
            scanf("%s", nomeDiretorioArquivo);
            if(strcmp(nomeDiretorioArquivo, "/")!=0){
                if(verificaExistenciaDiretorioArquivo(path, nomeDiretorioArquivo)==0){
                       if(isArquivo(path, nomeDiretorioArquivo)==1){
                            path = cd(nomeDiretorioArquivo, path);

                            if(strcmp(nomeDiretorioArquivo, "..")==0){ //retorno
                                insereCaminhoDiretorio(caminhoDiretorio, path, 2);
                            }
                            else{
                                insereCaminhoDiretorio(caminhoDiretorio, path, 1);
                            }
                      }
                      else erro(5);
                }
            }
                else if(verificaExistenciaDiretorioArquivo(path, nomeDiretorioArquivo)==1){
                    erro(4);
                }

        }

        else if(strcmp(comando, "ls")==0){
            ls(path);
        }

        else if(strcmp(comando, "cls")==0){
            system("cls");
        }

        else erro(1);
    }while(1);

}





int main()
{
    tpNoDiretorioArquivo * raiz = criaNoDiretorioArquivo("/", 1);
    tpNoDiretorioArquivo * path = raiz; //path indica o nivel de localização atual (orientação). só percorre entre as ".."

    criaNivelDiretorio(path);

    comandosAcao(raiz, path);

    return 0;
}
