/* Allowed functions: pipe, fork, dup2, execvp, close, exit

write the following function:

    int    ft_popen(const char *file, char *const argv[], char type)

The function must launch the executable file with the arguments argv (using execvp).
If the type is 'r' the function must return a file descriptor connected to the output of the command.
If the type is 'w' the function must return a file descriptor connected to the input of the command.
In case of error or invalid parameter the function must return -1.

example:

int main() {
    int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');

    charline;
    while(line = get_next_line(fd))
        ft_putstr(line);
}*/

#include <unistd.h>
#include <stdlib.h>

int    ft_popen(const char *file, char *const argv[], char type)
{
	if (!file || !argv || !*argv || (type != 'r' && type != 'w'))
		return -1;
	
	int fd[2];
	if (pipe(fd) < 0)
        return -1;
	pid_t pid = fork();
    if (pid < 0) 
	{
        close(fd[1]);
        close(fd[0]);
        return -1;
    }
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(fd[0]);
			if (dup2(fd[1],1) < 0)
			{
				close(fd[1]);
				exit(-1);
			}
			close(fd[1]);
		}
		else if (type == 'w')
		{
			close(fd[1]);
			if (dup2(fd[0], 0) < 0)
			{
				close(fd[0]);
				exit(-1);
			}
			close(fd[0]);
		}
		execvp(file, argv);
		exit(-1);
	}
	if (type == 'w')
	{
		close(fd[0]);
		return (fd[1]);
	}
	else if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    char *const ls_argv[] = {"ls", NULL};
    char *const cat_argv[] = {"cat", NULL};

    char buf[1024];
    ssize_t n;

    int fd_r = ft_popen("ls", ls_argv, 'r');
    if (fd_r < 0)
	{ 
		perror("ft_popen r");
		return 1;
	}

    printf("=== Saida de 'ls' (tipo r) ===\n");
    while ((n = read(fd_r, buf, sizeof(buf))) > 0)
        write(1, buf, n);
    close(fd_r);

    int fd_w = ft_popen("cat", cat_argv, 'w');
    if (fd_w < 0)
	{
		perror("ft_popen w");
		return 1;
	}

    printf("\n=== Escrevo para 'cat' (tipo w) ===\n");
    write(fd_w, "Linha 1\n", 8);
    write(fd_w, "Linha 2\n", 8);
    close(fd_w);

    return 0;
}
