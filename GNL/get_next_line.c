/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:57:57 by guilherme         #+#    #+#             */
/*   Updated: 2026/07/28 18:41:56 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ARQUIVO: get_next_line.c
**
** OBJETIVO:
**   Implementar a função get_next_line(int fd), que lê e retorna uma linha
**   por vez de um file descriptor (fd). Cada chamada retorna a próxima linha,
**   incluindo o '\n' quando presente. Retorna NULL quando não há mais nada
**   para ler ou em caso de erro.
**
** POR QUE USAR:
**   Em vez de ler um arquivo inteiro de uma vez (o que consumiria muita
**   memória), get_next_line permite processar arquivos linha por linha,
**   sendo eficiente tanto para arquivos pequenos quanto gigantes.
**   Funciona também com stdin (fd = 0).
**
** COMO USAR:
**   #include "get_next_line.h"
**   char *line;
**   int fd = open("arquivo.txt", O_RDONLY);
**   while ((line = get_next_line(fd)) != NULL)
**   {
**       printf("%s", line);
**       free(line);   <- SEMPRE libere a linha após usar
**   }
**   close(fd);
**
** CONCEITO CENTRAL — VARIÁVEL ESTÁTICA:
**   A variável 'over' é estática: ela persiste entre chamadas da função.
**   Isso permite "lembrar" o conteúdo lido além do '\n' para a próxima
**   chamada, sem precisar reler o arquivo desde o início.
*/

#include "get_next_line.h"

/*
** read_and_accumulate:
**   Lê o file descriptor em blocos de BUFFER_SIZE bytes e vai concatenando
**   o resultado em 'over' até que um '\n' seja encontrado ou o arquivo acabe.
**
**   Parâmetros:
**     fd   - file descriptor de onde ler
**     over - string acumulada de chamadas anteriores (pode ser "")
**
**   Retorna:
**     A string acumulada atualizada, ou NULL em caso de erro.
*/
static char	*read_and_accumulate(int fd, char *over)
{
	char	*buffer;
	char	*tmp;
	int		bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(over, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			free(over);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(over, buffer);
		free(over);
		over = tmp;
	}
	free(buffer);
	return (over);
}

/*
** extract_line:
**   Extrai da string 'over' tudo até o '\n' (inclusive quando presente)
**   e retorna como uma nova string alocada — essa é a linha a retornar
**   ao chamador.
**
**   Parâmetros:
**     over - string acumulada contendo ao menos uma linha
**
**   Retorna:
**     A linha extraída, ou NULL se 'over' for vazia/nula.
*/
static char	*extract_line(char *over)
{
	char	*line;
	size_t	len;

	len = 0;
	if (!over || !over[0])
		return (NULL);
	while (over[len] && over[len] != '\n')
		len++;
	if (over[len] == '\n')
		len++;
	line = ft_substr(over, 0, len);
	return (line);
}

/*
** update_over:
**   Após extrair a linha, descarta tudo até o '\n' (inclusive) e guarda
**   apenas o que sobrou depois dele na variável estática, para a próxima
**   chamada de get_next_line.
**
**   Parâmetros:
**     over - string acumulada após a extração da linha
**
**   Retorna:
**     Nova string com o restante após o '\n', ou NULL se não sobrou nada.
*/
static char	*update_over(char *over)
{
	char	*new_over;
	size_t	i;

	i = 0;
	while (over[i] && over[i] != '\n')
		i++;
	if (!over[i])
	{
		free(over);
		return (NULL);
	}
	new_over = ft_substr(over, i + 1, ft_strlen(over) - i - 1);
	free(over);
	if (new_over && !new_over[0])
	{
		free(new_over);
		return (NULL);
	}
	return (new_over);
}

/*
** get_next_line:
**   Função principal. A cada chamada, retorna a próxima linha do fd.
**   Usa a variável estática 'over' para guardar o que foi lido além
**   do '\n' entre chamadas consecutivas.
**
**   Parâmetros:
**     fd - file descriptor (arquivo ou stdin)
**
**   Retorna:
**     A linha lida (incluindo '\n' quando presente), ou NULL no fim
**     do arquivo ou em caso de erro.
*/
char	*get_next_line(int fd)
{
	static char	*over;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!over)
	{
		over = malloc(1);
		if (!over)
			return (NULL);
		over[0] = '\0';
	}
	over = read_and_accumulate(fd, over);
	if (!over)
		return (NULL);
	line = extract_line(over);
	over = update_over(over);
	return (line);
}
