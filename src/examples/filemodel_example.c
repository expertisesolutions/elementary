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

static void _model_file_tree_constructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list);

//static void _model_file_tree_constructor(Eo*, Model_File_Tree*, const char*);
static void _model_file_tree_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list);

//static Elm_Model_Tree_Path* _model_file_tree_child_append(Eo*, Model_File_Tree*, Elm_Model_Tree_Path*, Eina_Value*);
static void _model_file_tree_child_append(Eo *obj EINA_UNUSED, void *class_data, va_list *list);

//static Eina_Bool _model_file_tree_list(Eo*, Model_File_Tree*, Elm_Model_Tree_Path*);

static void _model_file_tree_list(Eo *obj EINA_UNUSED, void *class_data, va_list *list);
//static Eina_Value* _model_file_tree_value_new(Eo*, Model_File_Tree*, const char*);


static void _model_file_tree_value_new(Eo *obj EINA_UNUSED, void *class_data, va_list *list);

static void _eio_main_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
static Eina_Bool _eio_filter_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info);
static void _eio_done_cb(void *data, Eio_File *handler);
static void _eio_error_cb(void *data, Eio_File *handler, int error);
static Eina_Bool _eio_event_cb(void *data, int type, void *event);
static void _eio_value_init(Eina_Value *value, const char *filepath, Eo *object);

#if 0
#define MODEL_FILE_TREE_CLASS model_file_tree                                                   \
 , constructor(model_file_tree_constructor, _model_file_tree_constructor, const char*)          \
 , destructor(_model_file_tree_destructor)                                                      \
 , function_override(elm_model_tree_child_append, _model_file_tree_child_append)                \
 , function(model_file_tree_list, _model_file_tree_list, Eina_Bool, Elm_Model_Tree_Path*)       \
 , function(model_file_tree_value_new, _model_file_tree_value_new, Eina_Value*, const char*)

// TODO: implement model_file_tree_mode_set() and model_file_tree_mode_get().

EO3_DECLARE_CLASS(MODEL_FILE_TREE_CLASS)
EO3_DEFINE_CLASS(MODEL_FILE_TREE_CLASS, ((ELM_MODEL_TREE_CLASS)), Model_File_Tree) 
#endif

/**
 * <ccarvalho>
 *
 */
#define __MODEL_FILE_TREE_CLASS

#ifdef __MODEL_FILE_TREE_CLASS

EAPI Eo_Op MODEL_FILE_TREE_CLASS_BASE_ID = 0;

enum {
   MODEL_FILE_TREE_SUB_ID_CONSTRUCTOR,
   //MODEL_FILE_TREE_SUB_ID_DESTRUCTOR,
   MODEL_FILE_TREE_SUB_ID_LIST,
   MODEL_FILE_TREE_SUB_ID_VALUE_NEW,
   MODEL_FILE_TREE_SUB_ID_LAST

};

#define MODEL_FILE_TREE_CLASS_ID(sub_id) (MODEL_FILE_TREE_CLASS_BASE_ID + sub_id)

#define model_file_tree_constructor(filepath) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(const char *, filepath) 
//#define model_file_tree_destructor() MODEL_FILE_TREE_CLASS_ID(MODEL_FILE_TREE_SUB_ID_DESTRUCTOR)
#define model_file_tree_list(model, node, ret) MODEL_FILE_TREE_CLASS_ID(MODEL_FILE_TREE_SUB_ID_LIST), EO_TYPECHECK(Model_File_Tree *, model), EO_TYPECHECK(Elm_Model_Tree_Path *, node), EO_TYPECHECK(Eina_Bool *, ret)
#define model_file_tree_value_new(model, path, ret) MODEL_FILE_TREE_CLASS_ID(MODEL_FILE_TREE_SUB_ID_VALUE_NEW), EO_TYPECHECK(Model_File_Tree *, model), EO_TYPECHECK(const char *, path), EO_TYPECHECK(Eina_Value **, ret)


#define MODEL_FILE_TREE_CLASS model_file_tree_class_get()
const Eo_Class *model_file_tree_class_get(void);

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _model_file_tree_constructor),
        //EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _model_file_tree_destructor), 
        EO_OP_FUNC(MODEL_FILE_TREE_CLASS_ID(MODEL_FILE_TREE_SUB_ID_LIST), _model_file_tree_list),
        EO_OP_FUNC(MODEL_FILE_TREE_CLASS_ID(MODEL_FILE_TREE_SUB_ID_VALUE_NEW), _model_file_tree_value_new),
        EO_OP_FUNC(ELM_OBJ_MODEL_TREE_ID(ELM_OBJ_MUTABLE_SUB_ID_CHILD_APPEND), _model_file_tree_child_append), // override
        EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

const Eo_Op_Description op_descs[] = {
     EO_OP_DESCRIPTION(MODEL_FILE_TREE_SUB_ID_CONSTRUCTOR, "Local - Constructor"),
     //EO_OP_DESCRIPTION(MODEL_FILE_TREE_SUB_ID_DESTRUCTOR, "Local - Destructor"),
     EO_OP_DESCRIPTION(MODEL_FILE_TREE_SUB_ID_LIST, "Local - Returns asynchronous I/O reference"),
     EO_OP_DESCRIPTION(MODEL_FILE_TREE_SUB_ID_VALUE_NEW, "Local - Registers I/O handlers for the path"),
     EO_OP_DESCRIPTION_SENTINEL
};

static Eo_Class_Description class_descs = {
   EO_VERSION, 
   "Model File Tree",
   EO_CLASS_TYPE_REGULAR,
   EO_CLASS_DESCRIPTION_OPS(&MODEL_FILE_TREE_CLASS_BASE_ID, op_descs, MODEL_FILE_TREE_SUB_ID_LAST),
   NULL,
   sizeof(Model_File_Tree),
   _class_constructor,
   NULL
};

EO_DEFINE_CLASS(model_file_tree_class_get, &class_descs, ELM_MODEL_TREE_CLASS, NULL);

#endif //__MODEL_FILE_TREE_CLASS

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

static void
_model_file_tree_value_new(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Model_File_Value *ptr = NULL;

   (void)va_arg(*list, Model_File_Tree *);  //unused
   const char *path = va_arg(*list, const char *);
   Eina_Value **value = va_arg(*list, Eina_Value **); //return value
   
   EINA_SAFETY_ON_NULL_RETURN(path);
   printf("###### %p\n", eina_value_new(&EINA_VALUE_TYPE_FILEMODEL));
   *value = eina_value_new(&EINA_VALUE_TYPE_FILEMODEL);
   printf("%d:value=%p\n",__LINE__, *value);
   puts("BBB");
   EINA_SAFETY_ON_NULL_RETURN(*value);
   ptr = calloc(1, sizeof(Model_File_Value));
   if(ptr == NULL)
     {
        eina_value_free(value);
        *value = NULL;
        return;

     }
   ptr->filepath = eina_stringshare_add(path);
   eina_value_pset(*value, ptr);

   // register Eio handlers for this path
   _eio_value_init(*value, path, obj); 
}

static void
_model_file_tree_constructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   printf("%s:%d\n", __FUNCTION__, __LINE__);
   Eina_Value *value = NULL;
   Eina_Bool ret;
   //Model_File_Tree *model = va_arg(*list, Model_File_Tree *);
   Model_File_Tree *model = class_data;
   const char *filepath = va_arg(*list, const char *);

   EINA_SAFETY_ON_NULL_RETURN(filepath);

   eo_do_super(obj, MODEL_FILE_TREE_CLASS, eo_constructor());

   eo_do(obj, model_file_tree_value_new(NULL, filepath, &value)); //ccarvalho
   //printf("%d:value=%p\n", __LINE__, value);
   //eo_do_super(obj, MODEL_FILE_TREE_CLASS, elm_model_tree_value_set(NULL, filepath, value));
   //here
   

   // @see elm_model_tree_const.h
   eo_do_super(obj, MODEL_FILE_TREE_CLASS, elm_model_tree_constructor(value)); //ccarvalho

   eo_do(obj, model_file_tree_list(model, elm_model_tree_path_new_from_string(""), &ret)); //TODO/FIXME/XXX: check ret
}

static void
_model_file_tree_destructor(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   eo_do_super(obj, MODEL_FILE_TREE_CLASS, eo_destructor());
}

static void
_model_file_tree_list(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Eina_Value *value = NULL;
   Model_File_Tuple *tuple;
   Model_File_Value *ptr;

   (EINA_UNUSED)va_arg(*list, Model_File_Tree *); // TODO/FIXME/XXX
   Elm_Model_Tree_Path *node = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);

   EINA_SAFETY_ON_NULL_GOTO(node, fail);
   EINA_SAFETY_ON_NULL_GOTO(obj, fail);

   eo_do(obj, elm_model_tree_value_get(node, &value));
   
   EINA_SAFETY_ON_NULL_GOTO(value, fail);

   ptr = eina_value_memory_get(value);

   EINA_SAFETY_ON_NULL_GOTO(ptr, fail);
   EINA_SAFETY_ON_NULL_GOTO(ptr->filepath, fail);

   tuple = malloc(sizeof(Model_File_Tuple));
   EINA_SAFETY_ON_NULL_GOTO(tuple, fail);
   eo_ref(obj);
   tuple->object = obj;
   tuple->node = elm_model_tree_path_new_copy(node);

   EINA_SAFETY_ON_NULL_GOTO(tuple->node, treelistfail);
   if (ptr->file != NULL) free(ptr->file);

   ptr->file = eio_file_stat_ls
      (ptr->filepath, _eio_filter_cb, _eio_main_cb, _eio_done_cb, _eio_error_cb, tuple);
   
   *ret = EINA_TRUE;
   return;

treelistfail:
   free(tuple);
   eo_unref(obj);

fail:
   *ret = EINA_FALSE;
}

static void 
_model_file_tree_child_append(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Eina_Bool ret;
   Model_File_Tree *model = va_arg(*list, Model_File_Tree *);
   Elm_Model_Tree_Path *node = va_arg(*list, Elm_Model_Tree_Path *);
   Eina_Value *value = va_arg(*list, Eina_Value *);
   Elm_Model_Tree_Path **child = va_arg(*list, Elm_Model_Tree_Path **);
   
   EINA_SAFETY_ON_NULL_GOTO(node, fail);
   EINA_SAFETY_ON_NULL_GOTO(value, fail);

   eo_do_super(obj, MODEL_FILE_TREE_CLASS, elm_model_tree_child_append(node, value, child /* return value */));
   
   EINA_SAFETY_ON_NULL_RETURN(*child);
   eo_do(obj, model_file_tree_list(model, child, &ret)); // TODO/FIXME/XXX: check ret

   return;

fail:
   *child = NULL;

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

   printf("%d:value=%p\n", __LINE__, value);
   eo_do(tuple->object, model_file_tree_value_new(NULL, info->path, &value)); // ccarvalho

   printf("### %d:%p\n", __LINE__, value);
   ptr = eina_value_memory_get(value);
   printf("### %d:%p\n", __LINE__, value);
   ptr->ftype = info->type;

   eo_do(tuple->object, elm_model_tree_child_append(tuple->node, value, &child)); //ccarvalho
   
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

   printf("### %d:%p\n", __LINE__, value);
   ptr = eina_value_memory_get(value);
   printf("### %d:%p\n", __LINE__, value);
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
   printf("### %d\n", __LINE__);
   Evas_Object *ic = elm_icon_add(obj);
   Eina_Value *value = NULL;

   if(!strcmp(part, "elm.swallow.icon"))
     {
        Model_File_Value *ptr;
        eo_do(model, elm_model_tree_value_get(path, &value));
   printf("->### %d:%p\n", __LINE__, value);
        ptr = eina_value_memory_get(value);
   printf("->### %d:%p\n", __LINE__, value);
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

   printf("_content_get_cb: %p\n", _content_get_cb);
   win = elm_win_util_standard_add("filemodel", "Filemodel");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   /**
    * Will switch from eo2 to eo soon. 
    */
   printf("### %d\n", __LINE__);
   _tree_m = eo_add_custom(MODEL_FILE_TREE_CLASS, NULL, model_file_tree_constructor("./"));
   printf("### %d\n", __LINE__);
   //_tree_m = eo_add_custom(ELM_OBJ_TREE_MUTABLE_CLASS .. to be continued

   _tree_v = eo_add_custom(ELM_VIEW_TREE_CLASS, NULL, 
                           elm_view_tree_add(win, _tree_m));

   //box init
   box = elm_box_add(win);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   //Directories tree widget
   // eo2_do(_tree_v, elm_view_tree_model_tree_set(ELM_VIEW_TREE_VIEWMODE_ONLYPARENTS)); //hide files, show only directories

   eo_do(_tree_v, elm_view_tree_evas_object_get(&widget));
   eo_do(_tree_v, elm_view_tree_getcontent_set(_content_get_cb));

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

   elm_run();
   elm_shutdown();
   eio_shutdown();
   ecore_shutdown();

   eo_unref(_tree_m);
   return 0;
}
ELM_MAIN()
