# Projeto de Reserva de Assentos

## Autores, descrição e requisitos

Este é um projeto desenvolvido pelos alunos Vinicius Penido, Vinicius Assis, João Henrique, Matheus Boletta, Guilherme Andrade e Felipe Mareca, com o objetivo de criar um sistema de reserva de assentos para sessões de filmes. 
O projeto é dividido em duas partes principais: a interface e o backend. Usamos arquivos para armazenar os ingressos comprados e também usamos Listas encadeadas para armazenar informações na parte da interface.

## Estrutura de Pastas

O projeto está divido em três pastas:

1. `backend`:
   1. `src`: Esta pasta contém o arquivo binário com os cadastros dos usuários.
   
   2. `lib`: Esta pasta contém as funções auxiliares necessárias para o funcionamento do programa. Essas funções podem ser divididas em arquivos separados para facilitar a manutenção e modularidade do código.
   
   3. `include`: Esta pasta contém os arquivos de cabeçalho (`.h`) que definem os tipos e protótipos das funções utilizadas no projeto. Esses arquivos de cabeçalho devem ser incluídos nos arquivos que fazem uso das funções correspondentes.
2. `interface`:
   1. `InterfaceLogin`: Esta pasta contém o arquivo principal do projeto, o main.c, onde roda toda a interface junto as funções do backend.
   2. `assets`: Esta pasta contém as imagens e fonte utilizada na interface.
3. `arquivos`: Esta pasta contém o arquivo binário com as informações de login
   
## Dependências

O projeto faz uso das seguintes dependências:

- Linux (testamos no Ubuntu)
- Clang ou GCC: Necessário para compilar o código
- Ncurses: O projeto utiliza da biblioteca Ncurses para desenvolver a interface visual da tela de selecionar assentos.
- SDL2, SDL2_image, SDL2_TTF: O projeto utiliza as bibliotecas SDL2 para desenvolver a interface visual da tela de login, cadastro, selecionar filme e selecionar horário.

## Compilação e Execução

Para compilar o projeto, siga as instruções abaixo:

1. Certifique-se de ter o Clang/GCC instalado em seu sistema.

2. Instale as dependências do projeto:
   ```bash
   sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libncurses5-dev
   ```

3. Navegue até a pasta raiz do projeto no terminal.

4. Execute o seguinte comando para compilar o projeto:
   
   GCC:
   ```bash
   gcc interface/src/InterfaceLogin/main.c backend/lib/funcoesCadastro.c backend/lib/funcoesCarrinho.c -I backend/include -o CINEMA -lSDL2 -lSDL2_image -lSDL2_ttf -lncurses
   ```
   CLANG:
   ```bash
   clang -Wall interface/src/InterfaceLogin/main.c backend/lib/funcoesCadastro.c backend/lib/funcoesCarrinho.c -I backend/include -o CINEMA -lSDL2 -lSDL2_image -lSDL2_ttf -lncurses
   ```
  
6. Após a compilação bem-sucedida, você pode executar o programa com o seguinte comando:

    ```bash
   ./CINEMA
    ```
## Funcionamento do Programa

Ao executar o programa, você será apresentado à interface de reserva de assentos. O programa fornecerá as seguintes opções:

1. **Fazer seu cadastro e login:** Você deverá inserir um usuário único e uma senha, estes dados serão utilizados para logar em sua conta. Logue-se para prosseguir.

2. **Escolher a sessão do filme:** Você poderá selecionar a sessão desejada a partir de uma lista de sessões disponíveis.

3. **Escolher o horário da sessão:** Você poderá selecionar o horário que deseja.

4. **Escolher um assento:** Será exibida uma matriz representando a disposição dos assentos na sala de cinema. Você poderá escolher um assento disponível a partir desta matriz.

5. **Finalizar reserva:** A reserva do assento será concluída, o assento será marcado como ocupado na matriz e as informações serão armazenadas no arquivo dentro da pasta arquivos.

## Organização:

O projeto será dividido em dois grupos BACKEND:(Vinicius Penido, Felipe Mareca, Vinícius Assis, Mateus Boleta) FRONTEND:(Guilherme Andrade, João Henrique) O grupo do Backend será responsável pela lógica do programa e pelo banco de dados, já o grupo Frontend será responsável pela parte visual do programa.


## *TODOS:

- ~~Pitch de apresentação
- ~~Construção do readme.md
- Implementar Arquivos
- Implementar cadastro
- Implementar login
- Implementar lista de assentos
- Implementar interface de login
- Implementar interface de cadastro
- Implementar interface de assentos
- Implementar mudança de cor de acordo com disponibilidade
