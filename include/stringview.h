#ifndef STRINGVIEW_H
#define STRINGVIEW_H

#include <stddef.h>

typedef struct {
    const char *data;
    size_t size;
} String_view;

String_view sv(const char *val);

String_view trim_left(String_view *sv);
String_view trim_right(String_view *sv);

String_view chop_from_left(String_view *sv,size_t i);
String_view chop_from_right(String_view *sv);
int sv_eq(String_view a, String_view b);

String_view chop_by_delim(String_view *sv, char delim);

#define SV_FMT "%.*s"
#define SV_ARGS(s) (int)(s).size, (s).data

#endif