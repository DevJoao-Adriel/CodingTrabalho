#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void LimparTela(void) {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

//função para limpar o buffer de entrada pois queria que em determinado momento do programa o usuario clicasse enter para prosseguir mas o buffer de entrada quase sempre ta cheio, então antes de eu pedir pro usuario clicar enter eu limpo o buffer de entrada
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

//funcao para ver se uma matricula e igual a uma que ja exista
bool VerificarMatricula(int alunosCadstrados, Aluno *aluno, int novaMatricula){
    for (int i = 0; i < alunosCadstrados; i++){
        if (novaMatricula == aluno[i].matricula){
            return false;
        }
    }
    return true;
}

//funcao para verificar se a idade e valida
bool VerificarIdade(int idade){
    if (idade <= 0){
        return false;
    }
    return true;
}

//verificar se a nota e valida entre 0 e 10
bool VerificarNota(double nota){
    if ((nota >= 0) && (nota <= 10)){
        return true;
    }
    return false;
}

//funcao para cadastrar um aluno, ela pede todas as informações da struct e cadastra ele por um ponteiro de vetor dinamico
void CadastrarAluno(int *alunosCadastrados, Aluno *aluno, int numeroOrdem){
    LimparTela();
    printf("--- Aluno %d ---\n", numeroOrdem);

    int i = *alunosCadastrados;
    int novaMatricula, novaIdade;
    double novaNota;

    getchar();
    printf("Digite o nome do(a) aluno(a):\n");
    fgets(aluno[i].nome, 50, stdin);
    aluno[i].nome[strcspn(aluno[i].nome, "\n")] = '\0';

    bool matriculaValida = false, idadeValida = false, notaValida = false;

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

    do{
        printf("Digite a idade do(a) aluno(a):\n");
        scanf("%d", &novaIdade);
        
        if (VerificarIdade(novaIdade)){
            aluno[i].idade = novaIdade;
            idadeValida = true;
        }else{
            printf("Digite uma idade valia!\n");
        }
        
    } while (!idadeValida);
    
    
    for (int j = 0; j < 4; j++){
        do{
            printf("Digite a %d nota do(a) aluno(a):\n", (j+1));
            scanf("%lf", &novaNota);
            if (VerificarNota(novaNota)){
                aluno[i].notas[j] = novaNota;
                notaValida = true;
            }else{
                printf("Digite uma nota valida, entre 0 e 10\n");
            }
        } while (!notaValida);
    }
    
    

    (*alunosCadastrados)++;
}

//funcao para listar os alunos ja cadastrados
void ListarAlunos(Aluno *aluno, int alunosCadastrados){
    for (int i = 0; i < alunosCadastrados; i++){
        printf("\n--- Aluno %d ---\n Matricula: %d\n Nome: %s\n Idade: %d\n", (i + 1), aluno[i].matricula, aluno[i].nome, aluno[i].idade);
        printf("Notas: \n");
        for (int j = 0; j < 4; j++){
            printf("Nota %d: %.2lf\n", (j+1), (aluno[i].notas[j])); 
        }
        
    }
    
}

//funcao para buscar e pegar as informações de um aluno pela matricula
int BuscarAluno(Aluno *aluno, int alunosCadastrados, int matricula){
    for (int i = 0; i < alunosCadastrados; i++){
        if (matricula == aluno[i].matricula){
            return i;
        }
    }
    return -1;   
}

//funcao para editar as especificacoes do aluno (menos a matricula pois ela e imutavel)
void EditarAluno(Aluno *aluno, int indice){
    int opcao, opcNota;
    int novaIdade;
    double novaNota;
    bool validaNota = false, validaIdade = false;
    
    do{
        printf("[1]Nome\n [2]Idade\n [3]Notas\n [0] Sair\n O que voce deseja editar do aluno?");
        scanf("%d", &opcao);
        switch (opcao){
            //muda o nome
        case 1:{
            printf("Nome atual: %s\n Novo nome: ", aluno[indice].nome);
            
            getchar();
            fgets(aluno[indice].nome, 50, stdin);
            aluno[indice].nome[strcspn(aluno[indice].nome, "\n")] = '\0';
            
            printf("\nOk, novo nome: %s", aluno[indice].nome);
            
            printf("\nPressione Enter para continuar...");
            getchar();
            LimparTela();

            break;
        }
            //muda a idade
        case 2:{
            do{
                printf("Idade atual: %d\n Nova idade: ", aluno[indice].idade);

                scanf("%d", &novaIdade);

                if (VerificarIdade(novaIdade)){
                    aluno[indice].idade = novaIdade;
                    validaIdade = true;
                }else{
                    printf("Digita uma idade valida!");
                }

                
            } while (!validaIdade);
            
            printf("\n Ok, nova idade: %d", aluno[indice].idade);
            
            LimparBufferEntrada();
            printf("\nPressione Enter para continuar...");
            getchar();
            LimparTela();
            
            break;
        }
            //muda as notas
        case 3:{
            do{  
                printf("\n[1] Nota 1\n [2] Nota 2\n [3] Nota 3\n [4] Nota 4\n [5] Todas\n [0] Sair\n Qual deseja alterar? ");
                scanf("%d", &opcNota);
                
                switch (opcNota){
                    //muda a primeira nota
                    case 1:{
                        validaNota = false;
                        do{
                            printf("\nPrimeira nota atual: %.2lf\n Nova nota: ", aluno[indice].notas[0]);
                            scanf("%lf", &novaNota);
                            if (VerificarNota(novaNota)){
                                aluno[indice].notas[0] = novaNota;
                                validaNota = true;
                            }else{
                                printf("Coloque uma nota valida, entre 0 e 10\n");
                            }
                            
                        } while (!validaNota);
                        
                        printf("\n Ok, Nova primeira nota: %.2lf", aluno[indice].notas[0]);
                        
                        LimparBufferEntrada();
                        printf("\nPressione Enter para continuar...");
                        getchar();
                        LimparTela();
                        break;
                    }
                    //muda a segunda nota
                    case 2:{
                        validaNota = false;
                        do{
                            printf("\nSegunda nota atual: %.2lf\n Nova nota: ", aluno[indice].notas[1]);
                            scanf("%lf", &novaNota);
                            if (VerificarNota(novaNota)){
                                aluno[indice].notas[1] = novaNota;
                                validaNota = true;
                            }else{
                                printf("Coloque uma nota valida, entre 0 e 10\n");
                            }
                            
                        } while (!validaNota);
                        
                        printf("\n Ok, Nova segunda nota: %.2lf", aluno[indice].notas[1]);
                        
                        LimparBufferEntrada();
                        printf("\nPressione Enter para continuar...");
                        getchar();
                        LimparTela();
                        break;
                    }
                    //muda a terceira nota
                    case 3:{
                        validaNota = false;
                        do{
                            printf("\nTerceira nota atual: %.2lf\n Nova nota: ", aluno[indice].notas[2]);
                            scanf("%lf", &novaNota);
                            if (VerificarNota(novaNota)){
                                aluno[indice].notas[2] = novaNota;
                                validaNota = true;
                            }else{
                                printf("Coloque uma nota valida, entre 0 e 10\n");
                            }
                            
                        } while (!validaNota);
                        
                        printf("\n Ok, Nova terceira nota: %.2lf", aluno[indice].notas[2]);
                        
                        LimparBufferEntrada();
                        printf("\nPressione Enter para continuar...");
                        getchar();
                        LimparTela();
                        break;
                    }
                    //muda a quarta nota
                    case 4:{
                        validaNota = false;
                        do{
                            printf("\nQuarta nota atual: %.2lf\n Nova nota: ", aluno[indice].notas[3]);
                            scanf("%lf", &novaNota);
                            if (VerificarNota(novaNota)){
                                aluno[indice].notas[3] = novaNota;
                                validaNota = true;
                            }else{
                                printf("Coloque uma nota valida, entre 0 e 10\n");
                            }
                            
                        } while (!validaNota);
                        
                        printf("\n Ok, Nova quarta nota: %.2lf", aluno[indice].notas[3]);
                        
                        LimparBufferEntrada();
                        printf("\nPressione Enter para continuar...");
                        getchar();
                        LimparTela();
                        break;
                    }
                    //muda todas as notas
                    case 5:{
                        validaNota = false;
                        for (int i = 0; i < 4; i++){
                            do{
                                printf("\nNota %d atual: %.2lf\n Nova nota: ", (i+1), aluno[indice].notas[i]);
                                scanf("%lf", &novaNota);
                                if (VerificarNota(novaNota)){
                                    aluno[indice].notas[i] = novaNota;
                                    validaNota = true;
                                }else{
                                    printf("Coloque uma nota valida, entre 0 e 10\n");
                                }
                            } while (!validaNota);
                        }
                        
                        
                        
                        
                        printf("\nOk, Novas notas: \n");
                        
                        for (int i = 0; i < 4; i++){    
                            printf("Nota %d: %.2lf\n", (i+1), aluno[indice].notas[i]);
                        }
                        
                        LimparBufferEntrada();
                        printf("\nPressione Enter para continuar...");
                        getchar();
                        LimparTela();
                        
                        break;            
                    }
                    
                    case 0:{
                        LimparTela();
                        printf("saindo...\n");
                        
                        break;
                    }
                    default:{
                        printf("Digite uma opcao valida!");
                        
                        LimparBufferEntrada();
                        printf("\nPressione Enter para continuar...");
                        getchar();
                        LimparTela();

                        break;
                    }
                }

            } while (opcNota != 0);
            break;
            //fim do case 3 do principal
        }

        case 0:{
            LimparTela();
            printf("saindo...\n");
                        
            break;
        }
        default:
            LimparTela();
            printf("Digite uma opcao valida!");
            
            LimparBufferEntrada();
            printf("\nPressione Enter para continuar...");
            getchar();
            LimparTela();
            break;
        }
        
    } while (opcao != 0);
    
}

//funcao para remover os alunos
bool RemoverAluno(Aluno *aluno, int *alunosCadastrados, int matricula){
    int indice = BuscarAluno(aluno, *alunosCadastrados, matricula);

    if (indice == -1){
        return false; 
    }

    // Desloca todos os elementos posteriores uma posição para trás
    for (int i = indice; i < (*alunosCadastrados) - 1; i++){
        aluno[i] = aluno[i + 1];
    }

    //eu fiz isso pois quando for cadastrar o aluno o indice recebe os alunos cadastrados, entao quando eu diminuo meus alunos cadastrados o proximo a ser cadastrado vai ser subrescrito, na vdd, em cima do ulitmo da lista, que o ultimo da lista atualmente é o aluno que eu quis deletar, efetivamente ele ainda existe no vetor, mas ele nao vai ser mostrado nem nada do tipo pois nunca vai chegar nele.
    (*alunosCadastrados)--;
    return true;
}

//painel meramente ilustrativo para mostrar as funcionalidades
void painel(){
    printf("\n[1] Cadastrar aluno\n [2] Listar Alunos\n [3]Buscar aluno pela matricula\n [4]Editar um aluno\n [5]Remover aluno\n [0]Sair\n Digite qual acao voce deseja realizar: ");
}

int main(){
    //iniciei os vetores dinamicos
    int resp = 0, alunosCadastrados = 0, capacidade = 2, indice, matriculaBusca;

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
                    printf("\nAluno nao encontrado!\n");
                }
                
                LimparBufferEntrada();
                printf("\nPressione Enter para continuar...");
                getchar();
                LimparTela();
                
                break;
            }

            case 4:{
                LimparTela();
                if (alunosCadastrados <= 0){
                    printf("Não ha alunos cadastrados ainda!");
                    break;
                }
                    
                printf("Digite a matricula do aluno que deseja editar: ");
                scanf("%d", &matriculaBusca);
                
                indice = BuscarAluno(aluno, alunosCadastrados, matriculaBusca);

                if (indice != -1){
                    EditarAluno(aluno, indice);
                }else{
                    printf("Aluno nao encontrado!");
                }
                
                printf("\nPressione Enter para continuar...");
                getchar();
                LimparTela();

                break;
            }

            case 5:{
                LimparTela();

                if (alunosCadastrados <= 0){
                    printf("Nao ha alunos cadastrados para remover!\n");
                    printf("\nPressione Enter para continuar...");
                    getchar();
                    break;
                }

                int matriculaRemover;
                printf("Digite a matricula do aluno que deseja remover: ");
                scanf("%d", &matriculaRemover);

                if (RemoverAluno(aluno, &alunosCadastrados, matriculaRemover)){
                    printf("\nAluno removido com sucesso!\n");
                } else {
                    printf("\nAluno nao encontrado!\n");
                }
                
                LimparBufferEntrada();
                printf("\nPressione Enter para continuar...");
                getchar();
                break;            
            }

            case 0:{
                LimparTela();
                printf("saindo...\n");

                break;
            }

            default:{
                        printf("Digite uma opcao valida!");
                        
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
