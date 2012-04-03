/** @file network.h
 *  @brief Declaracao das funcoes relacionadas 'a redes.
 *
 *  "$Id: $"
 */

#ifndef HTTP_H
#define HTTP_H


#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>


#define BUFFER_SIZE 256

#define DEFAULT_PORT  80


/** Contem os dados usados pelos sockets.
 *
 *  Alem da URI e do nome do arquivo, essa estrutura possui um buffer
 *  pra enviar a mensagem HTTP e receber a resposta.
 */
struct data_t
{
/** Contem o endereco URI recebido pela linha de comando */
  char *uri_name;

/** O tamanho do endereco URI */
  int   uri_size;

/** Contem o nome do arquivo no qual sera' salva a resposta, obtida pela linha de comando */
  char *local_file_name;

/** O nome do host, extraido do uri_name */
  char *host_name;

/** O nome do arquivo que se quer baixar, extraido do uri_name */
  char *remote_file_name;

/** Contem toda a mensagem HTTP enviada para o host. */
  char *request;

/** O tamanho da mensagem HTTP enviada para o host. */
  int   request_size;

/** Contem toda a resposta HTTP recebida pelo host. */
  char *answer;

/** O tamanho da resposta HTTP recebida pelo host. */
  int   answer_size;

/** O numero da porta onde sera' feita a conexao */
  int   port;

};


/** A estancia u'nica e global dos dados */
extern struct data_t data;


void  net_communicate ();

int   create_talking_socket();
void  establish_connection(int *socket, struct sockaddr_in *address);
void  establish_socket_address (struct sockaddr_in *socket_address);
char *get_host_ip(const char *name);
void  receive_data (int* sock);
void  send_data (int* sock);


#endif
