# 📚 Portal de Empregos Acadêmico em C

## 📌 Descrição

Este projeto consiste em um sistema de **Portal de Empregos Acadêmico**, desenvolvido em linguagem **C**, com o objetivo de conectar **empresas**, **alunos** e **ex-alunos** de uma instituição.

O sistema permite cadastro e login de usuários, publicação de vagas, candidaturas, seleção de candidatos e gerenciamento administrativo. Os dados são armazenados em arquivos `.txt`, simulando um banco de dados simples.

---

## 🎯 Objetivo

Criar uma aplicação simples e funcional onde:

- Empresas possam divulgar oportunidades
- Alunos e ex-alunos possam visualizar e se candidatar a vagas
- Empresas possam analisar e selecionar candidatos
- Administradores possam gerenciar usuários, empresas, vagas e relatórios

---

## 👥 Tipos de Usuários

### 🏢 Empresa

Pode:

- Se cadastrar
- Fazer login
- Criar vagas
- Editar vagas
- Excluir/desativar vagas
- Visualizar candidatos
- Selecionar candidatos

### 👨‍🎓 Aluno / Ex-aluno

Pode:

- Se cadastrar
- Fazer login
- Visualizar vagas disponíveis
- Buscar vagas por área
- Candidatar-se a vagas
- Visualizar vagas em que foi selecionado

### 🛡️ Administrador

Pode:

- Fazer login
- Criar novos administradores
- Listar usuários
- Remover usuários
- Listar empresas
- Listar vagas
- Remover/desativar vagas
- Gerar relatório geral do sistema

> Admin padrão:
>
> Email: `admin`  
> Senha: `admin123`

---

## 🔐 Funcionalidades Principais

### 📝 Cadastro de Usuários

#### Empresa

- Email
- Senha
- Área de atuação

#### Aluno / Ex-aluno

- Email
- Senha
- Área de atuação
- Situação acadêmica:
  - Aluno em formação
  - Ex-aluno
- Período atual, caso ainda esteja em formação

---

## 💼 Cadastro de Vagas

As empresas podem cadastrar vagas com:

- ID automático
- Título
- Área
- Descrição
- Requisitos
- Email da empresa responsável
- Quantidade total de vagas
- Quantidade de vagas preenchidas
- Status:
  - Ativa
  - Inativa/encerrada

---

## 🔍 Visualização e Busca de Vagas

Alunos podem:

- Listar vagas ativas
- Buscar vagas por área

Empresas e administradores também podem visualizar as vagas cadastradas.

---

## 📩 Sistema de Candidatura

Fluxo:

1. O aluno visualiza as vagas disponíveis
2. Escolhe uma vaga pelo ID
3. O sistema registra a candidatura
4. A empresa visualiza os candidatos
5. A empresa pode selecionar candidatos
6. Quando todas as vagas forem preenchidas, a vaga é encerrada automaticamente

### Regras

- Apenas alunos podem se candidatar
- Não é permitido se candidatar duas vezes na mesma vaga
- Não é possível se candidatar a vagas inativas
- Apenas empresas podem selecionar candidatos
- A vaga é encerrada automaticamente quando todas as vagas forem preenchidas

---

## 🛡️ Área Administrativa

O administrador pode:

- Criar novos administradores
- Listar todos os usuários
- Remover usuários
- Listar empresas cadastradas
- Listar vagas
- Remover/desativar vagas
- Gerar relatório geral com:
  - Total de usuários
  - Total de admins
  - Total de empresas
  - Total de alunos
  - Total de ex-alunos
  - Total de vagas ativas e inativas
  - Total de candidaturas enviadas, selecionadas e não selecionadas

---

## 💾 Armazenamento de Dados

O sistema utiliza arquivos `.txt` para armazenar os dados:

- `usuarios.txt`
- `vagas.txt`
- `candidaturas.txt`

Os dados são salvos em formato separado por `|`, permitindo leitura e gravação dos registros com `fprintf` e `fscanf`.

---

## ⚙️ Regras de Negócio

- Apenas empresas podem criar, editar e excluir vagas
- Apenas alunos e ex-alunos podem se candidatar
- Apenas administradores podem acessar o menu administrativo
- Não é permitido cadastro com email duplicado
- O sistema valida formato básico de email
- Campos obrigatórios não podem ficar vazios
- Textos muito grandes são bloqueados para evitar erros
- Vagas preenchidas são encerradas automaticamente
- Candidaturas pendentes de vagas encerradas são marcadas como “Não Selecionado”

---

## 🧱 Estrutura do Sistema

O sistema é organizado em:

- Menu principal
- Menu de login/cadastro
- Menu da empresa
- Menu do aluno
- Menu do administrador
- Módulo de usuários
- Módulo de vagas
- Módulo de candidaturas
- Módulo de testes
- Funções utilitárias para validação, leitura segura, limpeza de tela e barra de carregamento

---

## 🛠️ Tecnologias Utilizadas

- Linguagem C
- `struct`
- `fopen`
- `fprintf`
- `fscanf`
- `fclose`
- `strcmp`
- `strcpy`
- `fgets`
- Manipulação de arquivos `.txt`
- Menus interativos no terminal
- Validações de entrada
- Compatibilidade básica com Windows e Linux

---

## 🧪 Testes Internos

O sistema possui uma opção no menu principal para executar testes básicos, incluindo:

- Validação de email correto
- Rejeição de email sem `@`
- Rejeição de email sem ponto
- Verificação de vaga inexistente
- Verificação de candidatura inexistente

---

## ▶️ Como Executar

Compile o programa:

```bash
gcc trabalho.c -o trabalho
````

Execute:

```bash
./trabalho
```

No Windows, o executável pode ser iniciado com:

```bash
trabalho.exe
```

---

## 👨‍💻 Contribuintes

- Cássio Filipe Meireles de Carvalho Braga  
  GitHub: https://github.com/Cassio06

- Charlys Arthur Daher de Medeiros Salmento  
  GitHub: https://github.com/Charlys7

- Gabriel de Assis Buhatem  
  GitHub: https://github.com/GabrieldeAssisBuhatem

- Cezar Gabriel de Souza Vicente Mendes  
  GitHub: https://github.com/gavriEl07

- Lukas Souza Melo  
  GitHub: https://github.com/rdzzz-cmd

---

## 📄 Licença

Este projeto é de uso acadêmico e livre para estudos, testes e melhorias.

```
```
