#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "isomap.h"
#include "console.h"
#include "juustoloader.h"

#define ENSURE_ARGS(NARG) \
    { if (argc != NARG)   \
	{KLOG_WARNING("%s", "Ensure args check failed");	\
	 return;}}

#define DECL_CMD(FNAME, FPTR) \
    (struct command_t){FNAME, FPTR}

void cleanup_main(int status);

/* BEGIN COMMAND FUNCTION HANDLES  */

void _map_change(int argc, char **args)
{
    ENSURE_ARGS(2);

    load_map_from_file(args[1]);
}

void _exit_program()
{
    cleanup_main(0);
};

void _map_info()
{
    map_t current_map;

    if (!get_loaded_map_name(&current_map))
    {
	KLOG_INFO("%s", "No map loaded");
	return;
    }

    puts("------Map Info------");
    printf("name: %s\n", current_map.name);
    printf("addr: %p\n", current_map.map);
    puts("dim:");
    printf("  w: %4u\n", current_map.map_w);
    printf("  h: %4u\n", current_map.map_h);
    printf("  d: %4u\n", current_map.map_d);
    puts("------Map Info------");
}

void _juusto_test(int argc, char** args)
{
    ENSURE_ARGS(2);

    puts(load_juusto(args[1], NULL) ? "joo": "ei");
}

/* END COMMAND FUNCTION HANDLES  */

struct command_t {
    char* cvar;
    void (*func)(int argc, char** args);
};

static struct command_t bound_commands[] = {
    DECL_CMD("map", _map_change),
    DECL_CMD("exit", _exit_program),
    DECL_CMD("mapinfo", _map_info),
    DECL_CMD("jai", _juusto_test),
};

static inline int get_char_count(char* str, char cchar)
{
    char* tmp_p = str;
    int counter = 0;
    
    while ((tmp_p = strpbrk(tmp_p+1, &cchar)))
	counter++;

    return counter;
}

static char* trim_str(char* dest)
{
    size_t dest_index_len = strlen(dest) - 1;

    while (dest[dest_index_len] == ' ')
	dest[dest_index_len] = 0;

    while (*dest == ' ')
	dest++;

    return dest;
}

static struct command_t get_command_bound(const char* cvar)
{
    size_t command_table_index = 0;
    struct command_t current_command;
    static const size_t command_table_length = sizeof(bound_commands) / sizeof(struct command_t);

    for (; command_table_index < command_table_length; command_table_index++)
    {
	current_command = bound_commands[command_table_index];
	if (!strcmp(cvar, current_command.cvar))
	    return current_command;
    }
    return (struct command_t){0, 0};
}

static void exec_console_command(char* command)
{
    char _entrd[1024];
    char** cargs;
    int cargc;
    struct command_t exec_cmd;
    
    strncpy((char*)&_entrd, command, sizeof(_entrd));
    trim_str(_entrd);
    cargc = get_char_count(command, ' ') + 1;

    cargs = malloc(sizeof(char*) * cargc + 1);
    cargs[0] = strtok((char*)&_entrd, " ");
    for (int i = 1; i < cargc; i ++)
    {
	cargs[i] = strtok(NULL, " ");
    }

    exec_cmd = get_command_bound(cargs[0]);
    
    if (!exec_cmd.func)
    {
	KLOG_WARNING("No such command: %s", cargs[0]);
	return;
    }

    exec_cmd.func(cargc, cargs);
    
    free(cargs);
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
	    input_buffer[bp - 1] = 0;
	    exec_console_command(input_buffer);
	}
	else if (bp < sizeof(input_buffer)-1)
	    return;

	for (long unsigned int i = 0; i < bp; i++)
	    input_buffer[i] = 0;
	bp = 0;
	return;
    }

    return;    
}
