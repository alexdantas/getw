/** @file network.c
 *  @brief Definicao das funcoes relacionadas 'a redes.
 *
 *  "$Id: $"
 *
 * <h2> Cheque a Pagina Principal para informacoes sobre prefixos de funcoes </h2>
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <sys/types.h>  /* Socket data types                      */
#include <sys/socket.h> /* socket(), connect(), send() e recv()   */
#include <netinet/in.h> /* IP socket data types                   */
#include <arpa/inet.h>  /* sockaddr_in e inet_addr()              */

#include "network.h"
#include "text.h"
#include "arguments.h"


/** A estancia u'nica e global dos dados */
struct data_t data;


/** Cria, envia e armazena a comunicacao entre o cliente e o host.
 *
 *  Essa e a funcao principal desse modulo. @n
 *  Nela estao as chamadas para todas as funcoes secundarias, cada uma
 *  lidando com seus proprios erros, dentro de seus escopos. @n
 *  Pode-se dizer que aqui esta a logica de todo esse mo'dulo.
 *
 *  @see tex_parse_uri()
 *  @see create_talking_socket()
 *  @see establish_socket_address()
 *  @see establish_connection()
 *  @see send_data()
 *  @see receive_data()
 */
void net_communicate ()
{
  int                 local_socket;
  struct sockaddr_in  socket_address;


  tex_parse_uri ();

  local_socket = create_talking_socket();

  establish_socket_address(&socket_address);

  establish_connection(&local_socket, &socket_address);

  send_data(&local_socket);

  receive_data(&local_socket);
}


/** Cria e retorna um socket pronto para se comunicar com o host.
 *
 *  Aqui e' feito o papel do cliente.
 *  Cria-se sockets segundo a implementacao TCP/IP.
 */
int create_talking_socket()
{
  int sock;


  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    arg_abort_exec_errno("Error at socket()");

  if(options.extra_verbose == YES)
    printf("Socket created.\n");

  return sock;
}



/** Cria a conexao entre o cliente e o host.
 */
void establish_connection(int *socket, struct sockaddr_in *address)
{
  int err;

  err = connect(*socket, (struct sockaddr *) address, sizeof(*address));

  if (err == -1)
    arg_abort_exec_errno("Error at connect()");

  printf("Connection successful at port %d\n", data.port);
}



/** Cria e determina um endereco pro socket - com porta padrao e IP do host.
 *
 *  @see get_host_ip()
 *
 *  @note O #define da porta padrao esta em network.h.
 *        Por padrao, a porta para conexoes HTTP e 80. O usuario pode
 *        modificar isso passando uma outra porta pela linha de comando
 *        no estilo 'http://www.host.com/file.html:12', que usa a porta 12.
 */
void establish_socket_address(struct sockaddr_in* socket_address)
{
  char  *host_ip;
  int   i;


  host_ip = get_host_ip(data.host_name);

  socket_address->sin_family = AF_INET;
  socket_address->sin_port = htons(DEFAULT_PORT);
  socket_address->sin_addr.s_addr = inet_addr(host_ip);

/* TODO como lidar com o erro aqui? .s_addr eh unsigned
  if (socket_address->sin_addr.s_addr == -1)
  {
    arg_abort_exec("Error at inet_addr.");
  }
*/

  for (i = 0; i < 8; i++)
    socket_address->sin_zero[i] = '\0';

  if (options.extra_verbose == YES)
    printf("Socket Address established.\n");
}



/** Efetua o papel do DNS, recebendo um nome de host e retornando o IP
 *  do mesmo.
 *
 *  Em caso de erro, interrompe a execucao imediatamente.
 *
 *  @note O retorno esta' em network byte order.
 */
char *get_host_ip(const char *name)
{
  struct hostent  *host;
  char            *host_ip;

  host = gethostbyname(name);
  if (host == NULL)
  {
    herror("Error at gethostbyname()");
    printf("\t\t\t\t\t\"");
    printf(name);
    printf("\"\n");
    exit(EXIT_FAILURE);
  }

  host_ip = inet_ntoa(*(struct in_addr *)host->h_addr_list[0]);

  if(options.verbose == YES)
  {
    printf("Host Name: ");
    printf(name);
    printf("\nHost IP:   ");
    printf(host_ip);
    printf("\n");
  }

  return host_ip;
}



/** Recebe a resposta do host e armazena na estrutura de dados data_t.
 *
 *  Vamos receber a resposta ate' ela acabar, alocando a memoria do buffer
 *  dinamicamente.
 *
 *  @see tex_increase_answer_size();
 */
void receive_data(int* sock)
{
  char header[1000];
  int  is_content = 0;


  while (is_content == 0)
  {


  }

  while ()
  {


  }

  printf("Answer received.\n");

  if (options.verbose == YES)
  {
    printf("%d bytes received.\n", strlen(data.answer));

    if (options.extra_verbose == YES)
    {
      printf("HTTP Answer:\n");
      printf("-------------------------------------------------\n");
      printf(data.answer);
      printf("\n-------------------------------------------------\n");
    }
  }
}

/* Recebe a resposta do host e armazena na estrutura de dados data_t.
 *
 *  Vamos receber a resposta ate' ela acabar, alocando a memoria do buffer
 *  dinamicamente.
 *
 *  @see tex_increase_answer_size();
 *
void receive_data(int* sock)
{
  // answer_pointer permite que anexe pedacos da resposta no buffer.
  // Sem ele, a cada iteracao do loop de recv() iriamos gravar o pedaco
  // atual por cima do anterior.
  char  *answer_pointer;
  int    err = 0;
  int    remaining_size;
  int    progress;


  tex_init_answer();

  answer_pointer = data.answer;
  remaining_size = data.answer_size;
  progress = 0;

  file_start_saving ();

  // Loop principal do recv(), pega um pedaco da resposta por vez
  do {

    err = recv(*sock, answer_pointer, remaining_size, 0);
    if (err == -1)
      arg_abort_exec_errno("Error at recv()");

    answer_pointer += err;
    remaining_size -= err;
    progress += err;

    if (remaining_size < BUFFER_SIZE)
    {
      tex_increase_answer_size(BUFFER_SIZE);
      remaining_size += BUFFER_SIZE;
      answer_pointer = data.answer + (data.answer_size - remaining_size);
    }



    //Agora seria um bom momento pra implementar isso:
    //tex_print_progress_bar(data.answer_size, progress);

  } while ((err != 0) && (remaining_size > 0));


  if (*data.answer == '\0')
    arg_abort_exec("No Answer received.\n");

  if (remaining_size <= 0)
    arg_abort_exec("Error! Buffer Overflow.\n");

  printf("Answer received.\n");

  if (options.verbose == YES)
  {
    printf("%d bytes received.\n", strlen(data.answer));

    if (options.extra_verbose == YES)
    {
      printf("HTTP Answer:\n");
      printf("-------------------------------------------------\n");
      printf(data.answer);
      printf("\n-------------------------------------------------\n");
    }
  }
} */



/** Define e envia para o host a mensagem HTTP requisitando o arquivo.
 *
 *  @see tex_build_request()
 *  @see tex_copy_request()
 *  @see tex_end_request()
 */
void send_data (int* sock)
{
  int   err = 0;
  char *msg = NULL;


  msg = tex_build_request("GET", "HTTP/1.0");
  tex_copy_request(msg);

  err = send(*sock, data.request, data.request_size, 0);
  if (err == -1)
    arg_abort_exec_errno("Error at send()");

  ///@todo Confirmar que todo o request foi mandado

  printf("Request sent.\n");

  //////////////////////////////////////////////LOG(data.request);
  if (options.verbose == YES)
  {
    if (options.extra_verbose == YES)
    {
    printf("HTTP request: \n");
    printf("-------------------------------------------------\n");
    printf(data.request);
    printf("-------------------------------------------------\n");
    }
  }
}
