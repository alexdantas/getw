/** @file arguments.h
 *  @brief Definicao das funcoes que lidam com argumentos.
 *
 *  "$Id: $"
 */


#ifndef ARGS_H
#define ARGS_H


/** Estrutura que contem as opcoes globais do programa.
 */
struct options_t
{
  /** Caso ja exista um arquivo local com o mesmo nome especificado, essa flag
   *  vai determinar se vamos gravar por cima desse arquivo ou nao.
   */
  int overwrite_file;

  /** Flag para imprimir mais informacoes ao executar o programa.
   */
  int verbose;

  /** Flag para imprimir todas as informacoes ao executar o programa.
   *  Como se fosse um modo debug.
   */
  int extra_verbose;
};

/** Enum simples para facilitar o tratamento de opcoes */
enum {NO=0, YES};

/** Opcoes globais */
extern struct options_t options;


void arg_abort_exec(char *error_msg);
void arg_abort_exec_errno(char *error_msg);
void arg_handle(int argc, char *argv[]);

void handle_command_args(int argc, char* argv[]);
void handle_normal_args(int *count, char *arg);
void print_usage();
void print_help();
void print_error_arg(int arg);

#endif
