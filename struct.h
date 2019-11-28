#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef enum bool {
    false,
    true
} bool;

typedef struct case_s {
    int value;
    bool array[9];

} case_t;

#endif