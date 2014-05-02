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
