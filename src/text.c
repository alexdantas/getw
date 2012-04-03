/** @file text.c
 *  @brief Definicao das funcoes que lidam com strings e texto.
 *
 *  "$Id: $"
 *
 * <h2> Cheque a Pagina Principal para informacoes sobre prefixos de funcoes </h2>
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "network.h"
#include "text.h"


/** Constroi e retorna a mensagem HTTP que sera' enviada para o host.
 *
 *  @note HTTP 1.0 nao lida muito bem com URIs relativas. @n
 *        Por isso eu mandei na mensagem a URI inteira, inclusive com o host
 *        name. @n
 *        Fonte: http://www2.themanualpage.org/http/http_http10.php3 @n
 *               No finalzinho, na parte 'Restrictions'.
 */
char *tex_build_request(char *method, char *version)
{
  int   remaining_size;
  char *msg       = NULL;
  char *usr_agent = NULL;


  usr_agent = malloc(BUFFER_SIZE);
  if (usr_agent == NULL)
    arg_abort_exec("Memory Error!");

  sprintf(usr_agent, "User-Agent: %s/%s\r\n", PACKAGE, VERSION);
  // both PACKAGE and VERSION are defined from commandline

  msg = malloc(BUFFER_SIZE);
  if (msg == NULL)
    arg_abort_exec("Memory Error!");

  remaining_size = BUFFER_SIZE;

  sprintf(msg, "%s %s %s\r\n", method, data.uri_name, version);
  remaining_size -= strlen(msg);

  strncat(msg, usr_agent, remaining_size);
  remaining_size -= strlen(usr_agent);

  strncat(msg, "\r\n", remaining_size);
  remaining_size -= strlen("\r\n");

  msg[BUFFER_SIZE - remaining_size] = '\0';

  free (usr_agent);

  return msg;
}



/** Grava a URI especificada pela linha de comando na struct data_t.
 *
 *  @note Se a string do caminho uri for maior que a string na struct
 *        (data.uri_name), copia-se ate' onde se puder e ignora-se o resto.
 */
void tex_copy_uri (const char* uri)
{
  strncpy(data.uri_name, uri, BUFFER_SIZE);
  data.uri_name[strlen(uri)] = '\0';

  data.uri_size = strlen(data.uri_name) + 1;
}



/** Grava o nome do arquivo local especificado pela linha de comando
 *  na struct data_t.
 *
 *  O arquivo local vai ser a copia do arquivo que vamos baixar e salvar
 *  no caminho especificado pelo parametro 'filename'.
 *  Fica a cargo do modulo 'file' estabelecer a validade do caminho desse
 *  arquivo - exemplo, se o usuario tem permissoes pra escrever, se o
 *  caminho nao existe, se o arquivo ja existe...
 *
 *  @note Se a string do nome do arquivo (filename) for maior que a
 *  na struct data_t (data.local_file_name), copia-se ate' onde puder
 *  e ignora-se o resto.
 */
void tex_copy_filename(const char* filename)
{
  strncpy(data.local_file_name, filename, BUFFER_SIZE);
  data.local_file_name[strlen(filename)] = '\0';
}



/** Grava na struct data_t a mensagem HTTP que vamos mandar ao host.
 *
 *  @note Se a string da mensagem for maior que a string na struct data_t,
 *        copia-se ate' onde se puder e ignora-se o resto.
 */
void tex_copy_request(const char* msg)
{
  data.request = malloc(strlen(msg) + 1);
  if (data.request == NULL)
    arg_abort_exec("Memory Error!");

  strncpy(data.request, msg, strlen(msg));
  data.request[strlen(msg)] = '\0';
  data.request_size = strlen(data.request);
}



/** Aloca a memoria e seta os valores iniciais de toda a struct data_t.
 *
 *  @note Preferi realloc() em vez de malloc() porque ele ja' seta
 *        os valores iniciais como '\0' de toda a string.
 */
void tex_data_init()
{
  data.host_name = realloc(data.host_name, BUFFER_SIZE);
  if (data.host_name == NULL)
    arg_abort_exec("Memory error!");

  data.local_file_name = realloc(data.local_file_name, BUFFER_SIZE);
  if (data.local_file_name == NULL)
    arg_abort_exec("Memory error!");

  data.remote_file_name = realloc(data.remote_file_name, BUFFER_SIZE);
  if (data.remote_file_name == NULL)
    arg_abort_exec("Memory error!");

  data.uri_name = realloc(data.uri_name, BUFFER_SIZE);
  if (data.uri_name == NULL)
    arg_abort_exec("Memory error!");

  data.answer = NULL;
  data.answer_size = 0;
  data.request = NULL;
  data.uri_size = 0;
  data.port = DEFAULT_PORT;
}



/** Libera a memoria alocada para a struct data_t.
 */
void tex_data_free()
{
  if (data.host_name != NULL)
    free(data.host_name);

  if (data.local_file_name != NULL)
    free(data.local_file_name);

  if (data.remote_file_name != NULL)
    free(data.remote_file_name);
  if (data.uri_name != NULL)
    free(data.uri_name);

  if (data.request != NULL)
    free(data.request);

  if (data.answer != NULL)
    free(data.answer);
}



/** Aumenta a memoria alocada ao buffer que vai receber a resposta do host.
 *
 *  O buffer comeca com um tamanho definido (BUFFER_SIZE) e, caso
 *  seja necessario, tem o seu tamanho aumentado por essa funcao.
 *
 *  @warning  Essa funcao nao faz nada caso o parametro recebido
 *            seja menor ou igual a zero.
 *            Essa funcao nao lida com o caso de receber numero
 *            negativo por parametro.
 *
 *  Essa funcao nao determina nenhum limite ao tamanho maximo
 *  do buffer. Isso fica a cargo do limite de memoria do computador
 *  do usuario.
 */
void tex_increase_answer_size(const int add)
{
  if (add > 0)
  {
    data.answer = realloc(data.answer, data.answer_size + add);
    if (data.answer == NULL)
      arg_abort_exec("Memory Error!");

    data.answer_size += add;
  }
}



/** Aloca e inicializa o buffer para a mensagem HTTP que recebera como
 *  resposta do host.
 *
 *  Essa resposta contera todo o arquivo que requisitar, portanto deve
 *  possuir alocacao dinamica.
 *  Alem de alocar e zerar a memoria, definimos o seu tamanho atual
 *  atraves da variavel answer_size dentro de data_t.
 */
void tex_init_answer()
{
  data.answer = calloc(BUFFER_SIZE, sizeof(char));
  if (data.answer == NULL)
    arg_abort_exec("Memory Error!");

  data.answer_size = BUFFER_SIZE;
}



/** Seta os valores iniciais de toda a struct options_t.
 */
void tex_options_init()
{
  options.overwrite_file = NO;
  options.verbose = NO;
  options.extra_verbose = NO;
}



/** Efetua parsing completo no endereco URI (nome de arquivo remoto, nome
 *  do host, protocolo e numero da porta).
 *
 *  Ao receber a URI completa, precisamos separar o nome do arquivo remoto
 *  do nome do host (alem de desconsiderar outros protocolos que nao sejam
 *  HTTP). @n
 *  Entao, dividimos a URI em substrings com o uso de strstr().
 *  @note Interrompe a execucao caso o usuario especifique um protocolo
 *        diferente de 'http://'. @n
 *        Se o usuario nao especificar nenhum protocolo, supoe-se http por
 *        padrao.
 */
void tex_parse_uri()
{
  char *file;
  char *host;
  int   host_length;
  char *port;
  int   err;


  // Lidar com o endereco do host
  host = tex_search_for("//", data.uri_name);
  if (host == NULL)
  {
    strncat_beginning(data.uri_name, "http://", BUFFER_SIZE);
    data.uri_size += strlen("http://");

    host = data.uri_name + strlen("http://");
  }
  else
  {
    if (tex_search_for("http", data.uri_name) == NULL)
      arg_abort_exec("Error! Unsupported protocol! Use -u for options.");
  }

  // Lidar com o numero da porta
  port = tex_search_for(":", host);
  if (port != NULL)
  {
    err = sscanf(port, "%d", &(data.port));
    if (err != 1)
    {
      printf("Couldn't read custom port. Using default: %d.\n", DEFAULT_PORT);
      data.port = DEFAULT_PORT;
    }
    port -= 1;  // volta para o ':'

    data.uri_name[strlen(data.uri_name) - strlen(port)] = '\0';
    ///@todo deixar mais claro que estou ignorando todo o resto da string depois do numero da porta
  }

  // Lidar com os nomes dos arquivos
  file = strstr(host, "/");
  if (file == NULL)
  {
    strncpy(data.remote_file_name, "/", strlen("/"));
    data.remote_file_name[strlen("/")] = '\0';

    host_length = strlen(host);
  }
  else
  {
    strncpy(data.remote_file_name, file, BUFFER_SIZE);
    data.remote_file_name[strlen(file)] = '\0';

    host_length = strlen(host) - strlen(file);
  }

  strncpy(data.host_name, host, host_length);
  data.host_name[host_length] = '\0';
}



/* TODO
 * Implementar para futuras versoes.
 * Vou precisar mandar o HEAD antes, pegar Content-Size e chamar
 * essa funcao, imprimindo a porcentagem de download.
 * Usar ncurses?
void tex_print_progress_bar(int total, int remaining)
{
  static int progress = 0;
  int add;

  add = total - remaining;
  progress += add;

  printf("Downloading: %d bytes\n", progress);
}
*/


/** Procura dentro da segunda string pela primeira.
 *
 *  Retorna um ponteiro apontando para imediatamente apos a localidade
 *  da primeira string na segunda.
 *  Caso nao encontre, retorna NULL.
 */
char *tex_search_for(char *what, char *where)
{
  char *pointer;

  pointer = strstr(where, what);
  if (pointer == NULL)
    return NULL;
  else
  {
    pointer += strlen(what);    // ignora o what
    return pointer;
  }
}



/** Adiciona o conteudo de uma string em outra, a partir do comeco.
 *
 *  O tamanho maximo da string final e determinado pelo parametro 'size'.
 *  Caso as duas strings juntas possuam tamanho maior que 'size', copia-se
 *  a primeira string primeiro e o maximo possivel da segunda.
 */
void strncat_beginning(char *dest, char *src, size_t size)
{
  char *temp = NULL;

  temp = calloc(BUFFER_SIZE, sizeof(char));
  if (temp == NULL)
    arg_abort_exec("Memory Error!");

  strncpy(temp, dest, strlen(dest));
  dest = realloc(dest, size);

  strncpy(dest, src, strlen(src));
  dest[strlen(src)] = '\0';

  strncat(dest, temp, size);

  free(temp);
  //como garantir que, agora com 'http://' a url nao esta incompleta?
}
