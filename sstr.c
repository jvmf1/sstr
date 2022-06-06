#include "sstr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>

void sstr_tolower(sstr *str) {
	for(size_t i=0;i<str->len;i++) {
		str->data[i]=tolower(str->data[i]);
	}
}

void sstr_toupper(sstr *str) {
	for(size_t i=0;i<str->len;i++) {
		str->data[i]=toupper(str->data[i]);
	}
}

int sstr_replace_char(sstr *str, const char old, const char new) {
	if (new=='\0') return -1;
	for (size_t i=0;i<str->len;i++) {
		if (str->data[i]==old) str->data[i]=new;
	}
	return 0;
}

void sstr_clear(sstr *str) {
	str->len=0;
	if (str->cap != 0)
		str->data[0]='\0';
}

int sstr_trim_cap(sstr *str) {
	if (str->len + 1 == str->cap)
		return 0;
	char *new_data = realloc(str->data, str->len + 1);
	if (new_data == NULL)
		return -1;
	str->cap=str->len+1;
	return 0;
}

void sstr_reverse(sstr *str) {
	char temp;
	for (size_t i=0;i<str->len/2;i++) {
		temp = str->data[i];
		str->data[i] = str->data[str->len - i - 1];
		str->data[str->len - i - 1]=temp;
	}
}

sstr* sstr_create(const char *s) {
	sstr *str = malloc(sizeof(sstr));
	if (str == NULL)
		return NULL;

	if (s == NULL)
		str->len = 0;
	else
		str->len = strlen(s);

	if (str->len != 0) {
		str->cap = str->len + 1;
		str->data = malloc(sizeof(char) * str->cap);
		if (str->data==NULL) {
			free(str);
			return NULL;
		}
		memcpy(str->data, s, str->len+1);
	} else {
		str->cap = 0;
		str->data = NULL;
	}
	return str;
}

sstr* sstr_create_cap(size_t cap) {
	sstr *str = malloc(sizeof(sstr));
	if (str != NULL) {
		str->len = 0;
		str->cap = cap;

		if (str->cap == 0)
			str->data = NULL;
		else
			str->data = malloc(sizeof(char) * str->cap);

		if (str->data==NULL && str->cap != 0) {
			free(str);
			return NULL;
		}
		if (str->cap > 0)
			str->data[0]='\0';
	}
	return str;
}

int sstr_gets(sstr *str, size_t cap_incr) {
	return sstr_fgetsx(str, stdin, '\n', cap_incr);
}

int sstr_fgets(sstr *str, FILE *stream, size_t cap_incr) {
	return sstr_fgetsx(str, stream, EOF, cap_incr);
}

int sstr_gets2(sstr *str, size_t cap_incr) {
	return sstr_fgetsx2(str, stdin, '\n', cap_incr);
}

int sstr_fgets2(sstr *str, FILE *stream, size_t cap_incr) {
	return sstr_fgetsx2(str, stream, EOF, cap_incr);
}

int sstr_fgetsx(sstr *str, FILE *stream, const char x, size_t cap_incr) {
	char ch;
	size_t necessary_cap;
	while((ch=getc(stream))!=x) {
		if (ch==EOF) {
			if (str->cap == 0)
				return 1;
			str->data[str->len]='\0';
			return 1;
		}
		// new char + null;
		necessary_cap = str->len + 2;
		if (str->cap < necessary_cap) {
			if ((sstr_incr_cap(str, cap_incr)) == -1) {
				str->data[str->len]='\0';
				return -1;
			}
		}
		str->data[str->len]=ch;
		str->len++;
	}
	str->data[str->len]='\0';
	return 0;
}

int sstr_fgetsx2(sstr *str, FILE *stream, const char x, size_t cap_incr) {
	char ch;
	size_t necessary_cap;
	while((ch=getc(stream))!=x) {
		if (ch==EOF) {
			if (str->cap == 0)
				return 1;
			str->data[str->len]='\0';
			return 1;
		}
		// new char + null;
		necessary_cap = str->len + 2;
		if (str->cap < necessary_cap) {
			if ((sstr_incr_cap(str, cap_incr)) == -1) {
				str->data[str->len]='\0';
				return -1;
			}
			cap_incr = cap_incr * 2;
		}
		str->data[str->len]=ch;
		str->len++;
	}
	str->data[str->len]='\0';
	return 0;
}

int sstr_incr_cap(sstr *str, size_t cap) {
	if (cap==0) return 0;
	size_t new_cap = str->cap + cap;
	char *new_data = realloc(str->data, new_cap);
	if (new_data==NULL)
		return -1;
	str->cap=new_cap;
	str->data=new_data;
	return 0;
}

int sstr_cat(sstr *dest, const char *str) {
	size_t len = strlen(str);
	if (len==0) return 0;
	size_t new_len;
	new_len = dest->len + len;
	if (sstr_reserve(dest, new_len + 1) == -1)
		return -1;
	memcpy(&dest->data[dest->len], str, len+1);
	dest->len=new_len;
	return 0;
}		

int sstr_scat(sstr *dest, const sstr *str) {
	if (str->len==0) return 0;
	size_t new_len;
	new_len = dest->len + str->len;
	if (sstr_reserve(dest, new_len + 1) == -1)
		return -1;
	memcpy(&dest->data[dest->len], str->data, str->len+1);
	dest->len=new_len;
	return 0;
}		

void sstr_free(sstr *str) {
	free(str->data);
	free(str);
}

void sstr_print(const sstr *str) {
	printf("str:%p\n", str);
	if (str->len==0)
		printf("\tcap:%zu\n\tlen:%zu\n\tdata:(nil)\n", str->cap, str->len);
	else
		printf("\tcap:%zu\n\tlen:%zu\n\tdata:%s\n", str->cap, str->len, str->data);
}

int sstr_set(sstr *str, const char *s) {
	if (s == NULL) {
		sstr_clear(str);
		return 0;
	}
	size_t len = strlen(s);
	if (sstr_reserve(str, len + 1) == -1)
		return -1;
	memcpy(str->data, s, sizeof(char)*len+1);
	str->len=len;
	return 0;
}

int sstr_sset(sstr *str, const sstr *s) {
	if (sstr_reserve(str, s->len + 1) == -1)
		return -1;
	memcpy(str->data, s->data, sizeof(char)*s->len+1);
	str->len=s->len;
	return 0;
}

void sstr_trim(sstr *str, const char ch) {
	sstr_trim_right(str, ch);
	sstr_trim_left(str, ch);
}

void sstr_trim_right(sstr *str, const char ch) {
	if (str->cap <= 1)
		return;
	if (str->len == 0)
		return;
	size_t i;
	for (i = str->len -1; i != 0; i--) {
		if (str->data[i] != ch) {
			break;
		}
	}
	if (i == 0 && str->data[0] == ch) {
		sstr_clear(str);
		return;
	}
	str->data[i + 1] = '\0';
	str->len = i + 1;
}

void sstr_trim_left(sstr *str, const char ch) {
	if (str->cap <= 1)
		return;
	if (str->len == 0)
		return;
	size_t i;
	for (i = 0; i < str->len; i++) {
		if (str->data[i] != ch) {
			break;
		}
	}
	if (i==str->len) {
		sstr_clear(str);
		return;
	}
	memcpy(str->data, str->data + i, str->len -i + 1);
	str->len -= i;
}

sstr* sstr_fread(FILE *f) {
	fseek(f, 0, SEEK_END);
	long int size = ftell(f);
	if (size == -1)
		return NULL;
	fseek(f, 0, SEEK_SET);
	sstr *str = sstr_create_cap(size+1);
	if (str == NULL)
		return NULL;
	fread(str->data, size, 1, f);
	str->len = size;
	str->data[str->len] = '\0';
	return str;
}

int sstr_ccat(sstr *str, const char ch) {
	// null + ch
	if (sstr_reserve(str, str->len + 2) == -1)
		return -1;
	str->len++;
	str->data[str->len - 1] = ch;
	str->data[str->len] = '\0';
	return 0;
}

int sstr_breakline(sstr *str, size_t count) {

	if (count == 0) {
		return 0;
	}

	size_t addcap=0;
	size_t j=0;
	size_t i;
	// get the amount to increase capacity
	for (i = 0; i < str->len; i++) {
		if (str->data[i] == '\n') {
			j = 0;
		}

		if (j >= count) {
			j = 0;
			addcap++;
		}
		j++;
	}

	if (addcap == 0)
		return 0;

	sstr *new_str = sstr_create_cap(str->len + addcap + 1);

	if (new_str == NULL)
		return -1;

	j=0;

	for (i = 0; i < str->len; i++) {
		if (str->data[i] == '\n') {
			j = 0;
		}

		if (j >= count) {
			j = 0;
			new_str->data[new_str->len] = '\n';
			new_str->len++;
		}
		new_str->data[new_str->len] = str->data[i];
		new_str->len++;
		j++;
	}

	free(str->data);
	str->cap = new_str->cap;
	str->len = new_str->len;
	str->data = new_str->data;
	str->data[str->len+1]='\0';
	free(new_str);

	return 0;
}

int sstr_count_char(const sstr *str, const char ch) {
	int c = 0;
	for (size_t i = 0; i < str->len; i++) {
		if (str->data[i] == ch)
			c++;
	}
	return c;
}

int sstr_reserve(sstr *str, size_t cap) {
	if (cap > str->cap) {
		if (sstr_incr_cap(str, cap - str->cap) == -1)
			return -1;
		return 1;
	}
	return 0;
}

int sstr_printf(sstr *str, const char *fmt, ...) {
	va_list args;
	va_list cp;
	int n;

	va_start(args, fmt);
	va_copy(cp, args);

	n = vsnprintf(str->data + str->len, str->cap - str->len, fmt, cp);
	va_end(cp);

	if (n == -1)
		return -1;

	if ((size_t) n < str->cap - str->len) {
		str->len = n + str->len;
		return 0;
	}

	if (sstr_reserve(str, n + str->len + 1) == -1)
		return -1;

	va_copy(cp, args);

	n = vsnprintf(&str->data[str->len], str->cap - str->len, fmt, args);

	if (n == -1)
		return -1;

	str->len = n + str->len;
	va_end(cp);
	va_end(args);

	return 0;
}

int sstr_replace(sstr *str, const char *old, const char *new) {
	char *tmp;
	char buf[str->len + 1];
	char *ins;
	size_t count = 0;
	size_t oldlen = strlen(old);
	size_t newlen = strlen(new);
	
	if (oldlen == 0)
		return -1;

	if ((strcmp(old, new) == 0))
		return 0;

	count = sstr_count(str, old);
	if (count == 0)
		return 0;

	if ((sstr_reserve(str, str->len + (newlen - oldlen) * count + 1)) == -1)
		return -1;

	memcpy(buf, str->data, str->len + 1);
	ins = buf;

	sstr_clear(str);
	while ((tmp = strstr(ins, old))) {
		sstr_ncat(str, tmp - ins, ins);
		sstr_cat(str, new);
		ins = tmp + oldlen;
	}

	sstr_cat(str, ins);

	return 0;
}

int sstr_ncat(sstr *str, size_t size, const char *src) {
	size_t lensrc = strlen(src);

	if (lensrc == 0)
		return 0;

	if (size == 0)
		return 0;

	if (size > lensrc)
		return -1;

	if (sstr_reserve(str, str->len + size + 1) == -1)
		return -1;
	memcpy(str->data+str->len, src, size);
	str->len += size;
	str->data[str->len] = '\0';
	return 0;
}

bool sstr_contains(const sstr *str, const char *s) {
	if (str->cap == 0)
		return false;
	if ((strstr(str->data, s) == NULL))
		return false;
	return true;
}

int sstr_count(const sstr *str, const char *s) {
	if (str->cap == 0)
		return 0;
	int lens = strlen(s);
	if (lens == 0)
		return 1;
	int count;
	char *ins = str->data;
	char *tmp;
	for (count = 0; (tmp = strstr(ins, s)); count++) {
		ins = tmp + lens;
	}
	return count;
}

int sstr_scount(const sstr *str, const sstr *s) {
	if (str->cap == 0)
		return 0;
	if (s->len == 0)
		return 1;
	int count;
	char *ins = str->data;
	char *tmp;
	for (count = 0; (tmp = strstr(ins, s->data)); count++) {
		ins = tmp + s->len;
	}
	return count;
}

sstr* sstr_read(int fd) {
	sstr *str = sstr_create(NULL);
	if (str == NULL)
		return NULL;

	if (sstr_reserve(str, lseek(fd, 0, SEEK_END)+1) == -1) {
		sstr_free(str);
		return NULL;
	}
	str->len = str->cap - 1;
	lseek(fd, 0, SEEK_SET);
	read(fd, str->data, str->cap - 1);
	str->data[str->len] = '\0';
	return str;
}

int sstr_fdgets(sstr *str, int fd, size_t bufsize, size_t cap_incr) {
	char buf[bufsize];
	size_t rb;
	size_t necessary_cap;
	while(1) {
		rb = read(fd, &buf, bufsize);
		if (rb <= 0)
			break;
		necessary_cap = rb + 1 + str->len;
		if (necessary_cap > str->cap) {
			if (cap_incr > rb + 1) {
				if (sstr_incr_cap(str, cap_incr) == -1) {
					str->data[str->len] = '\0';
					return -1;
				}
			} else {
				if (sstr_incr_cap(str, rb + 1 + cap_incr) == -1) {
					str->data[str->len] = '\0';
					return -1;
				}
			}
		}
		memcpy(&str->data[str->len], buf, rb);
		str->len += rb;
	}
	str->data[str->len] = '\0';
	return 0;
}

int sstr_fdgets2(sstr *str, int fd, size_t bufsize, size_t cap_incr) {
	char buf[bufsize];
	size_t rb;
	size_t necessary_cap;
	while(1) {
		rb = read(fd, &buf, bufsize);
		if (rb <= 0)
			break;
		necessary_cap = rb + 1 + str->len;
		if (necessary_cap > str->cap) {
			if (cap_incr > rb + 1) {
				if (sstr_incr_cap(str, cap_incr) == -1) {
					str->data[str->len] = '\0';
					return -1;
				}
				cap_incr = cap_incr * 2;
			} else {
				if (sstr_incr_cap(str, rb + 1 + cap_incr) == -1) {
					str->data[str->len] = '\0';
					return -1;
				}
				cap_incr = cap_incr * 2;
			}
		}
		memcpy(&str->data[str->len], buf, rb);
		str->len += rb;
	}
	str->data[str->len] = '\0';
	return 0;
}

sstr* sstr_copy(const sstr *str) {
	sstr *s = malloc(sizeof(sstr));
	if (s == NULL)
		return NULL;
	s->len = str->len;
	s->cap = str->cap;
	s->data = malloc(sizeof(s->cap));
	if (s->data == NULL) {
		free(s);
		return NULL;
	}
	memcpy(s->data, str->data, str->len+1);
	return s;
}

void sstr_rm_consecutive_char(sstr *str, const char ch) {
	if (str->cap <= 1)
		return;
	size_t j = 0;
	size_t i;
	bool addchar = true;
	for (i=0;i<str->len;i++) {
		if (str->data[i]==ch) {
			if (addchar==true) {
				addchar=false;
				str->data[j]=str->data[i];
				j++;
			}
		} else {
			addchar=true;
			str->data[j]=str->data[i];
			j++;
		}
	}
	str->len=j;
	str->data[j]='\0';
}

void sstr_init(sstr *str) {
	str->len=0;
	str->cap=0;
	str->data=NULL;
}
