#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void LimparTela(void){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

typedef struct {
    char nome[50];
    int matricula;
    int idade;
    double notas[4];    
}Aluno;

bool AumentarCapacidade(int *capacidade, Aluno **v){
    int novaCapacidade = (*capacidade)*2;
    Aluno *aux = realloc(*v, novaCapacidade * sizeof(Aluno));
    
    if (aux == NULL){
        printf("Erro ao realocar memoria!\n");
        return false;
    }

    *v = aux;
    *capacidade = novaCapacidade;
    return true;
}

bool VerificarCapacidade(int alunosCadastrados, int *capacidade, Aluno **v){
    if (alunosCadastrados == *capacidade){
        return AumentarCapacidade(capacidade, v);
    }
    return true;
}

void CadastrarAluno(int *alunosCadastrados, Aluno *v ){
    LimparTela();
    int i = *alunosCadastrados;

    getchar();
    printf("Digite o nome do(a) aluno(a): ");
    fgets(v[i].nome, 50, stdin);
    v[i].nome[strcspn(v[i].nome, "\n")] = '\0';

    printf("Digite a matricula dele(a): ");
    scanf("%d", &v[i].matricula);

    printf("Digite a idade do(a) aluno(a)");
    scanf("%d",&v[i].idade);

    for (int j = 0; j < 4; j++){
        printf("Digite a %d nota do(a) aluno(a): ", (j+1));
        scanf("%lf", &v[i].notas[j]);
    }

    (*alunosCadastrados)++;
}

void painel(){
    printf("[1] Cadastrar aluno\n [2] Listar Alunos\n [3]Buscar aluno pela matricula\n [4]Editar um aluno\n [5]Remover aluno\n [0]Sair\n Digite qual acao voce deseja realizar: ");
}




int main(int argc, char const *argv[]){
    int resp = 0, alunosCadastrados = 0;
    int capacidade = 2;

    Aluno *v = malloc(capacidade * sizeof(Aluno));

    if (v == NULL){
        printf("[ERRO] Não há mais capacidade!");
        return 1;
    }
    

    do{
        painel();

        scanf("%d", &resp);

        switch (resp){
            case 1:{
                int qtd;
                printf("Quantos alunos deseja cadastrar? ");
                scanf("%d", &qtd);

                for (int i = 0; i < qtd; i++){
                    if (VerificarCapacidade(alunosCadastrados, &capacidade, &v)){
                        printf("\n--- Aluno %d ---\n", i + 1);
                        CadastrarAluno(&alunosCadastrados, v);    
                    }else{
                        printf("Nao ha espaço na memoria suficiente!");
                        break;
                    }    
                }
            break;
            }
            case 2:

            break;

            default:

            break;
        }
        
    } while (resp != 0);
    
    free(v);
    return 0;
}
