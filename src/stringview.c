#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "stringview.h"


String_view sv(const char *val){
    return (String_view){
        .data=val,
        .size=strlen(val)
    };

}

String_view trim_left(String_view *sv){
    while(sv->size && sv->data[0] == ' '){
        sv->data++;
        sv->size-=1;
    }
    return *sv;
}
String_view chop_from_right(String_view *sv){
    sv->size-=1;
    return *sv;
}
String_view chop_from_left(String_view *sv, size_t i)
{
    if (i > sv->size)
        i = sv->size;

    sv->data += i;
    sv->size -= i;

    return *sv;
}
int sv_eq(String_view a, String_view b)
{
    if (a.size != b.size)
        return 0;

    return memcmp(a.data, b.data, a.size) == 0;
}




String_view trim_right(String_view *sv){
    while (sv->size &&
           (sv->data[sv->size - 1] == ' ' ||
            sv->data[sv->size - 1] == '\n' ||
            sv->data[sv->size - 1] == '\r'))  // add this
    {
        sv->size--;
    }
    return *sv;
}

String_view chop_by_delim(String_view *sv, char delim){
    size_t i = 0;

    while(i < sv->size && sv->data[i] != delim){
        i++;
    }
   

    String_view res = {
        .data = sv->data,
        .size = i
    };

     if(i < sv->size)
    chop_from_left(sv,i+1);
    else
    chop_from_left(sv,i);

    return res;
}

