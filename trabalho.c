#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX 100
#define ARQUIVO "usuarios.txt"

#define ARQUIVO_VAGAS "vagas.txt"
#define ARQUIVO_CANDIDATURAS "candidaturas.txt"
#define MAX_CANDIDATURAS 300

#define EMPRESA 1
#define ALUNO 2
#define ADMIN 3

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
    char alunoEmail[50];
    char status[30];
};

struct Vaga vagas[MAX];
int totalVagas = 0;
int proximoIdVaga = 1;

struct Candidatura candidaturas[MAX_CANDIDATURAS];
int totalCandidaturas = 0;
int proximoIdCandidatura = 1;

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

// ===== PROTÓTIPOS =====
void carregarUsuarios();
void salvarUsuarios();
void criarAdminPadrao();
void cadastro(int tipo);
int login(int tipo);
int validarEmail(char email[]);
int emailExiste(char email[]);

void menuPrincipal();
void menuLoginCadastro(int tipo);
void menuAdmin();
void menuEmpresa();
void menuAluno();

void listarUsuariosAdmin();
void removerUsuarioAdmin();
void listarEmpresasAdmin();
void removerVagaAdmin();
void relatorioGeral();

void carregarVagas();
void salvarVagas();
void criarVaga();
void editarVagaEmpresa();
void excluirVagaEmpresa();
void listarVagas();
void buscarVagas();
int vagaExiste(int idVaga);
int indiceVagaPorId(int idVaga);

void carregarCandidaturas();
void salvarCandidaturas();
void candidatarAluno();
void verCandidatos();
void selecionarCandidato();
void verVagasSelecionado();
int jaCandidatado(int idVaga, char alunoEmail[]);

void esperar(int milissegundos);
void limparTela();
void barraCarregamento(char mensagem[]);
void limparBuffer();
int textoVazio(char texto[]);
void removerQuebraLinha(char texto[]);
void lerTextoObrigatorio(char mensagem[], char destino[], int tamanho);
int lerInteiro(char mensagem[], int min, int max);
void executarTestes();

// ===== UTILITÁRIOS =====
void esperar(int milissegundos) {
#ifdef _WIN32
    Sleep(milissegundos);
#else
    usleep(milissegundos * 1000);
#endif
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void barraCarregamento(char mensagem[]) {
    limparTela();

    printf("\n%s\n", mensagem);
    printf("[");

    for(int i = 0; i < 41; i++) {
        printf("=");
        fflush(stdout);
        esperar(37);
    }

    printf("]\n");
    esperar(300);
}

void limparBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraLinha(char texto[]) {
    texto[strcspn(texto, "\n")] = '\0';
}

int textoVazio(char texto[]) {
    for(int i = 0; texto[i] != '\0'; i++) {
        if(!isspace((unsigned char)texto[i])) {
            return 0;
        }
    }

    return 1;
}

void lerTextoObrigatorio(char mensagem[], char destino[], int tamanho) {
    int textoGrande;

    do {
        textoGrande = 0;
        printf("%s", mensagem);

        if(fgets(destino, tamanho, stdin) == NULL) {
            destino[0] = '\0';
        }

        // Se o usuario digitar mais caracteres do que o limite,
        // o fgets corta o texto. Nesse caso, limpamos o restante
        // que ficou no buffer para nao quebrar a proxima leitura.
        if(strchr(destino, '\n') == NULL) {
            textoGrande = 1;
            limparBuffer();
        }

        removerQuebraLinha(destino);

        if(textoVazio(destino)) {
            printf("Campo obrigatorio! Digite novamente.\n");
        } else if(textoGrande) {
            printf("Texto muito grande! Digite no maximo %d caracteres.\n", tamanho - 2);
        }

    } while(textoVazio(destino) || textoGrande);
}

int lerInteiro(char mensagem[], int min, int max) {
    int valor;
    int resultado;

    do {
        printf("%s", mensagem);
        resultado = scanf("%d", &valor);

        if(resultado != 1) {
            printf("Entrada invalida! Digite apenas numeros.\n");
            limparBuffer();
        } else if(valor < min || valor > max) {
            printf("Valor invalido! Digite entre %d e %d.\n", min, max);
            limparBuffer();
        } else {
            limparBuffer();
            return valor;
        }

    } while(1);
}

void strToLower(char *dest, const char *src, int maxLen) {
    int i;

    for(i = 0; i < maxLen - 1 && src[i] != '\0'; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }

    dest[i] = '\0';
}

int strCmpInsensitive(const char *a, const char *b) {
    char la[200], lb[200];

    strToLower(la, a, sizeof(la));
    strToLower(lb, b, sizeof(lb));

    return strcmp(la, lb);
}

// ===== MAIN =====
int main() {
    barraCarregamento("Inicializando sistema");

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

// ===== ARQUIVOS =====
void carregarUsuarios() {
    FILE *f = fopen(ARQUIVO, "r");

    if(f == NULL) return;

    totalUsuarios = 0;

    while(totalUsuarios < MAX &&
          fscanf(f, " %49[^|]|%19[^|]|%d|%49[^|]|%d|%d\n",
                 usuarios[totalUsuarios].email,
                 usuarios[totalUsuarios].senha,
                 &usuarios[totalUsuarios].tipo,
                 usuarios[totalUsuarios].area,
                 &usuarios[totalUsuarios].ehExAluno,
                 &usuarios[totalUsuarios].periodo) == 6) {
        totalUsuarios++;
    }

    fclose(f);
}

void salvarUsuarios() {
    FILE *f = fopen(ARQUIVO, "w");

    if(f == NULL) {
        printf("Erro ao salvar arquivo!\n");
        return;
    }

    for(int i = 0; i < totalUsuarios; i++) {
        fprintf(f, "%s|%s|%d|%s|%d|%d\n",
                usuarios[i].email,
                usuarios[i].senha,
                usuarios[i].tipo,
                usuarios[i].area,
                usuarios[i].ehExAluno,
                usuarios[i].periodo);
    }

    fclose(f);
}

void carregarVagas() {
    FILE *f = fopen(ARQUIVO_VAGAS, "r");

    if(f == NULL) return;

    totalVagas = 0;

    while(totalVagas < MAX &&
          fscanf(f, " %d|%79[^|]|%49[^|]|%49[^|]|%199[^|]|%199[^|]|%d|%d|%d\n",
                 &vagas[totalVagas].id,
                 vagas[totalVagas].titulo,
                 vagas[totalVagas].empresaEmail,
                 vagas[totalVagas].area,
                 vagas[totalVagas].descricao,
                 vagas[totalVagas].requisitos,
                 &vagas[totalVagas].ativa,
                 &vagas[totalVagas].vagasTotais,
                 &vagas[totalVagas].vagasPreenchidas) == 9) {
        totalVagas++;
    }

    fclose(f);

    proximoIdVaga = 1;

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id >= proximoIdVaga) {
            proximoIdVaga = vagas[i].id + 1;
        }
    }
}

void salvarVagas() {
    FILE *f = fopen(ARQUIVO_VAGAS, "w");

    if(f == NULL) {
        printf("Erro ao salvar vagas!\n");
        return;
    }

    for(int i = 0; i < totalVagas; i++) {
        fprintf(f, "%d|%s|%s|%s|%s|%s|%d|%d|%d\n",
                vagas[i].id,
                vagas[i].titulo,
                vagas[i].empresaEmail,
                vagas[i].area,
                vagas[i].descricao,
                vagas[i].requisitos,
                vagas[i].ativa,
                vagas[i].vagasTotais,
                vagas[i].vagasPreenchidas);
    }

    fclose(f);
}

void carregarCandidaturas() {
    FILE *f = fopen(ARQUIVO_CANDIDATURAS, "r");

    if(f == NULL) return;

    totalCandidaturas = 0;

    while(totalCandidaturas < MAX_CANDIDATURAS &&
          fscanf(f, " %d|%d|%49[^|]|%29[^\n]\n",
                 &candidaturas[totalCandidaturas].id,
                 &candidaturas[totalCandidaturas].idVaga,
                 candidaturas[totalCandidaturas].alunoEmail,
                 candidaturas[totalCandidaturas].status) == 4) {
        totalCandidaturas++;
    }

    fclose(f);

    proximoIdCandidatura = 1;

    for(int i = 0; i < totalCandidaturas; i++) {
        if(candidaturas[i].id >= proximoIdCandidatura) {
            proximoIdCandidatura = candidaturas[i].id + 1;
        }
    }
}

void salvarCandidaturas() {
    FILE *f = fopen(ARQUIVO_CANDIDATURAS, "w");

    if(f == NULL) {
        printf("Erro ao salvar candidaturas!\n");
        return;
    }

    for(int i = 0; i < totalCandidaturas; i++) {
        fprintf(f, "%d|%d|%s|%s\n",
                candidaturas[i].id,
                candidaturas[i].idVaga,
                candidaturas[i].alunoEmail,
                candidaturas[i].status);
    }

    fclose(f);
}

// ===== VALIDAÇÃO =====
int validarEmail(char email[]) {
    char *arroba = strchr(email, '@');
    char *ponto = strrchr(email, '.');

    if(textoVazio(email)) {
        return 0;
    }

    if(arroba == NULL || ponto == NULL) {
        return 0;
    }

    if(arroba == email || *(arroba + 1) == '\0') {
        return 0;
    }

    if(ponto < arroba || *(ponto + 1) == '\0') {
        return 0;
    }

    return 1;
}

int emailExiste(char email[]) {
    for(int i = 0; i < totalUsuarios; i++) {
        if(strcmp(usuarios[i].email, email) == 0) {
            return 1;
        }
    }

    return 0;
}

// ===== ADMIN =====
void criarAdminPadrao() {
    for(int i = 0; i < totalUsuarios; i++) {
        if(usuarios[i].tipo == ADMIN) {
            return;
        }
    }

    if(totalUsuarios < MAX) {
        barraCarregamento("Criando administrador padrao");

        strcpy(usuarios[totalUsuarios].email, "admin");
        strcpy(usuarios[totalUsuarios].senha, "admin123");
        usuarios[totalUsuarios].tipo = ADMIN;
        strcpy(usuarios[totalUsuarios].area, "Administrador");
        usuarios[totalUsuarios].ehExAluno = -1;
        usuarios[totalUsuarios].periodo = -1;

        totalUsuarios++;
        salvarUsuarios();

        printf("Admin padrao criado! (email: admin | senha: admin123)\n");
        esperar(1200);
    }
}

// ===== CADASTRO =====
void cadastro(int tipo) {
    limparTela();

    if(totalUsuarios >= MAX) {
        printf("Limite de usuarios atingido.\n");
        esperar(1200);
        return;
    }

    char email[50];

    printf("\n===== CADASTRO =====\n");

    do {
        lerTextoObrigatorio("Email: ", email, 50);

        if(!validarEmail(email)) {
            printf("Email invalido! Use um formato como nome@email.com e respeite o limite de caracteres.\n");
        } else if(emailExiste(email)) {
            printf("Email ja cadastrado!\n");
        }

    } while(!validarEmail(email) || emailExiste(email));

    strcpy(usuarios[totalUsuarios].email, email);

    lerTextoObrigatorio("Senha: ", usuarios[totalUsuarios].senha, 20);

    usuarios[totalUsuarios].tipo = tipo;

    lerTextoObrigatorio("Area de atuacao: ", usuarios[totalUsuarios].area, 50);

    if(tipo == ALUNO) {
        int opcao;

        opcao = lerInteiro("1 - Aluno\n2 - Ex-aluno\nEscolha: ", 1, 2);

        if(opcao == 2) {
            usuarios[totalUsuarios].ehExAluno = 1;
            usuarios[totalUsuarios].periodo = -1;
        } else {
            usuarios[totalUsuarios].ehExAluno = 0;
            usuarios[totalUsuarios].periodo = lerInteiro("Periodo atual: ", 1, 12);
        }
    } else {
        usuarios[totalUsuarios].ehExAluno = -1;
        usuarios[totalUsuarios].periodo = -1;
    }

    barraCarregamento("Cadastrando usuario");

    totalUsuarios++;
    salvarUsuarios();

    printf("Cadastro realizado com sucesso!\n");
    esperar(1200);
}

// ===== LOGIN =====
int login(int tipo) {
    limparTela();

    char email[50], senha[20];

    printf("\n===== LOGIN =====\n");

    lerTextoObrigatorio("Email: ", email, 50);

    lerTextoObrigatorio("Senha: ", senha, 20);

    barraCarregamento("Processando login");

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

// ===== MENUS =====
void menuPrincipal() {
    int opcao;

    do {
        limparTela();

        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1 - Empresa\n");
        printf("2 - Aluno/Ex-aluno\n");
        printf("3 - Admin\n");
        printf("9 - Executar testes\n");
        printf("0 - Sair\n");
        opcao = lerInteiro("Escolha: ", 0, 9);

        switch(opcao) {
            case 1:
                barraCarregamento("Carregando area da empresa");
                menuLoginCadastro(EMPRESA);
                break;

            case 2:
                barraCarregamento("Carregando area do aluno");
                menuLoginCadastro(ALUNO);
                break;

            case 3:
                barraCarregamento("Carregando area administrativa");
                menuLoginCadastro(ADMIN);
                break;

            case 9:
                executarTestes();
                printf("\nPressione ENTER para voltar...");
                getchar();
                break;

            case 0:
                barraCarregamento("Encerrando sistema");
                salvarUsuarios();
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                esperar(1200);
        }

    } while(opcao != 0);
}

void menuLoginCadastro(int tipo) {
    int opcao;

    do {
        limparTela();

        printf("\n===== LOGIN / CADASTRO =====\n");
        printf("1 - Login\n");
        printf("2 - Cadastro\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Escolha: ", 0, 9);

        switch(opcao) {
            case 1:
                if(login(tipo)) {
                    printf("Login realizado com sucesso!\n");
                    esperar(1000);

                    if(tipo == ADMIN) {
                        menuAdmin();
                    } else if(tipo == EMPRESA) {
                        printf("Bem-vindo, Empresa!\n");
                        esperar(1000);
                        menuEmpresa();
                    } else {
                        printf("Bem-vindo, Aluno!\n");
                        esperar(1000);
                        menuAluno();
                    }

                } else {
                    printf("Dados incorretos!\n");
                    esperar(1200);
                }
                break;

            case 2:
                if(tipo == ADMIN) {
                    printf("Apenas admins logados podem criar outros admins!\n");
                    esperar(1200);
                } else {
                    cadastro(tipo);
                }
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida!\n");
                esperar(1200);
        }

    } while(opcao != 0);
}

void menuEmpresa() {
    int opcao;

    do {
        limparTela();

        printf("\n===== MENU EMPRESA =====\n");
        printf("1 - Criar vaga\n");
        printf("2 - Listar vagas\n");
        printf("3 - Ver candidatos\n");
        printf("4 - Selecionar candidato\n");
        printf("5 - Editar vaga\n");
        printf("6 - Excluir vaga\n");
        printf("0 - Logout\n");
        opcao = lerInteiro("Escolha: ", 0, 9);

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

            case 5:
                editarVagaEmpresa();
                break;

            case 6:
                excluirVagaEmpresa();
                break;

            case 0:
                barraCarregamento("Saindo da conta da empresa");
                usuarioLogado = -1;
                printf("Logout realizado.\n");
                esperar(1000);
                break;

            default:
                printf("Opcao invalida!\n");
                esperar(1200);
        }

    } while(opcao != 0);
}

void menuAluno() {
    int opcao;

    do {
        limparTela();

        printf("\n===== MENU ALUNO =====\n");
        printf("1 - Ver vagas\n");
        printf("2 - Buscar vagas\n");
        printf("3 - Se candidatar\n");
        printf("4 - Minhas selecoes\n");
        printf("0 - Logout\n");
        opcao = lerInteiro("Escolha: ", 0, 9);

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
                barraCarregamento("Saindo da conta do aluno");
                usuarioLogado = -1;
                printf("Logout realizado.\n");
                esperar(1000);
                break;

            default:
                printf("Opcao invalida!\n");
                esperar(1200);
        }

    } while(opcao != 0);
}

void menuAdmin() {
    int opcao;

    do {
        limparTela();

        printf("\n===== MENU ADMIN =====\n");
        printf("1 - Criar novo admin\n");
        printf("2 - Listar usuarios\n");
        printf("3 - Remover usuario\n");
        printf("4 - Listar empresas\n");
        printf("5 - Listar vagas\n");
        printf("6 - Remover vaga\n");
        printf("7 - Relatorio geral\n");
        printf("0 - Sair\n");
        opcao = lerInteiro("Escolha: ", 0, 9);

        switch(opcao) {
            case 1:
                cadastro(ADMIN);
                break;

            case 2:
                listarUsuariosAdmin();
                break;

            case 3:
                removerUsuarioAdmin();
                break;

            case 4:
                listarEmpresasAdmin();
                break;

            case 5:
                listarVagas();
                break;

            case 6:
                removerVagaAdmin();
                break;

            case 7:
                relatorioGeral();
                break;

            case 0:
                barraCarregamento("Saindo do menu admin");
                break;

            default:
                printf("Opcao invalida!\n");
                esperar(1200);
        }

    } while(opcao != 0);
}

void listarUsuariosAdmin() {
    limparTela();

    barraCarregamento("Carregando usuarios");

    printf("\n===== USUARIOS CADASTRADOS =====\n");

    for(int i = 0; i < totalUsuarios; i++) {
        printf("\n----------------------------\n");
        printf("Email: %s\n", usuarios[i].email);

        printf("Tipo: ");
        if(usuarios[i].tipo == ADMIN) {
            printf("ADMIN\n");
        } else if(usuarios[i].tipo == EMPRESA) {
            printf("EMPRESA\n");
        } else {
            printf("ALUNO/EX-ALUNO\n");
        }

        printf("Area: %s\n", usuarios[i].area);

        if(usuarios[i].tipo == ALUNO) {
            if(usuarios[i].ehExAluno == 1) {
                printf("Situacao: Ex-aluno\n");
            } else {
                printf("Situacao: Aluno\n");
                printf("Periodo: %d\n", usuarios[i].periodo);
            }
        }
    }

    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}

void removerUsuarioAdmin() {
    limparTela();

    char email[50];
    int encontrou = 0;

    printf("\n===== REMOVER USUARIO =====\n");
    lerTextoObrigatorio("Digite o email do usuario que deseja remover: ", email, 50);

    if(usuarioLogado != -1 && strcmp(usuarios[usuarioLogado].email, email) == 0) {
        printf("Voce nao pode remover o proprio usuario logado.\n");
        esperar(1500);
        return;
    }

    for(int i = 0; i < totalUsuarios; i++) {
        if(strcmp(usuarios[i].email, email) == 0) {
            encontrou = 1;

            for(int j = i; j < totalUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }

            totalUsuarios--;
            salvarUsuarios();

            barraCarregamento("Removendo usuario");
            printf("Usuario removido com sucesso!\n");
            esperar(1200);
            break;
        }
    }

    if(!encontrou) {
        printf("Usuario nao encontrado.\n");
        esperar(1200);
    }
}

void listarEmpresasAdmin() {
    limparTela();

    int encontrou = 0;

    barraCarregamento("Carregando empresas");

    printf("\n===== EMPRESAS CADASTRADAS =====\n");

    for(int i = 0; i < totalUsuarios; i++) {
        if(usuarios[i].tipo == EMPRESA) {
            printf("\n----------------------------\n");
            printf("Email da empresa: %s\n", usuarios[i].email);
            printf("Area de atuacao: %s\n", usuarios[i].area);
            encontrou = 1;
        }
    }

    if(!encontrou) {
        printf("Nenhuma empresa cadastrada.\n");
    }

    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}

void removerVagaAdmin() {
    limparTela();

    int id;
    int encontrou = 0;

    printf("\n===== REMOVER VAGA =====\n");
    id = lerInteiro("Digite o ID da vaga que deseja remover: ", 1, 999999);

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == id) {
            encontrou = 1;
            vagas[i].ativa = 0;
            salvarVagas();

            barraCarregamento("Removendo vaga");
            printf("Vaga removida/desativada com sucesso!\n");
            esperar(1200);
            break;
        }
    }

    if(!encontrou) {
        printf("Vaga nao encontrada.\n");
        esperar(1200);
    }
}

void relatorioGeral() {
    limparTela();

    int admins = 0;
    int empresas = 0;
    int alunos = 0;
    int exAlunos = 0;
    int vagasAtivas = 0;
    int vagasInativas = 0;
    int candidaturasEnviadas = 0;
    int candidaturasSelecionadas = 0;
    int candidaturasNaoSelecionadas = 0;

    barraCarregamento("Gerando relatorio geral");

    for(int i = 0; i < totalUsuarios; i++) {
        if(usuarios[i].tipo == ADMIN) {
            admins++;
        } else if(usuarios[i].tipo == EMPRESA) {
            empresas++;
        } else if(usuarios[i].tipo == ALUNO) {
            if(usuarios[i].ehExAluno == 1) {
                exAlunos++;
            } else {
                alunos++;
            }
        }
    }

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].ativa == 1) {
            vagasAtivas++;
        } else {
            vagasInativas++;
        }
    }

    for(int i = 0; i < totalCandidaturas; i++) {
        if(strcmp(candidaturas[i].status, "Enviada") == 0) {
            candidaturasEnviadas++;
        } else if(strcmp(candidaturas[i].status, "Selecionado") == 0) {
            candidaturasSelecionadas++;
        } else if(strcmp(candidaturas[i].status, "Nao Selecionado") == 0) {
            candidaturasNaoSelecionadas++;
        }
    }

    printf("\n========== RELATORIO GERAL ==========" );
    printf("\nTotal de usuarios: %d", totalUsuarios);
    printf("\nAdmins: %d", admins);
    printf("\nEmpresas: %d", empresas);
    printf("\nAlunos: %d", alunos);
    printf("\nEx-alunos: %d", exAlunos);

    printf("\n\nTotal de vagas: %d", totalVagas);
    printf("\nVagas ativas: %d", vagasAtivas);
    printf("\nVagas inativas/encerradas: %d", vagasInativas);

    printf("\n\nTotal de candidaturas: %d", totalCandidaturas);
    printf("\nCandidaturas enviadas: %d", candidaturasEnviadas);
    printf("\nCandidaturas selecionadas: %d", candidaturasSelecionadas);
    printf("\nCandidaturas nao selecionadas: %d", candidaturasNaoSelecionadas);
    printf("\n====================================\n");

    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}

// ===== VAGAS =====
int indiceVagaPorId(int idVaga) {
    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga) {
            return i;
        }
    }

    return -1;
}

void criarVaga() {
    limparTela();

    if(totalVagas >= MAX) {
        printf("Limite de vagas atingido.\n");
        esperar(1200);
        return;
    }

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem criar vagas.\n");
        esperar(1200);
        return;
    }

    vagas[totalVagas].id = proximoIdVaga++;

    strcpy(vagas[totalVagas].empresaEmail, usuarios[usuarioLogado].email);

    lerTextoObrigatorio("Titulo da vaga: ", vagas[totalVagas].titulo, 80);

    lerTextoObrigatorio("Area da vaga: ", vagas[totalVagas].area, 50);

    lerTextoObrigatorio("Descricao da vaga: ", vagas[totalVagas].descricao, 200);

    lerTextoObrigatorio("Requisitos da vaga: ", vagas[totalVagas].requisitos, 200);

    vagas[totalVagas].vagasTotais = lerInteiro("Quantas pessoas voce esta buscando para esta vaga: ", 1, 100);

    vagas[totalVagas].vagasPreenchidas = 0;
    vagas[totalVagas].ativa = 1;

    barraCarregamento("Publicando vaga");

    totalVagas++;
    salvarVagas();

    printf("Vaga cadastrada com sucesso!\n");
    esperar(1200);
}


void editarVagaEmpresa() {
    limparTela();

    int idVaga;
    int indiceVaga = -1;
    int encontrouVaga = 0;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem editar vagas.\n");
        esperar(1200);
        return;
    }

    printf("\n===== EDITAR VAGA =====\n");
    printf("\nSuas vagas ativas:\n");

    for(int i = 0; i < totalVagas; i++) {
        if(strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            printf("ID: %d | Titulo: %s | Vagas: %d/%d preenchidas\n",
                   vagas[i].id,
                   vagas[i].titulo,
                   vagas[i].vagasPreenchidas,
                   vagas[i].vagasTotais);
            encontrouVaga = 1;
        }
    }

    if(!encontrouVaga) {
        printf("Voce ainda nao possui vagas ativas para editar.\n");
        esperar(1500);
        return;
    }

    idVaga = lerInteiro("\nDigite o ID da vaga que deseja editar: ", 1, 999999);

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga &&
           strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            indiceVaga = i;
            break;
        }
    }

    if(indiceVaga == -1) {
        printf("Vaga invalida, inativa ou nao pertence a sua empresa.\n");
        esperar(1500);
        return;
    }

    printf("\nDados atuais da vaga:\n");
    printf("Titulo: %s\n", vagas[indiceVaga].titulo);
    printf("Area: %s\n", vagas[indiceVaga].area);
    printf("Descricao: %s\n", vagas[indiceVaga].descricao);
    printf("Requisitos: %s\n", vagas[indiceVaga].requisitos);
    printf("Vagas preenchidas: %d/%d\n", vagas[indiceVaga].vagasPreenchidas, vagas[indiceVaga].vagasTotais);

    printf("\nDigite os novos dados da vaga:\n");

    lerTextoObrigatorio("Novo titulo da vaga: ", vagas[indiceVaga].titulo, 80);
    lerTextoObrigatorio("Nova area da vaga: ", vagas[indiceVaga].area, 50);
    lerTextoObrigatorio("Nova descricao da vaga: ", vagas[indiceVaga].descricao, 200);
    lerTextoObrigatorio("Novos requisitos da vaga: ", vagas[indiceVaga].requisitos, 200);

    vagas[indiceVaga].vagasTotais = lerInteiro("Nova quantidade total de vagas: ", vagas[indiceVaga].vagasPreenchidas, 100);

    barraCarregamento("Atualizando vaga");

    salvarVagas();

    printf("Vaga editada com sucesso!\n");
    esperar(1200);
}

void excluirVagaEmpresa() {
    limparTela();

    int idVaga;
    int indiceVaga = -1;
    int encontrouVaga = 0;
    int confirmacao;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem excluir vagas.\n");
        esperar(1200);
        return;
    }

    printf("\n===== EXCLUIR VAGA =====\n");
    printf("\nSuas vagas ativas:\n");

    for(int i = 0; i < totalVagas; i++) {
        if(strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            printf("ID: %d | Titulo: %s | Vagas: %d/%d preenchidas\n",
                   vagas[i].id,
                   vagas[i].titulo,
                   vagas[i].vagasPreenchidas,
                   vagas[i].vagasTotais);
            encontrouVaga = 1;
        }
    }

    if(!encontrouVaga) {
        printf("Voce ainda nao possui vagas ativas para excluir.\n");
        esperar(1500);
        return;
    }

    idVaga = lerInteiro("\nDigite o ID da vaga que deseja excluir: ", 1, 999999);

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga &&
           strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            indiceVaga = i;
            break;
        }
    }

    if(indiceVaga == -1) {
        printf("Vaga invalida, inativa ou nao pertence a sua empresa.\n");
        esperar(1500);
        return;
    }

    printf("\nVaga selecionada: %s\n", vagas[indiceVaga].titulo);
    printf("Essa acao ira desativar a vaga e marcar candidaturas pendentes como Nao Selecionado.\n");
    confirmacao = lerInteiro("Confirmar exclusao? 1 - Sim | 2 - Nao: ", 1, 2);

    if(confirmacao == 2) {
        printf("Exclusao cancelada.\n");
        esperar(1200);
        return;
    }

    vagas[indiceVaga].ativa = 0;

    for(int i = 0; i < totalCandidaturas; i++) {
        if(candidaturas[i].idVaga == idVaga &&
           strcmp(candidaturas[i].status, "Enviada") == 0) {
            strcpy(candidaturas[i].status, "Nao Selecionado");
        }
    }

    barraCarregamento("Excluindo vaga");

    salvarVagas();
    salvarCandidaturas();

    printf("Vaga excluida/desativada com sucesso!\n");
    esperar(1200);
}

void listarVagas() {
    limparTela();

    int encontrou = 0;

    barraCarregamento("Carregando vagas");

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].ativa == 1) {
            printf("\nID: %d\n", vagas[i].id);
            printf("Titulo: %s\n", vagas[i].titulo);
            printf("Empresa: %s\n", vagas[i].empresaEmail);
            printf("Area: %s\n", vagas[i].area);
            printf("Descricao: %s\n", vagas[i].descricao);
            printf("Requisitos: %s\n", vagas[i].requisitos);
            printf("Vagas: %d/%d preenchidas\n", vagas[i].vagasPreenchidas, vagas[i].vagasTotais);

            encontrou = 1;
        }
    }

    if(!encontrou) {
        printf("Nenhuma vaga cadastrada ou disponivel.\n");
    }

    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}

void buscarVagas() {
    limparTela();

    char areaBusca[50];
    int encontrou = 0;

    lerTextoObrigatorio("Digite a area desejada: ", areaBusca, 50);

    barraCarregamento("Buscando vagas");

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].ativa == 1 && strCmpInsensitive(vagas[i].area, areaBusca) == 0) {
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

    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}

int vagaExiste(int idVaga) {
    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga && vagas[i].ativa == 1) {
            return 1;
        }
    }

    return 0;
}


void executarTestes() {
    limparTela();

    printf("\n===== TESTES DO SISTEMA =====\n");

    printf("\nTeste 1 - Email valido com @ e ponto: ");
    if(validarEmail("teste@email.com")) {
        printf("PASSOU\n");
    } else {
        printf("FALHOU\n");
    }

    printf("Teste 2 - Email sem @ deve falhar: ");
    if(!validarEmail("testeemail.com")) {
        printf("PASSOU\n");
    } else {
        printf("FALHOU\n");
    }

    printf("Teste 3 - Email sem ponto deve falhar: ");
    if(!validarEmail("teste@email")) {
        printf("PASSOU\n");
    } else {
        printf("FALHOU\n");
    }

    printf("Teste 4 - Vaga inexistente deve falhar: ");
    if(!vagaExiste(999999)) {
        printf("PASSOU\n");
    } else {
        printf("FALHOU\n");
    }

    printf("Teste 5 - Candidatura inexistente deve falhar: ");
    if(!jaCandidatado(999999, "aluno@email.com")) {
        printf("PASSOU\n");
    } else {
        printf("FALHOU\n");
    }

    printf("\n===== FIM DOS TESTES =====\n");
}

// ===== CANDIDATURAS =====
int jaCandidatado(int idVaga, char alunoEmail[]) {
    for(int i = 0; i < totalCandidaturas; i++) {
        if(candidaturas[i].idVaga == idVaga &&
           strcmp(candidaturas[i].alunoEmail, alunoEmail) == 0) {
            return 1;
        }
    }

    return 0;
}

void candidatarAluno() {
    limparTela();

    int idVaga;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != ALUNO) {
        printf("Apenas alunos/ex-alunos podem se candidatar.\n");
        esperar(1200);
        return;
    }

    if(totalCandidaturas >= MAX_CANDIDATURAS) {
        printf("Limite de candidaturas atingido.\n");
        esperar(1200);
        return;
    }

    listarVagas();

    limparTela();

    idVaga = lerInteiro("\nDigite o ID da vaga desejada: ", 1, 999999);

    if(!vagaExiste(idVaga)) {
        printf("Vaga inexistente ou inativa.\n");
        esperar(1200);
        return;
    }

    if(jaCandidatado(idVaga, usuarios[usuarioLogado].email)) {
        printf("Voce ja se candidatou a essa vaga.\n");
        esperar(1200);
        return;
    }

    candidaturas[totalCandidaturas].id = proximoIdCandidatura++;
    candidaturas[totalCandidaturas].idVaga = idVaga;

    strcpy(candidaturas[totalCandidaturas].alunoEmail, usuarios[usuarioLogado].email);
    strcpy(candidaturas[totalCandidaturas].status, "Enviada");

    barraCarregamento("Enviando candidatura");

    totalCandidaturas++;
    salvarCandidaturas();

    printf("Candidatura realizada com sucesso!\n");
    esperar(1200);
}

void verCandidatos() {
    limparTela();

    int idVaga;
    int encontrouVaga = 0;
    int encontrouCandidato = 0;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem ver candidatos.\n");
        esperar(1200);
        return;
    }

    barraCarregamento("Carregando candidatos");

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
        esperar(1200);
        return;
    }

    idVaga = lerInteiro("\nDigite o ID da vaga para ver candidatos: ", 1, 999999);

    int vagaValida = 0;

    for(int i = 0; i < totalVagas; i++) {
        if(vagas[i].id == idVaga &&
           strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0) {
            vagaValida = 1;
            break;
        }
    }

    if(!vagaValida) {
        printf("Vaga invalida ou nao pertence a sua empresa.\n");
        esperar(1200);
        return;
    }

    barraCarregamento("Buscando candidatos da vaga");

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

    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}

void selecionarCandidato() {
    limparTela();

    int idVaga;
    int encontrouVaga = 0;
    int encontrouCandidato = 0;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != EMPRESA) {
        printf("Apenas empresas podem selecionar candidatos.\n");
        esperar(1200);
        return;
    }

    printf("\nSuas vagas:\n");

    for(int i = 0; i < totalVagas; i++) {
        if(strcmp(vagas[i].empresaEmail, usuarios[usuarioLogado].email) == 0 &&
           vagas[i].ativa == 1) {
            printf("ID: %d | Titulo: %s | Vagas: %d/%d\n",
                   vagas[i].id,
                   vagas[i].titulo,
                   vagas[i].vagasPreenchidas,
                   vagas[i].vagasTotais);

            encontrouVaga = 1;
        }
    }

    if(!encontrouVaga) {
        printf("Voce ainda nao cadastrou vagas.\n");
        esperar(1200);
        return;
    }

    idVaga = lerInteiro("\nDigite o ID da vaga para selecionar candidatos: ", 1, 999999);

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
        esperar(1200);
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
        esperar(1200);
        return;
    }

    printf("--------------------------------------------\n");

    while(vagas[indiceVaga].vagasPreenchidas < vagas[indiceVaga].vagasTotais) {
        int restam = vagas[indiceVaga].vagasTotais - vagas[indiceVaga].vagasPreenchidas;

        printf("\nAinda faltam %d vaga(s). Digite o ID da candidatura (0 para parar): ", restam);

        int idCandidatura;
        idCandidatura = lerInteiro("", 0, 999999);

        if(idCandidatura == 0) break;

        int achou = 0;

        for(int i = 0; i < totalCandidaturas; i++) {
            if(candidaturas[i].id == idCandidatura &&
               candidaturas[i].idVaga == idVaga &&
               strcmp(candidaturas[i].status, "Enviada") == 0) {

                barraCarregamento("Selecionando candidato");

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

    if(vagas[indiceVaga].vagasPreenchidas >= vagas[indiceVaga].vagasTotais) {
        vagas[indiceVaga].ativa = 0;

        for(int i = 0; i < totalCandidaturas; i++) {
            if(candidaturas[i].idVaga == idVaga &&
               strcmp(candidaturas[i].status, "Enviada") == 0) {
                strcpy(candidaturas[i].status, "Nao Selecionado");
            }
        }

        printf("Vaga encerrada automaticamente pois todas as vagas foram preenchidas.\n");
    }

    barraCarregamento("Salvando selecao");

    salvarVagas();
    salvarCandidaturas();

    printf("\nSelecao salva com sucesso!\n");
    esperar(1200);
}

void verVagasSelecionado() {
    limparTela();

    int encontrou = 0;

    if(usuarioLogado == -1 || usuarios[usuarioLogado].tipo != ALUNO) {
        printf("Apenas alunos podem ver suas selecoes.\n");
        esperar(1200);
        return;
    }

    barraCarregamento("Carregando suas selecoes");

    printf("\n===== VAGAS QUE FUI SELECIONADO =====\n");

    for(int i = 0; i < totalCandidaturas; i++) {
        if(strcmp(candidaturas[i].alunoEmail, usuarios[usuarioLogado].email) == 0 &&
           strcmp(candidaturas[i].status, "Selecionado") == 0) {

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

    printf("\nPressione ENTER para voltar...");
    getchar();
    getchar();
}