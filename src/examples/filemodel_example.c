
#define EFL_EO_API_SUPPORT
#define EFL_BETA_API_SUPPORT

#include <assert.h>
#include <Eo.h>
#include <Elementary.h>

/*#define ELM_VIEW_TREE_CLASS EO_BASE_CLASS*/
/*#define ELM_VIEW_GRID_CLASS EO_BASE_CLASS*/

typedef enum
{
   ELM_VIEW_TREE_VIEWMODE_GROUP
} Elm_View_Tree_Mode;

Eo* elm_view_tree_add(Evas_Object* parent, Eo* file_model) { return NULL; }
Eo* elm_view_grid_add(Evas_Object* parent, Eo* file_model) { return NULL; }
Eo* elm_view_tree_evas_object_get(Evas_Object** widget) { return NULL; }
Eo* elm_view_grid_evas_object_get(Evas_Object** widget) { return NULL; }
Eo* elm_view_tree_model_tree_set(Elm_View_Tree_Mode mode) { return NULL; }


////////////////////////////////////////////////////////////////////////////////

#include <Eio.h>
#include <Ecore.h>

struct _Model_File_Tree_Node
{
   struct _Model_File_Tree_Node *parent;
   struct _Model_File_Tree_Node *first_child;
   struct _Model_File_Tree_Node *next_sibling;
  
   Eina_Value *value;
   Eio_File *file;                  // XXX: Eina_Value_Struct_Member
   Eio_Monitor *monitor;            // XXX: Eina_Value_Struct_Member
   Ecore_Event_Handler **handlers;  // XXX: Eina_Value_Struct_Member
};

typedef struct _Model_File_Tree_Node Model_File_Tree_Node;

struct _Model_File_Tree_Data
{
   Model_File_Tree_Node *root;     // XXX: Elm_Model_Tree_Iter
   Elm_Model_Tree_Path* selected;
   Eina_Lock lock;
};

typedef struct _Model_File_Tree_Data Model_File_Tree_Data;

struct _Model_File_Tree_Info
{
   Eo *object;
   Model_File_Tree_Data *data;
   Model_File_Tree_Node *node;
};

typedef struct _Model_File_Tree_Info Model_File_Tree_Info;

static void _model_file_tree_constructor(Eo*, Model_File_Tree_Data*, const char*);
static void _model_file_tree_destructor(Eo*, Model_File_Tree_Data*);
static void _model_file_tree_destructor(Eo*, Model_File_Tree_Data*);
static void _model_file_tree_select(Eo*, Model_File_Tree_Data *, Elm_Model_Tree_Path*);
static Eina_Value* _model_file_tree_value_get(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*);
static Elm_Model_Tree_Path* _model_file_tree_root_get(Eo*, Model_File_Tree_Data*);
static Eina_List* _model_file_tree_children_get(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*);
static Eina_List* _model_file_tree_children_get(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*);
static Elm_Model_Tree_Path* _model_file_tree_selected_get(Eo*, Model_File_Tree_Data*);

static Elm_Model_Tree_Path* _model_file_tree_child_append(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*, Eina_Value*);
static Elm_Model_Tree_Path* _model_file_tree_child_prepend(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*, Eina_Value*);
static Elm_Model_Tree_Path* _model_file_tree_child_append_relative(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*, Eina_Value*);
static Elm_Model_Tree_Path* _model_file_tree_child_prepend_relative(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*, Eina_Value*);
static void _model_file_tree_delete(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*);
static void _model_file_tree_value_set(Eo*, Model_File_Tree_Data*, Elm_Model_Tree_Path*, Eina_Value*);

static Eina_Bool _eio_filter_cb(void*, Eio_File*, const Eina_File_Direct_Info*);
static void _eio_main_cb(void*, Eio_File*, const Eina_File_Direct_Info*);
static void _eio_done_cb(void*, Eio_File*);
static void _eio_error_cb(void*, Eio_File*, int);
static Eina_Bool _eio_event_cb(void*, int, void*);
//static void _eio_stat_cb(void*, Eio_File*, const Eina_Stat*);

#define MODEL_FILE_TREE_CLASS model_file_tree                                             \
    , constructor(model_file_tree_constructor, _model_file_tree_constructor, const char*) \
    , destructor(_model_file_tree_destructor)                                             \
    , function_override(elm_model_tree_select, _model_file_tree_select)                   \
    , function_override(elm_model_tree_value_get, _model_file_tree_value_get)             \
    , function_override(elm_model_tree_root_get, _model_file_tree_root_get)               \
    , function_override(elm_model_tree_children_get, _model_file_tree_children_get)       \
    , function_override(elm_model_tree_selected_get, _model_file_tree_selected_get)       \
    , function_override(elm_model_tree_child_append, _model_file_tree_child_append)       \
    , function_override(elm_model_tree_child_prepend, _model_file_tree_child_prepend)     \
    , function_override(elm_model_tree_child_append_relative, _model_file_tree_child_append_relative)   \
    , function_override(elm_model_tree_child_prepend_relative, _model_file_tree_child_prepend_relative) \
    , function_override(elm_model_tree_delete, _model_file_tree_delete)                   \
    , function_override(elm_model_tree_value_set, _model_file_tree_value_set)

EO3_DECLARE_CLASS(MODEL_FILE_TREE_CLASS)

EO3_DEFINE_CLASS(MODEL_FILE_TREE_CLASS,
                 ((EO3_BASE_CLASS),(ELM_MODEL_TREE_INTERFACE)),
                 Model_File_Tree_Data)


////////////////////////////////////////////////////////////////////////////////

// XXX: Eina_Bool elm_model_tree_value_set(Elm_Model_Tree_Iter *iter, Eina_Value *value)
// XXX: Eina_Bool elm_model_tree_value_set(Elm_Model_Tree_Path *path, Eina_Value *value)
static inline Eina_Bool
_node_value_add(Model_File_Tree_Node *node,
                const char *path)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(path, EINA_FALSE);
   node->value = eina_value_new(EINA_VALUE_TYPE_STRINGSHARE);
   eina_value_set(node->value, eina_stringshare_add(path));
   return EINA_TRUE;
}

// XXX: Eina_Value* elm_model_tree_value_get(Elm_Model_Tree_Iter *iter)
static inline Eina_Stringshare*
_node_value_get(Model_File_Tree_Node *node)
{
   Eina_Stringshare *ptr = NULL;
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);   
   eina_value_pget(node->value, &ptr);
   assert(ptr);
   return ptr;
}

// XXX: part of elm_model_tree_delete
static inline void
_node_value_del(Model_File_Tree_Node *node)
{
   assert(node != NULL);
   eina_stringshare_del(_node_value_get(node));
   eina_value_free(node->value);
}

// XXX: user-defined function override of elm_model_tree_add
static inline Eina_Bool
_node_monitor_add(Model_File_Tree_Node *node)
{
   int events[] = {
      EIO_MONITOR_FILE_CREATED,
      EIO_MONITOR_FILE_DELETED,
      EIO_MONITOR_DIRECTORY_CREATED,
      EIO_MONITOR_DIRECTORY_DELETED,
      EIO_MONITOR_SELF_RENAME,
      EIO_MONITOR_SELF_DELETED,
      EIO_MONITOR_ERROR
   };
   size_t i, nevents = sizeof(events) / sizeof(events[0]);
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, EINA_FALSE);
   node->monitor = eio_monitor_stringshared_add(_node_value_get(node));
   assert(node->monitor);
   node->handlers = malloc(sizeof(Ecore_Event_Handler*)*(nevents+1));
   EINA_SAFETY_ON_NULL_RETURN_VAL(node->handlers, EINA_FALSE);
   
   for(i = 0; i < nevents; i++)
     {
        node->handlers[i] =
          ecore_event_handler_add(events[i], _eio_event_cb, node);
     }

   node->handlers[i] = NULL;
   
   return EINA_TRUE;
}

// XXX: part of user-define function override of elm_model_tree_delete
static inline void
_node_monitor_del(Model_File_Tree_Node *node)
{
   Ecore_Event_Handler **ptr;
   assert(node != NULL);

   for(ptr = node->handlers; ptr != NULL; ptr++)
     {
        ecore_event_handler_del(*ptr);
     }

   assert(node->monitor != NULL);
   eio_monitor_del(node->monitor);
}

// XXX: Elm_Model_Tree_Iter* elm_model_tree_append(Elm_Model_Tree_Iter*, Eina_Value*)
static inline Model_File_Tree_Node*
_node_add(const char *filename, Model_File_Tree_Node *parent)
{
   Model_File_Tree_Node *node = malloc(sizeof(Model_File_Tree_Node));
   Model_File_Tree_Node *ptr;

   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);

   node->first_child = NULL;
   node->next_sibling = NULL;
   
   _node_value_add(node, filename);
   _node_monitor_add(node);

   if(parent)
     {
        if(parent->first_child)
          {
             for(ptr = parent->first_child;
                 ptr->next_sibling != NULL;
                 ptr = ptr->next_sibling)
                ;
             ptr->next_sibling = node;
          }
        else
          {
             parent->first_child = node;
          }
     }

   node->parent = parent;
   return node;
}

// XXX: void elm_model_tree_delete(Elm_Model_Tree_Iter*)
static inline void
_node_del(Model_File_Tree_Node *node)
{
   Model_File_Tree_Node *child, *tmp;
   if(node->first_child != NULL)
     {
        for(child = node->first_child; child != NULL; child = tmp)
          {
             tmp = child->next_sibling;
             _node_del(child);
          }
     }
   _node_value_del(node);
   _node_monitor_del(node);
   //eio_file_free(node->file); // XXX
   free(node);
}

// XXX: user-defined, shall be invoked from overridden elm_model_tree_{append,select,...?}
static inline void
_node_flush(Eo *object, Model_File_Tree_Data *data, Model_File_Tree_Node *node)
{
   Model_File_Tree_Info *info = malloc(sizeof(Model_File_Tree_Info));

   info->object = object;
   info->data = data;
   info->node = node;
   
   node->file = eio_file_stat_ls(_node_value_get(node),
                                 _eio_filter_cb,
                                 _eio_main_cb,
                                 _eio_done_cb,
                                 _eio_error_cb,
                                 info);
}

/// XXX: Elm_Model_Tree_Iter* elm_model_tree_iter_get(Elm_Model_Tree_Iter *path)
static inline Model_File_Tree_Node*
_node_find(Model_File_Tree_Data *model, Elm_Model_Tree_Path *path)
{
   Model_File_Tree_Node *ptr, *child;
   unsigned int i, depth;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   depth = elm_model_tree_path_get_depth(path);
   if(!depth) return NULL;

   for(ptr = model->root, i = 0; i < depth; i++)
     {
        for(child = ptr->first_child; child != NULL; child = child->next_sibling)
           ;
        EINA_SAFETY_ON_FALSE_RETURN_VAL(child != NULL, NULL);
        ptr = child;
     }
   return ptr;
}

// XXX: part of elm_model_tree_path_get (see bellow)
static inline unsigned int
_node_index(Model_File_Tree_Node *node)
{
   Model_File_Tree_Node *child;
   Eina_List *l;
   unsigned int index = 0;
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, (unsigned int)-1);
   if(node->parent == NULL) return (unsigned int)-1;
   assert(node->parent->first_child);

   for(child = node->parent->first_child; child != NULL; child = child->next_sibling)
     {
        if(child == node)
          {
             return index;
          }
        index++;
     }
   return (unsigned int)-1;
}

/// XXX: Elm_Model_Tree_Path* elm_model_tree_path_get(Elm_Model_Tree_Iter *iter)
static inline Elm_Model_Tree_Path*
_node_path(Model_File_Tree_Node *node)
{
   Elm_Model_Tree_Path *path;
   Model_File_Tree_Node *ptr;
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   path = elm_model_tree_path_new();
   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);

   for(ptr = node; ptr->parent != NULL; ptr = ptr->parent)
     {
        elm_model_tree_path_prepend_index(path, _node_index(ptr));
     }
   return path;
}

static Elm_Model_Tree_Path*
_model_file_tree_child_append(Eo *obj,
                              Model_File_Tree_Data *self,
                              Elm_Model_Tree_Path *path,
                              Eina_Value *value)
{
   return NULL;
}

static Elm_Model_Tree_Path*
_model_file_tree_child_prepend(Eo *obj,
                               Model_File_Tree_Data *self,
                               Elm_Model_Tree_Path *path,
                               Eina_Value *value)
{
   return NULL;
}

static Elm_Model_Tree_Path*
_model_file_tree_child_append_relative(Eo *obj,
                                       Model_File_Tree_Data *self,
                                       Elm_Model_Tree_Path *path,
                                       Eina_Value *value)
{
   return NULL;
}

static Elm_Model_Tree_Path*
_model_file_tree_child_prepend_relative(Eo *obj,
                                        Model_File_Tree_Data *self,
                                        Elm_Model_Tree_Path *path,
                                        Eina_Value *value)
{
   return NULL;
}

static void
_model_file_tree_delete(Eo *obj, Model_File_Tree_Data *self, Elm_Model_Tree_Path *path)
{
}

static void
_model_file_tree_value_set(Eo *obj,
                           Model_File_Tree_Data *self,
                           Elm_Model_Tree_Path *path,
                           Eina_Value *value)
{
}

static void
_model_file_tree_constructor(Eo *obj,
                             Model_File_Tree_Data *self,
                             const char* root)
{
   Model_File_Tree_Node *node;
   assert(self);
   assert(root);
   eo2_do_super(obj, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS), eo2_constructor());

   eina_lock_new(&self->lock);   
   node = _node_add(root, NULL);
   assert(node);
   self->root = node;

   eina_lock_take(&self->lock);
   eo_ref(obj);
   _node_flush(obj, self, node);
   eina_lock_release(&self->lock);
}

static void
_model_file_tree_destructor(Eo *obj, Model_File_Tree_Data *self)
{
   assert(self);
   _node_del(self->root);
   eina_lock_free(&self->lock);
}

static void
_model_file_tree_select(Eo *obj,
                        Model_File_Tree_Data *self,
                        Elm_Model_Tree_Path *path)
{
   Model_File_Tree_Node *node;
   
   assert(path != NULL);
   assert(self);
   eina_lock_take(&self->lock);
   node = _node_find(self, path);
   EINA_SAFETY_ON_NULL_RETURN(node);
   self->selected = path;
   if(node->first_child == NULL)
     {
        _node_flush(obj, self, node);
     }
   eina_lock_release(&self->lock);
   eo2_do(obj, elm_model_tree_select_callback_call(path));
}

static Eina_Value*
_model_file_tree_value_get(Eo *obj,
                           Model_File_Tree_Data *self,
                           Elm_Model_Tree_Path *path)
{
   eina_lock_take(&self->lock);
   assert(_node_find(self, path) != NULL);
   self->selected = path;
   eina_lock_release(&self->lock);
   return NULL;
}

static Elm_Model_Tree_Path*
_model_file_tree_root_get(Eo *obj, Model_File_Tree_Data *self)
{
   assert(self != NULL);
   return self->root;
}

// XXX: Elm_Model_Tree_Iter* _model_file_tree_children_get(...)
static Eina_List*
_model_file_tree_children_get(Eo *obj,
                              Model_File_Tree_Data *self,
                              Elm_Model_Tree_Path *path)
{
   #if 0
   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   Model_File_Tree_Node *node = _node_find(self, path);
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   return node->first_children;
   #endif
   return NULL;
}

static Elm_Model_Tree_Path*
_model_file_tree_selected_get(Eo *obj, Model_File_Tree_Data *self)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(self, NULL);
   return self->selected;
}

/* static Elm_Model_Tree_Path* */
/* _model_file_tree_child_append(Eo *obj, Model_File_Tree_Data *self, Elm_Model_Tree_Path *path, Eina_Value *value) */
/* { */
/*    return NULL; */
/* } */

static Eina_Bool
_eio_filter_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   return EINA_TRUE; /* i.e., list everything */
}

static void
_eio_main_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Model_File_Tree_Info *model = (Model_File_Tree_Info*)data;
   Model_File_Tree_Node *node;
   Elm_Model_Tree_Path *path;
   
   assert(model);
   assert(model->object);
   assert(model->data);
   assert(model->node);

   eina_lock_take(&model->data->lock);
   node = _node_add(info->path, model->node);
   path = _node_path(node);
   eina_lock_release(&model->data->lock);

   eo2_do(model->object, elm_model_tree_child_append_callback_call(path));
}

static void
_eio_done_cb(void *data, Eio_File *handler)
{
   Model_File_Tree_Info *model = (Model_File_Tree_Info*)data;
   eo_unref(model->object);
   free(data);
}

static void
_eio_error_cb(void *data, Eio_File *handler, int error)
{
   Model_File_Tree_Data *self = (Model_File_Tree_Data*)data;
   printf("file: error! (code=%d)\n", error);
}

static Eina_Bool
_eio_event_cb(void *data, int type, void *event)
{
   Model_File_Tree_Node *node;

   EINA_SAFETY_ON_NULL_RETURN_VAL(data, EINA_FALSE);
   node = (Model_File_Tree_Node*)data;
   
   if(type == EIO_MONITOR_FILE_CREATED)
     {
     }
   else if(type == EIO_MONITOR_FILE_DELETED)
     {
     }
   else if(type == EIO_MONITOR_DIRECTORY_CREATED)
     {
     }
   else if(type == EIO_MONITOR_DIRECTORY_DELETED)
     {
     }
   else if(type == EIO_MONITOR_SELF_RENAME)
     {
     }
   else if(type == EIO_MONITOR_SELF_DELETED)
     {
     }
   else if(type == EIO_MONITOR_ERROR)
     {
     }
   return EINA_TRUE;
}

////////////////////////////////////////////////////////////////////////////////

struct _Model_File_Grid_Cell
{
   Eina_Value *value;
};

typedef struct _Model_File_Grid_Cell Model_File_Grid_Cell;

struct _Model_File_Grid_Data
{
   Elm_Model_Grid_Row selected_row;
   Elm_Model_Grid_Column selected_column;
   Eina_List **grid;
   Eina_Lock lock;
};
typedef struct _Model_File_Grid_Data Model_File_Grid_Data;

void _model_file_grid_constructor(Eo*, Model_File_Grid_Data*, const char*);
void _model_file_grid_construct_from_tree(Eo*, Model_File_Grid_Data*, Eo*);
void _model_file_grid_destructor(Eo*, Model_File_Grid_Data*);
void _model_file_grid_row_select(Eo*, Model_File_Grid_Data*, Elm_Model_Grid_Row);
void _model_file_grid_cell_select(Eo*, Model_File_Grid_Data*, Elm_Model_Grid_Row, Elm_Model_Grid_Column);
Eina_List* _model_file_grid_columns_get(Eo*, Model_File_Grid_Data*);
Eina_List* _model_file_grid_columns_get(Eo*, Model_File_Grid_Data*);
int _model_file_grid_rows_count(Eo*, Model_File_Grid_Data*);
int _model_file_grid_columns_count(Eo*, Model_File_Grid_Data*);
Elm_Model_Grid_Row _model_file_grid_selected_row_get(Eo*, Model_File_Grid_Data*);
Elm_Model_Grid_Column _model_file_grid_selected_column_get(Eo*, Model_File_Grid_Data*);
Eina_Value* _model_file_grid_value_get(Eo*, Model_File_Grid_Data*, Elm_Model_Grid_Row, Elm_Model_Grid_Column);

#define MODEL_FILE_GRID_CLASS model_file_grid                                                    \
   , constructor(model_file_grid_constructor, _model_file_tree_constructor, const char*)         \
   , constructor(model_file_grid_construct_from_tree, _model_file_grid_construct_from_tree, Eo*) \
   , destructor(_model_file_tree_destructor)                                                     \
   , function_override(elm_model_grid_row_select, _model_file_grid_row_select)                   \
   , function_override(elm_model_grid_rows_count, _model_file_grid_rows_count)                   \
   , function_override(elm_model_grid_cell_select, _model_file_grid_cell_select)                 \
   , function_override(elm_model_grid_columns_get, _model_file_grid_columns_get)                 \
   , function_override(elm_model_grid_columns_count, _model_file_grid_columns_count)             \
   , function_override(elm_model_grid_selected_row_get, _model_file_grid_selected_row_get)       \
   , function_override(elm_model_grid_selected_column_get, _model_file_grid_selected_column_get) \
   , function_override(elm_model_grid_value_get, _model_file_grid_value_get)

EO3_DECLARE_CLASS(MODEL_FILE_GRID_CLASS)

EO3_DEFINE_CLASS(MODEL_FILE_GRID_CLASS,
                 ((EO3_BASE_CLASS),(ELM_MODEL_GRID_INTERFACE)),
                 Model_File_Grid_Data)

void
_model_file_grid_constructor(Eo *obj, Model_File_Grid_Data *self, const char *root)
{
}

void
_model_file_grid_construct_from_tree(Eo *obj, Model_File_Grid_Data *self, Eo *tree)
{
}

void
_model_file_grid_destructor(Eo *obj, Model_File_Grid_Data *self)
{
}

void
_model_file_grid_row_select(Eo *obj, Model_File_Grid_Data *self, Elm_Model_Grid_Row row)
{
}
  
void
_model_file_grid_cell_select(Eo *obj,
                             Model_File_Grid_Data *self,
                             Elm_Model_Grid_Row row,
                             Elm_Model_Grid_Column column)
{
}

Eina_List*
_model_file_grid_columns_get(Eo *obj, Model_File_Grid_Data *self)
{
   return NULL;
}

int
_model_file_grid_rows_count(Eo *obj, Model_File_Grid_Data *self)
{
   return -1;
}

int
_model_file_grid_columns_count(Eo *obj, Model_File_Grid_Data *self)
{
   return -1;
}

Elm_Model_Grid_Row
_model_file_grid_selected_row_get(Eo *obj, Model_File_Grid_Data *self)
{
   return (Elm_Model_Grid_Row)-1;
}

Elm_Model_Grid_Column
_model_file_grid_selected_column_get(Eo *obj, Model_File_Grid_Data *self)
{
   return (Elm_Model_Grid_Column)-1;
}

Eina_Value*
_model_file_grid_value_get(Eo *obj,
                           Model_File_Grid_Data *self,
                           Elm_Model_Grid_Row row,
                           Elm_Model_Grid_Column column)
{
   return NULL;
}

////////////////////////////////////////////////////////////////////////////////

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   Evas_Object *box;
   Evas_Object *widget;
   Eo *_tree_m; //implements file as a tree data model
   Eo *_grid_m; //shows the selected group_node content as a grid
   Eo *_tree_v = NULL;
   Eo *_grid_v = NULL;
   
   ecore_init();
   eio_init();
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   _tree_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                            NULL, model_file_tree_constructor("/tmp"));
#if 0   
   win = elm_win_util_standard_add("filemodel", "Filemodel");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   _tree_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                            NULL, model_file_tree_constructor("/tmp"));
   _grid_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_GRID_CLASS),
                            NULL, model_file_grid_construct_from_tree(_tree_m));

   /* _tree_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_TREE_CLASS), */
   /*                          NULL, elm_view_tree_add(win, _tree_m)); */
   /* _grid_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_GRID_CLASS), */
   /*                          NULL, elm_view_grid_add(win, _grid_m)); */
   
   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
   eo2_do(_tree_v, elm_view_tree_model_tree_set(ELM_VIEW_TREE_VIEWMODE_GROUP)); //hide files, show only directories
   eo2_do(_tree_v, elm_view_tree_evas_object_get(&widget));

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_genlist_mode_set(widget, ELM_LIST_LIMIT);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);

   //file grid widget
   eo2_do(_grid_v, elm_view_grid_evas_object_get(&widget));

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);
 
   evas_object_resize(win, 800, 400);
   evas_object_show(win);
#endif
   
   elm_run();
   elm_shutdown();

   eio_shutdown();
   ecore_shutdown();
   
   return 0;
}
ELM_MAIN()
