#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <setjmp.h>

#include "juustoloader.h"

jmp_buf parse_exit;

unsigned int get_file_section_count(FILE* stream)
{
    long file_position;
    unsigned section_count = 0;
    unsigned open_sections = 0;
    char temp_char;

    if ((file_position = ftell(stream)) < 0)
	longjmp(parse_exit, true);

    rewind(stream);
    
    while ((temp_char = fgetc(stream)) != EOF)
    {
	printf("read: %x as %c\n", temp_char, temp_char);
	if (temp_char == '{')
	    open_sections++;

	if (temp_char == '}')
	{
	    if (section_count < 1)
		longjmp(parse_exit, true);
	    open_sections--;
	    section_count++;
	}
    }

    if (open_sections == 0)
	longjmp(parse_exit, true);

    return section_count;
}

bool load_juusto(const char* fpath, jst_section* jst_root)
{
    FILE* jst_stream;
    long line_end_ptr;

    if (!(jst_stream = fopen(fpath, "r")))
	return false;

    if(setjmp(parse_exit))
    {
	printf("parse failed\n");
	return false;
    }
    
    printf("section count for %s: %u\n", fpath, get_file_section_count(jst_stream));

    return true;
}
