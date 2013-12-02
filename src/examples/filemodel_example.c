
#define EFL_EO_API_SUPPORT
#define EFL_BETA_API_SUPPORT

#include <Eo.h>
#include <Eio.h>
#include <Ecore.h>
#include <Elementary.h>

#include <assert.h>

/**
 * @brief The value stored in the nodes of this Model.
 */
struct _Model_File_Value
{
   Eina_Stringshare *filepath;
   Eio_File *file;
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
static Elm_Model_Tree_Path* _model_file_tree_child_append(Eo*, Model_File_Tree*, Elm_Model_Tree_Path*, const char*);
static Eina_Bool _model_file_tree_list(Eo*, Model_File_Tree*, Elm_Model_Tree_Path*);
static Eina_Value* _model_file_tree_value_new(Eo*, Model_File_Tree*, const char*);
static void _model_file_tree_value_free(Eo*, Model_File_Tree*, Eina_Value*);
static void _eio_main_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
static Eina_Bool _eio_filter_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
static void _eio_done_cb(void *data, Eio_File *handler);
static void _eio_error_cb(void *data, Eio_File *handler, int error);
static Eina_Bool _eio_event_cb(void *data, int type, void *event);
static void _eio_value_init(Eina_Value *value, const char *filepath, Eo *object);

#define MODEL_FILE_TREE_CLASS model_file_tree                                          \
 , constructor(model_file_tree_constructor, _model_file_tree_constructor, const char*) \
 , destructor(_model_file_tree_destructor)                                             \
 , function_override(elm_model_tree_child_append, _model_file_tree_child_append)       \
 , function(model_file_tree_list, _model_file_tree_list, Eina_Bool, Elm_Model_Tree_Path*) \
 , function(model_file_tree_value_new, _model_file_tree_value_new, Eina_Value*, const char*) \
 , function(model_file_tree_value_free, _model_file_tree_value_free, void, Eina_Value*)

// TODO: implement model_file_tree_mode_set() and model_file_tree_mode_get().

EO3_DECLARE_CLASS(MODEL_FILE_TREE_CLASS)
EO3_DEFINE_CLASS(MODEL_FILE_TREE_CLASS, ((ELM_MODEL_TREE_CLASS)), Model_File_Tree)

static Eina_Value*
_model_file_tree_value_new(Eo *object, Model_File_Tree *model, const char *path)
{
   static Eina_Value_Struct_Member members[] =
   {
      EINA_VALUE_STRUCT_MEMBER(NULL, Model_File_Value, filepath),
      EINA_VALUE_STRUCT_MEMBER(NULL, Model_File_Value, file),
      EINA_VALUE_STRUCT_MEMBER(NULL, Model_File_Value, monitor),
      EINA_VALUE_STRUCT_MEMBER(NULL, Model_File_Value, handlers)
   };
   static Eina_Value_Struct_Desc desc = 
   {
      EINA_VALUE_STRUCT_DESC_VERSION,
      NULL,                                             // XXX special ops
      members,
      EINA_C_ARRAY_LENGTH(members),
      sizeof(Model_File_Value)
   };
   // non-static initialization
   members[0].type = EINA_VALUE_TYPE_STRINGSHARE;
   members[1].type = EINA_VALUE_TYPE_INT;
   members[2].type = EINA_VALUE_TYPE_INT;
   members[3].type = EINA_VALUE_TYPE_INT;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);
   Eina_Value *value = eina_value_struct_new(&desc);
   Eina_Stringshare *str = eina_stringshare_add(path);
   EINA_SAFETY_ON_NULL_RETURN_VAL(value, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(str, NULL);

   eina_value_struct_set(value, "filepath", str);
   eina_value_struct_set(value, "file", 0);
   eina_value_struct_set(value, "monitor", 0);
   eina_value_struct_set(value, "handlers", 0);
   eina_stringshare_del(str);

   // bind this path to Eio subsystem
   _eio_value_init(value, path, object);

   return value;
}

static void
_model_file_tree_value_free(Eo *object, Model_File_Tree *model, Eina_Value *value)
{
   Eina_Stringshare *filepath = NULL;
   Eio_File *pfile = NULL;
   Eio_Monitor *monitor = NULL;
   Ecore_Event_Handler **handlers = NULL, **ptr = NULL;

   eina_value_struct_pget(value, "filepath", &filepath);
   eina_value_struct_get(value, "file", pfile);
   eina_value_struct_get(value, "monitor", monitor);
   eina_value_struct_get(value, "handlers", handlers);
   eina_stringshare_del(filepath);
   for(ptr = handlers; ptr != NULL; ptr++)
     {
        ecore_event_handler_del(*ptr);
     }
   eio_monitor_del(monitor);
   eina_value_free(value);
   free(pfile);
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
   Eina_Stringshare *filepath = NULL;
   Model_File_Tuple *tuple;
   Eio_File *file = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(object, NULL);

   eo2_do(object, value = elm_model_tree_value_get(node));
   EINA_SAFETY_ON_NULL_RETURN_VAL(value, EINA_FALSE);

   eina_value_struct_pget(value, "filepath", &filepath);
   EINA_SAFETY_ON_NULL_RETURN_VAL(filepath, NULL);

   tuple = malloc(sizeof(Model_File_Tuple));
   EINA_SAFETY_ON_NULL_RETURN_VAL(tuple, NULL);
   eo_ref(object);
   tuple->object = object;
   tuple->node = node;
   file = eio_file_stat_ls(filepath,
                           _eio_filter_cb,
                           _eio_main_cb,
                           _eio_done_cb,
                           _eio_error_cb,
                           tuple);

   eina_value_struct_set(value, "file", (int)file);
   return EINA_TRUE;
}

static Elm_Model_Tree_Path*
_model_file_tree_child_append(Eo *object,
                              Model_File_Tree *model,
                              Elm_Model_Tree_Path *node,
                              const char *filepath)
{
   Eina_Value *value = NULL;
   Elm_Model_Tree_Path *child = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(filepath, NULL);

   eo2_do(object, value = model_file_tree_value_new(filepath));
   EINA_SAFETY_ON_NULL_RETURN_VAL(value, NULL);
   eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                child = elm_model_tree_child_append(node, value));
   EINA_SAFETY_ON_NULL_RETURN_VAL(child, NULL);

   eo2_do(object, model_file_tree_list(child)); // XXX don't.
   return child;
}

////////////////////////////////////////////////////////////////////////////////

static void
_eio_main_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Model_File_Tuple *tuple = (Model_File_Tuple*)data;
   Elm_Model_Tree_Path *child = NULL;
   
   EINA_SAFETY_ON_NULL_RETURN(tuple);
   EINA_SAFETY_ON_NULL_RETURN(info);
   eo2_do(tuple->object, child = elm_model_tree_child_append(tuple->node, info->path));
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
   Eio_Monitor *monitor;
   Ecore_Event_Handler **handlers;
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
   EINA_SAFETY_ON_NULL_RETURN(value);
   EINA_SAFETY_ON_NULL_RETURN(object);

   monitor = eio_monitor_add(filepath);
   eina_value_struct_set(value, "monitor", monitor);

   handlers = malloc(sizeof(Ecore_Event_Handler*)*(nevents+1));
   EINA_SAFETY_ON_NULL_RETURN_VAL(handlers, NULL);
   for(i = 0; i < nevents; i++)
     {
        handlers[i] =
           ecore_event_handler_add(events[i], _eio_event_cb, object);
     }
   eina_value_struct_set(value, "handlers", handlers);
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

////////////////////////////////////////////////////////////////////////////////

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
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   _tree_m = eo2_add_custom(EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                            NULL, model_file_tree_constructor("./"));

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

   eo_unref(_tree_m);
   return 0;
}
ELM_MAIN()
