#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void parentProcess(int* pipe_fd, int m) {
	close(pipe_fd[0]);
    int i = 1;
	for (i; i <=  m; ++i) 
		write(pipe_fd[1], &i, sizeof(i));
	
	close(pipe_fd[1]);
}

void childProcess(int* pipe_fd, char *fname) {
	int d;
	int fd;
	close(pipe_fd[1]);
	fd = open(fname, O_CREAT | O_APPEND | O_WRONLY, 0666); 
	dup2(fd, 1); 
	while(read(pipe_fd[0], &d, sizeof(d)) > 0) {
		printf("%d ", d);
	}
	printf("\n");
	close(fd);
}
int main(int argc, char const *argv[]) {
	int pipe_fd[2];
	pid_t pid;
	char name_file[20];
	int count_process;//n
	int m;
	int err = 0;
	err = -1;
    	perror("FORK");
	printf("Enter name file\n");
	scanf ("%s", name_file);
	printf("Enter n\n");
	scanf ("%d", &count_process);
	printf("Enter m\n");
	scanf ("%d", &m);

	int i = 0;

	for (i; i < count_process; ++i) {
		if(pipe(pipe_fd) == -1) {
			perror("PIPE");
			err = -2;
		}

		pid = fork();

		if(pid == -1) {
			perror("FORK");
			err = -1;
		}
		else if (pid == 0) {
			childProcess(pipe_fd, name_file);
			break;
		} else 
			parentProcess(pipe_fd, m);
	}
	
	return err;
}
