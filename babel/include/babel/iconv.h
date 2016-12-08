/*
* iconv implementation using Win32 API to convert by Yukihiro Nakadaira
*/

#ifndef _BABEL_ICONV_H_
#define _BABEL_ICONV_H_

#include <stddef.h>

typedef void* iconv_t;

iconv_t iconv_open(const char* tocode, const char* fromcode);
int iconv_close(iconv_t cd);
size_t iconv(iconv_t cd, char** inbuf, size_t* inbytesleft, char** outbuf, size_t* outbytesleft);

#endif