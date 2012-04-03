/** @file main.c
 *  Contem a funcao 'main' e a logica principal do programa.
 *
 *  "$Id: $"
 * <h2> Cheque a Pagina Principal para informacoes sobre prefixos de funcoes </h2>
 */

#include "file.h"
#include "arguments.h"
#include "network.h"
#include "text.h"

/*! A logica principal do programa.
 *
 *  Aqui esta apenas descrita a lo'gica do programa. As funcoes que
 *  fazem tudo estao distribuidas pelos modulos.
 *  \see arg_handle_args()
 *  \see net_communicate()
 *
 */
int main(int argc, char *argv[])
{
  tex_data_init();
  tex_options_init();

  arg_handle(argc, argv);

  net_communicate();

  fil_save();

  tex_data_free();

  return 0;
}
