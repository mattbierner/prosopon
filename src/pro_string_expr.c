#include "pro_string_expr.h"

#include "prosopon_stdlib.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#pragma mark Private

static pro_ref string_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_STRING_EXPR_TYPE);
    return pro_string_create(s, t->value.string);
}

static void string_expr_print(pro_state_ref s, const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_STRING_EXPR_TYPE);
    const char* value = t->value.string;
    printf("<string %s>%s", value, end);
}

static void string_expr_release(pro_state_ref s, void* data)
{
    pro_expr* t = data;
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    alloc(t->value.string, 0);
    alloc(t, 0);
}


#pragma mark -
#pragma mark Internal

const pro_expr_type_info pro_string_expr_type_info = {
    .eval = string_expr_eval,
    .print = string_expr_print,
    .release = string_expr_release
};


PRO_INTERNAL pro_ref pro_string_expr_create(pro_state_ref s, char* value)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_STRING_EXPR_TYPE, &t);
    t->value.string = value;
    return ref;
}
