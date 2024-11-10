#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Estrutura para armazenar os dados do usuario
struct Usuario {
    char nome[100];
    char senha[100];
};

// Funcao de criptografia (simetrica)
void criptografar(char *texto, const char chave) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = texto[i] ^ chave;  // XOR com a chave
    }
}

// Funcao de descriptografia XOR (simetrica)
void descriptografar(char *texto, const char chave) {
    criptografar(texto, chave);  // XOR e simetrico, a mesma funcao serve para descriptografar
}

// Funcao para incluir um novo usuario
void incluirUsuario(const char *nome, const char *senha) {
    FILE *arq = fopen("teste01.txt", "a");
    if (!arq) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    struct Usuario novoUsuario;
    strcpy(novoUsuario.nome, nome);
    strcpy(novoUsuario.senha, senha);

    // Criptografa a senha antes de salvar
    const char chave = 'K';  // Exemplo de chave
    criptografar(novoUsuario.senha, chave);

    // Grava no arquivo
    fprintf(arq, "%s,%s\n", novoUsuario.nome, novoUsuario.senha);
    fclose(arq);
}

// Funcao para listar os usuarios com senha criptografada e descriptografada
void listarUsuarios() {
    FILE *arq = fopen("teste01.txt", "r");
    if (!arq) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    char linha[300];
    const char chave = 'K';  // A mesma chave usada na criptografia

    while (fgets(linha, sizeof(linha), arq)) {
        struct Usuario usuario;
        sscanf(linha, "%99[^,],%99[^\n]", usuario.nome, usuario.senha);

        // Descriptografa a senha
        char senhaDescriptografada[100];
        strcpy(senhaDescriptografada, usuario.senha);
        descriptografar(senhaDescriptografada, chave);

        printf("Nome: %s, Senha (Criptografada): %s, Senha (Descriptografada): %s\n",
               usuario.nome, usuario.senha, senhaDescriptografada);
    }

    fclose(arq);
}

// Funcao para excluir um usuario
void excluirUsuario(const char *nome) {
    FILE *arq = fopen("teste01.txt", "r");
    if (!arq) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(arq);
        return;
    }

    char linha[300];
    int encontrado = 0;
    while (fgets(linha, sizeof(linha), arq)) {
        struct Usuario usuario;
        sscanf(linha, "%99[^,],%99[^\n]", usuario.nome, usuario.senha);

        if (strcmp(usuario.nome, nome) != 0) {
            fprintf(temp, "%s", linha);
        } else {
            encontrado = 1;
        }
    }

    fclose(arq);
    fclose(temp);

    // Substitui o arquivo original pelo temporario
    if (encontrado) {
        remove("teste01.txt");
        rename("temp.txt", "teste01.txt");
        printf("Usuario excluido com sucesso!\n");
    } else {
        remove("temp.txt");
        printf("Usuario nao encontrado!\n");
    }
}

// Funcao principal com o menu interativo
int main() {
    int opcao;
    char nome[100], senha[100];
    while (1) {
        printf("\nMenu de Opcoes:\n");
        printf("1. Incluir usuario\n");
        printf("2. Excluir usuario\n");
        printf("3. Listar usuarios\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\nInforme o nome do usuario: ");
                scanf("%s", nome);
                printf("Informe a senha: ");
                scanf("%s", senha);
                incluirUsuario(nome, senha);
                break;
            case 2:
                printf("\nInforme o nome do usuario a ser excluido: ");
                scanf("%s", nome);
                excluirUsuario(nome);
                break;
            case 3:
                listarUsuarios();
                break;
            case 4:
                printf("Saindo do sistema...\n");
                return 0;
            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}

