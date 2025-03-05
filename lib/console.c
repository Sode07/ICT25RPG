#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "isomap.h"
#include "console.h"

/* BEGIN COMMAND FUNCTION HANDLES  */

void _map_change(const char*);
void _exit_program();

/* END COMMAND FUNCTION HANDLES  */

struct command_t {
    char* cvar;
    void* func;
};

static struct command_t bound_commands[] = {
    (struct command_t){"map", _map_change},
    (struct command_t){"exit", _exit_program},
    (struct command_t){NULL, NULL},
};

static struct command_t get_command_bound(const char* cvar)
{
    size_t command_table_index = 0;
    struct command_t current_command;
    
    do
    {
	current_command = bound_commands[command_table_index];
	if (!current_command.cvar)
	    return bound_commands[(sizeof(bound_commands) / sizeof(command_t)) - 1];
    }
    while (strcmp(command, current_command.cvar));

    return current_command;
}

static void exec_console_command(const char* command)
{
    
}

void konsoli_single_thread()
{
    static char input_buffer[1024];
    static uint16_t bp = 0;
    int bt = 0;
    
    if ((bt = read(0, input_buffer + bp, sizeof(input_buffer) - bp - 1)) > 0)
    {
	bp += bt;
	if (input_buffer[bp-1] == '\n')
	{
	    input_buffer[bp-1] = 0;
	}
	else if (bp < sizeof(input_buffer)-1)
	    return;

	for (long unsigned int i = 0; i < sizeof(input_buffer); i++)
	    input_buffer[i] = 0;
	bp = 0;
	return;
    }

    return;    
}
