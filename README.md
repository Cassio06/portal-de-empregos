# portal-de-empregos
📚 Portal de Empregos Acadêmico (em C)
📌 Descrição

Este projeto consiste em um sistema de portal de empregos acadêmico, desenvolvido na linguagem C, com o objetivo de conectar empresas a alunos e ex-alunos de uma instituição.

A aplicação permite o gerenciamento de usuários, publicação de vagas e realização de candidaturas, utilizando arquivos .dat como armazenamento de dados, simulando um banco de dados.

🎯 Objetivo

Criar uma plataforma simples e funcional onde:

Empresas possam divulgar oportunidades
Alunos possam encontrar vagas e se candidatar
O sistema gerencie essas interações de forma organizada
👥 Tipos de Usuários
🏢 Empresa

Pode:

Se cadastrar
Publicar vagas
Visualizar candidatos
👨‍🎓 Aluno / Ex-aluno

Pode:

Se cadastrar
Visualizar vagas disponíveis
Se candidatar às vagas
🔐 Funcionalidades Principais
📝 Cadastro de Usuários
Empresa:
Nome da empresa
Área de atuação
Email corporativo
Aluno / Ex-aluno:
Nome
Email
Telefone
Área de atuação
Situação acadêmica:
Em formação → informar período
Formado → período não obrigatório
💼 Cadastro de Vagas

As empresas podem cadastrar vagas com:

Título
Descrição
Requisitos
Carga horária diária
Área da vaga
Empresa responsável
Status:
Aberta
Fechada
🔍 Visualização e Busca de Vagas

Alunos podem:

Listar todas as vagas abertas
Buscar por:
Nome da vaga
Área
Empresa

📌 Regra: Apenas vagas com status aberta são exibidas

📩 Sistema de Candidatura

Fluxo:

Aluno seleciona uma vaga
Sistema registra a candidatura

📌 Regras:

Não é permitido se candidatar duas vezes na mesma vaga
Não é possível se candidatar a vagas fechadas
🔄 Fluxo Geral do Sistema
Usuário escolhe o tipo (Empresa ou Aluno)
Realiza cadastro/login
Empresa publica vagas
Aluno visualiza vagas
Aluno se candidata
Empresa visualiza candidatos
💾 Armazenamento de Dados

O sistema utiliza arquivos binários (.dat) para simular um banco de dados:

usuarios.dat
vagas.dat
candidaturas.dat
📌 Funcionamento:
Dados são gravados nos arquivos
O sistema lê os arquivos para exibir informações
Buscas são feitas percorrendo os registros
⚙️ Regras de Negócio
Apenas empresas podem cadastrar vagas
Apenas alunos podem se candidatar
Não é permitido candidatura duplicada
Vagas possuem status (aberta/fechada)
Campos obrigatórios devem ser preenchidos
Período só é exigido para alunos em formação
🧱 Estrutura do Sistema

O sistema é organizado em:

Menu principal
Submenus por tipo de usuário
Módulos separados:
Cadastro
Vagas
Candidaturas
Uso de structs para organização dos dados
🛠️ Tecnologias Utilizadas
Linguagem: C
Manipulação de arquivos: fopen, fwrite, fread, fclose
Estruturas: struct
Controle de fluxo: menus interativos
▶️ Como Executar
Compile o programa:
gcc sistema.c -o sistema
Execute:
./sistema
👨‍💻 Contribuintes
Cássio Filipe Meireles de Carvalho Braga
GitHub: https://github.com/Cassio06
Charlys Arthur Daher de Medeiros Salmento
GitHub: https://github.com/Charlys7
Gabriel de Assis Buhatem
GitHub: https://github.com/GabrieldeAssisBuhatem
gavriEl07
GitHub: https://github.com/gavriEl07
rdzzz-cmd
GitHub: https://github.com/rdzzz-cmd
📄 Licença

Este projeto é de uso acadêmico e livre para estudos e melhorias.
