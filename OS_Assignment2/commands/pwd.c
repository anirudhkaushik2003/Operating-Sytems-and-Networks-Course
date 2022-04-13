#include "pwd.h"

string Initialize_root()
{
    string output;
    output.buf = malloc(sizeof(char) * 100);
    output.len = 100;
    getcwd(output.buf, output.len);

    return output;
}

string getpwd()
{
    string output;
    output.buf = malloc(sizeof(char) * 100);
    output.len = 100;
    if (getcwd(output.buf, output.len) == NULL)
    {
        printf(RESET);
        printf(RED);
        perror("Error");
        printf(RESET);
    }
    return output;
}

string current_dir(string root)
{
    root.len = strlen(root.buf);
    string cwd;
    cwd.buf = malloc(sizeof(char) * 100);
    if (getcwd(cwd.buf, 100) == NULL)
    {
        printf(RESET);
        printf(RED);
        perror("Error");
        printf(RESET);
    }
    cwd.len = strlen(cwd.buf);

    if (strcmp(root.buf, cwd.buf) == 0)
    {
        strcpy(cwd.buf, "~");
        return cwd;
    }
    else if (root.len < cwd.len)
    {
        string ret;

        ret = remove_match(root, cwd);
        char *t = malloc(sizeof(char) * 100);
        strcpy(t, "~");
        strcat(t, ret.buf);
        strcpy(ret.buf, t);
        return ret;
    }
    else
    {
        return cwd;
    }
}