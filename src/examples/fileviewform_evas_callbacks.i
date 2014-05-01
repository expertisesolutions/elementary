/**
 * fileviewform.c extension impl.
 */
static void
_win_focused_cb(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   const char *name = (const char *)data;
   fprintf(stdout, "%s:%d Window focused: %p:%s\n", __FUNCTION__, __LINE__, obj, name);
}

static void
_main_win_del_cb(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   const char *name = (const char *)data;
   fprintf(stdout, "%s:%d Window deleted: %p:%s\n", __FUNCTION__, __LINE__, obj, name);
}


static Eina_Bool
_prop_change_cb(void *data, Eo *obj EINA_UNUSED, 
                const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   fprintf(stdout, "%s:%d: Got property change event.\n", __FUNCTION__, __LINE__);
   return EINA_TRUE;
}

static Eina_Bool
_properties_cb(void *data, Eo *obj EINA_UNUSED, 
                const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   fprintf(stdout, "%s:%d: Got properties event.\n", __FUNCTION__, __LINE__);
   return EINA_TRUE;
}
