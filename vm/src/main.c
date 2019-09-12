/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemian <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 19:13:59 by pdemian           #+#    #+#             */
/*   Updated: 2019/09/11 19:14:00 by pdemian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/corewar.h"

t_list   *read_file(char *path)
{
    t_list	*str;
    t_list	*tmp;
    char	*line;
    int		fd;

    str = NULL;
    line = NULL;
    fd = open(path, O_RDONLY);
    while (get_next_line(fd, &line) > 0)
    {
        tmp = ft_lstnew(line, ft_strlen(line));
        if (!str)
            str = tmp;
        tmp = tmp->next;
    }
    return (str);
}

t_champ *get_i(t_champ *champ, int i)
{
    int j;

    j = 0;
    while (champ)
    {
        if (j == i)
            return (champ);
        j++;
        champ = champ->next_champ;
    }
    return (NULL);
}

t_champ *set_champ_to_pos(t_champ *champ, int from, int to)
{
    t_champ *prev_fr;
    t_champ *t_from;
    t_champ *prev_to;
    t_champ *t_to;
    t_champ *tmp;

    prev_fr = get_i(champ, from - 1);
    t_from = get_i(champ, from);
    prev_to = get_i(champ, to - 1);
    t_to = get_i(champ, from);
    tmp = t_from;
    t_from->next_champ = t_to->next_champ;
    t_to->next_champ = tmp->next_champ;
    if (prev_fr)
        prev_fr->next_champ = t_to;
    if (prev_to)
        prev_to->next_champ = t_from;
    if (!prev_fr)
        return (t_to);
    if (!prev_to)
        return (t_from);
    return (champ);
}

t_champ *sort_champs(t_champ *champ, int argc, char **argv)
{
    int i;
    int pos;
    int curr;
    
    i = 0;
    curr = -1;
    while (++i < argc)
    {
        if (ft_strlen(argv[i]) >= 4 && ft_strequ(argv[i] + (ft_strlen(argv[i]) - 4), ".cor"))
            curr++;
        if (ft_strequ(argv[i], "-n") && i < argc - 3)
        {
            pos = ft_atoi(argv[i + 1]);
            if (pos > 0 && ft_strlen(argv[i + 2]) >= 4 && ft_strequ(argv[i + 2] + (ft_strlen(argv[i + 2]) - 4), ".cor"))
                champ = set_champ_to_pos(champ, curr + 1, pos);
        }
    }
    return (champ);
}

t_champ *get_champ(t_list *str_champ, int id)
{
    t_champ *champ;
	char	*name;
	char	*code;
	unsigned int		*magic;
	int		magic2;
	int		res;
	char	*tmp;
	t_header	*header;
	int		i;

	tmp = (char*)(str_champ->content);
	// tmp = ft_strsub(tmp, 0, 4);
	magic = (unsigned int*)malloc(sizeof(int));
	ft_memcpy(magic, str_champ->content, sizeof(int));
	// if (*magic != COREWAR_EXEC_MAGIC)
	// 	return NULL;
	// tmp = (char*)(str_champ->content);
	// tmp = ft_strsub(tmp, 4, 4 + );
	magic2 = ft_strlen(tmp);
    header = (t_header*)malloc(sizeof(t_header));
    *header = (t_header){*magic, 0, get_code_size(code), 0};
	i = 3;
	ft_memcpy(header->prog_name, &(str_champ->content[i]), PROG_NAME_LENGTH);
	i += PROG_NAME_LENGTH + 4;
	ft_memcpy(&(header->prog_size), &(str_champ->content[i]), sizeof(int));
	i += 4;
	ft_memcpy(header->comment, &(str_champ->content[i]), COMMENT_LENGTH);
	i += COMMENT_LENGTH + 4;
    champ = (t_champ*)malloc(sizeof(t_champ));
    *champ = (t_champ){id, header, code, NULL};
    return (champ);
}

t_champ *get_champs(int argc, char **argv)
{
    t_champ *champ;
    t_champ *tmp;
    int i;

    i = 0;
    champ = NULL;
    while (++i < argc)
    {
        if (ft_strlen(argv[i]) >= 4 && ft_strequ(argv[i] + (ft_strlen(argv[i]) - 4), ".cor"))
        {
			ft_printf("%s\n", argv[i]);
            if (champ)
            {
                tmp->next_champ = get_champ(read_file(argv[i]), i);
                tmp = tmp->next_champ;
            }
            else
            {
                tmp = get_champ(read_file(argv[i]), i);
                champ = tmp;
            }
        }
    }
    // champ = sort_champs(champ, argc, argv);
    return (champ);
}

int get_quantity_players(t_champ *champ)
{
    int i;
    int size;

    i = 0;
    size = 0;
    while (champ)
    {
        champ = champ->next_champ;
        size++;
    }
    return (size);
}

void    set_players_to_arena(t_champ *champ, char **arena)
{
    int i;
    int quantity;

    i = 0;
    quantity = get_quantity_players(champ);
    while(champ)
    {
        ft_memcpy(&arena[0][MEM_SIZE / quantity * i], champ->code, ft_strlen(champ->code));
        champ = champ->next_champ;
        i++;
    }
}

int     main(int argc, char **argv)
{
    char *arena;
    int pl_dist;
    t_champ *champ;
    
    champ = get_champs(argc, argv);
    // pl_dist = MEM_SIZE * 2 / (get_quantity_players(champ) - 1);
    // arena = ft_memalloc(MEM_SIZE * 2 + 1);
    // arena = ft_memset(arena, '0', MEM_SIZE * 2);
    // set_players_to_arena(champ, &arena);
    // ft_printf("%s", arena);
    return (0);
}