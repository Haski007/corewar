#include "corewar.h"

int     get_code_arg_types(t_code *code_line)
{
    char args_byte_code[9];

    ft_memset(args_byte_code, '0', 8);
    if (code_line->arg1[0] == 'r' || code_line->arg1[0] != DIRECT_CHAR)
        args_byte_code[1] = '1';
    else
        args_byte_code[0] = '1';
    if (code_line->arg2)
    {
        if (code_line->arg2[0] == 'r' || code_line->arg2[0] != DIRECT_CHAR)
            args_byte_code[3] = '1';
        else
            args_byte_code[2] = '1';
    }
    if (code_line->arg3)
    {
        if (code_line->arg3[0] == 'r' || code_line->arg3[0] != DIRECT_CHAR)
            args_byte_code[5] = '1';
        else
            args_byte_code[4] = '1';
    }
    return (get_int_from_bytes(args_byte_code));
}

int     get_byte_size_to_marked(t_code *code, t_code *marked_code)
{
    int size;

    size = 0;
    if (code->id > marked_code->id)
    {
        while (marked_code)
        {
            if (marked_code->id == code->id)
                break;
            size += marked_code->byte_size;
            marked_code = marked_code->next;
        }
        size *= -1;
    }
    else
    {
        while (code)
        {
            if (marked_code->id == code->id)
                break;
            size += code->byte_size;
            code = code->next;
        }
    }
    return (size);
}

int     get_arg_code(t_code *codes, t_code *code, char *arg)
{
    unsigned int res;
    char *label;
    t_code *marked_code;

    if ((arg[0] == DIRECT_CHAR && arg[1] == LABEL_CHAR) || arg[0] == LABEL_CHAR)
    {
        if (arg[0] == LABEL_CHAR)
            label = ft_strsub(arg, 1, ft_strlen(arg) - 1);
        else
            label = ft_strsub(arg, 2, ft_strlen(arg) - 2);
        marked_code = get_code_by_mark(codes, label);
        res = (unsigned int)get_byte_size_to_marked(code, marked_code);
    }
    else if (arg[0] == 'r' || arg[0] == DIRECT_CHAR)
        res = ft_atoi(arg + 1);
    else
        res = ft_atoi(arg);
    return (res);
}

int     get_size_arg(char *arg, t_command *com)
{
    if (!arg)
        return (0);
    if (arg[0] == 'r')
        return (1);
    if (arg[0] == DIRECT_CHAR)
        return (com->dir_size);
    return (2);
}

void    write_command_line(int fd, t_code *codes, t_code *code_line, t_command *catalog)
{
    t_command *com;

    com = get_com_byname(catalog, code_line->name);
    set_int(fd, com->code, 1);
    if (com->codage_octal)
        set_int(fd, get_code_arg_types(code_line), 1);
    set_int(fd, get_arg_code(codes, code_line, code_line->arg1), get_size_arg(code_line->arg1, com));
    if (code_line->arg2)
        set_int(fd, get_arg_code(codes, code_line, code_line->arg2), get_size_arg(code_line->arg2, com));
    if (code_line->arg3)
        set_int(fd, get_arg_code(codes, code_line, code_line->arg3), get_size_arg(code_line->arg3, com));
}

void    write_exec_code(int fd, t_code *code, t_command *catalog)
{
    t_code *tmp;

    tmp = code;
    while (tmp)
    {
        write_command_line(fd, code, tmp, catalog);
        tmp = tmp->next;
    }
}