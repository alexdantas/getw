/** @file text.h
 *  @brief Declaracao das funcoes que lidam com strings e texto.
 *
 *  "$Id: $"
 */


#ifndef TEXT_H
#define TEXT_H


char *tex_build_request(char *method, char *version);
char *tex_build_request_proxy();
void  tex_copy_uri (const char *uri);
void  tex_copy_filename (const char *filename);
void  tex_copy_request(const char *msg);
void  tex_data_init();
void  tex_data_free();
void  tex_get_content_size();
void  tex_increase_answer_size(int add);
void  tex_init_answer();
void  tex_options_init();
void  tex_parse_uri();
char *tex_search_for(char *what, char *where);

void  strncat_beginning(char *dest, char *src, size_t size);


#endif
