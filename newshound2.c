#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>


void error(char *msg)
{
    fprintf(stderr,"%s:%s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    char *phrase = argv[1];
    char *vars[] = {"RSS_FEED=https://ngs.ru/rss", NULL};
    FILE *f = fopen("stories.txt", "w");
    if (!f){
        error("Не могу открыть stories.txt");
    }
    pid_t pid = fork();
    if (pid == -1){
        error ("Не могу клонировать процесс");
    }
    if (!pid){
        if(dup2(fileno(f),1)==-1){
            error("Не могу перенаправить стандартный вывод");
        }
        if(execle("/usr/bin/python3","/usr/bin/python3","./rssgossip.py",
            phrase, NULL,vars)== -1){
                error("Не могу запустить скрипт");
            }
    }
    int pid_status;
    if(waitpid(pid, &pid_status, 0)== -1){
        error("Ошибка во время ожидания дочернего процеса");
    }
    return 0;
}
