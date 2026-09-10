#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char* self;
    size_t length;
} sv_t;

#define shift_cargs(argc, argv) (assert(argc > 0), (argc)--, (argv)++)
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define SV_INIT(s) (sv_t){s, sizeof(*s)*strlen(s)}

#define SV_FORMAT "%.*s"
#define SV_ARGS(s) (s).length, (s).self
#define SV_NULL (sv_t)(NULL, 0) // taken from

#define SV_Q_PRINTLN(s) do {printf(SV_FORMAT"\n", SV_ARGS(s));} while(0)
#define SV_Q_PRINT(s) do {printf(SV_FORMAT, SV_ARGS(s));} while(0)

static inline int sv_maxi(int fx, int sx) {
    if (fx == sx) return fx;
    return fx < sx ? sx : fx;
}

static inline int sv_mini(int fx, int sx) {
    if (fx == sx) return fx;
    return fx > sx ? sx : fx;
}

bool sv_is_empty(sv_t* s) {
    return s->length <= 0;
}

bool sv_char_exists_with_idx(sv_t* s, char c, size_t* idx) {
    if (sv_is_empty(s)) return false;
    for (size_t i = 0; i <= s->length; ++i) {
        if (s->self[i] == c) {
            *idx = i;
            return true;
        }
    }
    *idx = -1;
    return false;
}

bool sv_char_exists_with_idx_back(sv_t* s, char c, size_t* idx) {
    if (sv_is_empty(s)) return false;
    for (int i = s->length - 1; i >= 0; --i) {
        if (s->self[i] == c) {
            if (idx != NULL){
                *idx = i;
            }
            return true;
        }
    }

    if (idx != NULL) {*idx = -1;}
    return false;
}

bool sv_char_exists(sv_t *s, char c) {
    if (sv_is_empty(s)) return false;
    for (size_t i = 0; i <= s->length; ++i) {
        if (s->self[i] == c) return true;
    }

    return false;
}

char sv_get_at_idx(sv_t* s, size_t idx) {
    assert(idx < s->length && "ERROR: index out of range");

    if (sv_is_empty(s)) {
        printf("ERROR: tried sv_get_at_idx() on an empty string!\n");
        printf("file: %s\nline: %d\n", __FILE__, __LINE__);
        return '\0';
    }

    return s->self[idx];
}

void sv_rem_right(sv_t* s){
    if (sv_is_empty(s)) {
        printf("ERROR: tried to sv_rem_left an empty string!\n");
        exit(-1);
    }

    s->length--;
}

void sv_rem_left(sv_t* s) {
    if (sv_is_empty(s)) {
        printf("ERROR: tried to sv_rem_left an empty string!\n");
        exit(-1);
    }

    s->self++;
    s->length--;
}

void sv_cut_by_delim_right(sv_t* s, char c) {
    size_t idx = 0;

    if (sv_is_empty(s)) {
        printf("ERROR: tried to sv_cut_by_delim_right an empty string!\n");
        exit(-1);
    } else if (!sv_char_exists_with_idx_back(s, c, &idx)) {
        printf("ERROR: tried to sv_cut_by_delim_right but no such delimiter exists!\n");
        printf("string: "SV_FORMAT", delimchar: %c\n", SV_ARGS(*s), c);
        exit(-1);
    }

    size_t remcount = s->length - idx - 1;

    for (size_t i = 0; i <= remcount; ++i) {
        sv_rem_right(s);
    }
}

void sv_cut_by_delim_left(sv_t* s, char c) {
    size_t idx = 0;

    if (sv_is_empty(s)) {
        printf("ERROR: tried to sv_cut_by_delim_left an empty string!");
        exit(-1);
    } else if (!sv_char_exists_with_idx(s, c, &idx)) {
        printf("ERROR: tried to sv_cut_by_delim_left but no such delimiter exists!");
        printf("string: "SV_FORMAT"\n, delimchar: %c\n", SV_ARGS(*s), c);
        exit(-1);
    }

    size_t remcount = idx;

    for (size_t i = 0; i <= remcount; ++i) {
        sv_rem_left(s);
    }
}

/* basically string_view.copy(), but has to be freed */
char* sv_to_cstr(sv_t* s) {
    return strndup(s->self, s->length);
}

sv_t sv_copy(sv_t* s) {
    sv_t temp_sv = {
        s->self,
        s->length
    };

    return temp_sv;
}

sv_t sv_substring_from_sv(sv_t* s, size_t fidx, size_t sidx) {
    if (fidx == sidx) {
        // from https://stackoverflow.com/a/22622187
        return (sv_t) SV_INIT((char[2]){s->self[fidx]});
    }

    assert(sidx < s->length && fidx < s->length);

    int maxidx = sv_maxi(fidx, sidx);
    int minidx = sv_mini(fidx, sidx);

    sv_t temp_sv = sv_copy(s);
    
    temp_sv.self += minidx;
    temp_sv.length -= minidx;

    for (int i = s->length; i > maxidx + 1; --i) {
        sv_rem_right(&temp_sv);
    }
    
    return temp_sv;
}

sv_t sv_substring_from_cstr(char* cs, int fidx, int sidx) {
    if (fidx == sidx) {
        // from https://stackoverflow.com/a/22622187
        return SV_INIT((char[2]){cs[fidx]});
    }

    int maxidx = sv_maxi(fidx, sidx);
    int minidx = sv_mini(fidx, sidx);

    sv_t temp_sv = SV_INIT(cs);

    temp_sv.self += minidx;
    temp_sv.length -= minidx;

    for (int i = strlen(cs); i > maxidx + 1; --i) {
        sv_rem_right(&temp_sv);
    }

    return temp_sv;
}

void sv_trim_right(sv_t* s) {
    size_t i = 0;

    while (isspace(s->self[s->length - 1 - i]) != 0) {
        sv_rem_right(s);
        ++i;
    }
}

void sv_trim_left(sv_t* s) {
    while (isspace(s->self[0]) != 0) {
        sv_rem_left(s);
    }
}

void sv_trim(sv_t* s) {
    sv_trim_left(s);
    sv_trim_right(s);
}

void sv_rem_rightn(sv_t* s, int n) {
    for (int i = 0; i < n; ++i) {
        sv_rem_right(s);
    }
}

void sv_rem_leftn(sv_t* s, int n) {
    for (int i = 0; i < n; ++i) {
        sv_rem_left(s);
    }
}

int sv_char_occ_count(sv_t* s, char c) {
    int count = 0;
    for (size_t i = 0; i <= s->length; ++i) {
        if (s->self[i] == c) ++count;
    }
    return count;
}

bool sv_eq(sv_t s1, sv_t s2) {
    assert(s1.length > 0 && s2.length > 0);
    if (s1.length != s2.length) return false;
    if (strncmp(s1.self, s2.self, s1.length + 1) == 0) {
        return true;
    };
    return false;
}

void sv_expect_char_exists(sv_t* s1, char c) {
    assert(sv_char_exists(s1, c));
}

#define SV_FREE(s) do {s.self = NULL; s.length = 0;} while (0);
