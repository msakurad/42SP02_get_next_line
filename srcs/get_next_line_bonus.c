/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakurad <msakurad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:58:11 by msakurad          #+#    #+#             */
/*   Updated: 2023/07/17 18:45:53 by msakurad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	storage_to_current_line(char **line, char *buf)
{
	char	*tmp;

	if (!line && !buf)
		tmp = ft_strdup("");
	else if (*line && !buf)
		tmp = ft_strdup(*line);
	else if (!*line && buf)
		tmp = ft_strdup(buf);
	else
		tmp = ft_strjoin(*line, buf);
	free(*line);
	*line = ft_strdup(tmp);
	free(tmp);
}

static int	scan_and_check_new_line(int fd, char **line)
{
	ssize_t	bytes_read;
	char	*buf;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (-1);
	while (1)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buf[bytes_read] = '\0';
		if (!line)
			*line = ft_strdup("");
		storage_to_current_line(line, buf);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	free(buf);
	return (bytes_read);
}

static char	*split_to_next_and_bckup(char **line)
{
	char	*next_line;
	char	*tmp;
	char	*pos_new_line;

	pos_new_line = ft_strchr(*line, '\n');
	if (pos_new_line)
	{
		next_line = ft_substr(*line, 0, pos_new_line - *line + 1);
		tmp = ft_strdup(pos_new_line + 1);
		free(*line);
		*line = ft_strdup(tmp);
		free(tmp);
	}
	else
		next_line = ft_strdup(*line);
	if (!pos_new_line || !**line)
	{
		free(*line);
		*line = NULL;
	}
	return (next_line);
}

char	*get_next_line(int fd)
{
	int			bytes_read;
	static char	*bckup_line[1024];
	char		*next_line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	bytes_read = scan_and_check_new_line(fd, &bckup_line[fd]);
	if (bytes_read < 0 || (!bytes_read && !bckup_line[fd]))
		return (NULL);
	next_line = split_to_next_and_bckup(&bckup_line[fd]);
	return (next_line);
}
