#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <unistd.h>

#include "processor/prompt.h"
#include "processor/parser.h"
#include "processor/tokenizer.h"
#include "processor/pipe.h"
#include "processor/signal.h"

#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/echo.h"
#include "commands/ls.h"
#include "commands/exec.h"
#include "commands/pinfo.h"
#include "commands/repeat.h"
#include "commands/history.h"
#include "commands/jobs.h"
#include "commands/syscom.h"
#include "commands/replay.h"
#include "commands/baywatch.h"

#include "utils/string.h"
#include "utils/token_mat.h"

#define MAX_TOKEN_LENGTH 100
#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 100

#define BOLD "\033[1m"
#define NO_BOLD "\033[22m"
#define RED "\033[38;5;1m"
#define GREEN "\033[38;5;121m"
#define ORANGE "\033[38;5;209m"
#define PURPLE "\033[38;5;205m"
#define LIGHT_PINK "\033[38;5;225m"
#define LIGHT_PURPLE "\033[38;5;213m"
#define YELLOW "\033[38;5;223m"
#define RESET "\033[0m"
#define BLUE "\033[0;34m"

extern int output_redirection;
extern int shell_pid;

#endif