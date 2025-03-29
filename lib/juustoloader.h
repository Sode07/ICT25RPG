#ifndef H_JSTLOADER_H
#define H_JSTLOADER_H

#include <wchar.h>

struct jst_symbol;
struct jst_section;

union jst_value {
    int jst_int;
    float jst_float;
    wchar_t* jst_char;
    struct jst_symbol* jst_symbol;
    struct jst_section* jst_section;
};

enum jst_value_type {
  JST_INT = 0,
  JST_FLOAT = 1,
  JST_STRING = 2,
  JST_SECTION = 3,
};

typedef struct jst_symbol {
    enum jst_value_type type;
    wchar_t* jst_label;
    union jst_value value;
} jst_symbol;

typedef struct jst_section {
    int member_count;
    jst_symbol* members;
} jst_section;

#ifdef __cplusplus
extern "C" {
#endif

bool load_juusto(const char* fpath, jst_section* jst_root);
    
#ifdef __cplusplus
}
#endif    

#endif
