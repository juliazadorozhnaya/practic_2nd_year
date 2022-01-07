#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define SIZE_S 10
#define SIZE_A 5
#define MAX_CWD 1000

struct cmd_inf
{
    char **argv; //массив с аргументами и командой
    char **infile; //файл ввод 
    char **outfile; //файл вывод
    char **misfile;
    struct cmd_inf *conv; //конвейер
} typedef cmd_inf;


void stop_proc(int sig);
char **make_com_mass(char *str);
int check_word(char *word);
cmd_inf *make_comm();
void alloc_check(void *new);
char *read_str();
char **make_mass(char *str);
int file_redirect(cmd_inf *cmd, int prev, int next);
char *stand(char *str);
int count_cmd(cmd_inf *cmd);
int run_com(cmd_inf *cmd);
int in_com(cmd_inf *cmd);
//void get_out(int sig);
void free_com(cmd_inf *cmd);
char *find_com(char *str);

int status;
pid_t *child_mass;
int child_count = 0;
char **com_table;
char *com;
char **rem_mass;
char *rem_str;

//cmd_inf **cmd_table;


int main()
{
    cmd_inf *cmd;
    char *tp = NULL, *line = NULL;
    char **mass = NULL;
    int i = 0, j = 0;
    char **com_mass = NULL;
    char *s = NULL;
    int cur_comm = 0;
    size_t size_com = SIZE_A;
    char cwd[MAX_CWD];
    
    /*sigset_t mask;
    sigset_t orig;
    
    sigemptyset(&mask);
    sigaddset(&mask, SIG_IGN);
    sigaction*/
    //signal(SIGTSTP, stop_proc);
    signal(SIGINT, SIG_IGN);
    
    //cmd_table = (cmd_inf **)malloc(sizeof(cmd_inf *) * size_com);
    //alloc_check(com_table);
    while(1)
    {
        i = 0;
        printf("%s ~ good_shell$ ", getcwd(cwd, sizeof(cwd)));
        tp = read_str();
        if (tp == NULL)
        {
            printf("\nctrl-d\n");
            exit(0);
        }
        if(tp[0] == '#')
        {
            free(tp);
            continue;
        }
        if (tp[0] == '|')
        {
            printf("wrong input\n");
            free(tp);
            continue;
        }
        
        rem_str = tp;
        tp = find_com(tp);
        
        com_mass = make_com_mass(tp);
        
        com_table = com_mass; //для того, чтобы в будущем осовободить память 
        while(com_mass[i] != NULL)
        {
            
            line = stand(com_mass[i]);
            com = line; //для того, чтобы в будущем осободить память - строка аргументов с пробелами
            
            mass = make_mass(line);
            rem_mass = mass; //для того, чтобы в будущем осободить память
            
            if (mass == NULL)
            {
                printf("er\n");
                return 0;
            }
            if ((cmd = make_comm(mass)) != NULL)
            {
                if(in_com(cmd) == 1)
                {
                    run_com(cmd);
                    //while(wait(&status) != -1);
                }
            }
            else
            {
                printf("wrong input\n");
                break;
            }
            i++;
            if (cmd != NULL) free_com(cmd);
            
            if (rem_mass != NULL) free(rem_mass);
            if (com != NULL) free(com);
            
        }
        
        if (com_mass != NULL) free(com_mass);
        if (tp != NULL) free(tp);

    }

    
    return 0;
}
        
char **make_com_mass(char *str)
{
    char **tp;
    size_t size = SIZE_A;
    int cur_com = 0;
    char *com;
    
    tp = (char **)malloc(sizeof(char *) * size);
    alloc_check(tp);
    
    com = strtok(str, ";\n");
    
    while(com != NULL)
    {
        //printf("%s\n", com);
        tp[cur_com] = com;
        cur_com++;
        if (cur_com == size)
        {
            size *= 2;
            tp = (char **)realloc(tp, size * sizeof(char *));
            alloc_check(tp);
        }
        com = strtok(NULL, ";\n");
    }
    tp[cur_com] = NULL;
    return tp;
}
    
        
        
cmd_inf *make_comm(char **mass)
{
    
    size_t size_str = SIZE_S; /*память слова*/
    int cur_arg = 0; /*нынешний аргумент*/
    size_t size_arg = SIZE_A; /*память аргументов*/
    cmd_inf *cmd_tp;
    int i = 0;
    int ind = 0;
    
    
    cmd_tp = (cmd_inf *)malloc(sizeof(cmd_inf));
    alloc_check(cmd_tp);
    
    cmd_tp -> infile = NULL;
    cmd_tp -> outfile = NULL;
    cmd_tp -> misfile = NULL;
    cmd_tp -> conv = NULL;
    
    cmd_tp -> argv = (char**)malloc(size_arg * sizeof(char *));
    alloc_check(cmd_tp -> argv);
        
    while((ind = check_word(mass[i])) > 0)
    {
        switch(ind)
        {
            case 1: // слово аругмент или команда
                (cmd_tp -> argv)[cur_arg] = mass[i];
                if ((cmd_tp -> argv)[0] == NULL)
                    return NULL;
                cur_arg++;
                if (cur_arg == size_arg)
                {
                    size_arg *= 2;
                    cmd_tp -> argv = (char **) realloc(cmd_tp -> argv, size_arg * sizeof(char *));
                    alloc_check(cmd_tp);
                }
                i++;
                break;
            case 2: //есть конвейер
                if ((mass + i + 1) == NULL)
                {
                    return NULL;
                }
                (cmd_tp -> conv) = make_comm(mass + i + 1);
                if ((cmd_tp -> conv) == NULL) //команда после | введена неверно
                {
                    return NULL;
                }
                mass[i] = NULL;
                break;
            case 3: //есть перенаправление потока ввода
                if (cmd_tp -> infile == NULL)
                {
                    cmd_tp -> infile = (char **)malloc(2 * sizeof(char *)); // 2, т.к. нужно сохранить имя файла и > или >>
                    alloc_check(cmd_tp -> infile);
                }
                (cmd_tp -> infile)[0] = mass[i];
                (cmd_tp -> infile)[1] = mass[i + 1];
                if (mass[i + 1] != NULL) //если все-таки NULL - ошибка
                    i += 2;
                else 
                    return NULL;
                break;
            case 4: //есть перенаправление потока вывода
            case 5: //есть перенаправление потока вывода в файл с добавлением
            
                if (cmd_tp -> outfile == NULL)
                {
                    cmd_tp -> outfile = (char **)malloc(2 * sizeof(char *));
                    alloc_check(cmd_tp -> outfile);
                }
                (cmd_tp -> outfile)[0] = mass[i];
                (cmd_tp -> outfile)[1] = mass[i + 1];
                if (mass[i + 1] != NULL)
                    i += 2;
                else 
                    return NULL;
            
                break;
            case 6: //есть перенаправление потока ошибок
                if (cmd_tp -> misfile == NULL)
                {
                    cmd_tp -> misfile = (char **)malloc(2 * sizeof(char *));
                    alloc_check(cmd_tp -> misfile);
                }
                (cmd_tp -> misfile)[0] = mass[i];
                (cmd_tp -> misfile)[1] = mass[i + 1];
                if (mass[i + 1] != NULL)
                    i += 2;
                else return NULL;
                break;
                
            default:
                return NULL;
                break;
        }
    }
    //if (ind == -1)
        //return NULL;
    (cmd_tp -> argv)[cur_arg] = NULL;
    return cmd_tp;
}
       
            
int check_word(char *word)
{
    char *red[] = {"<", ">", ">>", "2>"};
    char *conv = "|";
    int i = 0;
    
    if (word == NULL)
    {
        return -1;
    }
    
    if (strcmp(word, "|") == 0)
    {
        return 2;
    }
    
        
    for (; i < 4; i++)
    {
        if (strcmp(red[i], word) == 0)
        {
            return i + 3;
        }
    }
    return 1;
}

void alloc_check(void *new)
{
    if (new == NULL)
    {
        printf("alloc error\n");
        exit(0);
    }
}


char **make_mass(char *str)
{
    int cur_c = 0;
    int cur_w = 0;
    size_t size = SIZE_S;
    char **tp;
    char *word;
    int i = 0;
    
    
    tp = (char **)malloc(size * sizeof(char *));
    alloc_check(tp);
    
    word = strtok(str, " \n");
    while(word != NULL)
    {
        tp[cur_w] = word;
        
        cur_w ++;
        if (cur_w == size)
        {
            size *= 2;
            tp = (char **)realloc(tp, size * sizeof(char *));
            alloc_check(tp);
        }
        word = strtok(NULL, " \n");
    }
    tp[cur_w] = NULL;
   
    return tp;
}
        
char *read_str()
{
    char *tp = NULL;
    size_t size = 0;
    if (getline(&tp, &size, stdin) == -1)
    {
        free(tp);
        return NULL;
    }
    if (size == 0)
    {
        return NULL;
    }
    return tp;
}

int in_com(cmd_inf *cmd)
{
    int i = 0;
    
    if ((cmd->argv)[0] != NULL && strcmp((cmd -> argv)[0], "exit") == 0)
    {
        for(i = 0; i < child_count - 1; i++)
        {
            
            kill(SIGTERM, child_mass[i]);
            wait(&status);
        }
        if (rem_str != NULL) free(rem_str);
        if (com_table != NULL)
            free(com_table);
        if (com != NULL)
            free(com);
        if (rem_mass != NULL)
            free(rem_mass);
        free_com(cmd);
        exit(0);
    }
    
    if ((cmd->argv)[0] != NULL && strcmp((cmd -> argv)[0], "cd") == 0)
    {
        //printf("%s %s\n", (cmd -> argv)[0], (cmd -> argv)[1]);
        //chdir((cmd -> argv)[1]);
        if ((cmd -> argv)[1] == NULL || chdir((cmd -> argv)[1]) == -1)
        {
            printf("can't change directory\n");
            return 0;
        }
        return 0;
        
        
    }
    
    return 1;
}

int run_com(cmd_inf *cmd)
{
    int fd[2];
    int cur_child = 1;
    int i = 0;
    int prev_fd, next_fd;
    
    pid_t t_pid;

    child_count = count_cmd(cmd);
    child_mass = (pid_t *)malloc(child_count * sizeof(pid_t));
    alloc_check(child_mass);
    
    
    
    while(cmd != NULL)
    {
        if (pipe(fd) == -1)
        {
            printf("pipe error\n");
            return -1;
        }
        next_fd = fd[1];
        switch(t_pid = fork())
        {
            case -1:
                printf("fork error\n");
                close(fd[1]);
                close(fd[0]);
                return -1;
                break;
                
            case 0: //сын
                signal(SIGTSTP, SIG_DFL);
                signal(SIGINT, SIG_DFL);
                free(child_mass);
                
                if (cur_child == 1)
                    prev_fd = 0;
                
                if (cur_child == child_count)
                {
                   
                    next_fd = 1;
                }
                
                dup2(prev_fd, 0);  // откуда читает          
                dup2(next_fd, 1); //куда пишет
            
                if (file_redirect(cmd, prev_fd, next_fd) == -1)
                {
                    printf("can't redirect\n");
                    return -1;
                }
                
                execvp((cmd -> argv)[0], cmd -> argv);
                
                printf("\texec error\n");
                if (rem_str != NULL) free(rem_str);
                if (com != NULL) free(com);
                if (com_table != NULL) free(com_table);
                if (cmd != NULL) free_com(cmd);
                if (rem_mass != NULL) free(rem_mass);
                exit(-1);
                break;
                
            default:
                signal(SIGTSTP, SIG_IGN);
                child_mass[cur_child - 1] = t_pid;
                prev_fd = fd[0];
                if (next_fd != 1)
                    close(next_fd);
                
                break;
            
        }
        cmd = cmd -> conv;
        cur_child++;
    }
    //stop_proc(child_count);
    //printf("%d\n", child_count);
    for (i = 0; i < child_count; i++)
    {
        waitpid(child_mass[i], &status, WUNTRACED);
        if (WIFSTOPPED(status))
        {
            printf("\n%d was stopped\n", child_mass[i]);
        }
        
    }
    free(child_mass);
    return 0;
}
    
    
    
    
int count_cmd(cmd_inf *cmd)
{
    int i = 0;
    
    while(cmd != NULL)
    {
        i++;
        cmd = cmd -> conv;
    }
    return i;
}



// перед и после каждого спец символа(<, >, >>, 2>, |) добавляет пробел
char *stand(char *str)
{
    char *tp;
    int cur_cs = 0;
    int cur_cl = 0;
    size_t size = SIZE_S;
    int len = strlen(str);
    tp = (char *)malloc(size * sizeof(char));
    
    while(cur_cs < len)
    {
        if (str[cur_cs] == '<' || str[cur_cs] == '|') 
        {
            size += 5;
            tp = (char *) realloc(tp, size * sizeof(char));
            alloc_check(tp);
            
            tp[cur_cl] = ' ';
            cur_cl++;
            
            tp[cur_cl] = str[cur_cs];
            cur_cl++;
            
            tp[cur_cl] = ' ';
            cur_cl++;

            cur_cs++;
            continue;
        }
        
        if (str[cur_cs] == '>')
        {
            size += 5;
            tp = (char *) realloc(tp, size * sizeof(char));
            alloc_check(tp);
            tp[cur_cl] = ' ';
            cur_cl++;
            
            tp[cur_cl] = '>';
            cur_cl++;
            
            cur_cs++;
            
            if (str[cur_cs] == '>')
            {
                size += 5;
                tp = (char *) realloc(tp, size * sizeof(char));
                alloc_check(tp);
                
                tp[cur_cl] = '>';
                cur_cl++;
                
                cur_cs++;
            }
            tp[cur_cl] = ' ';
            cur_cl++;
            continue;
        }
        
        if (str[cur_cs] == '2')
        {
            if (str[cur_cs + 1] == '>')
            {
                size += 5;
                tp = (char *) realloc(tp, size * sizeof(char));
                alloc_check(tp);
                
                tp[cur_cl] = ' ';
                cur_cl++;
                tp[cur_cl] = '2';
                cur_cl++;
                tp[cur_cl] = '>';
                cur_cl++;
                tp[cur_cl] = ' ';
                cur_cl++;
                cur_cs += 2;
                continue;
            }
            
        }
        tp[cur_cl] = str[cur_cs];
        cur_cs++;
        cur_cl++;
        if (cur_cl == size)
        {
            size *= 2;
            tp = (char *)realloc(tp, size * sizeof(char));
            alloc_check(tp);
        }
    }
    tp[cur_cl] = '\0';
    return tp;
}
                
int file_redirect(cmd_inf *cmd, int prev, int next)
{
    int vv;
    
    if ((cmd -> infile) != NULL)
    {
        vv = open((cmd -> infile)[1], O_RDONLY, 0666);
        if (vv == -1)
        {
            return -1;
        }
        close(prev);
        dup2(vv, 0);
    }
    
    if ((cmd -> outfile) != NULL)
    {
        if (strcmp((cmd -> outfile)[0], ">") == 0)
        {
            vv = open((cmd -> outfile)[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (vv == -1)
            {
                return -1;
            }
            close(next);
            dup2(vv, 1);
        }
        
        if (strcmp((cmd -> outfile)[0], ">>") == 0)
        {
            vv = open((cmd -> outfile)[1], O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (vv == -1)
            {
                return -1;
            }
            close(next);
            dup2(vv, 1);
        }
    }
    
    if ((cmd -> misfile) != NULL)
    {
        vv = open((cmd -> misfile)[1], O_WRONLY | O_CREAT, 0666);
        if (vv == -1)
        {
            return -1;
        }
        close(2);
        dup2(vv, 2);
    }
    
    return 0;
}

//void get_out(int sig)
//{
    
    
    
void free_com(cmd_inf *cmd)
{
    if (cmd == NULL)
    {
        return;
    }
    
    if (cmd -> argv != NULL)
        free(cmd -> argv);
    if (cmd -> infile)
        free(cmd -> infile);
    if (cmd -> outfile)
        free(cmd -> outfile);
    if (cmd -> misfile)
        free(cmd -> misfile);
    if (cmd -> conv != NULL)
        free_com(cmd -> conv);
    //free(cmd -> conv);
    free(cmd);
}

char *find_com(char *str)
{
    char *com;
    
    com = strtok(str, "#");
    if (com == NULL)
    {
        return str;
    }
    
    return com;
}

void stop_proc(int sig)
{
    int i = 0;
    signal(SIGTSTP, SIG_IGN);
    for(i = 0; i < child_count; i++)
    {
        if (kill(child_mass[i], 0) != 0)
        {
            printf("\n%d-process was stopped\n", child_mass[i]);
        }
    }
}
            
            
            
            
            
            
            
            
            
            
    
