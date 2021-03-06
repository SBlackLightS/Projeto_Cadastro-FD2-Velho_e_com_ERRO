/**
 * @file CadastroCelulares_1.c
 * @author Matheus Hirata Vanzela (matheusvanzela@alunos.utfpr.edu.br)
 *         Vitor Hugo L. A. de Oliveira (vitoroliveira.2021@alunos.utfpr.edu.br)
 * @brief Realiza o cadastro dos usuarios
 * @version 1.0
 * @date 2022-06-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "includes_sistema.h"
#include "cadastro_usuarios.h"
#include "struct_usuarios.h"
#include "cripto.h"
#include "interfaces.h"

/*
FORMATO DO ARQUIVO BINARIO DE CADASTROS:
CABECALHO:
INTEIRO quantidade de usuarios
STRUCTs Cadastros usuarios
*/


int verifica_arquivo_cadastros (int* quantidade_usuarios) {
  int tentativas = 0;
  int opcao;
  FILE* arquivo = fopen("arquivo_cadastros.dat", "r+b");

  do
  {
    if (arquivo == NULL)
      tentativas++;

    if (tentativas == 3) {
      printf("Erro em encontrar arquivo com os cadastros.\n");
      printf("Criar novo? (0 - SIM | 1 - NAO) R: ");
      scanf("%d", &opcao);
      setbuf(stdin,NULL);

      if (opcao == 1)
        break;
      else {
        limpa_tela();
        printf("Erro para encontrar e ler arquivo de cadastros!\n");
        printf("Encerando...");
        printf("\n\n");
        exit(1);
      }//else
    }//if tentativas
  } while (tentativas < 3);

  if (arquivo == NULL) {
    arquivo = fopen("arquivo_cadastros.dat","wb");
    fwrite(*quantidade_usuarios, sizeof(int), 1, arquivo);
    fclose(arquivo);
    return *quantidade_usuarios;
  }//caso o arquivo nao exista

  if (arquivo != NULL) {
    fread(*quantidade_usuarios, sizeof(int), 1, arquivo);
    if (*quantidade_usuarios < 0) {
      //MENSAGEM DE ERRO, BIPS, CORES ***********************************************************
      printf("Erro na leitura da quantidade de cadastros!!");
      printf("Encerando...");
      printf("\n\n");
      fclose(arquivo);
      exit(1);
    }//caso a quantidade venha como lixo ou negativa

    fclose(arquivo);
    return *quantidade_usuarios;
  }//caso o arquivo exista

}//verifica_arquivo_cadastros

Cadastros* realoca_usuarios (Cadastros* usuarios, int* quantidade_usuarios) {
  int quantidade_realocada;

  quantidade_realocada = (*quantidade_usuarios / 5) * 5 + 5;
  /*ex: quantidade_usuarios = 13
  quantidade_usuarios % 5 = 13 / 5 = 2 (divisao inteira)
  quantidade_realocada = 2 * 5 + 5 = 10 + 5 = 15
  analogamente para quantidade usuarios = n
  */

  usuarios = (Cadastros*) realloc(usuarios, quantidade_realocada*sizeof(Cadastros));

  return;
}//realoca_usuarios

int encontra_posicao_vazia (Cadastros* usuarios, int* quantidade_usuarios) {
  int existe_vazio = 0;
  int posicao_vazia;

  if (*quantidade_usuarios == 0) {
    usuarios = realoca_usuarios(usuarios,quantidade_usuarios);
  }//if (nao existem cadastros)

  for (int pos = 0; pos < *quantidade_usuarios; pos++) {
    if (usuarios[pos].chave != 0)
      continue;

    if (usuarios[pos].chave == 0) {
      existe_vazio = 1;
      posicao_vazia = pos;
      break;
    }//achou posicao vazia
  }//for acha posicoes vazias

  if (existe_vazio == 1)
    return posicao_vazia;
  else
    return -1;

}//encontra_posicao_vazia

void cadastro_usuarios(Cadastros* usuarios, int* quantidade_usuarios)
{
  /* Declaracoes: ----------------------------- */
  int posicao_cadastro = -1;
  int erros = 0;
  int confirmacao;
  FILE* arquivo;
  /* Declaracoes ============================== */

  limpa_tela();

  *quantidade_usuarios = verifica_arquivo_cadastros (quantidade_usuarios);

  do {
    posicao_cadastro = encontra_posicao_vazia(usuarios, quantidade_usuarios);

    if (posicao_cadastro == -1) {
      usuarios = realoca_usuarios(usuarios,quantidade_usuarios);
      erros++;
    }//if

    if (erros > 3) {
      //MENSAGEM DE ERRO, BIPS, CORES ***********************************************************
      printf("Erro para realocar memoria para novos cadastros!!");
      printf("\n\n");
      exit(1);
    }//if

  } while (posicao_cadastro == -1);

  //CADASTRAR NOVO USUARIO
  do {
    printf("CADASTRAR NOVO USUARIO\n\n");

    //Recebe o nome
    printf("Nome(%d): ", TAM_NOME - 2);
    fgets(usuarios[posicao_cadastro].nome_usuario, TAM_NOME, stdin);
    usuarios[posicao_cadastro].nome_usuario[ strcspn( usuarios[posicao_cadastro].nome_usuario ,"\n") ] = '\0';
    setbuf(stdin,NULL);

    //Recebe a senha
    printf("Senha(%d): ", PS_USER - 2);
    fgets(usuarios[posicao_cadastro].senha_usuario, TAM_NOME, stdin);
    usuarios[posicao_cadastro].senha_usuario[ strcspn( usuarios[posicao_cadastro].senha_usuario ,"\n") ] = '\0';
    setbuf(stdin,NULL);

    //Recebe a cifra de cesar para criptografia
    printf("Chave deve estar entre -64 e 5.\n");
    printf("Chave: ");
    scanf("%d", &usuarios[posicao_cadastro].chave);
    setbuf(stdin,NULL);

    //Confirmar dados
    printf("Nome: %s\n",usuarios[posicao_cadastro].nome_usuario);
    printf("Senha: %s\n",usuarios[posicao_cadastro].senha_usuario);
    printf("Chave: %d\n",usuarios[posicao_cadastro].chave);
    printf("Confirma (0 - NAO | 1 - SIM): ");
    scanf("%d", &confirmacao);

    printf("\n\ncriptografando");

    //escrever no arquivo, abrir a+b
    arquivo = fopen ("arquivo_cadastros.dat", "ab");

    if (arquivo == NULL) {
      printf("Erro ao encontrar arquivo com cadastros!\n");
      exit(1);
    }//if

    criptografa(usuarios[posicao_cadastro].senha_usuario, usuarios[posicao_cadastro].chave);

    //escreve no arquivo (cadastra)
    fwrite((usuarios+posicao_cadastro), sizeof(Cadastros), 1, arquivo);

    for (int delay = 0; delay <= 600; delay += 200) {
      //delay 200
      printf(".");
    }
    if (confirmacao != 1)
      limpa_tela();
    
  } while(confirmacao != 1);


  printf("\n\n");
  printf("CADASTRADO! ");
  for (int delay = 0; delay <= 600; delay += 200){
    printf(".");
    //sleep(delay);
  }//animacao carregando

  limpa_tela();

  return;
}//cadastro_usuarios