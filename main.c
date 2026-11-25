#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LimparTela(void) {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

void LimparBufferEntrada(){
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF);
}

typedef struct {
    char nome[50];
    int matricula;
    int idade;
    double notas[4];    
}Aluno;

//funcao para aumentar a capacidade do vetor dinamico ele é bool para retornar se deu certo ou nao, assim como o VerificarCapacidade()
bool AumentarCapacidade(int *capacidade, Aluno **aluno){ 
    /*foi usado **aluno pois é para aumentar a capacidade do vetor dentro de uma funcao, se não fosse usado isso poderia causar problemas com o realloc pois ele muda o endereco de memoria pois ele busca um novo espaco e isso poderia fazer perder o endereco original, e como se fosse um ponteiro de um ponteiro*/

    int novaCapacidade = (*capacidade)*2;
    Aluno *aux = realloc(*aluno, novaCapacidade * sizeof(Aluno));
    
    if (aux == NULL){
        printf("Erro ao realocar memoria!\n");
        return false;
    }

    *aluno = aux;
    *capacidade = novaCapacidade;
    return true;
}

//funcao para verificar a capacidade do vetor dinamico, se ainda tem espaco ou nao dentro dele, se nao tiver espaço ele vai chamar a funcao AumentarCapacidade()
bool VerificarCapacidade(int alunosCadastrados, int *capacidade, Aluno **aluno){
    if (alunosCadastrados == *capacidade){
        return AumentarCapacidade(capacidade, aluno);
    }
    return true;
}

bool VerificarMatricula(int alunosCadstrados, Aluno *aluno, int novaMatricula){
    for (int i = 0; i < alunosCadstrados; i++){
        if (novaMatricula == aluno[i].matricula){
            return false;
        }
    }
    return true;
}

//funcao para cadastrar um aluno, ela pede todas as informações da struct e cadastra ele por um ponteiro de vetor dinamico
// ... (mantenha LimparTela, struct Aluno, LimparBufferEntrada, AumentarCapacidade e VerificarCapacidade iguais)

void CadastrarAluno(int *alunosCadastrados, Aluno *aluno, int numeroOrdem){
    LimparTela();
    printf("--- Aluno %d ---\n", numeroOrdem);

    int i = *alunosCadastrados;
    int novaMatricula;

    getchar();
    printf("Digite o nome do(a) aluno(a):\n");
    fgets(aluno[i].nome, 50, stdin);
    aluno[i].nome[strcspn(aluno[i].nome, "\n")] = '\0';

    bool matriculaValida = false;
    do{
        printf("Digite a matricula dele(a):\n");
        scanf("%d", &novaMatricula);

        if (VerificarMatricula(*alunosCadastrados, aluno, novaMatricula)){
            aluno[i].matricula = novaMatricula;
            matriculaValida = true;
        }else{
            printf("[ERRO] A matricula digitada ja esta cadastrada, favor digitar uma matricula valida\n");
        }    
    } while (!matriculaValida);

    printf("Digite a idade do(a) aluno(a):\n");
    scanf("%d", &aluno[i].idade);

    for (int j = 0; j < 4; j++){
        printf("Digite a %d nota do(a) aluno(a):\n", (j+1));
        scanf("%lf", &aluno[i].notas[j]);
    }

    (*alunosCadastrados)++;
}

void ListarAlunos(Aluno *aluno, int alunosCadastrados){
    for (int i = 0; i < alunosCadastrados; i++){
        printf("\n--- Aluno %d ---\n Matricula: %d\n Nome: %s\n Idade: %d\n", (i + 1), aluno[i].matricula, aluno[i].nome, aluno[i].idade);
        printf("Notas: \n");
        for (int j = 0; j < 4; j++){
            printf("Nota %d: %.2lf\n", (j+1), (aluno[i].notas[j])); 
        }
        
    }
    
}

int BuscarAluno(Aluno *aluno, int alunosCadastrados, int matricula){
    for (int i = 0; i < alunosCadastrados; i++){
        if (matricula == aluno[i].matricula){
            return i;
        }
    }
    return -1;   
}



//painel meramente ilustrativo para mostrar as funcionalidades
void painel(){
    printf("\n[1] Cadastrar aluno\n [2] Listar Alunos\n [3]Buscar aluno pela matricula\n [4]Editar um aluno\n [5]Remover aluno\n [0]Sair\n Digite qual acao voce deseja realizar: ");
}




int main(){
    //iniciei os vetores dinamicos
    int resp = 0, alunosCadastrados = 0;
    int capacidade = 2;

    Aluno *aluno = malloc(capacidade * sizeof(Aluno));

    if (aluno == NULL){
        printf("[ERRO] Não há mais capacidade!");
        return 1;
    }
    

    do{
        painel();

        scanf("%d", &resp);

        switch (resp){
            case 1:{
                LimparTela();
                int qtd;

                printf("Quantos alunos deseja cadastrar? ");
                scanf("%d", &qtd);
                
                if (qtd <= 0){
                    printf("[ERRO] Impossivel cadastrar esta quantidade de alunos!!");
                    break;
                }
                
                for (int i = 0; i < qtd; i++){
                    if (VerificarCapacidade(alunosCadastrados, &capacidade, &aluno)){
                        CadastrarAluno(&alunosCadastrados, aluno, i + 1);    
                    } else {
                        printf("Nao ha espaco na memoria suficiente!\n");
                        break;
                    }    
                }
                break;
            }
            case 2:
                LimparTela();
                if (alunosCadastrados <= 0){
                    printf("Nao ha alunos cadastrados para listar");
                    break;
                }
                
                ListarAlunos(aluno, alunosCadastrados);
                LimparBufferEntrada();
                printf("\nPressione Enter para continuar...");
                getchar();
                LimparTela();
                break;
            
            case 3:{
                LimparTela();

                int matriculaBusca;
                int indice;

                if (alunosCadastrados <= 0){
                    printf("Nao ha alunos cadastrados ainda!");
                    break;
                }
                printf("Digite a matricula do aluno que deseja encontrar: ");
                scanf("%d", &matriculaBusca);
                
                indice = BuscarAluno(aluno, alunosCadastrados, matriculaBusca);
                if (indice != -1){
                    printf("Aluno encontrado!\n Matricula: %d\n Nome: %s\n Idade: %d\n Notas:\n", aluno[indice].matricula, aluno[indice].nome, aluno[indice].idade);
                    for (int i = 0; i < 4; i++){
                        printf("Nota %d: %.2lf\n", (i+1), aluno[indice].notas[i]);
                    }
                }else{
                    printf("\nAluno não encontrado!\n");
                }
                
                LimparBufferEntrada();
                printf("\nPressione Enter para continuar...");
                getchar();
                LimparTela();
                break;
            }
        }
        
    } while (resp != 0);
    
    free(aluno);
    return 0;
}
