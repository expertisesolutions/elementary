#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif
#include <Elementary.h>

#include "elm_priv.h"
#include "elm_model_tree.h"
#include <assert.h>

#include "elm_model_tree_private.h"

struct _Elm_Model_Tree
{
   Elm_Model_Tree_Node *root;
   Elm_Model_Tree_Path* selected;
   Eina_Lock lock;
};

typedef struct _Elm_Model_Tree Elm_Model_Tree;

/// Const Class Functions ///

static void
_model_tree_constructor(Eo *object, Elm_Model_Tree *model, Eina_Value *value)
{
   Elm_Model_Tree_Node *root;
   eo2_do_super(object, EO3_GET_CLASS(ELM_MODEL_TREE_CONST_CLASS), eo2_constructor());

   assert(model != NULL);
   root = _tree_node_append(value, NULL);
   assert(root != NULL);
   model->root = root;
   model->selected = NULL;
   eina_lock_new(&model->lock);
}

static void
_model_tree_destructor(Eo *object, Elm_Model_Tree *model)
{
   assert(model != NULL);
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
   assert(model != NULL);

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
   assert(model != NULL);
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
   assert(model != NULL);
   return model->selected;
}

/// Mutable Class Functions ///

static Elm_Model_Tree_Path*
_model_tree_child_append(Eo *object, Elm_Model_Tree *model,
                         Elm_Model_Tree_Path *path, Eina_Value *value)
{
   Elm_Model_Tree_Path *ret = NULL;
   Elm_Model_Tree_Node *node, *parent;

   assert(model != NULL);
   eina_lock_take(&model->lock);
   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   node = _tree_node_append(value, parent);
   ret = _tree_node_path(node);
   eina_lock_release(&model->lock);
   // TODO: raise event
   return ret;
}

static Elm_Model_Tree_Path*
_model_tree_child_prepend(Eo *object, Elm_Model_Tree *model,
                          Elm_Model_Tree_Path *path, Eina_Value *value)
{
   Elm_Model_Tree_Path *ret = NULL;
   Elm_Model_Tree_Node *node, *parent;

   assert(model != NULL);
   eina_lock_take(&model->lock);   
   parent = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   node = _tree_node_prepend(value, parent);
   ret = _tree_node_path(node);
   eina_lock_release(&model->lock);
   // TODO: raise event.
   return ret;
}

static Elm_Model_Tree_Path*
_model_tree_child_append_relative(Eo *object, Elm_Model_Tree *model,
                                  Elm_Model_Tree_Path *path, Eina_Value *value)
{
   return NULL; // TODO implement
}

static Elm_Model_Tree_Path*
_model_tree_child_prepend_relative(Eo *object, Elm_Model_Tree *model,
                                   Elm_Model_Tree_Path *path, Eina_Value *value)
{
   return NULL; // TODO implement
}

static void
_model_tree_delete(Eo *object,
                   Elm_Model_Tree *model,
                   Elm_Model_Tree_Path *path)
{
   Elm_Model_Tree_Node *node;
   if(path == NULL) return;
   assert(model != NULL);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   EINA_SAFETY_ON_NULL_RETURN(node);
   _tree_node_del(node);
   eina_lock_release(&model->lock);
   // TODO: raise event.
}

static void
_model_tree_value_set(Eo *object, Elm_Model_Tree *model,
                      Elm_Model_Tree_Path *path, Eina_Value *value)
{
   Elm_Model_Tree_Node *node;
   if(path == NULL) return;
   assert(model != NULL);
   eina_lock_take(&model->lock);
   node = _tree_node_find(model->root, path);
   _tree_node_value_set(node, value);
   eina_lock_release(&model->lock);
   // TODO: raise event.
}

/// Class definition ///

EO3_DEFINE_CLASS(ELM_MODEL_TREE_CONST_CLASS, ((EO3_BASE_CLASS)), Elm_Model_Tree)

EO3_DEFINE_CLASS(ELM_MODEL_TREE_CLASS, ((ELM_MODEL_TREE_CONST_CLASS)), Elm_Model_Tree)
