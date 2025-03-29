#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdint.h>
#include <ctype.h>
#include <wchar.h>

#include "juustoloader.h"
#include "console.h"

#define PEEK_CHAR(STREAM) ungetc(fgetc(STREAM), STREAM)

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

static const wchar_t anon_label_string[] = {'(', 'A', 'n', 'o', 'n', 'y', 'm', 'o', 'u', 's', ')'};

static wchar_t* read_until_wstring(const wchar_t* dest, const wchar_t* bset)
{
    wchar_t* temp_c = (wchar_t*)dest;

    while (!*(temp_c++))
    {
	if (!wcschr(bset, *temp_c))
	    break;
    }

    if (!*temp_c) return NULL;
    else return temp_c;
}

static jst_symbol* parse_value_str(const wchar_t* str)
{
    wchar_t* sep_chr;
    jst_symbol* outSym;
    bool is_section = false;
    
    if (!(outSym = malloc(sizeof(jst_symbol))))
	return NULL;

    if (!(sep_chr = read_until_wstring(str, (wchar_t*)":{")))
	return NULL;

    /* Extract data label from string */
    if (*(sep_chr-1) == ',' || sep_chr == str)
	outSym->jst_label = (wchar_t*)anon_label_string;
    else
    {
	outSym->jst_label = malloc((sep_chr - str) * sizeof(wchar_t) + 1);
	wcsncpy(outSym->jst_label, str, (sep_chr - str));
    }

    if (*sep_chr == '{' || *(sep_chr+1) == '{')
    {

    }
    else
    {
	if ()
    }
    
    return outSym;
}

static void free_jst_value(jst_symbol* sym)
{
    if (!sym) return;

    if (sym->jst_label != anon_label_string)
	free(sym->jst_label);
    
    switch (sym->type)
    {
	case JST_STRING:
	{
	    KLOG_WARNING("%s", "Unimplemented!");
	    break;
	}
	case JST_SECTION:
	{
	    KLOG_WARNING("%s", "Unimplemented!");
	    break;
	}
    }
}

static long read_until_file(FILE* stream, const char* tokens)
{
    char temp_char;
    long position_counter = 0;

    do
    {
	temp_char = fgetc(stream);
	if (temp_char == EOF)
	    break;
	position_counter++;
    }
    while (!strchr(tokens, temp_char));
    ungetc(temp_char, stream);
    
    return position_counter;
}

static bool extract_symbol(FILE* stream, wchar_t** out)
{
    long open_scopes = 0;
    long value_scope_begin;
    long value_scope_end;
    long scope_count;
    wchar_t peek_char;
    int* out_array;
    bool is_only_wspace = true;
    
    if ((value_scope_begin = ftell(stream)) < 0)
	longjmp(parse_exit, JSTE_FTELL_ERR);

    value_scope_end = read_until_file(stream, "{,");

    if (fgetc(stream) == '{')
    {
	open_scopes++;
	while (peek_char != EOF)
	{
	    value_scope_end += read_until_file(stream, "{}");
	    peek_char = fgetc(stream);
	    
	    if (peek_char == '{')
	    {
		open_scopes++;
		scope_count++;
	    }
	    else if (peek_char == '}')
	    {	
		if (open_scopes <= 0)
		{
		    longjmp(parse_exit, JSTE_UNOPENED_SECTION);
		}
		else if (--open_scopes == 0)
		{
		    if (PEEK_CHAR(stream) == ',')
			value_scope_end++;
		    break;
		}
	    }		  
	}
    }

    // Immediate values
    if (open_scopes != 0)
	longjmp(parse_exit, JSTE_UNCLOSED_SECTION);

    if (PEEK_CHAR(stream) == EOF)
	return false;

    out_array = malloc(value_scope_end * sizeof(wchar_t) + 1);
    
    fseek(stream, value_scope_begin, SEEK_SET);
    for (long counter = 0; counter < value_scope_end; counter++)
    {
	wchar_t tmp_c = (out_array[counter] = (wchar_t)fgetc(stream));
	if (!isspace(tmp_c) && is_only_wspace)
	    is_only_wspace = false;
    }

    if (is_only_wspace)
    {
	free(out_array);
	return false;
    }

    *out = out_array;
    return true;
}

static bool pum(FILE* stream)
{
    wchar_t* sym_data;
    jst_symbol* sym_parsed;
    
    if (!extract_symbol(stream, &sym_data))
	return false;

    if ((sym_parsed = parse_value_str(sym_data)))
	printf("%ls\n", sym_parsed->jst_label);
    
    printf("%ls\n", sym_data);
    
    free(sym_data);
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
    pum(jst_stream);
    
    return true;
}
