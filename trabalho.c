#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define ARQUIVO "usuarios.dat"

// Tipos de usuário
#define EMPRESA 1
#define ALUNO 2
#define ADMIN 3

struct Usuario {
    char email[50];
    char senha[20];
    int tipo;

    char area[50];

    int ehExAluno;
    int periodo;
};

struct Usuario usuarios[MAX];
int totalUsuarios = 0;

// ===== PROTÓTIPOS =====
void carregarUsuarios();
void salvarUsuarios();
void menuPrincipal();
void menuLoginCadastro(int tipo);
void cadastro(int tipo);
int login(int tipo);
void criarAdminPadrao();
void menuAdmin();
int validarEmail(char email[]);
int emailExiste(char email[]);

// ===== MAIN =====
int main() {
    carregarUsuarios();
    criarAdminPadrao();
    menuPrincipal();
    salvarUsuarios();
    return 0;
}

// ===== ARQUIVO =====
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

// ===== VALIDAÇÃO =====
int validarEmail(char email[]) {
    return strchr(email, '@') != NULL;
}

int emailExiste(char email[]) {
    for(int i = 0; i < totalUsuarios; i++) {
        if(strcmp(usuarios[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}

// ===== ADMIN PADRÃO =====
void criarAdminPadrao() {
    int existe = 0;

    for(int i = 0; i < totalUsuarios; i++) {
        if(usuarios[i].tipo == ADMIN) {
            existe = 1;
            break;
        }
    }

    if(!existe && totalUsuarios < MAX) {
        strcpy(usuarios[totalUsuarios].email, "admin");
        strcpy(usuarios[totalUsuarios].senha, "admin123");
        usuarios[totalUsuarios].tipo = ADMIN;

        strcpy(usuarios[totalUsuarios].area, "Administrador");
        usuarios[totalUsuarios].ehExAluno = -1;
        usuarios[totalUsuarios].periodo = -1;

        totalUsuarios++;
        salvarUsuarios();

        printf("Admin padrao criado!\n");
    }
}

// ===== CADASTRO =====
void cadastro(int tipo) {
    if(totalUsuarios >= MAX) {
        printf("Limite de usuarios atingido.\n");
        return;
    }

    char email[50];

    printf("\n===== CADASTRO =====\n");

    do {
        printf("Email: ");
        scanf("%s", email);

        if(!validarEmail(email)) {
            printf("Email invalido! Precisa conter '@'\n");
        } else if(emailExiste(email)) {
            printf("Email ja cadastrado!\n");
        }

    } while(!validarEmail(email) || emailExiste(email));

    strcpy(usuarios[totalUsuarios].email, email);

    printf("Senha: ");
    scanf("%s", usuarios[totalUsuarios].senha);

    usuarios[totalUsuarios].tipo = tipo;

    printf("Area de atuacao: ");
    scanf(" %[^\n]", usuarios[totalUsuarios].area);

    if(tipo == ALUNO) {
        int opcao;

        printf("1 - Aluno\n2 - Ex-aluno\nEscolha: ");
        scanf("%d", &opcao);

        if(opcao == 2) {
            usuarios[totalUsuarios].ehExAluno = 1;
            usuarios[totalUsuarios].periodo = -1;
        } else {
            usuarios[totalUsuarios].ehExAluno = 0;
            printf("Periodo atual: ");
            scanf("%d", &usuarios[totalUsuarios].periodo);
        }
    } else {
        usuarios[totalUsuarios].ehExAluno = -1;
        usuarios[totalUsuarios].periodo = -1;
    }

    totalUsuarios++;
    salvarUsuarios();

    printf("Cadastro realizado com sucesso!\n");
}

// ===== LOGIN =====
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

// ===== MENU ADMIN =====
void menuAdmin() {
    int opcao;

    do {
        printf("\n===== MENU ADMIN =====\n");
        printf("1 - Criar novo admin\n");
        printf("2 - Listar usuarios\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastro(ADMIN);
                break;

            case 2:
                for(int i = 0; i < totalUsuarios; i++) {
                    printf("\nEmail: %s\n", usuarios[i].email);

                    printf("Tipo: ");
                    if(usuarios[i].tipo == ADMIN) printf("ADMIN\n");
                    else if(usuarios[i].tipo == EMPRESA) printf("EMPRESA\n");
                    else printf("ALUNO\n");
                }
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

// ===== LOGIN / CADASTRO =====
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

                    if(tipo == ADMIN) {
                        menuAdmin();
                    } else if(tipo == EMPRESA) {
                        printf("Bem-vindo, Empresa!\n");
                    } else {
                        printf("Bem-vindo, Aluno!\n");
                    }

                } else {
                    printf("Dados incorretos!\n");
                }
                break;

            case 2:
                if(tipo == ADMIN) {
                    printf("Apenas admins logados podem criar outros admins!\n");
                } else {
                    cadastro(tipo);
                }
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

// ===== MENU PRINCIPAL =====
void menuPrincipal() {
    int opcao;

    do {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1 - Empresa\n");
        printf("2 - Aluno/Ex-aluno\n");
        printf("3 - Admin\n");
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

            case 3:
                menuLoginCadastro(ADMIN);
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