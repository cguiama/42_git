/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 18:42:17 by guilherme         #+#    #+#             */
/*   Updated: 2026/07/28 18:42:44 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// /*
// ** ARQUIVO DE ESTUDO — NÃO ENTREGAR
// ** Versão comentada linha a linha do get_next_line.c
// */

// #include "get_next_line.h"

// static char	*read_and_accumulate(int fd, char *over)
// {
// 	char *buffer;
// 		// buffer temporário onde o read() vai depositar os bytes lidos
// 	char *tmp;
// 		// variável auxiliar para guardar o resultado do strjoin antes de
// liberar over
// 	int bytes_read; // quantidade de bytes que o read() leu de fato
// 	buffer = malloc(BUFFER_SIZE + 1); // aloca BUFFER_SIZE bytes
// 		+ 1 para o '\0' do fim
// 	if (!buffer)                      // se o malloc falhou, retorna NULL
// 		return (NULL);
// 	bytes_read = 1;
// 		// inicializa com 1 só para entrar no loop (qualquer valor > 0 serve)
// 	while (!ft_strchr(over, '\n') && bytes_read > 0)
// 	// continua lendo enquanto:
// 	// - não encontrou '\n' em over (ft_strchr retorna NULL se não achar)
// 	//
// 		- e o read ainda tem bytes para dar (bytes_read > 0 significa que não
// chegou no EOF)
// 	{
// 		bytes_read = read(fd, buffer, BUFFER_SIZE);
// 		// lê até BUFFER_SIZE bytes do fd e armazena em buffer
// 		// retorna: número de bytes lidos, 0 se EOF, -1 se erro
// 		if (bytes_read < 0) // se houve erro de leitura
// 		{
// 			free(buffer);  // libera o buffer alocado
// 			free(over);    // libera o acumulado até agora
// 			return (NULL); // retorna NULL sinalizando erro
// 		}
// 		buffer[bytes_read] = '\0';
// 		// coloca '\0' na posição logo após os bytes lidos
// 		// isso transforma o buffer em uma string C válida
// 		tmp = ft_strjoin(over, buffer);
// 		// concatena o que já estava acumulado em over com os novos bytes do
// buffer
// 		// o resultado é uma nova string alocada com malloc dentro do strjoin
// 		free(over); // libera a string antiga de over (já não precisamos dela)
// 		over = tmp; // over agora aponta para a nova string concatenada
// 	}
// 	free(buffer);  // libera o buffer temporário (não precisamos mais dele)
// 	return (over);
// 		// retorna o acumulado que contém ao menos um '\n' ou chegou no EOF
// }

// static char	*extract_line(char *over)
// {
// 	char *line; // string que vai ser retornada ao chamador
// 	size_t len; // comprimento da linha até o '\n' (inclusive)
// 	len = 0;
// 	if (!over || !over[0]) // se over é NULL ou string vazia,
// 		não há linha para extrair
// 		return (NULL);
// 	while (over[len] && over[len] != '\n')
// 		len++; // avança len até encontrar o '\n' ou o fim da string
// 	if (over[len] == '\n')
// 		len++; // se encontrou '\n',
// 			inclui ele no tamanho (a linha deve conter o '\n')
// 	line = ft_substr(over, 0, len);
// 	// extrai os primeiros 'len' caracteres de over — essa é a linha completa
// 	// ft_substr aloca uma nova string com malloc
// 	return (line); // retorna a linha extraída
// }

// static char	*update_over(char *over)
// {
// 	char *new_over; // nova string que vai guardar o resto após o '\n'
// 	size_t i;       // índice para percorrer over até o '\n'
// 	i = 0;
// 	while (over[i] && over[i] != '\n')
// 		i++; // avança i até o '\n' ou fim da string
// 	if (!over[i])
// 	// se chegou no fim da string sem encontrar '\n',
// 	// significa que não sobrou nada após a linha extraída
// 	{
// 		free(over);    // libera over, não tem mais conteúdo útil
// 		return (NULL); // retorna NULL: na próxima chamada, over começa do zero
// 	}
// 	new_over = ft_substr(over, i + 1, ft_strlen(over) - i - 1);
// 	// extrai tudo que vem DEPOIS do '\n' (posição i+1 em diante)
// 	// esse é o conteúdo que vai ser reaproveitado na próxima chamada do GNL
// 	free(over); // libera a string antiga de over
// 	if (new_over && !new_over[0])
// 	// se new_over existe mas é uma string vazia (nada sobrou após o '\n')
// 	{
// 		free(new_over); // libera a string vazia
// 		return (NULL);  // retorna NULL: na próxima chamada, over começa do zero
// 	}
// 	return (new_over); // retorna o resto para ser guardado na variável estática
// }

// char	*get_next_line(int fd)
// {
// 	static char *over;
// 	// variável estática: persiste entre chamadas da função
// 	// guarda os bytes lidos além do '\n' para reuso na próxima chamada
// 	// é inicializada como NULL automaticamente pelo compilador na primeira
// chamada
// 	char *line; // a linha que será retornada ao chamador

// 	if (fd < 0 || BUFFER_SIZE <= 0)
// 		// fd inválido (negativo) ou BUFFER_SIZE configurado errado
// 		return (NULL);
// 	if (!over)
// 	// primeira chamada ou after um NULL (over foi zerado): inicializa
// como string vazia
// 	{
// 		over = malloc(1); // aloca 1 byte para guardar só o '\0'
// 		if (!over)        // se malloc falhou
// 			return (NULL);
// 		over[0] = '\0'; // garante que over começa como string vazia válida
// 	}
// 	over = read_and_accumulate(fd, over);
// 	// lê do fd e acumula em over até encontrar '\n' ou EOF
// 	// over é atualizado com o novo conteúdo lido
// 	if (!over) // se houve erro na leitura
// 		return (NULL);
// 	line = extract_line(over);
// 	// extrai a primeira linha de over (até o '\n' inclusive)
// 	over = update_over(over);
// 	// descarta a linha extraída e guarda o resto em over para a próxima chamada
// 	return (line);
// 		// retorna a linha ao chamador — ele é responsável por fazer free()
// }
