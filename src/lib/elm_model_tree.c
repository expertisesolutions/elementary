#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_tree.h"

#include "elm_model_tree_private.h"

#include <assert.h>

/**
 * @see elm_model_tree_mutable.h
 */
EAPI Eo_Op ELM_MODEL_TREE_MUTABLE_BASE_ID = 0;

#define MY_CLASS ELM_OBJ_TREE_MUTABLE_CLASS

EAPI const Eo_Event_Description _TREE_CHILD_APPEND_EVT = 
   EO_EVENT_DESCRIPTION("child,append","Append a new child cointaining value to the \
                        list of children of the node pointed by path.");

EAPI const Eo_Event_Description _TREE_DELETE_EVT = 
   EO_EVENT_DESCRIPTION("tree,delete","Delete the sub-tree pointed path and all its children.");

EAPI const Eo_Event_Description _TREE_VALUE_SET_EVT = 
   EO_EVENT_DESCRIPTION("value,set","Set value to the node pointed by path");

struct _Elm_Model_Tree
{
   Elm_Model_Tree_Node *root;
   Elm_Model_Tree_Path* selected;
   Eina_Lock lock;
};

typedef struct _Elm_Model_Tree Elm_Model_Tree;


/*
 * Const Class definition
 */

static void
_model_tree_constructor(Eo *object, Elm_Model_Tree *model, Eina_Value *value)
{
   Elm_Model_Tree_Node *root;
   eo2_do_super(object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS), eo2_constructor());

   EINA_SAFETY_ON_NULL_RETURN(model);
   root = _tree_node_append(value, NULL);
   EINA_SAFETY_ON_NULL_RETURN(root);
   model->root = root;
   model->selected = NULL;
   eina_lock_new(&model->lock);
}

static void
_model_tree_destructor(Eo *object, Elm_Model_Tree *model)
{
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   _tree_node_del(model->root);
   elm_model_tree_path_free(model->selected);
   eina_lock_release(&model->lock);
   eina_lock_free(&model->lock);
   eo2_do_super(object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS), eo2_destructor());
}

static Eina_Bool
_model_tree_select(Eo *object, Elm_Model_Tree *model, Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *node;

   EINA_SAFETY_ON_NULL_RETURN_VAL(model, EINA_FALSE);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   model->selected = node ? path : NULL;
   eina_lock_release(&model->lock);
   eo2_do(object, elm_model_tree_select_callback_call(path));
   
   return node ? EINA_TRUE : EINA_FALSE;
}

static Eina_Value*
_model_tree_value_get(Eo *object EINA_UNUSED,
                      Elm_Model_Tree *model,
                      Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *node;
   Eina_Value *value;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   if(node == NULL)
     {
        return NULL;
     }
   value = _tree_node_value_get(node);
   eina_lock_release(&model->lock);
   
   return value;
}

static Eina_List*
_model_tree_children_get(Eo *object EINA_UNUSED,
                         Elm_Model_Tree *model,
                         Elm_Model_Tree_Path *path)
{
   return NULL; // TODO implement
}
 
static Elm_Model_Tree_Path*
_model_tree_selected_get(Eo *object EINA_UNUSED, Elm_Model_Tree *model)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   return model->selected;
}

Eina_Bool
_model_tree_release_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info)
{
}

static void
_model_tree_release(Eo *object, void *_unused EINA_UNUSED, Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = eo_data_scope_get
     (object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   if(path == NULL) return;
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_RETURN(node);
   _tree_node_del(node);
   eina_lock_release(&model->lock);
   //eo2_do(object, elm_model_tree_delete_callback_call(path));
}

EO3_DEFINE_CLASS(ELM_MODEL_TREE_CONST_CLASS, ((EO3_BASE_CLASS)), Elm_Model_Tree)


/*
 * Mutable Class definition
 */
//TODO/FIXME/XXX
static void
_model_tree_child_append(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree_Node *node, *parent;
   Elm_Model_Tree *model = eo_data_scope_get
     (obj, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));

   
   Elm_Model_Tree_Path **ret = va_arg(*list, Elm_Model_Tree_Path **);
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value *value = va_arg(*list, Eina_Value *);


   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   eina_lock_take(&model->lock);
   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(parent, exit_err);
   node = _tree_node_append(value, parent);
   EINA_SAFETY_ON_NULL_GOTO(node, exit_err);
   *ret = _tree_node_path(node);
   eina_lock_release(&model->lock);
   
   eo_do(obj, eo_event_callback_call(TREE_CHILD_APPEND_EVT, *ret, NULL));
   
 exit_err:
   eina_lock_release(&model->lock);
}

static void
_model_tree_child_prepend(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree_Path *ret = NULL;
   Elm_Model_Tree_Node *node, *parent;
   Elm_Model_Tree *model = eo_data_scope_get
     (obj, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(model, NULL);
   eina_lock_take(&model->lock);   
   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_GOTO(parent, exit_err);
   node = _tree_node_prepend(value, parent);
   EINA_SAFETY_ON_NULL_GOTO(node, exit_err);
   ret = _tree_node_path(node);
   eina_lock_release(&model->lock);
   
   eo_do(obj, eo_event_callback_call(TREE_CHILD_APPEND_EVT, ret, NULL));
   
   return ret;

exit_err:
   eina_lock_release(&model->lock);
   return NULL;
}

static void
_model_tree_child_append_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   return NULL; // TODO implement
}

static void
_model_tree_child_prepend_relative(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   return NULL; // TODO implement
}

static void
_model_tree_delete(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
  
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = eo_data_scope_get
     (obj, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *); 

   EINA_SAFETY_ON_NULL_RETURN(path);
   EINA_SAFETY_ON_NULL_RETURN(model);
   eo2_do(obj, elm_model_tree_release(path));
   
   eo_do(obj, eo_event_callback_call(TREE_DELETE_EVT, path, NULL));
}

static void
_model_tree_value_set(Eo *obj EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list)
{
   Elm_Model_Tree_Node *node;
   Elm_Model_Tree *model = eo_data_scope_get(obj, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS));
   Elm_Model_Tree_Path *path = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   
   if(path == NULL) return;
   EINA_SAFETY_ON_NULL_RETURN(model);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   _tree_node_value_set(node, value);
   eina_lock_release(&model->lock);

   //TODO/FIXME <ccarvalho>
   //eo2_do(obj, _model_tree_value_set_callback_call(path, value));   
   //eo_do(obj, eo_event_callback_call(TREE_VALUE_SET_EVT, "x", NULL));
}

static void 
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(ELM_OBJ_MUTABLE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND), _model_tree_child_append),
      EO_OP_FUNC(ELM_OBJ_MUTABLE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND), _model_tree_child_prepend),
      EO_OP_FUNC(ELM_OBJ_MUTABLE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND_RELATIVE), _model_tree_child_append_relative),
      EO_OP_FUNC(ELM_OBJ_MUTABLE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND_RELATIVE), _model_tree_child_prepend_relative),
      EO_OP_FUNC(ELM_OBJ_MUTABLE_ID(ELM_OBJ_MUTABLE_SUB_ID_TREE_VALUE_SET), _model_tree_value_set),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}


static const Eo_Op_Description op_descs[] = {
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND, "Append new child"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND, "Prepend new child"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND_RELATIVE, "Append as a siblind node"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_CHILD_PREPEND_RELATIVE, "Prepend as a sibling node"),
   EO_OP_DESCRIPTION(ELM_OBJ_MUTABLE_SUB_ID_TREE_VALUE_SET, "Set value to the node"),
   EO_OP_DESCRIPTION_SENTINEL
};

static const Eo_Event_Description *event_descs[] = {
   TREE_CHILD_APPEND_EVT,
   TREE_DELETE_EVT,
   TREE_VALUE_SET_EVT,
   NULL
};

static Eo_Class_Description class_descs = {
   EO_VERSION,
   "Model Tree",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&ELM_MODEL_TREE_MUTABLE_BASE_ID, op_descs, ELM_OBJ_MUTABLE_SUB_ID_LAST),
   event_descs, // event_descs
   0, /* sizeof(Elm_View_Tree_Private), */
   _class_constructor,
   NULL
};

EO_DEFINE_CLASS(elm_obj_tree_mutable_class_get, &class_descs, EO_BASE_CLASS, NULL);
//EO_DEFINE_CLASS(elm_obj_tree_mutable_class_get, &class_descs, ELM_MODEL_TREE_CONST_CLASS, NULL);

//EO3_DEFINE_CLASS(ELM_MODEL_TREE_CLASS, ((ELM_MODEL_TREE_CONST_CLASS)), NULL)


