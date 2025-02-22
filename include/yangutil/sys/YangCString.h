/*
 * YangCString.h
 *
 *  Created on: 2022年1月6日
 *      Author: yang
 */

#ifndef INCLUDE_YANGUTIL_SYS_YANGCSTRING_H_
#define INCLUDE_YANGUTIL_SYS_YANGCSTRING_H_


#include <stdlib.h>
#include <string.h>

typedef struct {
    int32_t capacity;
    int32_t vlen;
    char **str;
}YangStrings;
void yang_cstr_random(int32_t len,char* data);
int32_t yang_cstr_split(char *src, char *delim, YangStrings* istr);
void yang_cstr_replace(char *str,char* dst, char *orig, char *rep);
void yang_destroy_strings(YangStrings* strs);
int32_t yang_cstr_userfindindex(char* p,char c);
int32_t yang_cstr_userfindupindex(char* p,char c,int32_t n);
int32_t yang_cstr_isnumber(char* p,int32_t n);
int32_t yang_strcmp(char* str1,char* str2);
void yang_itoa(int32_t num,char* data,int32_t n);
#endif /* INCLUDE_YANGUTIL_SYS_YANGCSTRING_H_ */
