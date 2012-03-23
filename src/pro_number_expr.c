#include "pro_number_expr.h"

#include "prosopon_libcore.h"

#include <stdio.h>
#include <assert.h>


static void number_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_NUMBER_EXPR_TYPE);
    t->data.lookup = pro_number_create(s, t->value.number);
}

static void number_expr_print(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(s, t) == PRO_NUMBER_EXPR_TYPE);
    double value = t->value.number;
    
    printf("<number %f>", value);
}


const pro_expr_type_info pro_number_expr_type_info = {
    .eval = number_expr_eval,
    .print = number_expr_print
};


PRO_INTERNAL pro_expr* pro_number_expr_create(pro_state* s, double value)
{
    pro_expr* t = pro_expr_create(s, PRO_NUMBER_EXPR_TYPE);
    t->value.number = value;
    return t;
}
