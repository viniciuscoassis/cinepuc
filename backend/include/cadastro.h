typedef struct usuario {
    char email[50];
    char senha[20];
} Usuario;

// Funcoes de cadastro e leitura de usuarios
void cadastrarUsuario(Usuario* usuario);
void salvarUsuarioArquivo(Usuario usuario);
void lerUsuarios();
int validarUsuario(const char email[50], const char senha[20]);