/**
 * @file CadastroCelulares_1.c
 * @author Matheus Hirata Vanzela (matheusvanzela@alunos.utfpr.edu.br)
 *         Vitor Hugo L. A. de Oliveira (vitoroliveira.2021@alunos.utfpr.edu.br)
 * @brief Projeto de cadastro de marcas e celulares
 * @version 1.0
 * @date 2022-06-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include "interfaces.h"

void limpaTela () {
  #ifdef _WIN32
    system("cls");
  #elif __linux__
    system("clear");
  #else
  #endif
}//limpaTela