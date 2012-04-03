/** @file file.c
 *  Implementacao das funcoes que lidam com arquivos (por exemplo, salvar).
 *
 *  "$Id: $"
 *
 * <h2> Cheque a Pagina Principal para informacoes sobre prefixos de funcoes </h2>
 */

#include <stdio.h>
#include <string.h>

#include "network.h"
#include "arguments.h"
#include "file.h"
#include "text.h"


FILE* filep;


/** A logica de salvar o arquivo.
 *
 *  Localiza o conteudo dentro da resposta recebida e chama outra funcao
 *  para grava-lo num arquivo.
 */
void fil_save()
{
  char *content;

  content = tex_search_for("\r\n\r\n", data.answer);
  if(*content == '\0')
    arg_abort_exec("Host error: No file at specified location.");

  write_to_file(content);
}



/** Efetivamente grava no arquivo o corpo da resposta HTTP.
 *
 *  Aqui que vamos ver se escreve por cima ou nao do arquivo local.
 */
void write_to_file(char *content)
{
  FILE *file;
  int   ch;
  int   file_length;
  int   i = 0;


  file = fopen(data.local_file_name, "r");
  if (file != NULL)
  {
    fclose(file);
    if (options.overwrite_file == NO)
      arg_abort_exec("Error! File already exists! Use -f to overwrite.");
  }

  file = fopen(data.local_file_name, "w");
  if (file == NULL)
    arg_abort_exec_errno("Error at fopen()");

  file_length = strlen(content);

  while(i < file_length)
  {
    ch = content[i];
    fputc(ch, file);
    i++;
  }
  fputc(EOF, file);

  fclose(file);


  printf("Finished writing local file!\n");
  if (options.verbose == YES)
    printf("%d bytes written to file.\n", i);
}


int file_start_saving()
{
  filep = fopen(data.local_file_name, "r");
  if (filep != NULL)
  {
    fclose(filep);
    if (options.overwrite_file == NO)
      arg_abort_exec("Error! File already exists! Use -f to overwrite.");
  }

  filep = fopen(data.local_file_name, "w");
  if (filep == NULL)
    arg_abort_exec_errno("Error at fopen()");

  return 0;
}

int file_keep_saving(char* content, int size)
{
  int   total = size;
  int   saved = 0;
  char* data  = content;
  int   returnval;

  if (content == NULL)
    return -1

  while (saved < total)
  {
    returnval = fprintf(filep, "%s", data);
    if (returnval == -1)
      return -1;
    if (returnval == 0)
      return 1;

    saved += returnval;
    data  += returnval;
  }

  return 0;
}

int file_stop_saving()
{
  fclose(filep);
  return 0;
}

