#include "include/test.h"
#include <termios.h>

void		raw_term(void)
{
	t_termios		term;
    t_termios       term_backup;

	tcgetattr(0, &term);
    tcgetattr(0, &term_backup);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
}

int ft_init_term()
{
    char    *term_type;
    int     ret;
    
    if (!(term_type = getenv("TERM")))
    {
        printf("getenv failed\n");
        return (-1);
    }
    ret = tgetent(NULL, term_type);
    raw_term();
    if (ret == 0)
    {
        printf("Pas assez d'infos\n");
        return (-1);
    }
    if (ret == -1)
    {
        printf("Pas accès au database\n");
        return (-1);
    }
    return (0);
}

char **get_tab()
{
    int  fd;
    char **str;
    char *line;
    char *line2;

    str = NULL;
    line = NULL;
    line2 = ft_strdup("");
    if ((fd = open("minishell_history.txt", O_RDONLY)) < 0)
        return (NULL);
    while (get_next_line3d(fd, &line) > 0)
    {
        line2 = ft_strjoin(line2, line);
        line2 = ft_strjoin(line2, "\n");
    }
    str = ft_split(line2, '\n');
//    printf("%s\n", line2);
    return (str);
}

int main(int argc, char **argv)
{
    char    *line;
    int     ret;
    int     fd;
    char    str[120];
    int     len;
    int     i;
	int		a;
    int     y;
    char    **history;
    char    *tmp;

    (void)argc;
    (void)argv;
    (void)line;  
    (void)fd; 
    (void)len;
    (void)str;
    line = NULL;
    i = 0;
	y = 0;
	a = 0;
    tmp = ft_strdup("");
    ft_bzero(str, 0);
	if ((ret = ft_init_term()) == -1)
      	return (-1);
	write(1, "START\n", 6);
    while (1)
    {
		line = ft_strdup("");
        if ((history = get_tab()))
        {
            i = 0;
            while (history[i])
                i++;
			y = i;
        }
        while ((len = read(0, str, 100)) != 0)
        {
//            printf("|%s|", str);
    		tmp = ft_strdup("");
            tputs(save_cursor, 1, putchar);
            if (!ft_strcmp(str, "\e[A"))
            {
//				tputs(restore_cursor, 1, putchar);
//				tputs(tgetstr("ed", NULL), 1, putchar);
//				write(1, "UP", 2);
                if (i != 0)
                {
                    i--;
                    ft_putstr(history[i]);
                } 
			}	
            else if (!ft_strcmp(str, "\e[B"))
            {
                if (line[0] != '\0' && a == 0)
				{
					tmp = line;
					a = 1;
				}
//				tputs(restore_cursor, 1, putchar);
//				tputs(tgetstr("ed", NULL), 1, putchar);
                if (i + 1 < y)
                {
                    i++;
                    ft_putstr(history[i]);
                }
                else if (tmp[0] != '\0')
                {
					printf("\n%s", tmp);
					a = 0;
				}
//				write(1, "down", 4);
			}	
            else if (!ft_strcmp(str, key_backspace))
			{
				write(1, "DEL", 3);
//				tputs(cursor_left, 1, putchar);
//				tputs(tgetstr("ed", NULL), 1, putchar);
			}
            else if (!ft_strcmp(str, "\n"))
        	{
//                write(1, "OK", 2);
				write(1, "\n", 1);
				break ;
			}
            else
            {
                line = ft_strjoin(line, str);
                write(1, str, len);
            }
            ft_bzero(str, 0);
        }
        if ((fd = open("minishell_history.txt", O_WRONLY |
			O_APPEND | O_CREAT, 0644)) < 0)
            return (-1);
        if (line[0] != '\0')
        {
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
        }
    }
//    printf("|%s|\n", line);
    close(fd);
    return (ret);
}