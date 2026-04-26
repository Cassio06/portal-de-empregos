#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define ARQUIVO "usuarios.dat"

// Tipos de usuário
#define EMPRESA 1
#define ALUNO 2

struct Usuario {
    char email[50];
    char senha[20];
    int tipo; // 1 = empresa, 2 = aluno

    char area[50];

    // Só para aluno
    int ehExAluno;   // 0 = aluno, 1 = ex-aluno
    int periodo;     // só usado se NÃO for ex-aluno
};

struct Usuario usuarios[MAX];
int totalUsuarios = 0;


// ================= MAIN =================

int main() {
    carregarUsuarios();
    menuPrincipal();
    salvarUsuarios();
    return 0;
}

// ================= ARQUIVO =================

void carregarUsuarios() {
    FILE *f = fopen(ARQUIVO, "rb");

    if(f == NULL) return;

    totalUsuarios = fread(usuarios, sizeof(struct Usuario), MAX, f);
    fclose(f);
}

void salvarUsuarios() {
    FILE *f = fopen(ARQUIVO, "wb");

    if(f == NULL) {
        printf("Erro ao salvar arquivo!\n");
        return;
    }

    fwrite(usuarios, sizeof(struct Usuario), totalUsuarios, f);
    fclose(f);
}

// ================= VALIDAÇÃO =================

int validarEmail(char email[]) {
    return strchr(email, '@') != NULL;
}

// ================= CADASTRO =================

void cadastro(int tipo) {
    if(totalUsuarios >= MAX) {
        printf("Limite de usuarios atingido.\n");
        return;
    }

    char email[50];

    printf("\n===== CADASTRO =====\n");

    // EMAIL
    do {
        printf("Email: ");
        scanf("%s", email);

        if(!validarEmail(email)) {
            printf("Email invalido! Precisa conter '@'\n");
        }

    } while(!validarEmail(email));

    strcpy(usuarios[totalUsuarios].email, email);

    // SENHA
    printf("Senha: ");
    scanf("%s", usuarios[totalUsuarios].senha);

    usuarios[totalUsuarios].tipo = tipo;

    // ÁREA (para todos)
    printf("Area de atuacao (ex: TI, ADM, etc): ");
    scanf(" %[^\n]", usuarios[totalUsuarios].area);

    // SE FOR ALUNO
    if(tipo == ALUNO) {
        int opcao;

        printf("Voce e:\n");
        printf("1 - Aluno\n");
        printf("2 - Ex-aluno\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if(opcao == 2) {
            usuarios[totalUsuarios].ehExAluno = 1;
            usuarios[totalUsuarios].periodo = -1; // não usa
        } else {
            usuarios[totalUsuarios].ehExAluno = 0;

            printf("Informe o periodo atual: ");
            scanf("%d", &usuarios[totalUsuarios].periodo);
        }
    } else {
        // empresa não usa esses campos
        usuarios[totalUsuarios].ehExAluno = -1;
        usuarios[totalUsuarios].periodo = -1;
    }

    totalUsuarios++;

    salvarUsuarios();

    printf("Cadastro realizado com sucesso!\n");
}
// ================= LOGIN =================

int login(int tipo) {
    char email[50], senha[20];

    printf("\n===== LOGIN =====\n");

    printf("Email: ");
    scanf("%s", email);

    printf("Senha: ");
    scanf("%s", senha);

    for(int i = 0; i < totalUsuarios; i++) {
        if(strcmp(email, usuarios[i].email) == 0 &&
           strcmp(senha, usuarios[i].senha) == 0 &&
           usuarios[i].tipo == tipo) {
            return 1;
        }
    }

    return 0;
}

// ================= MENUS =================

void menuLoginCadastro(int tipo) {
    int opcao;

    do {
        printf("\n===== LOGIN / CADASTRO =====\n");
        printf("1 - Login\n");
        printf("2 - Cadastro\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                if(login(tipo)) {
                    printf("Login realizado com sucesso!\n");

                    if(tipo == EMPRESA)
                        printf("Bem-vindo, Empresa!\n");
                    else
                        printf("Bem-vindo, Aluno!\n");

                } else {
                    printf("Dados incorretos!\n");
                }
                break;

            case 2:
                cadastro(tipo);
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuPrincipal() {
    int opcao;

    do {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1 - Empresa\n");
        printf("2 - Aluno/Ex-aluno\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                menuLoginCadastro(EMPRESA);
                break;

            case 2:
                menuLoginCadastro(ALUNO);
                break;

            case 0:
                salvarUsuarios();
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}