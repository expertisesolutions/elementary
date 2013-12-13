
#define EFL_EO_API_SUPPORT
#define EFL_BETA_API_SUPPORT

#include <assert.h>
#include <Eo.h>
#include <Eio.h>
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

struct _Model_File_Tree
{
   Eina_Bool enable_filter_dironly;
};

typedef struct _Model_File_Tree Model_File_Tree;

static void _model_file_tree_constructor(Eo *object, Model_File_Tree *model, const char* root);
static void _model_file_tree_destructor(Eo *object, Model_File_Tree *model);
static Eina_Bool _model_file_tree_select(Eo *object, Model_File_Tree *model, Elm_Model_Tree_Path *path);

#define MODEL_FILE_TREE_CLASS model_file_tree                           \
 , constructor(model_file_tree_constructor, _model_file_tree_constructor, const char*) \
 , destructor(_model_file_tree_destructor)                                             \
 , function_override(elm_model_tree_select, _model_file_tree_select)

EO3_DECLARE_CLASS(MODEL_FILE_TREE_CLASS)

EO3_DEFINE_CLASS(MODEL_FILE_TREE_CLASS, ((ELM_MODEL_TREE_CLASS)), Model_File_Tree)


////////////////////////////////////////////////////////////////////////////////

struct _Model_File_Tree_Value
{
   Eina_Stringshare *filename;
   Eio_File *file;
   Eio_Monitor *monitor;
   Ecore_Event_Handler **handlers;
};

typedef struct _Model_File_Tree_Value Model_File_Tree_Value;

static Eina_Bool _file_tree_filter_cb(void*, Eio_File*, const Eina_File_Direct_Info*);
static void _file_tree_main_cb(void*, Eio_File*, const Eina_File_Direct_Info*);
static void _file_tree_done_cb(void*, Eio_File*);
static void _file_tree_error_cb(void*, Eio_File*, int);
static Eina_Bool _file_tree_event_cb(void*, int, void*);
//static void _file_tree_stat_cb(void*, Eio_File*, const Eina_Stat*);

static void
_model_file_tree_constructor(Eo *object,
                             Model_File_Tree *model,
                             const char* root)
{
   Eina_Value *value = NULL;
   printf("<D> _model_file_tree_constructor().\n");
   EINA_SAFETY_ON_NULL_RETURN(root);
   //value = _model_file_value_new(root);

   eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS),
                elm_model_tree_constructor(value));

   //eo_ref(object); // will be unref'd at _eio_done_cb.
   /* (XXX) = eio_file_stat_ls(root, */
   /*                        _file_tree_filter_cb, */
   /*                        _file_tree_main_cb, */
   /*                        _file_tree_done_cb, */
   /*                        _file_tree_error_cb, */
   /*                        object); */
}

static void
_model_file_tree_destructor(Eo *object, Model_File_Tree *model)
{
   eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS), eo2_destructor());
   printf("<D> _model_file_tree_destructor().\n");
}

static Eina_Bool
_model_file_tree_select(Eo *object, Model_File_Tree *model, Elm_Model_Tree_Path *path)
{
   Eina_Bool ret = EINA_FALSE;
   eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS), ret = elm_model_tree_select(path));
   printf("<D> _model_file_tree_select().\n");
   return ret;
}

static Eina_Bool
_file_tree_filter_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   return EINA_TRUE; /* i.e., list everything */
}

static void
_file_tree_main_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   // TODO: implement

   //eo2_do_super(object, EO3_GET_CLASS(MODEL_FILE_TREE_CLASS), ret =
   //elm_model_tree_select(path));
}

static void
_file_tree_done_cb(void *data, Eio_File *handler)
{
   //eo_unref(model->object);
   //free(data);
}

static void
_file_tree_error_cb(void *data, Eio_File *handler, int error)
{
   //Model_File_Tree_Data *self = (Model_File_Tree_Data*)data;
   printf("file: error! (code=%d)\n", error);
}

static Eina_Bool
_file_tree_event_cb(void *data, int type, void *event)
{
   //Model_File_Tree_Node *node;

   EINA_SAFETY_ON_NULL_RETURN_VAL(data, EINA_FALSE);
   //node = (Model_File_Tree_Node*)data;
   
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
                 ((EO3_BASE_CLASS),(ELM_MODEL_GRID_CLASS)),
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

   //elm_run();
   elm_shutdown();
   eio_shutdown();
   ecore_shutdown();

   eo_unref(_tree_m);
   return 0;
}
ELM_MAIN()
