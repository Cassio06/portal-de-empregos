#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define ARQUIVO "usuarios.dat"

//Gerenciamento de vagas e sistema de candidatura 
#define ARQUIVO_VAGAS "vagas.dat"
#define ARQUIVO_CANDIDATURAS "candidaturas.dat"
#define MAX_CANDIDATURAS 300

// Tipos de usuário
#define EMPRESA 1
#define ALUNO 2
#define ADMIN 3

// Estruturas
struct Vaga {
    int id;
    char titulo[80];
    char empresaEmail[50];
    char area[50];
    char descricao[200];
    char requisitos[200];
    int ativa;
    int vagasTotais;
    int vagasPreenchidas;
};

struct Candidatura {
    int id;
    int idVaga;
    int ativa;
    char alunoEmail[50];
    char status[30];
};

struct Vaga vagas[MAX];
int totalVagas = 0;

struct Candidatura candidaturas[MAX_CANDIDATURAS];
int totalCandidaturas = 0;

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
int usuarioLogado = -1;

// ===== PROTÓTIPOS - USUÁRIOS =====
void carregarUsuarios();
void salvarUsuarios();
void criarAdminPadrao();
void cadastro(int tipo);
int login(int tipo);
int validarEmail(char email[]);
int emailExiste(char email[]);


// ===== PROTÓTIPOS - MENUS =====
void menuPrincipal();
void menuLoginCadastro(int tipo);
void menuAdmin();
void menuEmpresa();
void menuAluno();


// ===== PROTÓTIPOS - VAGAS =====
void carregarVagas();
void salvarVagas();
void criarVaga();
void listarVagas();
void buscarVagas();
int vagaExiste(int idVaga);


// ===== PROTÓTIPOS - CANDIDATURAS =====
void carregarCandidaturas();
void salvarCandidaturas();
void candidatarAluno();
void verCandidatos();
void selecionarCandidato();
void verVagasSelecionado();
int jaCandidatado(int idVaga, char alunoEmail[]);

// ===== MAIN =====
int main() {
    carregarUsuarios();
    carregarVagas();
    carregarCandidaturas();

    criarAdminPadrao();
    menuPrincipal();

    salvarUsuarios();
    salvarVagas();
    salvarCandidaturas();
    
    return 0;
}

// ===== ARQUIVO DE USUARIO =====
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
// ===== ARQUIVO DE VAGAS =====
void carregarVagas() {
    FILE *f = fopen(ARQUIVO_VAGAS, "rb");
    if(f == NULL) return;

    totalVagas = fread(vagas, sizeof(struct Vaga), MAX, f);
    fclose(f);
}

void salvarVagas() {
    FILE *f = fopen(ARQUIVO_VAGAS, "wb");
    if(f == NULL) {
        printf("Erro ao salvar vagas!\n");
        return;
    }

    fwrite(vagas, sizeof(struct Vaga), totalVagas, f);
    fclose(f);
}
// ===== ARQUIVO DE CANDIDATURA =====
void carregarCandidaturas() {
    FILE *f = fopen(ARQUIVO_CANDIDATURAS, "rb");
    if(f == NULL) return;

    totalCandidaturas = fread(candidaturas, sizeof(struct Candidatura), MAX_CANDIDATURAS, f);
    fclose(f);
}

void salvarCandidaturas() {
    FILE *f = fopen(ARQUIVO_CANDIDATURAS, "wb");
    if(f == NULL) {
        printf("Erro ao salvar candidaturas!\n");
        return;
    }

    fwrite(candidaturas, sizeof(struct Candidatura), totalCandidaturas, f);
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
            usuarioLogado = i;
            return 1;
        }
    }
    usuarioLogado = -1; 
    return 0;
}

// ===== MENU EMPRESA =====
void menuEmpresa() {
    int opcao;

    do {
        printf("\n===== MENU EMPRESA =====\n");
        printf("1 - Criar vaga\n");
        printf("2 - Listar vagas\n");
        printf("3 - Ver candidatos\n");
        printf("4 - Selecionar candidato\n");
        printf("0 - Logout\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                criarVaga();
                break;

            case 2:
                listarVagas();
                break;

            case 3:
                verCandidatos();
                break;

            case 4:
                selecionarCandidato();
                break;

            case 0:
                usuarioLogado = -1;
                printf("Saindo da conta da empresa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

// ===== MENU ALUNO =====
void menuAluno() {
    int opcao;

    do {
        printf("\n===== MENU ALUNO =====\n");
        printf("1 - Ver vagas\n");
        printf("2 - Buscar vagas\n");
        printf("3 - Se candidatar\n");
        printf("4 - Minhas selecoes\n");
        printf("0 - Logout\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                listarVagas();
                break;

            case 2:
                buscarVagas();
                break;

            case 3:
                candidatarAluno();
                break;

            case 4:
                verVagasSelecionado();
                break;

            case 0:
                usuarioLogado = -1;
                printf("Saindo da conta do aluno...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
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
                    } 
                    else if(tipo == EMPRESA) {
                        printf("Bem-vindo, Empresa!\n");
                        menuEmpresa();
                    } 
                    else {
                        printf("Bem-vindo, Aluno!\n");
                        menuAluno();
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
// ===== FUNÇÃO DE CRIAR VAGAS =====
void criarVaga(){
    if(totalVagas >= MAX) {
        printf("Limite de vagas atingido.\n");
        return;
    }

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem criar vagas.\n");
        return;
    }

    vagas[totalVagas].id = totalVagas + 1;

    strcpy(vagas[totalVagas].empresaEmail, usuarios[usuarioLogado].email);

    printf("Titulo da vaga: ");
    scanf(" %[^\n]", vagas[totalVagas].titulo);

    printf("Area da vaga: ");
    scanf(" %[^\n]", vagas[totalVagas].area);

    printf("Descricao da vaga: ");
    scanf(" %[^\n]", vagas[totalVagas].descricao);

    printf("Requisitos da vaga: ");
    scanf(" %[^\n]", vagas[totalVagas].requisitos);

    printf("Quantas pessoas voce esta buscando para esta vaga: ");
    scanf("%d", &vagas[totalVagas].vagasTotais);
    if(vagas[totalVagas].vagasTotais <= 0) vagas[totalVagas].vagasTotais = 1;
    vagas[totalVagas].vagasPreenchidas = 0;

    vagas[totalVagas].ativa = 1;

    totalVagas++;
    salvarVagas();

    printf("Vaga cadastrada com sucesso!\n");

}
// ===== FUNÇÃO DE LISTAR VAGAS =====
void listarVagas() {
    if(totalVagas == 0) {
        printf("Nenhuma vaga cadastrada.\n");
        return;
    }

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].ativa == 1) {
            printf("\nID: %d\n", vagas[i].id);
            printf("Titulo: %s\n", vagas[i].titulo);
            printf("Empresa: %s\n", vagas[i].empresaEmail);
            printf("Area: %s\n", vagas[i].area);
            printf("Descricao: %s\n", vagas[i].descricao);
            printf("Requisitos: %s\n", vagas[i].requisitos);
            printf("Vagas: %d/%d preenchidas\n", vagas[i].vagasPreenchidas, vagas[i].vagasTotais);
        }
    }
}
// ===== FUNÇÃO DE BUSCAR VAGAS =====
void buscarVagas() {
    char areaBusca[50];
    int encontrou = 0;

    printf("Digite a area desejada: ");
    scanf(" %[^\n]", areaBusca);

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].ativa == 1 && strcmp(vagas[i].area, areaBusca) == 0) {
            printf("\nID: %d\n", vagas[i].id);
            printf("Titulo: %s\n", vagas[i].titulo);
            printf("Empresa: %s\n", vagas[i].empresaEmail);
            printf("Descricao: %s\n", vagas[i].descricao);
            printf("Requisitos: %s\n", vagas[i].requisitos);
            encontrou = 1;
        }
    }

    if(!encontrou) {
        printf("Nenhuma vaga encontrada nessa area.\n");
    }
}
int vagaExiste(int idVaga) {
    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga && vagas[i].ativa == 1) {
            return 1;
        }
    }
    return 0;
}

int jaCandidatado(int idVaga, char alunoEmail[]) {
    for(int i = 0; i < totalCandidaturas; i++) {
        if(candidaturas[i].idVaga == idVaga &&
           strcmp(candidaturas[i].alunoEmail, alunoEmail) == 0) {
            return 1;
        }
    }
    return 0;
}

// ===== FUNÇÃO DE CADASTRO DE ALUNO =====
void candidatarAluno() {
    int idVaga;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != ALUNO) {
        printf("Apenas alunos/ex-alunos podem se candidatar.\n");
        return;
    }

    if(totalCandidaturas >= MAX_CANDIDATURAS) {
        printf("Limite de candidaturas atingido.\n");
        return;
    }

    listarVagas();

    printf("\nDigite o ID da vaga desejada: ");
    scanf("%d", &idVaga);

    if(!vagaExiste(idVaga)) {
        printf("Vaga inexistente ou inativa.\n");
        return;
    }

    if(jaCandidatado(idVaga, usuarios[usuarioLogado].email)) {
        printf("Voce ja se candidatou a essa vaga.\n");
        return;
    }

    candidaturas[totalCandidaturas].id = totalCandidaturas + 1;
    candidaturas[totalCandidaturas].idVaga = idVaga;
    strcpy(candidaturas[totalCandidaturas].alunoEmail, usuarios[usuarioLogado].email);
    strcpy(candidaturas[totalCandidaturas].status, "Enviada");

    totalCandidaturas++;
    salvarCandidaturas();

    printf("Candidatura realizada com sucesso!\n");
}
// ===== FUNÇÃO DE VER CANDIDATOS =====
void verCandidatos() {
    int idVaga;
    int encontrouVaga = 0;
    int encontrouCandidato = 0;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem ver candidatos.\n");
        return;
    }

    printf("\nSuas vagas:\n");

    for(int i = 0; i < totalVagas; i++) {
        if(strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            printf("ID: %d | Titulo: %s\n", vagas[i].id, vagas[i].titulo);
            encontrouVaga = 1;
        }
    }

    if(!encontrouVaga) {
        printf("Voce ainda nao cadastrou vagas.\n");
        return;
    }

    printf("\nDigite o ID da vaga para ver candidatos: ");
    scanf("%d", &idVaga);

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga &&
           strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0) {
            encontrouVaga = 1;
            break;
        }
    }

    printf("\nCandidatos:\n");

    for(int i = 0; i < totalCandidaturas; i++) {
        if(candidaturas[i].idVaga == idVaga) {
            printf("Aluno: %s | Status: %s\n",
                   candidaturas[i].alunoEmail,
                   candidaturas[i].status);
            encontrouCandidato = 1;
        }
    }

    if(!encontrouCandidato) {
        printf("Nenhum candidato para esta vaga.\n");
    }
}

// ===== FUNÇÃO DE SELECIONAR CANDIDATO(S) =====
void selecionarCandidato() {
    int idVaga;
    int encontrouVaga = 0;
    int encontrouCandidato = 0;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem selecionar candidatos.\n");
        return;
    }

    printf("\nSuas vagas:\n");

    for(int i = 0; i < totalVagas; i++) {
        if(strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            printf("ID: %d | Titulo: %s | Vagas: %d/%d\n",
                   vagas[i].id, vagas[i].titulo,
                   vagas[i].vagasPreenchidas, vagas[i].vagasTotais);
            encontrouVaga = 1;
        }
    }

    if(!encontrouVaga) {
        printf("Voce ainda nao cadastrou vagas.\n");
        return;
    }

    printf("\nDigite o ID da vaga para selecionar candidatos: ");
    scanf("%d", &idVaga);

    int indiceVaga = -1;
    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga &&
           strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            indiceVaga = i;
            break;
        }
    }

    if(indiceVaga == -1) {
        printf("Vaga invalida ou nao pertence a sua empresa.\n");
        return;
    }

    int vagasRestantes = vagas[indiceVaga].vagasTotais - vagas[indiceVaga].vagasPreenchidas;
    printf("\nVaga '%s': %d/%d preenchidas. Ainda faltam %d pessoa(s).\n",
           vagas[indiceVaga].titulo,
           vagas[indiceVaga].vagasPreenchidas,
           vagas[indiceVaga].vagasTotais,
           vagasRestantes);

    printf("\nCandidatos disponiveis:\n");
    printf("--------------------------------------------\n");

    for(int i = 0; i < totalCandidaturas; i++) {
        if(candidaturas[i].idVaga == idVaga &&
           strcmp(candidaturas[i].status, "Enviada") == 0) {
            printf("ID Candidatura: %d | Aluno: %s\n",
                   candidaturas[i].id,
                   candidaturas[i].alunoEmail);
            encontrouCandidato = 1;
        }
    }

    if(!encontrouCandidato) {
        printf("Nenhum candidato pendente para esta vaga.\n");
        return;
    }

    printf("--------------------------------------------\n");

    while(vagas[indiceVaga].vagasPreenchidas < vagas[indiceVaga].vagasTotais) {
        int restam = vagas[indiceVaga].vagasTotais - vagas[indiceVaga].vagasPreenchidas;
        printf("\nAinda faltam %d vaga(s). Digite o ID da candidatura (0 para parar): ", restam);

        int idCandidatura;
        scanf("%d", &idCandidatura);

        if(idCandidatura == 0) break;

        int achou = 0;
        for(int i = 0; i < totalCandidaturas; i++) {
            if(candidaturas[i].id == idCandidatura &&
               candidaturas[i].idVaga == idVaga &&
               strcmp(candidaturas[i].status, "Enviada") == 0) {
                strcpy(candidaturas[i].status, "Selecionado");
                vagas[indiceVaga].vagasPreenchidas++;
                printf("Aluno %s selecionado! (%d/%d)\n",
                       candidaturas[i].alunoEmail,
                       vagas[indiceVaga].vagasPreenchidas,
                       vagas[indiceVaga].vagasTotais);
                achou = 1;
                break;
            }
        }

        if(!achou) {
            printf("ID invalido ou candidato ja selecionado.\n");
        }

        if(vagas[indiceVaga].vagasPreenchidas == vagas[indiceVaga].vagasTotais) {
            printf("\nTodas as vagas foram preenchidas!\n");
            break;
        }
    }

    // Fechar vaga se atingiu o número total
    if(vagas[indiceVaga].vagasPreenchidas >= vagas[indiceVaga].vagasTotais) {
        vagas[indiceVaga].ativa = 0;

        // Marcar restantes como Nao Selecionado
        for(int i = 0; i < totalCandidaturas; i++) {
            if(candidaturas[i].idVaga == idVaga &&
               strcmp(candidaturas[i].status, "Enviada") == 0) {
                strcpy(candidaturas[i].status, "Nao Selecionado");
            }
        }

        printf("Vaga encerrada automaticamente pois todas as vagas foram preenchidas.\n");
    }

    salvarVagas();
    salvarCandidaturas();
    printf("\nSelecao salva com sucesso!\n");
}

// ===== FUNÇÃO MINHAS SELEÇÕES (ALUNO) =====
void verVagasSelecionado() {
    int encontrou = 0;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != ALUNO) {
        printf("Apenas alunos podem ver suas selecoes.\n");
        return;
    }

    printf("\n===== VAGAS QUE FUI SELECIONADO =====\n");

    for(int i = 0; i < totalCandidaturas; i++) {
        if(strcmp(candidaturas[i].alunoEmail, usuarios[usuarioLogado].email) == 0 &&
           strcmp(candidaturas[i].status, "Selecionado") == 0) {

            // Buscar dados da vaga correspondente
            for(int j = 0; j < totalVagas; j++) {
                if(vagas[j].id == candidaturas[i].idVaga) {
                    printf("\n----------------------------------------\n");
                    printf("Vaga:    %s\n", vagas[j].titulo);
                    printf("Empresa: %s\n", vagas[j].empresaEmail);
                    printf("Area:    %s\n", vagas[j].area);
                    printf("Status:  Selecionado\n");
                    printf("----------------------------------------\n");
                    encontrou = 1;
                    break;
                }
            }
        }
    }

    if(!encontrou) {
        printf("Voce ainda nao foi selecionado em nenhuma vaga.\n");
    }
}