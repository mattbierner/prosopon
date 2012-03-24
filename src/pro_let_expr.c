#include "pro_let_expr.h"

#include "prosopon.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    pro_env_lookup* env;
    pro_expr* constructor_expr;
    pro_expr* actor_expr;
} constructor_data;

static void bind_arguments(pro_state* s, pro_expr_list* id_list, pro_lookup_list* values)
{
    pro_expr_list* list = id_list;
    pro_lookup_list* lookup_list;
    while (list)
    {
        pro_expr* value = list->value;
        pro_lookup* lookup = lookup_list->value;
        if (value)
        {
            assert(pro_expr_get_type(value) == PRO_IDENTIFIER_EXPR_TYPE);
            pro_bind(s, lookup, value->value.identifier);
        }
        id_list = id_list->next;
        lookup_list = lookup_list->next;
    }
}

static pro_lookup* contructor(pro_state* s, pro_lookup_list* arguments, void* d)
{
    pro_lookup* actor = 0;
    const constructor_data* data = d;
    
    pro_expr* constructor_expr = data->constructor_expr;
    pro_expr* actor_expr = data->actor_expr;

    // Create a new environment and make it current.
    pro_env_lookup* env = pro_env_create(s, data->env);
    pro_push_env(s, env);
    
    // bind all arguments in the new environment
    bind_arguments(s, constructor_expr->value.constructor.arguments, arguments);
    
    // Create a new actor in the new environment.
    actor = pro_actor_create(s);
    pro_actor_become(s, actor, pro_actor_expr_get_behavior(s, actor_expr));

    // Restore the old environment.
    pro_pop_env(s);
    
    return actor;
}

static void let_expr_eval(pro_state* s, pro_expr* t)
{
    assert(pro_expr_get_type(t) == PRO_LET_EXPR_TYPE);
    
    pro_expr* left = t->value.binary.left;
    pro_expr* right = t->value.binary.right;
    
    switch (pro_expr_get_type(left))
    {
    case PRO_IDENTIFIER_EXPR_TYPE:
        pro_eval_expr(s, right);
        pro_bind(s, right->data.lookup, left->value.identifier);
        break;
    case PRO_CONSTRUCTOR_EXPR_TYPE:
    {
        constructor_data* data = malloc(sizeof(*data));
        data->env = pro_get_env(s);
        data->constructor_expr = left;
        data->actor_expr = right;
        pro_constructor_create(s, contructor, data);
    }   break;
    default:
        assert(0);
        break;
    }
}

static void let_expr_print(pro_state* s, pro_expr* t, const char* end)
{
    assert(pro_expr_get_type(t) == PRO_LET_EXPR_TYPE);
    pro_expr* identifier =  t->value.binary.left;
    pro_expr* value = t->value.binary.right;
    
    printf("<let identifier:");
    pro_print_expr(s, identifier, " ");
    printf("value:");
    pro_print_expr(s, value, "");
    printf(">%s", end);
}

const pro_expr_type_info pro_let_expr_type_info = {
    .eval = let_expr_eval,
    .print = let_expr_print
};


PRO_INTERNAL pro_expr* pro_let_expr_create(pro_expr* identifier, pro_expr* value)
{
    pro_type identifier_type = pro_expr_get_type(identifier);
    pro_type value_type = pro_expr_get_type(value);

    switch (identifier_type)
    {
    case PRO_IDENTIFIER_EXPR_TYPE:
        assert(
            value_type == PRO_ACTOR_EXPR_TYPE ||
            value_type == PRO_STRING_EXPR_TYPE ||
            value_type == PRO_NUMBER_EXPR_TYPE);
        break;
    case PRO_CONSTRUCTOR_EXPR_TYPE:
        assert(value_type == PRO_ACTOR_EXPR_TYPE);
        break;
    default:
        assert(0);
        break;
    }
    
    pro_expr* t = pro_expr_create(PRO_LET_EXPR_TYPE);
    t->value.binary.left = identifier;
    t->value.binary.right = value;
    return t;
}
