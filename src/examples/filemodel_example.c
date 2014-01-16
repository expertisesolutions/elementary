#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>

#include <Eio.h>
#include <Ecore.h>

#include <assert.h>

/**
 * @brief The value stored in the nodes of this Model.
 */
struct _Model_File_Value
{
   Eina_Stringshare *filepath;
   Eio_File *file;
   Eina_File_Type ftype;
   Eio_Monitor *monitor;
   Ecore_Event_Handler **handlers;
};

typedef struct _Model_File_Value Model_File_Value;

/**
 * @brief The state of this Model.
 */
struct _Model_File_Tree
{
  //Eina_Bool enable_dir_only;
  //Eina_Bool enable_recursive;
};

typedef struct _Model_File_Tree Model_File_Tree;

/**
 * @brief Auxiliary type used to transmit multiple values through @ref
 * eio_file_stat_ls() event callbacks.
 */
struct _Model_File_Tuple
{
  Eo *object;
  Elm_Model_Tree_Path *node;
};

typedef struct _Model_File_Tuple Model_File_Tuple;

static void _model_file_tree_constructor(Eo*, Model_File_Tree*, const char*);
static void _model_file_tree_destructor(Eo*, Model_File_Tree*);
static Elm_Model_Tree_Path* _model_file_tree_child_append(Eo*, Model_File_Tree*, Elm_Model_Tree_Path*, Eina_Value*);
static Eina_Bool _model_file_tree_list(Eo*, Model_File_Tree*, Elm_Model_Tree_Path*);
static Eina_Value* _model_file_tree_value_new(Eo*, Model_File_Tree*, const char*);
static void _eio_main_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
static Eina_Bool _eio_filter_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
static void _eio_done_cb(void *data, Eio_File *handler);
static void _eio_error_cb(void *data, Eio_File *handler, int error);
static Eina_Bool _eio_event_cb(void *data, int type, void *event);
static void _eio_value_init(Eina_Value *value, const char *filepath, Eo *object);

#define MODEL_FILE_TREE_CLASS model_file_tree                                                   \
 , constructor(model_file_tree_constructor, _model_file_tree_constructor, const char*)          \
 , destructor(_model_file_tree_destructor)                                                      \
 , function_override(elm_model_tree_child_append, _model_file_tree_child_append)                \
 , function(model_file_tree_list, _model_file_tree_list, Eina_Bool, Elm_Model_Tree_Path*)       \
 , function(model_file_tree_value_new, _model_file_tree_value_new, Eina_Value*, const char*)

// TODO: implement model_file_tree_mode_set() and model_file_tree_mode_get().

EO3_DECLARE_CLASS(MODEL_FILE_TREE_CLASS)
EO3_DEFINE_CLASS(MODEL_FILE_TREE_CLASS, ((ELM_MODEL_TREE_CLASS)), Model_File_Tree)


static Eina_Bool
_model_file_setup(const Eina_Value_Type *type, void *mem)
{
   assert(type && type->value_size == sizeof(Model_File_Value));
   memset(mem, 0, type->value_size);
   return EINA_TRUE;
}

static Eina_Bool
_model_file_flush(const Eina_Value_Type *type, void *mem)
{
   Model_File_Value *value = (Model_File_Value*)mem;
   Ecore_Event_Handler **ptr;

   EINA_SAFETY_ON_NULL_RETURN_VAL(type, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mem, EINA_FALSE);

   if(value->filepath)
     {
        eina_stringshare_del(value->filepath);
        value->filepath = NULL;
     }
   if(value->monitor)
     {
        eio_monitor_del(value->monitor);
        value->monitor = NULL;
     }
   if(value->handlers)
     {
        for(ptr = value->handlers; ptr != NULL; ptr++)
          {
             ecore_event_handler_del(*ptr);
          }
        free(value->handlers);
        value->handlers = NULL;
     }
   if(value->file)
     {
        free(value->file);
        value->file = NULL;
     }
   return EINA_TRUE;
}

static int
_model_file_compare(const Eina_Value_Type *type, const void *_a, const void *_b)
{
   printf("<D> _model_file_compare()\n");
   Model_File_Value *a = (Model_File_Value*)_a;
   Model_File_Value *b = (Model_File_Value*)_b;
   EINA_SAFETY_ON_NULL_RETURN_VAL(_a, -1);
   EINA_SAFETY_ON_NULL_RETURN_VAL(_b, -1);
   int alen = eina_stringshare_strlen(a->filepath);
   int blen = eina_stringshare_strlen(b->filepath);
   int n = (alen >= blen) ? alen : blen;
   return strncmp(a->filepath, b->filepath, n);
}

static Eina_Bool
_model_file_pset(const Eina_Value_Type *type, void *mem, const void *ptr)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(type, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mem, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ptr, EINA_FALSE);
   *(Model_File_Value*)mem = *(Model_File_Value*)ptr;
   return EINA_TRUE;
}

static Eina_Bool
_model_file_vset(const Eina_Value_Type *type, void *mem, va_list args)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(type, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mem, EINA_FALSE);
   const Model_File_Value x = va_arg(args, Model_File_Value);
   return _model_file_pset(type, mem, &x);
}

static Eina_Bool
_model_file_pget(const Eina_Value_Type *type, const void *mem, void *ptr)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(type, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mem, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ptr, EINA_FALSE);
   assert(type->value_size == sizeof(Model_File_Value));
   memcpy(ptr, mem, type->value_size);
   return EINA_TRUE;
}

static Eina_Bool
_model_file_convert_to(const Eina_Value_Type *type, const Eina_Value_Type *convert, const void *type_mem, void *convert_mem)
{
   Model_File_Value v = *(Model_File_Value*)type_mem;
   eina_error_set(0);
   if (convert == EINA_VALUE_TYPE_STRINGSHARE || convert == EINA_VALUE_TYPE_STRING)
     {
        return eina_value_type_pset(convert, convert_mem, &v.filepath);
     }
   eina_error_set(EINA_ERROR_VALUE_FAILED);
   return EINA_FALSE;
}

static Eina_Value_Type EINA_VALUE_TYPE_FILEMODEL =
{
  EINA_VALUE_TYPE_VERSION,
  sizeof(Model_File_Value),
  "File-Model Type",
  _model_file_setup,
  _model_file_flush,
  NULL, // XXX is non-copyable OK?
  _model_file_compare,
  _model_file_convert_to,
  NULL,
  _model_file_vset,
  _model_file_pset,
  _model_file_pget
};

static Eina_Value*
_model_file_tree_value_new(Eo *object, Model_File_Tree *model, const char *path)
{
   Eina_Value *value = NULL;
   Model_File_Value *ptr = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   value = eina_value_new(&EINA_VALUE_TYPE_FILEMODEL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(value, NULL);
   ptr = calloc(1, sizeof(Model_File_Value));
   if(ptr == NULL)
     {
        eina_value_free(value);
        return NULL;
     }
   ptr->filepath = eina_stringshare_add(path);
   eina_value_pset(value, ptr);

   // register Eio handlers for this path
   _eio_value_init(value, path, object); 
   
   return value;
}

static void
_model_file_tree_constructor(Eo *object, Model_File_Tree *model, const char* filepath)
{
   Eina_Value *value = NULL;
   EINA_SAFETY_ON_NULL_RETURN(filepath);

   eo2_do(object, value = model_file_tree_value_new(filepath));
   eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS), elm_model_tree_constructor(value));
   eo2_do(object, model_file_tree_list(elm_model_tree_path_new_from_string("")));
}

static void
_model_file_tree_destructor(Eo *object, Model_File_Tree *model)
{
   eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS), eo2_destructor());
}

static Eina_Bool
_model_file_tree_list(Eo *object, Model_File_Tree *model, Elm_Model_Tree_Path *node)
{
   Eina_Value *value = NULL;
   Model_File_Tuple *tuple;
   Model_File_Value *ptr;

   EINA_SAFETY_ON_NULL_RETURN_VAL(node, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(object, EINA_FALSE);

   eo2_do(object, value = elm_model_tree_value_get(node));
   EINA_SAFETY_ON_NULL_RETURN_VAL(value, EINA_FALSE);
   ptr = eina_value_memory_get(value);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ptr, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ptr->filepath, EINA_FALSE);

   tuple = malloc(sizeof(Model_File_Tuple));
   EINA_SAFETY_ON_NULL_RETURN_VAL(tuple, EINA_FALSE);
   eo_ref(object);
   tuple->object = object;
   tuple->node = elm_model_tree_path_new_copy(node);
   if(!tuple->node)
     {
       free(tuple);
       eo_unref(object);
       return EINA_FALSE;
     }

   if (ptr->file != NULL)
     {
        free(ptr->file);
     }
   ptr->file = eio_file_stat_ls(ptr->filepath,
                                _eio_filter_cb,
                                _eio_main_cb,
                                _eio_done_cb,
                                _eio_error_cb,
                                tuple);
   return EINA_TRUE;
}

static Elm_Model_Tree_Path*
_model_file_tree_child_append(Eo *object,
                              Model_File_Tree *model,
                              Elm_Model_Tree_Path *node,
                              Eina_Value *value
  )
{
   Elm_Model_Tree_Path *child = NULL;
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(value, NULL);
   eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                child = elm_model_tree_child_append(node, value));
   EINA_SAFETY_ON_NULL_RETURN_VAL(child, NULL);
   eo2_do(object, model_file_tree_list(child));
   return child;
}

////////////////////////////////////////////////////////////////////////////////

static void
_eio_main_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Model_File_Tuple *tuple = (Model_File_Tuple*)data;
   Elm_Model_Tree_Path *child = NULL;
   Eina_Value *value = NULL;
   Model_File_Value *ptr;

   EINA_SAFETY_ON_NULL_RETURN(tuple);
   EINA_SAFETY_ON_NULL_RETURN(info);

   eo2_do(tuple->object, value = model_file_tree_value_new(info->path));
   ptr = eina_value_memory_get(value);
   ptr->ftype = info->type;
   eo2_do(tuple->object, child = elm_model_tree_child_append(tuple->node, value));
   
   printf("+(%s): %s\n", elm_model_tree_path_to_string(child), info->path);
}

static Eina_Bool
_eio_filter_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   return EINA_TRUE; /* i.e., list everything */
}

static void
_eio_done_cb(void *data, Eio_File *handler)
{
   Model_File_Tuple *tuple = (Model_File_Tuple*)data;
   eo_unref(tuple->object);
   elm_model_tree_path_free(tuple->node);
   free(tuple);
}

static void
_eio_error_cb(void *data, Eio_File *handler, int error)
{
   //printf("<E> file model eio error %d\n", error);
}

static Eina_Bool
_eio_event_cb(void *data, int type, void *event)
{
   Eo *object = (Eo*)data;
   EINA_SAFETY_ON_NULL_RETURN_VAL(object, EINA_FALSE);
   if(type == EIO_MONITOR_FILE_CREATED)
     {
       // TODO implement
     }
   else if(type == EIO_MONITOR_FILE_DELETED)
     {
       // TODO implement
     }
   else if(type == EIO_MONITOR_DIRECTORY_CREATED)
     {
       // TODO implement
     }
   else if(type == EIO_MONITOR_DIRECTORY_DELETED)
     {
       // TODO implement
     }
   else if(type == EIO_MONITOR_SELF_RENAME)
     {
       // TODO implement
     }
   else if(type == EIO_MONITOR_SELF_DELETED)
     {
       // TODO implement
     }
   else if(type == EIO_MONITOR_ERROR)
     {
       // TODO implement
     }
   return EINA_TRUE;
}

static void
_eio_value_init(Eina_Value *value, const char *filepath, Eo *object)
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
   int nevents = sizeof(events) / sizeof(events[0]);
   int i = 0;
   Model_File_Value *ptr = NULL;

   EINA_SAFETY_ON_NULL_RETURN(value);
   EINA_SAFETY_ON_NULL_RETURN(object);

   ptr = eina_value_memory_get(value);
   EINA_SAFETY_ON_NULL_RETURN(ptr);
   ptr->monitor = eio_monitor_add(filepath);
   ptr->handlers = malloc(sizeof(Ecore_Event_Handler*)*(nevents+1));
   EINA_SAFETY_ON_NULL_RETURN(ptr->handlers);
   for (i = 0; i < nevents; i++)
     {
        ptr->handlers[i] = ecore_event_handler_add(events[i], _eio_event_cb, object);
     }
}

////////////////////////////////////////////////////////////////////////////////

struct _Model_File_Grid_Cell
{
  // TODO implement.
};

typedef struct _Model_File_Grid_Cell Model_File_Grid_Cell;

struct _Model_File_Grid_Data
{
  // TODO implement
};
typedef struct _Model_File_Grid_Data Model_File_Grid_Data;

void _model_file_grid_constructor(Eo*, Model_File_Grid_Data*, const char*);
void _model_file_grid_construct_from_tree(Eo*, Model_File_Grid_Data*, Eo*);
void _model_file_grid_destructor(Eo*, Model_File_Grid_Data*);
void _model_file_grid_row_select(Eo*, Model_File_Grid_Data*, Elm_Model_Grid_Row);
void _model_file_grid_cell_select(Eo*, Model_File_Grid_Data*, Elm_Model_Grid_Row, Elm_Model_Grid_Column);
Eina_Value* _model_file_grid_value_get(Eo*, Model_File_Grid_Data*, Elm_Model_Grid_Row, Elm_Model_Grid_Column);

#define MODEL_FILE_GRID_CLASS model_file_grid                                                    \
   , constructor(model_file_grid_constructor, _model_file_tree_constructor, const char*)         \
   , constructor(model_file_grid_construct_from_tree, _model_file_grid_construct_from_tree, Eo*) \
   , destructor(_model_file_tree_destructor)                                                     \
   , function_override(elm_model_grid_row_select, _model_file_grid_row_select)                   \
   , function_override(elm_model_grid_cell_select, _model_file_grid_cell_select)                 \
   , function_override(elm_model_grid_value_get, _model_file_grid_value_get)

EO3_DECLARE_CLASS(MODEL_FILE_GRID_CLASS)

EO3_DEFINE_CLASS(MODEL_FILE_GRID_CLASS, ((ELM_MODEL_GRID_CLASS)), Model_File_Grid_Data)

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

Eina_Value*
_model_file_grid_value_get(Eo *obj,
                           Model_File_Grid_Data *self,
                           Elm_Model_Grid_Row row,
                           Elm_Model_Grid_Column column)
{
   return NULL;
}


Evas_Object*
_content_get_cb(Eo *model, Elm_Model_Tree_Path *path, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);

   if(!strcmp(part, "elm.swallow.icon"))
     {
        Eina_Value *value;
        Model_File_Value *ptr;
        eo2_do(model, value = elm_model_tree_value_get(path));
        ptr = eina_value_memory_get(value);
        if(ptr && ptr->ftype == EINA_FILE_DIR)
           elm_icon_standard_set(ic, "folder");
        else
           elm_icon_standard_set(ic, "file");
     }

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;

}


EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   Evas_Object *box;
   Evas_Object *widget;
   Eo *_tree_m; // implements a tree data model over the underlying filesystem
   Eo *_grid_m; // shows the selected group_node content as a grid
   Eo *_tree_v = NULL;
   Eo *_grid_v = NULL;

   ecore_init();
   eio_init();
#if 0
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   _tree_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                            NULL, model_file_tree_constructor("./"));

#else
   win = elm_win_util_standard_add("filemodel", "Filemodel");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   _tree_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                            NULL, model_file_tree_constructor("./"));
   /* _grid_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_GRID_CLASS), */
   /*                          NULL, model_file_grid_construct_from_tree(_tree_m)); */

   _tree_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_TREE_CLASS),
                             NULL, elm_view_tree_add(win, _tree_m));
   /* _grid_v = eo2_add_custom(EO3_GET_CLASS(ELM_VIEW_GRID_CLASS), */
   /*                          NULL, elm_view_grid_add(win, _grid_m)); */

   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
//   eo2_do(_tree_v, elm_view_tree_model_tree_set(ELM_VIEW_TREE_VIEWMODE_ONLYPARENTS)); //hide files, show only directories
   eo2_do(_tree_v, widget = elm_view_tree_evas_object_get());
   eo2_do(_tree_v, elm_view_tree_getcontent_set(_content_get_cb));

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_genlist_mode_set(widget, ELM_LIST_LIMIT);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);

   //file grid widget
/*
   eo2_do(_grid_v, elm_view_grid_evas_object_get(&widget));

   evas_object_size_hint_weight_set(widget, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(widget, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, widget);
   evas_object_show(widget);
*/
   evas_object_resize(win, 800, 400);
   evas_object_show(win);
#endif

   elm_run();
   elm_shutdown();
   eio_shutdown();
   ecore_shutdown();

   eo_unref(_tree_m);
   return 0;
}
ELM_MAIN()
