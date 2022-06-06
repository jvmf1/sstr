#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct sstr {
	size_t cap;
	size_t len;
	char *data;
} sstr;

void sstr_tolower(sstr *str);

void sstr_toupper(sstr *str);

int sstr_replace(sstr *str, const char *old, const char *newstr);

int sstr_replace_char(sstr *str, const char old, const char newstr);

void sstr_clear(sstr *str);

/* adjust capacity to exactly fit the string */
int sstr_trim_cap(sstr *str);

void sstr_reverse(sstr *str);

void sstr_print(const sstr *str);

void sstr_init(sstr *str);

sstr* sstr_create(const char *s);

/* creates empty string with pre allocated capacity */
sstr* sstr_create_cap(size_t cap);

void sstr_free(sstr *str);

/* automaticly concatenates and adjust capacity if needed */
int sstr_scat(sstr *dest, const sstr *str);

/* automaticly concatenates and adjust capacity if needed */
int sstr_cat(sstr *dest, const char *str);

int sstr_incr_cap(sstr *str, size_t cap);

/* automaticly sets string to s and adjust capacity if needed */
int sstr_set(sstr *str, const char *s);

int sstr_sset(sstr *str, const sstr *s);

/* get stdin and increases cap by cap_incr when needed until '\n' or EOF
 * returns:
 *		0 if finds '\n'
 *		1 if finds EOF
 *		-1 if fail to incr cap */
int sstr_gets(sstr *str, size_t cap_incr);

/* get FILE stream and increases cap by cap_incr when needed until EOF
 * return:
 *		0 if finds EOF
 *		-1 if fail to incr cap */
int sstr_fgets(sstr *str, FILE *stream, size_t cap_incr);

/* get FILE stream and increases cap by cap_incr when needed until 'x' or EOF
 * returns:
 *		0 if finds 'x'
 *		1 if finds EOF
 *		-1 if fail to incr cap */
int sstr_fgetsx(sstr *str, FILE *stream, const char x, size_t cap_incr);

/* same as above but cap_incr is multiplied by 2 everytime */
int sstr_gets2(sstr *str, size_t cap_incr);
int sstr_fgets2(sstr *str, FILE *stream, size_t cap_incr);
int sstr_fgetsx2(sstr *str, FILE *stream, const char x, size_t cap_incr);

/* removes 'ch' at the beginning and end */
void sstr_trim(sstr *str, const char ch);

void sstr_trim_right(sstr *str, const char ch);

void sstr_trim_left(sstr *str, const char ch);

/* sstr_*read does not close FILE or SEEK_SET back to beginning */
sstr* sstr_fread(FILE *f);
sstr* sstr_read(int fd);

int sstr_ccat(sstr *str, const char ch);

/* automatically breaks line if it doesn't find any in 'count' times */
int sstr_breakline(sstr *str, size_t count);

int sstr_count_char(const sstr *str, const char ch);

/* increases str->cap by cap - str->cap if needed
 * returns:
 *		0 if didn't need to increase cap
 *		1 if needed to incr cap
 *		-1 if failed to incr cap */
int sstr_reserve(sstr *str, size_t cap);

/* concatenates fmt to str */
int sstr_printf(sstr *str, const char *fmt, ...);

/* concatenates n chars of src */
int sstr_ncat(sstr *str, size_t size, const char *src);

bool sstr_contains(const sstr *str, const char *s);

int sstr_count(const sstr *str, const char *s);

int sstr_scount(const sstr *str, const sstr *s);

int sstr_fdgets(sstr *str, int fd, size_t bufsize, size_t cap_incr);

int sstr_fdgets2(sstr *str, int fd, size_t bufsize, size_t cap_incr);

sstr* sstr_copy(const sstr *str);

void sstr_rm_consecutive_char(sstr *str, const char ch);
