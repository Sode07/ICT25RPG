#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdint.h>

#include "juustoloader.h"

jmp_buf parse_exit;

enum jst_parse_error_type {
    JSTE_FILE_NOT,
    JSTE_UNCLOSED_SECTION,
    JSTE_UNOPENED_SECTION,
    JSTE_FTELL_ERR,
};

static const char* jst_parse_error_str[] = {
    "JSTE_FILE_NOT",
    "JSTE_UNCLOSED_SECTION",
    "JSTE_UNOPENED_SECTION",
    "JSTE_FTELL_ERR",
};

static const char value_scope_terminators[] = {
    ',',
    '}',
    EOF
};

unsigned int get_file_section_count(FILE* stream)
{
    long file_position;
    unsigned section_count = 0;
    unsigned open_sections = 0;
    char temp_char;

    if ((file_position = ftell(stream)) < 0)
	longjmp(parse_exit, JSTE_FTELL_ERR);

    rewind(stream);
    
    while ((temp_char = fgetc(stream)) != EOF)
    {
	if (temp_char == '{')
	    open_sections++;

	if (temp_char == '}')
	{
	    if (open_sections < 1)
		longjmp(parse_exit, JSTE_UNOPENED_SECTION);
	    open_sections--;
	    section_count++;
	}
    }

    if (open_sections != 0)
    {
	longjmp(parse_exit, JSTE_UNCLOSED_SECTION);
    }
    
    return section_count;
}

static void read_value_scope(FILE* stream, long* scope_start, long* scope_end)
{
    long value_scopes_open = 0;
    long temp_scope_end = 0;
    char temp_char;
    char temptemp_char;
    
    if ((*scope_start = ftell(stream)) < 0)
	longjmp(parse_exit, JSTE_FTELL_ERR);

    do
    {
	temp_char = fgetc(stream);
	temp_scope_end++;

	switch (temp_char)
	{
	    case ':':
	    {
		if ((temptemp_char = fgetc(stream)) == '{')
		    value_scopes_open++;
		else
		    ungetc(temptemp_char, stream);
		break;
	    }
	    case '}':
	    {
		if ((temptemp_char = fgetc(stream)) == ',')
		{
		    if (value_scopes_open < 0)
		    {
			longjmp(parse_exit, JSTE_UNOPENED_SECTION);
		    }

		    if (--value_scopes_open == 0)
		    {
			*scope_end = temp_scope_end;
		    }
		}
		else
		    ungetc(temptemp_char, stream);
		break;
	    }
	}
    }
    while (temp_char != EOF);

    if (value_scopes_open != 0)
	longjmp(parse_exit, JSTE_UNCLOSED_SECTION);
}

static bool extract_symbol(FILE* stream)
{
    long value_scope_begin;
    long value_scope_end;

    read_value_scope(stream, &value_scope_begin, &value_scope_end);

    printf("scope begin: %li, end: %li\n", value_scope_begin, value_scope_end);
    
    return true;
}

bool load_juusto(const char* fpath, jst_section* jst_root)
{
    FILE* jst_stream;
    long line_end_ptr;
    uint8_t parse_error;

    if ((parse_error = setjmp(parse_exit)) > 0)
    {
	printf("Error reading juusto: %s\n", jst_parse_error_str[parse_error]);
	return false;
    }

    if (!(jst_stream = fopen(fpath, "r")))
    {
	longjmp(parse_exit, JSTE_FILE_NOT);
    }

    rewind(jst_stream);
    extract_symbol(jst_stream);

    return true;
}
