# Projeto de Reserva de Assentos em Clang

## Autores, descrição e requisitos

Este é um projeto desenvolvido pelos alunos Vinicius Penido, Vinicius Assis, João Henrique, Matheus Boletta, Guilherme Andrade e Felipe Mareca, com o objetivo de criar um sistema de reserva de assentos para sessões de filmes. 
O projeto é dividido em duas partes principais: a interface e o backend. O SQLite é utilizado para armazenar os dados das sessões e usuários, enquanto uma matriz é utilizada para representar a disposição dos assentos na sala. 
Além disso, uma fila é implementada para armazenar os assentos escolhidos pelos usuários.

## Estrutura de Pastas

O projeto está organizado em três pastas principais:

1. `src`: Esta pasta contém o arquivo principal do projeto, chamado `main.c`. É neste arquivo que a função `main` é definida e a execução do programa é iniciada.

2. `lib`: Esta pasta contém as funções auxiliares necessárias para o funcionamento do programa. Essas funções podem ser divididas em arquivos separados para facilitar a manutenção e modularidade do código.

3. `include`: Esta pasta contém os arquivos de cabeçalho (`.h`) que definem os tipos e protótipos das funções utilizadas no projeto. Esses arquivos de cabeçalho devem ser incluídos nos arquivos que fazem uso das funções correspondentes.

## Dependências

O projeto faz uso das seguintes dependências:

- Clang: O projeto é desenvolvido em Clang, um compilador de código C. Certifique-se de ter o Clang instalado em seu sistema antes de compilar e executar o programa.
- Ncurses: O projeto utiliza da biblioteca Ncurses para desenvolver as interfaces visuais da aplicação.
- SQLite: O projeto utiliza o SQLite como banco de dados para armazenar informações sobre as sessões e usuários. Você precisará ter a biblioteca do SQLite instalada e configurada corretamente em seu ambiente.

## Compilação e Execução

Para compilar o projeto, siga as instruções abaixo:

1. Certifique-se de ter o Clang instalado em seu sistema.

2. Navegue até a pasta raiz do projeto no terminal.

3. Execute o seguinte comando para compilar o projeto:

   ```bash
   clang src/main.c lib/*.c -o cinema -lsqlite3
   ```
  
4. Após a compilação bem-sucedida, você pode executar o programa com o seguinte comando:

    ```bash
   ./cinema
    ```
## Funcionamento do Programa

Ao executar o programa, você será apresentado à interface de reserva de assentos. O programa fornecerá as seguintes opções:

1. **Fazer seu cadastro e login:** Você deverá inserir um usuário único e uma senha, estes dados serão utilizados para logar em sua conta. Logue-se para prosseguir.

2. **Escolher a sessão do filme:** Você poderá selecionar a sessão desejada a partir de uma lista de sessões disponíveis.

3. **Escolher um assento:** Será exibida uma matriz representando a disposição dos assentos na sala de cinema. Você poderá escolher um assento disponível a partir desta matriz.

4. **Visualizar pedido:** Será exibido um resumo do seu pedido, mostrando a sessão escolhida, o assento selecionado e o valor total.

5. **Finalizar reserva:** A reserva do assento será concluída, o assento será marcado como ocupado na matriz e as informações serão armazenadas no banco de dados SQLite.

Você poderá interagir com o programa selecionando as opções correspondentes e seguindo as instruções exibidas na interface. O programa garantirá que os assentos escolhidos estejam disponíveis e fornecerá um resumo do pedido antes de finalizá-lo.

Após a finalização da reserva, você poderá visualizar os assentos ocupados na matriz e acessar as informações das reservas no banco de dados SQLite, para fins de gerenciamento e acompanhamento das reservas realizadas.

Divirta-se usando o sistema de reserva de assentos para sessões de filmes implementado pelos alunos!

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
