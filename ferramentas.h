#ifndef FERRAMENTAS_H
#define FERRAMENTAS_H

void erro(int numeroErro){

    switch(numeroErro){

        case 1: printf("[ERRO %d] Nao e um comando interno valido\n", numeroErro);
                break;

        case 2: printf("[ERRO %d] Erro na criacao de um novo diretorio\n", numeroErro);
                break;

        case 3: printf("[ERRO %d] Erro na criacao de um novo arquivo\n", numeroErro);
                break;

        case 4: printf("[ERRO %d] Arquivo ou diretorio inexistente\n", numeroErro);
                break;

        case 5: printf("[ERRO %d] Nao pode ser aberto pois e um arquivo\n", numeroErro);
                break;

        case 6: printf("[ERRO %d] Impossivel criar este diretorio. Este nome ja esta sendo utilizado neste diretorio\n", numeroErro);
                break;

        case 7: printf("[ERRO %d] Impossivel criar este arquivo. Este nome ja esta sendo utilizado neste diretorio\n", numeroErro);
                break;

        case 8: printf("[AVISO %d] Voce ja esta neste diretorio\n", numeroErro);
                break;
        case 9: printf("[AVISO %d] Arquivo inexistente\n", numeroErro);
                break;
        case 10: printf("[AVISO %d] Diretorio inexistente\n", numeroErro);
                break;
        case 11: printf("[ERRO %d] Impossivel remover a raiz de um diretorio\n", numeroErro);
                break;
        case 12: printf("[ERRO %d] Impossivel remover este diretorio pois existem diretorios subsequentes.\n", numeroErro);
                break;
        case 13: printf("[ERRO %d] Nao eh um diretorio.\n", numeroErro);
                break;
        case 14: printf("[ERRO %d] Nao eh um arquivo.\n", numeroErro);
                break;

    }
}

void tipoDiretorioOuArquivo(int tipo){

    if(tipo == 1){
        printf("<DIR>");
    }
    else if(tipo == 2){
        printf("<FILE>");
    }
    else if(tipo == 3){
        printf("<DIR>");
    }

}

#endif
