/** @file arguments.c
 *  @brief Arquivo relacionado a implementacao das funcoes que lidam com
 *         argumentos.
 *
 *  "$Id: $"
 *
 * <h2> Cheque a Pagina Principal para informacoes sobre prefixos de funcoes </h2>
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "arguments.h"
#include "network.h"
#include "text.h"


/** As opcoes do programa - modo verboso, sobrescrever arquivo... */
struct options_t options;



/** Aborta a execucao do programa, exibindo uma mensagem de erro.
 */
void arg_abort_exec(char *error_msg)
{
  printf("\t");
  printf(error_msg);
  printf("\n");
  exit(EXIT_FAILURE);
}



/** Aborta a execucao do programa, imprime informacoes da variavel errno e
 *  exibe uma mensagem de erro customizavel.
 */
void arg_abort_exec_errno(char *error_msg)
{
  perror(error_msg);
  exit(EXIT_FAILURE);
}



/** Lida com todos os argumentos passados atraves da linha de comando.
 *
 *  O principal aqui e' que essa funcao precisa lidar de formas diferentes
 *  com os argumentos de comando (que comecam com '-') e com os argumentos
 *  normais (que sao a URI e o nome do arquivo destino).
 *  @see handle_command_args()
 */
void arg_handle(int argc, char *argv[])
{
  int normal_args_handled  = 0;
  int i;


  if (argc < 2)
  {
    print_usage();
    exit(EXIT_FAILURE);
  }

  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
      handle_command_args(argc, argv);
    else
      handle_normal_args(&normal_args_handled, argv[i]);
  }

  if(*data.local_file_name == '\0')
    arg_abort_exec("Error! No local file name specified! Use -u for options.");
}



/** Lida com os argumentos de comando (-v, -f, -h, dentre outros).
 *
 *  Argumentos de comando comecam com '-'. Eles mostram informacoes
 *  e alteram o estado do programa
 */
void handle_command_args(int argc, char* argv[])
{
  int c;
  extern int optind;
  extern int optopt;
  extern int opterr;


  opterr = 0;
  c = getopt(argc, argv, "hfuvV");

  switch (c)
  {
  case 'h':
    print_help();
    exit(EXIT_SUCCESS);
    break;
  case 'f':
    options.overwrite_file = YES;
    break;
  case 'u':
    print_usage();
    exit(EXIT_SUCCESS);
    break;
  case 'v':
    options.verbose = YES;
    break;
  case 'V':
    options.extra_verbose = YES;
    options.verbose = YES;
    break;
  case '?':
    print_error_arg(optopt);
    exit(EXIT_SUCCESS);
    break;
  default:
    arg_abort_exec("Error! Unexpected argument!");
    break;
  }

}



/** Lida com os argumentos normais (uri e nome do arquivo destino).
 *
 *  @note Caso sejam necessarios mais argumentos normais, a funcao
 *        ja esta' modularizada.
 */
void handle_normal_args(int *count, char *arg)
{
  switch (*count)
  {
  case 0:
    (*count)++;
    tex_copy_uri(arg);
    break;
  case 1:
    (*count)++;
    tex_copy_filename(arg);
    break;
  default:
    break;
  }
}



/** Imprime os modos de uso do programa: argumentos validos, ordem especifica...
 */
void print_usage()
{
  printf(PACKAGE);
  printf(" v");
  printf(VERSION);
  printf("\t(");
  printf(DATE);
  printf(")\n\n");
  printf("Usage:\n");
  printf("\tgetw [options] (uri/to/remote_filename) (local_filename)\n");
  printf("\n");
  printf("uri\t\tprotocol://host.domain/file/path\n");
  printf("options\t\t-v\tVerbose mode\n");
  printf("options\t\t-V\tExtra verbose mode\n");
  printf("\t\t-h\tDisplays the Help\n");
  printf("\t\t-u\tDisplays the Usage\n");
  printf("\n");
  printf("\tNote: The only supported protocol so far is HTTP. \n");
  printf("\n");
}



/** Imprime os topicos de ajuda rapidos.
 */
void print_help()
{
  printf("getw Help\n\n");
  printf("getw is an utility to fetch files from the internet through\n");
  printf("the HTTP protocol.\n");
  printf("First, you specify the URI to the remote file. Then, the\n");
  printf("relative path to the local file where it will be saved.\n");
  printf("\tExample:\n\n");
  printf("\t\tgetw [http://]www.google.com/index.html [index.html]\n\n");
  printf("\tNote: Since the only supported protocol is HTTP itself, the\n");
  printf("\t      'http://' is optional.\n\n");
  printf("\tNote: The second argument is optional. If it is not specified,\n");
  printf("\t      by default getw saves it on the current folder, with the\n");
  printf("\t      same name it has on the host.\n\n");
  printf("Use -u for usage informations\n");
  printf("\n");
}



/** Imprime erro caso o usuario envie um parametro desconhecido.
 */
void print_error_arg(int arg)
{
  printf("getw Error!\n");
  printf("\tUnrecognized parameter -%c!\n", arg);
  printf("\n");
  printf("Use -u for Usage informations or -h for Help\n");
  printf("\n");
}
