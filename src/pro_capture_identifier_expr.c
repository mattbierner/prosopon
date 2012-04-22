#include "pro_capture_identifier_expr.h"

#include <assert.h>
#include <stdio.h>


static pro_ref identifier_expr_eval(pro_state_ref s, pro_expr* t)
{
    assert(0);
    return PRO_EMPTY_REF;
}

static void identifier_expr_print(pro_state_ref s,
    const pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_CAPTURE_IDENTIFIER_EXPR_TYPE);
    char* value = t->value.identifier;
    
    printf("<capture_identifier %s>\n", value);
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

const pro_expr_type_info pro_capture_identifier_expr_type_info = {
    .eval = identifier_expr_eval,
    .print = identifier_expr_print,
    .release = string_expr_release
};


PRO_INTERNAL pro_ref pro_capture_identifier_expr_create(pro_state_ref s, 
    char* value)
{
    pro_expr* t;
    pro_ref ref = pro_expr_create(s, PRO_CAPTURE_IDENTIFIER_EXPR_TYPE, &t);
    t->value.identifier = value;
    return ref;
}
