typedef struct usuario {
    char email[50];
    char senha[20];
} Usuario;

// Funcoes de cadastro e leitura de usuarios
void cadastrarUsuario(Usuario* usuario); // função para cadastrar usuario
int salvarUsuarioArquivo(char email[50], char senha[20]); // função para salvar o cadastro no arquivoi
void lerUsuarios(); // função para mostrar os usuarios
int validarUsuario(const char email[50], const char senha[20]); // funçao para validar o login
int verificaUsuario(char email[50]); //funcao para verificar se o usuario ja existe pelo email