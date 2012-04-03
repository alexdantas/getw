/** @mainpage Usando getw
*
* @section intro Introdução
* <hr />
*
* Esse pacote recupera páginas HTML através do protocolo HTML. @n
*
* -# Recebe como parametros a URI da pagina e o nome do arquivo local onde
*    vamos salvar essa pagina. Esses dois parametros sao obrigatorios. <br />
* <br />
* -# Como parametros opcionais, pode receber: <br /> <br />
*   - <b>-u</b>    Mostrar modo de uso do programa. Sai imediatamente.
*   - <b>-h</b>    Mosdtrar ajuda do programa. Sai imediatamente.
*   - <b>-v</b>    Modo verboso. Continua a execucao normalmente.
*   - <b>-V</b>    Modo extra verboso. Continua a execucao normalmente.
*   - <b>-f</b>    Sobrescreve o arquivo local, caso exista um com o mesmo nome.
*
* @section Importante
* <hr />
*
*  Todas as funcoes que servirem de interface exterior ao modulo
*  contem o prefixo respectivo do modulo que pertence.
*
*  <table border="1" cellpadding="10">
*  <caption> Prefixos </caption>
*    <tr><td>  Modulo arguments  </td><td>  arg_  </td></tr>
*    <tr><td>  Modulo file       </td><td>  fil_  </td></tr>
*    <tr><td>  Modulo network    </td><td>  net_  </td></tr>
*    <tr><td>  Modulo text       </td><td>  tex_  </td></tr>
*  </table>
*  <br />
*  As funcoes que nao possuirem esses prefixos sao especificas do
*  modulos e nunca sao chamadas por outros modulos.
*
* @section ChangeLog
* <hr />

<br />
19/12/2011

  - Correcao do erro final: recv() agora passa a ser chamado multiplas vezes.
  - Finalizacao do programa. Versao 1.0 lancada.

--------------------------------------------------------------------------------
<br />
16/12/2011

  - Integracao entre os modulos, documentacao e correcao de erros resultantes.

--------------------------------------------------------------------------------
<br />
15/12/2011

  - Criação e finalizacao do modulo file: lida com operacoes de arquivos.

--------------------------------------------------------------------------------
<br />
14/12/2011

  - Renomear modulos: args para arguments, http para network.

--------------------------------------------------------------------------------
<br />
13/12/2011

  - Adequacao do codigo a padroes da Aker: completo.
  - Criacao do modulo text, para gerenciar strings.
  - Finalizacao do modulo http.

--------------------------------------------------------------------------------
<br />
12/12/2011

  - Parsing da string recebida pela linha de comando: Separacao entre nome
                                                      do host e nome do arquivo
                                                      remoto.
  - Correcao de pequenos bugs.
  - Documentacao do codigo.

--------------------------------------------------------------------------------
<br />
08/12/2011

  - Conexao com host feita: Sockets prontos.

--------------------------------------------------------------------------------
<br />
06/12/2011

  - Criação dos módulos main e args.
  - Modulo args completo: Agora receber argumentos da linha de comando funciona.

--------------------------------------------------------------------------------
<br />
05/12/2011  Alexandre Dantas <alex.dantas92@gmail.com>

  - Inicio do projeto

--------------------------------------------------------------------------------


*
*
*
*
*
*
*
*
*
*
*
*
* @authors Alexandre Dantas
*/
