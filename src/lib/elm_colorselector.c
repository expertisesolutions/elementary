#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"
#include "elm_widget_colorselector.h"

EAPI Eo_Op ELM_OBJ_COLORSELECTOR_BASE_ID = EO_NOOP;

#define MY_CLASS ELM_OBJ_COLORSELECTOR_CLASS

#define MY_CLASS_NAME "elm_colorselector"

#define BASE_STEP       360.0
#define HUE_STEP        360.0
#define SAT_STEP        128.0
#define LIG_STEP        256.0
#define ALP_STEP        256.0
#define DEFAULT_HOR_PAD 10
#define DEFAULT_VER_PAD 10

static const char SIG_CHANGED[] = "changed";
static const char SIG_COLOR_ITEM_SELECTED[] = "color,item,selected";
static const char SIG_COLOR_ITEM_LONGPRESSED[] = "color,item,longpressed";
static const Evas_Smart_Cb_Description _smart_callbacks[] =
{
   {SIG_COLOR_ITEM_SELECTED, ""},
   {SIG_COLOR_ITEM_LONGPRESSED, ""},
   {SIG_CHANGED, ""},
   {NULL, NULL}
};

enum Palette_Box_Direction
{
   PALETTE_BOX_UP,
   PALETTE_BOX_DOWN
};

static void
_items_del(Elm_Colorselector_Smart_Data *sd)
{
   Elm_Color_Item *item;

   if (!sd->items) return;

   EINA_LIST_FREE(sd->items, item)
     {
        free(item->color);
        elm_widget_item_free(item);
     }

   sd->items = NULL;
   sd->selected = NULL;
}

static void
_color_with_saturation(Elm_Colorselector_Smart_Data *sd)
{
   if (sd->er > 127)
     sd->sr = 127 + (int)((double)(sd->er - 127) * sd->s);
   else
     sd->sr = 127 - (int)((double)(127 - sd->er) * sd->s);

   if (sd->eg > 127)
     sd->sg = 127 + (int)((double)(sd->eg - 127) * sd->s);
   else
     sd->sg = 127 - (int)((double)(127 - sd->eg) * sd->s);

   if (sd->eb > 127)
     sd->sb = 127 + (int)((double)(sd->eb - 127) * sd->s);
   else
     sd->sb = 127 - (int)((double)(127 - sd->eb) * sd->s);
}

static void
_color_with_lightness(Elm_Colorselector_Smart_Data *sd)
{
   if (sd->l > 0.5)
     {
        sd->lr = sd->er + (int)((double)(255 - sd->er) * (sd->l - 0.5) * 2.0);
        sd->lg = sd->eg + (int)((double)(255 - sd->eg) * (sd->l - 0.5) * 2.0);
        sd->lb = sd->eb + (int)((double)(255 - sd->eb) * (sd->l - 0.5) * 2.0);
     }
   else if (sd->l < 0.5)
     {
        sd->lr = (double)sd->er * sd->l * 2.0;
        sd->lg = (double)sd->eg * sd->l * 2.0;
        sd->lb = (double)sd->eb * sd->l * 2.0;
     }
   else
     {
        sd->lr = sd->er;
        sd->lg = sd->eg;
        sd->lb = sd->eb;
     }
}

static void
_color_picker_init(Elm_Colorselector_Smart_Data *sd)
{
   char buf[12];
   unsigned int *pixels;
   unsigned int *copy;
   int color;
   int x, y, w, h;

   snprintf(buf, 12, "%i", sd->r);
   elm_object_text_set(sd->entries[0], buf);

   snprintf(buf, 12, "%i", sd->g);
   elm_object_text_set(sd->entries[1], buf);

   snprintf(buf, 12, "%i", sd->b);
   elm_object_text_set(sd->entries[2], buf);

   snprintf(buf, 12, "%i", sd->a);
   elm_object_text_set(sd->entries[3], buf);

   color = (sd->a << 24) + (sd->r << 16) + (sd->g << 8) + sd->b;

   if (sd->b == 255)
     evas_object_image_alpha_set(sd->picker_display, EINA_FALSE);
   else
     evas_object_image_alpha_set(sd->picker_display, EINA_TRUE);
   
   pixels = evas_object_image_data_get(sd->picker_display, EINA_TRUE);
   copy = pixels;
   for (y = 0; y < 17; y++)
     for (x = 0; x < 17; x++)
       {
          *(pixels++) = color;
       }
   evas_object_image_data_set(sd->picker_display, copy);
   evas_object_geometry_get(sd->picker_display, NULL, NULL, &w, &h);
   evas_object_image_data_update_add(sd->picker_display, 0, 0, w, h);
}


static void
_rgb_to_hsl(Elm_Colorselector_Smart_Data *sd)
{
   double r2, g2, b2;
   double v, m, vm;
   double r, g, b;

   r = sd->r;
   g = sd->g;
   b = sd->b;

   r /= 255.0;
   g /= 255.0;
   b /= 255.0;

   v = (r > g) ? r : g;
   v = (v > b) ? v : b;

   m = (r < g) ? r : g;
   m = (m < b) ? m : b;

   sd->h = 0.0;
   sd->s = 0.0;
   sd->l = 0.0;

   sd->l = (m + v) / 2.0;

   if (sd->l <= 0.0) return;

   vm = v - m;
   sd->s = vm;

   if (sd->s > 0.0) sd->s /= (sd->l <= 0.5) ? (v + m) : (2.0 - v - m);
   else return;

   r2 = (v - r) / vm;
   g2 = (v - g) / vm;
   b2 = (v - b) / vm;

   if (r == v) sd->h = (g == m ? 5.0 + b2 : 1.0 - g2);
   else if (g == v)
     sd->h = (b == m ? 1.0 + r2 : 3.0 - b2);
   else sd->h = (r == m ? 3.0 + g2 : 5.0 - r2);

   sd->h *= 60.0;
}

static void
_hsl_to_rgb(Elm_Colorselector_Smart_Data *sd)
{
   double sv, vsf, f, p, q, t, v;
   double r = 0, g = 0, b = 0;
   double _h, _s, _l;
   int i = 0;

   _h = sd->h;
   _s = sd->s;
   _l = sd->l;

   if (_s == 0.0) r = g = b = _l;
   else
     {
        if (_h == 360.0) _h = 0.0;
        _h /= 60.0;

        v = (_l <= 0.5) ? (_l * (1.0 + _s)) : (_l + _s - (_l * _s));
        p = _l + _l - v;

        if (v) sv = (v - p) / v;
        else sv = 0;

        i = (int)_h;
        f = _h - i;

        vsf = v * sv * f;

        t = p + vsf;
        q = v - vsf;

        switch (i)
          {
           case 0:
             r = v;
             g = t;
             b = p;
             break;

           case 1:
             r = q;
             g = v;
             b = p;
             break;

           case 2:
             r = p;
             g = v;
             b = t;
             break;

           case 3:
             r = p;
             g = q;
             b = v;
             break;

           case 4:
             r = t;
             g = p;
             b = v;
             break;

           case 5:
             r = v;
             g = p;
             b = q;
             break;
          }
     }
   i = (int)(r * 255.0);
   f = (r * 255.0) - i;
   sd->r = (f <= 0.5) ? i : (i + 1);

   i = (int)(g * 255.0);
   f = (g * 255.0) - i;
   sd->g = (f <= 0.5) ? i : (i + 1);

   i = (int)(b * 255.0);
   f = (b * 255.0) - i;
   sd->b = (f <= 0.5) ? i : (i + 1);
}

static void
_rectangles_redraw(Color_Bar_Data *cb_data, double x)
{
   double one_six = 1.0 / 6.0;

   ELM_COLORSELECTOR_DATA_GET(cb_data->parent, sd);

   switch (cb_data->color_type)
     {
      case HUE:
        sd->h = 360.0 * x;

        if (x < one_six)
          {
             sd->er = 255;
             sd->eg = (255.0 * x * 6.0);
             sd->eb = 0;
          }
        else if (x < 2 * one_six)
          {
             sd->er = 255 - (int)(255.0 * (x - one_six) * 6.0);
             sd->eg = 255;
             sd->eb = 0;
          }
        else if (x < 3 * one_six)
          {
             sd->er = 0;
             sd->eg = 255;
             sd->eb = (int)(255.0 * (x - (2.0 * one_six)) * 6.0);
          }
        else if (x < 4 * one_six)
          {
             sd->er = 0;
             sd->eg = 255 - (int)(255.0 * (x - (3.0 * one_six)) * 6.0);
             sd->eb = 255;
          }
        else if (x < 5 * one_six)
          {
             sd->er = 255.0 * (x - (4.0 * one_six)) * 6.0;
             sd->eg = 0;
             sd->eb = 255;
          }
        else
          {
             sd->er = 255;
             sd->eg = 0;
             sd->eb = 255 - (int)(255.0 * (x - (5.0 * one_six)) * 6.0);
          }

        evas_object_color_set
          (sd->cb_data[0]->arrow, sd->er, sd->eg, sd->eb, 255);
        evas_object_color_set
          (sd->cb_data[1]->bg_rect, sd->er, sd->eg, sd->eb, 255);
        evas_object_color_set
          (sd->cb_data[2]->bg_rect, sd->er, sd->eg, sd->eb, 255);
        evas_object_color_set
          (sd->cb_data[3]->bar, sd->er, sd->eg, sd->eb, 255);

        _color_with_saturation(sd);
        evas_object_color_set
          (sd->cb_data[1]->arrow, sd->sr, sd->sg, sd->sb, 255);

        _color_with_lightness(sd);
        evas_object_color_set
          (sd->cb_data[2]->arrow, sd->lr, sd->lg, sd->lb, 255);

        evas_object_color_set(sd->cb_data[3]->arrow,
                              (sd->er * sd->a) / 255,
                              (sd->eg * sd->a) / 255,
                              (sd->eb * sd->a) / 255,
                              sd->a);
        break;

      case SATURATION:
        sd->s = 1.0 - x;
        _color_with_saturation(sd);
        evas_object_color_set
          (sd->cb_data[1]->arrow, sd->sr, sd->sg, sd->sb, 255);
        break;

      case LIGHTNESS:
        sd->l = x;
        _color_with_lightness(sd);
        evas_object_color_set
          (sd->cb_data[2]->arrow, sd->lr, sd->lg, sd->lb, 255);
        break;

      case ALPHA:
        sd->a = 255.0 * x;
        evas_object_color_set(sd->cb_data[3]->arrow,
                              (sd->er * sd->a) / 255,
                              (sd->eg * sd->a) / 255,
                              (sd->eb * sd->a) / 255,
                              sd->a);
        break;

      default:
        break;
     }

   _hsl_to_rgb(sd);
   _color_picker_init(sd);
}

static void
_colors_set(Evas_Object *obj,
            int r,
            int g,
            int b,
            int a)
{
   double x, y;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   sd->r = r;
   sd->g = g;
   sd->b = b;
   sd->a = a;

   _rgb_to_hsl(sd);

   edje_object_part_drag_value_get
     (sd->cb_data[0]->colorbar, "elm.arrow", &x, &y);
   x = sd->h / 360.0;
   edje_object_part_drag_value_set
     (sd->cb_data[0]->colorbar, "elm.arrow", x, y);
   _rectangles_redraw(sd->cb_data[0], x);

   edje_object_part_drag_value_get
     (sd->cb_data[1]->colorbar, "elm.arrow", &x, &y);
   x = 1.0 - sd->s;
   edje_object_part_drag_value_set
     (sd->cb_data[1]->colorbar, "elm.arrow", x, y);
   _rectangles_redraw(sd->cb_data[1], x);

   edje_object_part_drag_value_get
     (sd->cb_data[2]->colorbar, "elm.arrow", &x, &y);
   x = sd->l;
   edje_object_part_drag_value_set(sd->cb_data[2]->colorbar, "elm.arrow", x, y);
   _rectangles_redraw(sd->cb_data[2], x);

   edje_object_part_drag_value_get
     (sd->cb_data[3]->colorbar, "elm.arrow", &x, &y);
   x = sd->a / 255.0;
   edje_object_part_drag_value_set
     (sd->cb_data[3]->colorbar, "elm.arrow", x, y);

   _rectangles_redraw(sd->cb_data[3], x);
}

static void
_entry_changed_cb(void *data,
                  Evas_Object *obj,
                  void *event_info __UNUSED__)
{
   Elm_Colorselector_Smart_Data *sd = data;
   Evas_Object *parent;
   const char *text;
   int i;
   int v;

   for (i = 0; i < 4 && sd->entries[i] != obj; i++)
     ;

   parent = evas_object_data_get(obj, "parent");
   text = elm_object_text_get(obj);
   v = atoi(text);
   if (v > 255) v = 255;
   else if (v < 0) v = 0;

   switch (i)
     {
      case 0:
         if (v != sd->r)
           _colors_set(parent, v, sd->g, sd->b, sd->a);
         break;
      case 1:
         if (v != sd->g)
           _colors_set(parent, sd->r, v, sd->b, sd->a);
         break;
      case 2:
         if (v != sd->b)
           _colors_set(parent, sd->r, sd->g, v, sd->a);
         break;
      case 3:
         if (v != sd->a)
           _colors_set(parent, sd->r, sd->g, sd->b, v);
         break;
     }
}

#ifdef HAVE_ELEMENTARY_X
static Eina_Bool _mouse_grab_pixels(void *data, int type __UNUSED__, void *event __UNUSED__);

static Ecore_X_Window
_x11_elm_widget_xwin_get(const Evas_Object *obj)
{
   Evas_Object *top;
   Ecore_X_Window xwin = 0;

   top = elm_widget_top_get(obj);
   if (!top) top = elm_widget_top_get(elm_widget_parent_widget_get(obj));
   if (top) xwin = elm_win_xwindow_get(top);
   if (!xwin)
     {
        Ecore_Evas *ee;
        Evas *evas = evas_object_evas_get(obj);
        if (!evas) return 0;
        ee = ecore_evas_ecore_evas_get(evas);
        if (!ee) return 0;
        xwin = _elm_ee_xwin_get(ee);
     }
   return xwin;
}

static void
_start_grab_pick_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Evas_Object *o = data;

   ELM_COLORSELECTOR_DATA_GET(o, sd);

   elm_object_disabled_set(obj, EINA_TRUE);

   ecore_event_handler_del(sd->grab.mouse_motion);
   sd->grab.mouse_motion = ecore_event_handler_add(ECORE_EVENT_MOUSE_MOVE, _mouse_grab_pixels, o);

   ecore_x_keyboard_grab(sd->grab.xroot);
   ecore_x_pointer_grab(sd->grab.xroot);
}

static Eina_Bool
_key_up_cb(void *data, int type __UNUSED__, void *event __UNUSED__)
{
   Evas_Object *o = data;

   /* key => cancel */
   ELM_COLORSELECTOR_DATA_GET(o, sd);

   elm_object_disabled_set(sd->button, EINA_FALSE);
   ecore_x_keyboard_ungrab();
   ecore_x_pointer_ungrab();
   ecore_event_handler_del(sd->grab.mouse_motion);
   sd->grab.mouse_motion = ecore_event_handler_add(ECORE_X_RAW_MOTION, _mouse_grab_pixels, o);

   return EINA_TRUE;
}

static Eina_Bool
_mouse_up_cb(void *data, int type __UNUSED__, void *event __UNUSED__)
{
   const unsigned int *pixels;
   Evas_Object *o = data;

   /* mouse up => check it */
   ELM_COLORSELECTOR_DATA_GET(o, sd);

   elm_object_disabled_set(sd->button, EINA_FALSE);
   ecore_x_keyboard_ungrab();
   ecore_x_pointer_ungrab();
   ecore_event_handler_del(sd->grab.mouse_motion);
   sd->grab.mouse_motion = ecore_event_handler_add(ECORE_X_RAW_MOTION, _mouse_grab_pixels, o);

   pixels = evas_object_image_data_get(sd->picker_display, EINA_FALSE);
   sd->a = 0xff;
   sd->r = (pixels[17 * 9 + 9] >> 16) & 0xFF;
   sd->g = (pixels[17 * 9 + 9] >> 8) & 0xFF;
   sd->b = pixels[17 * 9 + 9] & 0xFF;

   _color_picker_init(sd);

   return EINA_TRUE;
}

static Eina_Bool
_mouse_grab_pixels(void *data, int type __UNUSED__, void *event __UNUSED__)
{
   Evas_Object *obj = data;
   Ecore_X_Visual visual;
   Ecore_X_Display *display;
   Ecore_X_Screen *scr;
   Ecore_X_Image *img;
   Ecore_X_Window xwin;
   int *src;
   int bpl = 0, rows = 0, bpp = 0;
   int x, y, w, h;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   if (sd->grab.in) return EINA_TRUE;

   xwin = _x11_elm_widget_xwin_get(obj);
   sd->grab.xroot = ecore_x_window_root_get(xwin);
   ecore_x_pointer_xy_get(sd->grab.xroot, &x, &y);

   if (x < 8) x += 8;
   if (y < 8) y += 8;

   if (sd->grab.x == x && sd->grab.y == y) return EINA_TRUE;
   sd->grab.x = x;
   sd->grab.y = y;

   evas_object_image_alpha_set(sd->picker_display, EINA_FALSE);

   display = ecore_x_display_get();
   scr = ecore_x_default_screen_get();
   visual = ecore_x_default_visual_get(display, scr);
   img = ecore_x_image_new(17, 17, visual, ecore_x_window_depth_get(sd->grab.xroot));
   ecore_x_image_get(img, sd->grab.xroot, x - 8, y - 8, 0, 0, 17, 17);
   src = ecore_x_image_data_get(img, &bpl, &rows, &bpp);
   if (!ecore_x_image_is_argb32_get(img))
     {
        Ecore_X_Colormap colormap;
        unsigned int *pixels;

        colormap = ecore_x_default_colormap_get(display, scr);
        pixels = evas_object_image_data_get(sd->picker_display, EINA_TRUE);
        ecore_x_image_to_argb_convert(src, bpp, bpl, colormap, visual,
                                      0, 0, 17, 17,
                                      pixels, (17 * sizeof(int)), 0, 0);
     }
   else
     {
        evas_object_image_data_copy_set(sd->picker_display, src);
     }

   ecore_x_image_free(img);

   evas_object_geometry_get(sd->picker_display, NULL, NULL, &w, &h);
   evas_object_image_data_update_add(sd->picker_display, 0, 0, w, h);

   return EINA_TRUE;
}
#endif   

static void
_mouse_in_canvas(void *data, Evas *e __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *obj = data;
   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   sd->grab.in = EINA_TRUE;
}

static void
_mouse_out_canvas(void *data, Evas *e __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *obj = data;
   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   sd->grab.in = EINA_FALSE;
}

static void
_color_picker_add(Evas_Object *obj, Elm_Colorselector_Smart_Data *sd)
{
   Evas_Object *ed;
   Evas_Object *im;
   Evas_Object *label;
   Evas_Object *entry;
   Evas_Object *table;
   Evas_Object *bx;
   static const char *labels[4] = { "R:", "G:", "B:", "A:" };
   int i;
#ifdef HAVE_ELEMENTARY_X
   Ecore_X_Window xwin;

   xwin = _x11_elm_widget_xwin_get(obj);
   if (xwin)
     {
        sd->grab.xroot = ecore_x_window_root_get(xwin);
        ecore_x_input_raw_select(sd->grab.xroot);

        sd->grab.mouse_motion = ecore_event_handler_add(ECORE_X_RAW_MOTION, _mouse_grab_pixels, obj);
        sd->grab.key_up = ecore_event_handler_add(ECORE_EVENT_KEY_UP, _key_up_cb, obj);
        sd->grab.mouse_up = ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_UP, _mouse_up_cb, obj);
     }
#endif

   bx = elm_box_add(sd->picker);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(sd->picker, bx);
   evas_object_show(bx);

   ed = edje_object_add(evas_object_evas_get(sd->picker));
   elm_widget_theme_object_set(obj, ed, "colorselector", "picker", elm_widget_style_get(obj));
   evas_object_size_hint_weight_set(ed, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ed, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, ed);
   evas_object_show(ed);

#ifdef HAVE_ELEMENTARY_X
   if (xwin)
     {
        sd->button = elm_button_add(sd->picker);
        elm_object_text_set(sd->button, "Pick a color");
        evas_object_smart_callback_add(sd->button, "clicked", _start_grab_pick_cb, obj);
        elm_box_pack_end(bx, sd->button);
        evas_object_show(sd->button);
     }
#endif

   im = evas_object_image_add(evas_object_evas_get(sd->picker));
   evas_object_size_hint_aspect_set(im, EVAS_ASPECT_CONTROL_BOTH, 1, 1);
   evas_object_image_smooth_scale_set(im, EINA_FALSE);
   evas_object_image_colorspace_set(im, EVAS_COLORSPACE_ARGB8888);
   evas_object_image_size_set(im, 17, 17);
   evas_object_image_alpha_set(im, 1);
   evas_object_image_filled_set(im, EINA_TRUE);
   edje_object_part_swallow(ed, "elm.picker", im);
   elm_widget_sub_object_add(obj, im);

   sd->picker_display = im;

   table = elm_table_add(sd->picker);
   evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(sd->picker, table);
   evas_object_show(table);

   for (i = 0; i < 4; i++)
     {
        static Elm_Entry_Filter_Accept_Set accept_set = {
          .accepted = "0123456789",
          .rejected = NULL
        };

        label = elm_label_add(table);
        elm_object_text_set(label, labels[i]);
        evas_object_size_hint_weight_set(label, 0.0, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(label, 0.0, EVAS_HINT_FILL);
        elm_table_pack(table, label, 0, i, 1, 1);
        evas_object_show(label);

        entry = elm_entry_add(table);
        elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_set);
        elm_entry_single_line_set(entry, EINA_TRUE);
        elm_entry_scrollable_set(entry, EINA_TRUE);
        evas_object_data_set(entry, "parent", obj);
        evas_object_smart_callback_add(entry, "changed", _entry_changed_cb, sd);
        evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_table_pack(table, entry, 1, i, 1, 1);
        evas_object_show(entry);

        sd->entries[i] = entry;
     }

   evas_event_callback_add(evas_object_evas_get(obj), EVAS_CALLBACK_CANVAS_FOCUS_IN, _mouse_in_canvas, obj);
   evas_event_callback_add(evas_object_evas_get(obj), EVAS_CALLBACK_CANVAS_FOCUS_OUT, _mouse_out_canvas, obj);

   _color_picker_init(sd);
}

static void
_arrow_cb(void *data,
          Evas_Object *obj,
          const char *emission __UNUSED__,
          const char *source __UNUSED__)
{
   Color_Bar_Data *cb_data = data;
   double x, y;

   edje_object_part_drag_value_get(obj, "elm.arrow", &x, &y);

   _rectangles_redraw(data, x);
   evas_object_smart_callback_call(cb_data->parent, SIG_CHANGED, NULL);
}

static void
_colorbar_cb(void *data,
             Evas *e,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   Color_Bar_Data *cb_data = data;
   double arrow_x = 0, arrow_y;
   Evas_Coord x, y, w, h;
   ELM_COLORSELECTOR_DATA_GET(cb_data->parent, sd);

   evas_object_geometry_get(cb_data->bar, &x, &y, &w, &h);
   edje_object_part_drag_value_get
     (cb_data->colorbar, "elm.arrow", &arrow_x, &arrow_y);

   if (w > 0) arrow_x = (double)(ev->canvas.x - x) / (double)w;
   if (arrow_x > 1) arrow_x = 1;
   if (arrow_x < 0) arrow_x = 0;
   edje_object_part_drag_value_set
     (cb_data->colorbar, "elm.arrow", arrow_x, arrow_y);

   _rectangles_redraw(data, arrow_x);
   evas_object_smart_callback_call(cb_data->parent, SIG_CHANGED, NULL);
   evas_event_feed_mouse_cancel(e, 0, NULL);
   evas_event_feed_mouse_down(e, 1, EVAS_BUTTON_NONE, 0, NULL);
   sd->sel_color_type = cb_data->color_type;
   sd->focused = ELM_COLORSELECTOR_COMPONENTS;
}

static void
_button_clicked_cb(void *data,
                   Evas_Object *obj,
                   void *event_info __UNUSED__)
{
   Eina_Bool is_right = EINA_FALSE;
   Color_Bar_Data *cb_data = data;
   double x, y, step;
   ELM_COLORSELECTOR_DATA_GET(cb_data->parent, sd);

   if (obj == cb_data->rbt)
     {
        is_right = EINA_TRUE;
        step = 1.0;
     }
   else step = -1.0;

   edje_object_part_drag_value_get(cb_data->colorbar, "elm.arrow", &x, &y);

   switch (cb_data->color_type)
     {
      case HUE:
        x += step / HUE_STEP;
        break;

      case SATURATION:
        x += step / SAT_STEP;
        break;

      case LIGHTNESS:
        x += step / LIG_STEP;
        break;

      case ALPHA:
        x += step / ALP_STEP;
        break;

      default:
        break;
     }

   if (is_right)
     {
        if (x > 1.0) x = 1.0;
     }
   else
     {
        if (x < 0.0) x = 0.0;
     }

   edje_object_part_drag_value_set(cb_data->colorbar, "elm.arrow", x, y);
   _rectangles_redraw(data, x);
   evas_object_smart_callback_call(cb_data->parent, SIG_CHANGED, NULL);
   sd->sel_color_type = cb_data->color_type;
   sd->focused = ELM_COLORSELECTOR_COMPONENTS;
}

static void
_button_repeat_cb(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Eina_Bool is_right = EINA_FALSE;
   Color_Bar_Data *cb_data = data;
   double x, y, step;

   if (obj == cb_data->rbt)
     {
        is_right = EINA_TRUE;
        step = 1.0;
     }
   else step = -1.0;

   edje_object_part_drag_value_get(cb_data->colorbar, "elm.arrow", &x, &y);
   x += step / BASE_STEP;

   if (is_right)
     {
        if (x > 1.0) x = 1.0;
     }
   else
     {
        if (x < 0.0) x = 0.0;
     }

   edje_object_part_drag_value_set(cb_data->colorbar, "elm.arrow", x, y);
   _rectangles_redraw(data, x);
   evas_object_smart_callback_call(cb_data->parent, SIG_CHANGED, NULL);
}

static void
_access_colorbar_register(Evas_Object *obj,
                          Color_Bar_Data *cd,
                          const char* part)
{
   Evas_Object *ao;
   Elm_Access_Info *ai;
   const char* colorbar_type = NULL;

   ao = _elm_access_edje_object_part_object_register(obj, cd->colorbar, part);
   ai = _elm_access_object_get(ao);

   switch (cd->color_type)
     {
      case HUE:
        colorbar_type = "hue color bar";
        break;

      case SATURATION:
        colorbar_type = "saturation color bar";
        break;

      case LIGHTNESS:
        colorbar_type = "lightness color bar";
        break;

      case ALPHA:
        colorbar_type = "alpha color bar";
        break;

      default:
        break;
     }

   _elm_access_text_set(ai, ELM_ACCESS_TYPE, colorbar_type);

   // this will be used in focus_next();
   cd->access_obj = ao;
}

static void
_color_bars_add(Evas_Object *obj)
{
   char colorbar_name[128];
   char colorbar_s[128];
   char buf[1024];
   int i = 0;
   Evas *e;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   e = evas_object_evas_get(obj);

   for (i = 0; i < 4; i++)
     {
        sd->cb_data[i] = ELM_NEW(Color_Bar_Data);
        sd->cb_data[i]->parent = obj;

        switch (i)
          {
           case 0:
             sd->cb_data[i]->color_type = HUE;
             break;

           case 1:
             sd->cb_data[i]->color_type = SATURATION;
             break;

           case 2:
             sd->cb_data[i]->color_type = LIGHTNESS;
             break;

           case 3:
             sd->cb_data[i]->color_type = ALPHA;
             break;

           default:
             break;
          }

        /* load colorbar area */
        sd->cb_data[i]->colorbar = edje_object_add(e);
        elm_widget_theme_object_set
          (obj, sd->cb_data[i]->colorbar, "colorselector", "base",
          elm_widget_style_get(obj));
        snprintf(colorbar_name, sizeof(colorbar_name), "colorbar_%d", i);
        snprintf(colorbar_s, sizeof(colorbar_s), "elm.colorbar_%d", i);
        edje_object_signal_callback_add
          (sd->cb_data[i]->colorbar, "drag", "*", _arrow_cb, sd->cb_data[i]);
        edje_object_part_swallow
          (sd->col_bars_area, colorbar_s, sd->cb_data[i]->colorbar);
        elm_widget_sub_object_add(obj, sd->cb_data[i]->colorbar);

        /* load colorbar image */
        sd->cb_data[i]->bar = edje_object_add(e);
        snprintf(buf, sizeof(buf), "%s/%s", colorbar_name,
                 elm_widget_style_get(obj));
        elm_widget_theme_object_set
          (obj, sd->cb_data[i]->bar, "colorselector", "image", buf);
        edje_object_part_swallow
          (sd->cb_data[i]->colorbar, "elm.bar", sd->cb_data[i]->bar);
        elm_widget_sub_object_add(obj, sd->cb_data[i]->bar);

        /* provide expanded touch area */
        sd->cb_data[i]->touch_area = evas_object_rectangle_add(e);
        evas_object_color_set(sd->cb_data[i]->touch_area, 0, 0, 0, 0);
        edje_object_part_swallow
          (sd->cb_data[i]->colorbar, "elm.arrow_bg",
          sd->cb_data[i]->touch_area);
        evas_object_event_callback_add
          (sd->cb_data[i]->touch_area, EVAS_CALLBACK_MOUSE_DOWN, _colorbar_cb,
          sd->cb_data[i]);
        elm_widget_sub_object_add(obj, sd->cb_data[i]->touch_area);

        // ACCESS
        if (_elm_config->access_mode == ELM_ACCESS_MODE_ON)
           _access_colorbar_register(obj, sd->cb_data[i], "elm.arrow_bg_access");

        /* load background rectangle of the colorbar. used for
           changing color of the opacity bar */
        if ((i == 1) || (i == 2))
          {
             sd->cb_data[i]->bg_rect = evas_object_rectangle_add(e);
             evas_object_color_set
               (sd->cb_data[i]->bg_rect, sd->er, sd->eg, sd->eb, 255);
             edje_object_part_swallow
               (sd->cb_data[i]->colorbar, "elm.bar_bg",
               sd->cb_data[i]->bg_rect);

             elm_widget_sub_object_add(obj, sd->cb_data[i]->bg_rect);
          }
        if (i == 3)
          {
             sd->cb_data[i]->bg_rect = edje_object_add(e);
             snprintf(buf, sizeof(buf), "%s/%s", colorbar_name,
                      elm_widget_style_get(obj));
             elm_widget_theme_object_set
               (obj, sd->cb_data[i]->bg_rect, "colorselector", "bg_image",
               buf);
             edje_object_part_swallow
               (sd->cb_data[i]->colorbar, "elm.bar_bg",
               sd->cb_data[i]->bg_rect);
             elm_widget_sub_object_add(obj, sd->cb_data[i]->bg_rect);
             evas_object_color_set
               (sd->cb_data[i]->bar, sd->er, sd->eg, sd->eb, 255);
          }

        /* load arrow image, pointing the colorbar */
        sd->cb_data[i]->arrow = edje_object_add(e);
        elm_widget_theme_object_set
          (obj, sd->cb_data[i]->arrow, "colorselector", "arrow",
          elm_widget_style_get(obj));
        edje_object_part_swallow
          (sd->cb_data[i]->colorbar, "elm.arrow_icon",
          sd->cb_data[i]->arrow);
        elm_widget_sub_object_add(obj, sd->cb_data[i]->arrow);

        if (i == 2)
          evas_object_color_set(sd->cb_data[i]->arrow, 0, 0, 0, 255);
        else
          evas_object_color_set
            (sd->cb_data[i]->arrow, sd->er, sd->eg, sd->eb, 255);

        /* load left button */
        sd->cb_data[i]->lbt = elm_button_add(obj);
        snprintf(buf, sizeof(buf), "colorselector/left/%s",
                 elm_widget_style_get(obj));
        elm_object_style_set(sd->cb_data[i]->lbt, buf);
        elm_widget_sub_object_add(obj, sd->cb_data[i]->lbt);
        edje_object_part_swallow
          (sd->cb_data[i]->colorbar, "elm.l_button", sd->cb_data[i]->lbt);
        evas_object_smart_callback_add
          (sd->cb_data[i]->lbt, "clicked", _button_clicked_cb,
          sd->cb_data[i]);
        elm_button_autorepeat_set(sd->cb_data[i]->lbt, EINA_TRUE);
        elm_button_autorepeat_initial_timeout_set
          (sd->cb_data[i]->lbt, _elm_config->longpress_timeout);
        elm_button_autorepeat_gap_timeout_set
          (sd->cb_data[i]->lbt, (1.0 / _elm_config->fps));
        evas_object_smart_callback_add
          (sd->cb_data[i]->lbt, "repeated", _button_repeat_cb,
          sd->cb_data[i]);

        /* load right button */
        sd->cb_data[i]->rbt = elm_button_add(obj);
        snprintf(buf, sizeof(buf), "colorselector/right/%s",
                 elm_widget_style_get(obj));
        elm_object_style_set(sd->cb_data[i]->rbt, buf);
        elm_widget_sub_object_add(obj, sd->cb_data[i]->rbt);
        edje_object_part_swallow
          (sd->cb_data[i]->colorbar, "elm.r_button", sd->cb_data[i]->rbt);
        evas_object_smart_callback_add
          (sd->cb_data[i]->rbt, "clicked", _button_clicked_cb,
          sd->cb_data[i]);
        elm_button_autorepeat_set(sd->cb_data[i]->rbt, EINA_TRUE);
        elm_button_autorepeat_initial_timeout_set
          (sd->cb_data[i]->rbt, _elm_config->longpress_timeout);
        elm_button_autorepeat_gap_timeout_set
          (sd->cb_data[i]->rbt, (1.0 / _elm_config->fps));
        evas_object_smart_callback_add
          (sd->cb_data[i]->rbt, "repeated", _button_repeat_cb,
          sd->cb_data[i]);
     }
}

static void
_elm_colorselector_smart_theme(Eo *obj, void *_pd, va_list *list)
{
   int i;
   Eina_List *elist;
   Elm_Color_Item *item;
   const char *hpadstr, *vpadstr;
   unsigned int h_pad = DEFAULT_HOR_PAD;
   unsigned int v_pad = DEFAULT_VER_PAD;

   Elm_Colorselector_Smart_Data *sd = _pd;
   Elm_Widget_Smart_Data *wd = eo_data_scope_get(obj, ELM_OBJ_WIDGET_CLASS);
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);
   if (ret) *ret = EINA_FALSE;
   Eina_Bool int_ret = EINA_FALSE;

   eo_do_super(obj, MY_CLASS, elm_wdg_theme(&int_ret));
   if (!int_ret) return;

   if (!sd->col_bars_area) return;

   elm_widget_theme_object_set
     (obj, sd->col_bars_area, "colorselector", "bg",
     elm_widget_style_get(obj));

   hpadstr = edje_object_data_get
       (wd->resize_obj, "horizontal_pad");
   if (hpadstr) h_pad = atoi(hpadstr);
   vpadstr = edje_object_data_get
       (wd->resize_obj, "vertical_pad");
   if (vpadstr) v_pad = atoi(vpadstr);

   elm_box_padding_set
     (sd->palette_box,
     (h_pad * elm_widget_scale_get(obj) * elm_config_scale_get()),
     (v_pad * elm_widget_scale_get(obj) * elm_config_scale_get()));

   EINA_LIST_FOREACH(sd->items, elist, item)
     {
        if (!elm_layout_theme_set
            (VIEW(item), "colorselector", "item", elm_widget_style_get(obj)))
          CRITICAL("Failed to set layout!");

        elm_widget_theme_object_set
          (obj, item->color_obj, "colorselector", "item/color",
          elm_widget_style_get(obj));
     }
   for (i = 0; i < 4; i++)
     {
        if (sd->cb_data[i])
          {
             evas_object_del(sd->cb_data[i]->colorbar);
             sd->cb_data[i]->colorbar = NULL;
             evas_object_del(sd->cb_data[i]->bar);
             sd->cb_data[i]->bar = NULL;
             evas_object_del(sd->cb_data[i]->lbt);
             sd->cb_data[i]->lbt = NULL;
             evas_object_del(sd->cb_data[i]->rbt);
             sd->cb_data[i]->rbt = NULL;
             if (i != 0)
               {
                  evas_object_del(sd->cb_data[i]->bg_rect);
                  sd->cb_data[i]->bg_rect = NULL;
               }
             evas_object_del(sd->cb_data[i]->arrow);
             sd->cb_data[i]->arrow = NULL;
             evas_object_del(sd->cb_data[i]->touch_area);
             sd->cb_data[i]->touch_area = NULL;
          }
     }

   _color_bars_add(obj);
   elm_colorselector_color_set(obj, sd->r, sd->g, sd->b, sd->a);

   elm_layout_sizing_eval(obj);

   if (ret) *ret = EINA_TRUE;
}

static void
_sub_obj_size_hints_set(Evas_Object *sobj,
                        int timesw,
                        int timesh)
{
   Evas_Coord minw = -1, minh = -1;

   elm_coords_finger_size_adjust(timesw, &minw, timesh, &minh);
   if (sobj && eo_isa(sobj, EDJE_OBJ_CLASS))
     edje_object_size_min_restricted_calc(sobj, &minw, &minh, minw, minh);
   evas_object_size_hint_min_set(sobj, minw, minh);
   evas_object_size_hint_max_set(sobj, -1, -1);
}

static void
_item_sizing_eval(Elm_Color_Item *item)
{
   Evas_Coord minw = -1, minh = -1;
   Evas_Object *edje;

   if (!item) return;
   edje = elm_layout_edje_get(VIEW(item));
   if (!edje) return;

   elm_coords_finger_size_adjust(1, &minw, 1, &minh);
   edje_object_size_min_restricted_calc(edje, &minw, &minh, minw, minh);
   evas_object_size_hint_min_set(VIEW(item), minw, minh);
}

/* fix size hints of color palette items, so that the box gets it */
static void
_palette_sizing_eval(Evas_Object *obj)
{
   Eina_List *elist;
   Elm_Color_Item *item;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   EINA_LIST_FOREACH(sd->items, elist, item)
     _item_sizing_eval(item);
}

static void
_component_sizing_eval(Evas_Object *obj)
{
   Evas_Coord minw = -1, minh = -1;
   int i;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   for (i = 0; i < 4; i++)
     {
        if (sd->cb_data[i])
          {
             if (sd->cb_data[i]->bg_rect)
               _sub_obj_size_hints_set(sd->cb_data[i]->bg_rect, 1, 1);

             _sub_obj_size_hints_set(sd->cb_data[i]->bar, 1, 1);
             _sub_obj_size_hints_set(sd->cb_data[i]->rbt, 1, 1);
             _sub_obj_size_hints_set(sd->cb_data[i]->lbt, 1, 1);
             _sub_obj_size_hints_set(sd->cb_data[i]->colorbar, 4, 1);
          }
     }

   edje_object_size_min_restricted_calc
     (sd->col_bars_area, &minw, &minh, minw, minh);
   evas_object_size_hint_min_set(sd->col_bars_area, minw, minh);
}

static void
_full_sizing_eval(Evas_Object *obj)
{
   _palette_sizing_eval(obj);
   _component_sizing_eval(obj);
}

static void
_elm_colorselector_smart_sizing_eval(Eo *obj, void *_pd, va_list *list EINA_UNUSED)
{
   Evas_Coord minw = -1, minh = -1;

   Elm_Colorselector_Smart_Data *sd = _pd;
   Elm_Widget_Smart_Data *wd = eo_data_scope_get(obj, ELM_OBJ_WIDGET_CLASS);

   elm_coords_finger_size_adjust(1, &minw, 1, &minh);

   switch (sd->mode)
     {
      case ELM_COLORSELECTOR_PALETTE:
        _palette_sizing_eval(obj);
        break;

      case ELM_COLORSELECTOR_COMPONENTS:
        _component_sizing_eval(obj);
        break;

      case ELM_COLORSELECTOR_BOTH:
        _full_sizing_eval(obj);
        break;

      default:
        return;
     }

   edje_object_size_min_calc(wd->resize_obj, &minw, &minh);
   evas_object_size_hint_min_set(obj, minw, minh);
   evas_object_size_hint_max_set(obj, -1, -1);
}

static Eina_Bool
_on_color_long_press(void *data)
{
   Elm_Color_Item *item = (Elm_Color_Item *)data;

   ELM_COLORSELECTOR_DATA_GET(WIDGET(item), sd);

   sd->longpress_timer = NULL;
   sd->longpressed = EINA_TRUE;
   evas_object_smart_callback_call
     (WIDGET(item), SIG_COLOR_ITEM_LONGPRESSED, item);

   return ECORE_CALLBACK_CANCEL;
}

static void
_on_color_pressed(void *data,
                  Evas *e __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Elm_Color_Item *item = (Elm_Color_Item *)data;
   Evas_Event_Mouse_Down *ev = event_info;

   if (!item) return;

   ELM_COLORSELECTOR_DATA_GET(WIDGET(item), sd);

   if (ev->button != 1) return;
   elm_object_signal_emit(VIEW(item), "elm,state,selected", "elm");
   sd->longpressed = EINA_FALSE;

   if (sd->longpress_timer) ecore_timer_del(sd->longpress_timer);
   sd->longpress_timer = ecore_timer_add
       (_elm_config->longpress_timeout, _on_color_long_press, data);
}

static void
_on_color_released(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Elm_Color_Item *item = (Elm_Color_Item *)data;
   Eina_List *l;
   Elm_Color_Item *temp_item;
   Evas_Event_Mouse_Down *ev = event_info;
   if (!item) return;

   ELM_COLORSELECTOR_DATA_GET(WIDGET(item), sd);

   if (ev->button != 1) return;
   if (sd->longpress_timer)
     {
        ecore_timer_del(sd->longpress_timer);
        sd->longpress_timer = NULL;
     }
   elm_object_signal_emit(VIEW(item), "elm,state,unselected", "elm");
   if (!sd->longpressed)
     {
        evas_object_smart_callback_call
          (WIDGET(item), SIG_COLOR_ITEM_SELECTED, item);
        elm_colorselector_color_set
          (WIDGET(item), item->color->r, item->color->g, item->color->b,
          item->color->a);
     }
   EINA_LIST_FOREACH(sd->items, l, temp_item)
     if (item == temp_item) sd->selected = l;
   sd->focused = ELM_COLORSELECTOR_PALETTE;
}

static char *
_access_info_cb(void *data, Evas_Object *obj __UNUSED__)
{
   char *ret;
   Eina_Strbuf *buf;
   int r = 0, g = 0, b = 0 ,a = 0;

   Elm_Color_Item *it = data;
   ELM_COLORSELECTOR_ITEM_CHECK_OR_RETURN(it, NULL);

   elm_colorselector_palette_item_color_get((Elm_Object_Item *)it, &r, &g, &b, &a);

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "red %d, green %d, blue %d, alpha %d", r, g, b, a);
   ret = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);
   return ret;
}

static void
_access_widget_item_register(Elm_Color_Item *it)
{
   Elm_Access_Info *ai;

   _elm_access_widget_item_register((Elm_Widget_Item *)it);

   ai = _elm_access_object_get(it->base.access_obj);

   _elm_access_text_set(ai, ELM_ACCESS_TYPE, E_("color selector palette item"));
   _elm_access_callback_set(ai, ELM_ACCESS_INFO, _access_info_cb, it);
}

static void
_item_resize(void *data __UNUSED__,
             Evas *e __UNUSED__,
             Evas_Object *obj,
             void *event_info __UNUSED__)
{
   elm_layout_sizing_eval(obj);
}

static Elm_Color_Item *
_item_new(Evas_Object *obj)
{
   Elm_Color_Item *item;

   item = elm_widget_item_new(obj, Elm_Color_Item);
   if (!item) return NULL;

   VIEW(item) = elm_layout_add(obj);
   if (!elm_layout_theme_set
       (VIEW(item), "colorselector", "item", elm_widget_style_get(obj)))
     CRITICAL("Failed to set layout!");
   evas_object_size_hint_weight_set
     (VIEW(item), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(VIEW(item), EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_event_callback_add
     (VIEW(item), EVAS_CALLBACK_RESIZE, _item_resize, NULL);

   item->color_obj = edje_object_add(evas_object_evas_get(obj));
   elm_widget_theme_object_set
     (obj, item->color_obj, "colorselector", "item/color",
     elm_widget_style_get(obj));
   evas_object_size_hint_weight_set
     (item->color_obj, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set
     (item->color_obj, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_event_callback_add
     (item->color_obj, EVAS_CALLBACK_MOUSE_DOWN, _on_color_pressed, item);
   evas_object_event_callback_add
     (item->color_obj, EVAS_CALLBACK_MOUSE_UP, _on_color_released, item);
   elm_object_part_content_set(VIEW(item), "color_obj", item->color_obj);

   _item_sizing_eval(item);
   evas_object_show(VIEW(item));

   // ACCESS
   if (_elm_config->access_mode == ELM_ACCESS_MODE_ON)
     _access_widget_item_register(item);

   return item;
}

static void
_colors_remove(Evas_Object *obj)
{
   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   _items_del(sd);
   _elm_config_colors_free(sd->palette_name);
}

static void
_colors_save(Evas_Object *obj)
{
   Eina_List *elist;
   Elm_Color_Item *item;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   _elm_config_colors_free(sd->palette_name);
   EINA_LIST_FOREACH(sd->items, elist, item)
     _elm_config_color_set(sd->palette_name, item->color->r, item->color->g,
                           item->color->b, item->color->a);
}

static void
_palette_colors_load(Evas_Object *obj)
{
   Eina_List *elist;
   Elm_Color_Item *item;
   Eina_List *color_list;
   Elm_Color_RGBA *color;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   color_list = _elm_config_color_list_get(sd->palette_name);
   if (!color_list) return;

   EINA_LIST_FOREACH(color_list, elist, color)
     {
        item = _item_new(obj);
        if (!item) return;

        item->color = ELM_NEW(Elm_Color_RGBA);
        if (!item->color) return;
        item->color->r = color->r;
        item->color->g = color->g;
        item->color->b = color->b;
        item->color->a = color->a;

        elm_box_pack_end(sd->palette_box, VIEW(item));
        evas_object_color_set
          (item->color_obj, item->color->r, item->color->g, item->color->b,
          item->color->a);

        sd->items = eina_list_append(sd->items, item);
     }

   sd->config_load = EINA_TRUE;
}

static void
_elm_colorselector_smart_add(Eo *obj, void *_pd, va_list *list EINA_UNUSED)
{
   const char *hpadstr, *vpadstr;
   unsigned int h_pad = DEFAULT_HOR_PAD;
   unsigned int v_pad = DEFAULT_VER_PAD;

   Elm_Colorselector_Smart_Data *priv = _pd;
   Elm_Widget_Smart_Data *wd = eo_data_scope_get(obj, ELM_OBJ_WIDGET_CLASS);

   eo_do_super(obj, MY_CLASS, evas_obj_smart_add());

   if (!elm_layout_theme_set
       (obj, "colorselector", "palette", elm_object_style_get(obj)))
     CRITICAL("Failed to set layout!");

   priv->palette_box = elm_box_add(obj);
   elm_box_layout_set
     (priv->palette_box, evas_object_box_layout_flow_horizontal, NULL, NULL);
   elm_box_horizontal_set(priv->palette_box, EINA_TRUE);
   evas_object_size_hint_weight_set
     (priv->palette_box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set
     (priv->palette_box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_homogeneous_set(priv->palette_box, EINA_TRUE);

   hpadstr =
     edje_object_data_get(wd->resize_obj, "horizontal_pad");
   if (hpadstr) h_pad = atoi(hpadstr);
   vpadstr = edje_object_data_get
       (wd->resize_obj, "vertical_pad");
   if (vpadstr) v_pad = atoi(vpadstr);

   elm_box_padding_set
     (priv->palette_box,
     (h_pad * elm_widget_scale_get(obj) * elm_config_scale_get()),
     (v_pad * elm_widget_scale_get(obj) * elm_config_scale_get()));

   elm_box_align_set(priv->palette_box, 0.5, 0.5);
   elm_layout_content_set(obj, "palette", priv->palette_box);
   priv->palette_name = eina_stringshare_add("default");
   _palette_colors_load(obj);

   /* load background edj */
   priv->col_bars_area = edje_object_add(evas_object_evas_get(obj));
   elm_widget_theme_object_set
     (obj, priv->col_bars_area, "colorselector", "bg",
     elm_widget_style_get(obj));
   elm_layout_content_set(obj, "selector", priv->col_bars_area);

   /* setup the color picker */
   priv->picker = elm_box_add(obj);
   elm_box_horizontal_set(priv->picker, EINA_TRUE);
   evas_object_size_hint_weight_set(priv->picker, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(priv->picker, EVAS_HINT_FILL, EVAS_HINT_FILL);

   elm_box_padding_set(priv->picker,
                       (h_pad * elm_widget_scale_get(obj) * elm_config_scale_get()),
                       (v_pad * elm_widget_scale_get(obj) * elm_config_scale_get()));
   elm_box_align_set(priv->palette_box, 0.5, 0.5);

   priv->mode = ELM_COLORSELECTOR_BOTH;
   priv->focused = ELM_COLORSELECTOR_PALETTE;
   priv->sel_color_type = HUE;
   priv->selected = priv->items;
   priv->er = 255;
   priv->eg = 0;
   priv->eb = 0;
   priv->h = 0.0;
   priv->s = 1.0;
   priv->l = 0.0;
   priv->a = 255;
   priv->grab.x = -1;
   priv->grab.y = -1;
#ifdef HAVE_ELEMENTARY_X
   priv->grab.xroot = -1;
#endif
   priv->grab.in = EINA_TRUE;

   _hsl_to_rgb(priv);
   _color_bars_add(obj);
   _color_picker_add(obj, priv);

   elm_layout_sizing_eval(obj);
   elm_widget_can_focus_set(obj, EINA_TRUE);
}

static void
_elm_colorselector_smart_del(Eo *obj, void *_pd, va_list *list EINA_UNUSED)
{
   int i = 0;
   void *tmp[4];

   Elm_Colorselector_Smart_Data *sd = _pd;

   evas_event_callback_del_full(evas_object_evas_get(obj), EVAS_CALLBACK_CANVAS_FOCUS_IN, _mouse_in_canvas, obj);
   evas_event_callback_del_full(evas_object_evas_get(obj), EVAS_CALLBACK_CANVAS_FOCUS_OUT, _mouse_out_canvas, obj);

   if (sd->longpress_timer) ecore_timer_del(sd->longpress_timer);
   if (sd->palette_name) eina_stringshare_del(sd->palette_name);
#ifdef HAVE_ELEMENTARY_X
   if (sd->grab.mouse_motion) ecore_event_handler_del(sd->grab.mouse_motion);
   if (sd->grab.mouse_up) ecore_event_handler_del(sd->grab.mouse_up);
   if (sd->grab.key_up) ecore_event_handler_del(sd->grab.key_up);
#endif

   _items_del(sd);
   /* This cb_data are used during the destruction process of base.del */
   for (i = 0; i < 4; i++)
     tmp[i] = sd->cb_data[i];
   eo_do_super(obj, MY_CLASS, evas_obj_smart_del());
   for (i = 0; i < 4; i++)
     free(tmp[i]);
}

static Eina_List*
_palette_box_vertical_item_get(Eina_List* ref_item, enum Palette_Box_Direction dir)
{
   Evas_Coord basex, basey, x, y, dx, min_dx;
   Elm_Color_Item *item;
   Eina_List* l;
   Eina_List* res = NULL;
   Eina_List* (*dir_func)(const Eina_List*);

   if (!ref_item) return NULL;

   switch(dir)
     {
        case PALETTE_BOX_UP:
          dir_func = eina_list_prev;
          break;
        case PALETTE_BOX_DOWN:
          dir_func = eina_list_next;
          break;
        default:
          return NULL;
     }

   item = eina_list_data_get(ref_item);
   evas_object_geometry_get(VIEW(item), &basex, &basey, NULL, NULL);

   for (l = ref_item; l; l = dir_func(l))
     {
        item = eina_list_data_get(l);
        evas_object_geometry_get(VIEW(item), &x, &y, NULL, NULL);
        if (basey != y) break;
     }

   basey = y;
   min_dx = -1;

   for (; l; l = dir_func(l))
     {
        item = eina_list_data_get(l);
        evas_object_geometry_get(VIEW(item), &x, &y, NULL, NULL);
        if (basey != y) break;

        dx = abs(x - basex);
        if (dx < min_dx || min_dx < 0)
          {
             min_dx = dx;
             res = l;
          }
        else
          {
             break;
          }
      }

   return res;
}

static void
_elm_colorselector_smart_event(Eo *obj, void *_pd, va_list *list)
{
   Eina_List *cl = NULL;
   Elm_Color_Item *item = NULL;
   char colorbar_s[128];

   Evas_Object *src = va_arg(*list, Evas_Object *);
   (void) src;
   Evas_Callback_Type type = va_arg(*list, Evas_Callback_Type);
   void *event_info = va_arg(*list, void *);
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);
   if (ret) *ret = EINA_FALSE;

   if (type != EVAS_CALLBACK_KEY_DOWN) return;
   Evas_Event_Key_Down *ev = event_info;
   Elm_Colorselector_Smart_Data *sd = _pd;

   if (!sd) return;
   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) return;
   if (elm_widget_disabled_get(obj)) return;
   if (!sd->selected) sd->selected = sd->items;

   if ((!strcmp(ev->keyname, "Left")) ||
       ((!strcmp(ev->keyname, "KP_Left")) && (!ev->string)))
     {
        if (sd->focused == ELM_COLORSELECTOR_PALETTE && sd->selected)
          cl = eina_list_prev(sd->selected);
        else if (sd->focused == ELM_COLORSELECTOR_COMPONENTS)
          _button_clicked_cb(sd->cb_data[sd->sel_color_type], sd->cb_data[sd->sel_color_type]->lbt, NULL);
        else return;
     }
   else if ((!strcmp(ev->keyname, "Right")) ||
            ((!strcmp(ev->keyname, "KP_Right")) && (!ev->string)))
     {
        if (sd->focused == ELM_COLORSELECTOR_PALETTE && sd->selected)
          cl = eina_list_next(sd->selected);
        else if (sd->focused == ELM_COLORSELECTOR_COMPONENTS)
          _button_clicked_cb(sd->cb_data[sd->sel_color_type], sd->cb_data[sd->sel_color_type]->rbt, NULL);
        else return;
     }
   else if ((!strcmp(ev->keyname, "Up")) ||
            ((!strcmp(ev->keyname, "KP_Up")) && (!ev->string)))
     {
        if (sd->focused == ELM_COLORSELECTOR_COMPONENTS)
          {
             sd->sel_color_type = sd->sel_color_type - 1;
             if (sd->sel_color_type < HUE)
               {
                  if (sd->mode == ELM_COLORSELECTOR_BOTH)
                    {
                       sd->focused = ELM_COLORSELECTOR_PALETTE;
                       /*when focus is shifted to palette start from
                        * first item*/
                       sd->selected = sd->items;
                       cl = sd->selected;
                    }
                  else
                    {
                       sd->sel_color_type = HUE;
                       return;
                    }
               }
          }
        else if (sd->focused == ELM_COLORSELECTOR_PALETTE)
        {
            cl = _palette_box_vertical_item_get(sd->selected, PALETTE_BOX_UP);
            if (!cl) cl = sd->selected;
        }
     }
   else if ((!strcmp(ev->keyname, "Down")) ||
            ((!strcmp(ev->keyname, "KP_Down")) && (!ev->string)))
     {
        if (sd->focused == ELM_COLORSELECTOR_PALETTE)
          {
             cl = _palette_box_vertical_item_get(sd->selected, PALETTE_BOX_DOWN);
             if (sd->mode == ELM_COLORSELECTOR_BOTH && !cl)
               {
                  sd->focused = ELM_COLORSELECTOR_COMPONENTS;
                  /*when focus is shifted to component start from
                   * first color type*/
                  sd->sel_color_type = HUE;
               }
          }
        else if (sd->focused == ELM_COLORSELECTOR_COMPONENTS)
          {
             snprintf(colorbar_s, sizeof(colorbar_s), "elm.colorbar_%d",
                      (sd->sel_color_type + 1));
             /*Append color type only if next color bar is available*/
             if (edje_object_part_swallow_get(sd->col_bars_area, colorbar_s))
               sd->sel_color_type = sd->sel_color_type + 1;
             else return;
          }
     }
   else return;
   if (cl)
     {
        item = eina_list_data_get(cl);
        elm_object_signal_emit(VIEW(item), "elm,anim,activate", "elm");
        evas_object_smart_callback_call
          (WIDGET(item), SIG_COLOR_ITEM_SELECTED, item);
        elm_colorselector_color_set
          (WIDGET(item), item->color->r, item->color->g, item->color->b,
          item->color->a);
        sd->selected = cl;
     }
   else if (!cl && sd->focused == ELM_COLORSELECTOR_PALETTE)
     return;
   ev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
   if (ret) *ret = EINA_TRUE;
}

static Eina_Bool _elm_colorselector_smart_focus_next_enable = EINA_FALSE;

static void
_elm_colorselector_smart_focus_direction_manager_is(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED, va_list *list)
{
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);
   *ret = EINA_FALSE;
}

static void
_elm_colorselector_smart_focus_next_manager_is(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED, va_list *list)
{
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);
   *ret = _elm_colorselector_smart_focus_next_enable;
}

static void
_elm_colorselector_smart_focus_next(Eo *obj, void *_pd EINA_UNUSED, va_list *list)
{
   Elm_Focus_Direction dir = va_arg(*list, Elm_Focus_Direction);
   Evas_Object **next = va_arg(*list, Evas_Object **);
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);
   if (ret) *ret = EINA_FALSE;
   Eina_Bool int_ret;

   Eina_List *items = NULL;
   Eina_List *l;
   Elm_Widget_Item *item;
   int i = 0;

   Elm_Colorselector_Smart_Data *sd = _pd;
   if (!sd) return;

   if (!sd->items) return;

   EINA_LIST_FOREACH(sd->items, l, item)
     items = eina_list_append(items, item->access_obj);

   for (i = 0; i < 4; i++)
     {
        items = eina_list_append(items, sd->cb_data[i]->lbt);
        items = eina_list_append(items, sd->cb_data[i]->access_obj);
        items = eina_list_append(items, sd->cb_data[i]->rbt);
     }

   int_ret = elm_widget_focus_list_next_get
            (obj, items, eina_list_data_get, dir, next);
   if (ret) *ret = int_ret;
}

static void
_access_obj_process(Evas_Object *obj, Eina_Bool is_access)
{
   Eina_List *l;
   Elm_Color_Item *it;
   int i = 0;

   ELM_COLORSELECTOR_DATA_GET(obj, sd);

   if (is_access)
     {
        EINA_LIST_FOREACH(sd->items, l, it)
          _access_widget_item_register(it);

        for (i = 0; i < 4; i++)
          _access_colorbar_register(obj, sd->cb_data[i],
                                    "elm.arrow_bg_access");
     }
   else
     {
        EINA_LIST_FOREACH(sd->items, l, it)
          _elm_access_widget_item_unregister((Elm_Widget_Item *)it);

        //TODO: _elm_access_edje_object_part_object_unregister() ?
     }
}

static void
_elm_colorselector_smart_access(Eo *obj, void *_pd EINA_UNUSED, va_list *list)
{
   _elm_colorselector_smart_focus_next_enable = va_arg(*list, int);
   _access_obj_process(obj, _elm_colorselector_smart_focus_next_enable);
}

EAPI Evas_Object *
elm_colorselector_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = eo_add(MY_CLASS, parent);
   eo_unref(obj);
   return obj;
}

static void
_constructor(Eo *obj, void *_pd EINA_UNUSED, va_list *list EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());
   eo_do(obj,
         evas_obj_type_set(MY_CLASS_NAME),
         evas_obj_smart_callbacks_descriptions_set(_smart_callbacks, NULL));

   elm_widget_sub_object_add(eo_parent_get(obj), obj);
}

EAPI void
elm_colorselector_color_set(Evas_Object *obj,
                            int r,
                            int g,
                            int b,
                            int a)
{
   ELM_COLORSELECTOR_CHECK(obj);
   eo_do(obj, elm_obj_colorselector_color_set(r, g, b, a));
}

static void
_color_set(Eo *obj, void *_pd EINA_UNUSED, va_list *list)
{
   int r = va_arg(*list, int);
   int g = va_arg(*list, int);
   int b = va_arg(*list, int);
   int a = va_arg(*list, int);
   _colors_set(obj, r, g, b, a);
}


EAPI void
elm_colorselector_color_get(const Evas_Object *obj,
                            int *r,
                            int *g,
                            int *b,
                            int *a)
{
   ELM_COLORSELECTOR_CHECK(obj);
   eo_do((Eo *) obj, elm_obj_colorselector_color_get(r, g, b, a));
}

static void
_color_get(Eo *obj EINA_UNUSED, void *_pd, va_list *list)
{
   int *r = va_arg(*list, int *);
   int *g = va_arg(*list, int *);
   int *b = va_arg(*list, int *);
   int *a = va_arg(*list, int *);
   Elm_Colorselector_Smart_Data *sd = _pd;

   if (r) *r = sd->r;
   if (g) *g = sd->g;
   if (b) *b = sd->b;
   if (a) *a = sd->a;
}

EAPI void
elm_colorselector_mode_set(Evas_Object *obj,
                           Elm_Colorselector_Mode mode)
{
   ELM_COLORSELECTOR_CHECK(obj);
   eo_do(obj, elm_obj_colorselector_mode_set(mode));
}

static void
_mode_set(Eo *obj, void *_pd, va_list *list)
{
   Elm_Colorselector_Mode mode = va_arg(*list, Elm_Colorselector_Mode);
   Elm_Colorselector_Smart_Data *sd = _pd;
   Elm_Widget_Smart_Data *wd = eo_data_scope_get(obj, ELM_OBJ_WIDGET_CLASS);

   if (sd->mode == mode) return;
   sd->mode = mode;

   evas_object_hide(elm_layout_content_unset(obj, "selector"));
   evas_object_hide(elm_layout_content_unset(obj, "palette"));
   evas_object_hide(elm_layout_content_unset(obj, "picker"));

   switch (sd->mode)
     {
      case ELM_COLORSELECTOR_PALETTE:
        elm_layout_content_set(obj, "palette", sd->palette_box);
        elm_layout_signal_emit(obj, "elm,state,palette", "elm");
        sd->focused = ELM_COLORSELECTOR_PALETTE;
        sd->selected = sd->items;
        break;

      case ELM_COLORSELECTOR_COMPONENTS:
        elm_layout_content_set(obj, "selector", sd->col_bars_area);
        elm_layout_signal_emit(obj, "elm,state,components", "elm");
        sd->focused = ELM_COLORSELECTOR_COMPONENTS;
        sd->sel_color_type = HUE;
        break;

      case ELM_COLORSELECTOR_BOTH:
        elm_layout_content_set(obj, "palette", sd->palette_box);
        elm_layout_content_set(obj, "selector", sd->col_bars_area);
        elm_layout_signal_emit(obj, "elm,state,both", "elm");
        sd->focused = ELM_COLORSELECTOR_PALETTE;
        sd->selected = sd->items;
        break;

      case ELM_COLORSELECTOR_PICKER:
        elm_layout_content_set(obj, "picker", sd->picker);
        elm_layout_signal_emit(obj, "elm,state,picker", "elm");
        sd->focused = ELM_COLORSELECTOR_PICKER;
        break;
         
      case ELM_COLORSELECTOR_ALL:
        elm_layout_content_set(obj, "picker", sd->picker);
        elm_layout_content_set(obj, "palette", sd->palette_box);
        elm_layout_content_set(obj, "selector", sd->col_bars_area);
        elm_layout_signal_emit(obj, "elm,state,all", "elm");
        sd->focused = ELM_COLORSELECTOR_PALETTE;
        sd->selected = sd->items;
        break;

      default:
        return;
     }

   edje_object_message_signal_process(wd->resize_obj);

   elm_layout_sizing_eval(obj);
}

EAPI Elm_Colorselector_Mode
elm_colorselector_mode_get(const Evas_Object *obj)
{
   ELM_COLORSELECTOR_CHECK(obj) ELM_COLORSELECTOR_BOTH;
   Elm_Colorselector_Mode ret = ELM_COLORSELECTOR_BOTH;
   eo_do((Eo *) obj, elm_obj_colorselector_mode_get(&ret));
   return ret;
}

static void
_mode_get(Eo *obj EINA_UNUSED, void *_pd, va_list *list)
{
   Elm_Colorselector_Mode *ret = va_arg(*list, Elm_Colorselector_Mode *);
   Elm_Colorselector_Smart_Data *sd = _pd;
   *ret = sd->mode;
}

EAPI void
elm_colorselector_palette_item_color_get(const Elm_Object_Item *it,
                                         int *r,
                                         int *g,
                                         int *b,
                                         int *a)
{
   Elm_Color_Item *item;

   ELM_COLORSELECTOR_ITEM_CHECK_OR_RETURN(it);

   item = (Elm_Color_Item *)it;
   if (item)
     {
        if (r) *r = item->color->r;
        if (g) *g = item->color->g;
        if (b) *b = item->color->b;
        if (a) *a = item->color->a;
     }
}

EAPI void
elm_colorselector_palette_item_color_set(Elm_Object_Item *it,
                                         int r,
                                         int g,
                                         int b,
                                         int a)
{
   Elm_Color_Item *item;

   ELM_COLORSELECTOR_ITEM_CHECK_OR_RETURN(it);

   item = (Elm_Color_Item *)it;
   item->color->r = r;
   item->color->g = g;
   item->color->b = b;
   item->color->a = a;
   evas_object_color_set
     (item->color_obj, item->color->r, item->color->g, item->color->b,
     item->color->a);

   _colors_save(WIDGET(it));
}

EAPI Elm_Object_Item *
elm_colorselector_palette_color_add(Evas_Object *obj,
                                    int r,
                                    int g,
                                    int b,
                                    int a)
{
   ELM_COLORSELECTOR_CHECK(obj) NULL;
   Elm_Object_Item *ret = NULL;
   eo_do(obj, elm_obj_colorselector_palette_color_add(r, g, b, a, &ret));
   return ret;
}

static void
_palette_color_add(Eo *obj, void *_pd, va_list *list)
{
   int r = va_arg(*list, int);
   int g = va_arg(*list, int);
   int b = va_arg(*list, int);
   int a = va_arg(*list, int);
   Elm_Object_Item **ret = va_arg(*list, Elm_Object_Item **);
   if (ret) *ret = NULL;

   Elm_Color_Item *item;

   Elm_Colorselector_Smart_Data *sd = _pd;

   if (sd->config_load)
     {
        _colors_remove(obj);
        sd->config_load = EINA_FALSE;
     }
   item = _item_new(obj);
   if (!item) return;

   item->color = ELM_NEW(Elm_Color_RGBA);
   if (!item->color) return;

   item->color->r = r;
   item->color->g = g;
   item->color->b = b;
   item->color->a = a;
   _elm_config_color_set
     (sd->palette_name, item->color->r, item->color->g, item->color->b,
     item->color->a);

   elm_box_pack_end(sd->palette_box, VIEW(item));
   evas_object_color_set
     (item->color_obj, item->color->r, item->color->g, item->color->b,
     item->color->a);

   sd->items = eina_list_append(sd->items, item);

   elm_layout_sizing_eval(obj);

   if (ret) *ret = (Elm_Object_Item *)item;
}

EAPI void
elm_colorselector_palette_clear(Evas_Object *obj)
{
   ELM_COLORSELECTOR_CHECK(obj);
   eo_do(obj, elm_obj_colorselector_palette_clear());
}

static void
_palette_clear(Eo *obj, void *_pd, va_list *list EINA_UNUSED)
{
   Elm_Colorselector_Smart_Data *sd = _pd;

   _colors_remove(obj);
   if (sd->mode == ELM_COLORSELECTOR_BOTH)
     sd->focused = ELM_COLORSELECTOR_COMPONENTS;
}

EAPI void
elm_colorselector_palette_name_set(Evas_Object *obj,
                                   const char *palette_name)
{
   ELM_COLORSELECTOR_CHECK(obj);
   eo_do(obj, elm_obj_colorselector_palette_name_set(palette_name));
}

static void
_palette_name_set(Eo *obj, void *_pd, va_list *list)
{
   const char *palette_name = va_arg(*list, const char *);
   Elm_Colorselector_Smart_Data *sd = _pd;
   EINA_SAFETY_ON_NULL_RETURN(palette_name);

   if (!strcmp(sd->palette_name, palette_name)) return;

   _colors_remove(obj);
   eina_stringshare_replace(&sd->palette_name, palette_name);
   _palette_colors_load(obj);
}

EAPI const char *
elm_colorselector_palette_name_get(const Evas_Object *obj)
{
   ELM_COLORSELECTOR_CHECK(obj) NULL;
   const char *ret = NULL;
   eo_do((Eo *) obj, elm_obj_colorselector_palette_name_get(&ret));
   return ret;
}

static void
_palette_name_get(Eo *obj EINA_UNUSED, void *_pd, va_list *list)
{
   const char **ret = va_arg(*list, const char **);
   Elm_Colorselector_Smart_Data *sd = _pd;
   *ret = sd->palette_name;
}

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_desc[] = {

        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _constructor),

        EO_OP_FUNC(EVAS_OBJ_SMART_ID(EVAS_OBJ_SMART_SUB_ID_ADD), _elm_colorselector_smart_add),
        EO_OP_FUNC(EVAS_OBJ_SMART_ID(EVAS_OBJ_SMART_SUB_ID_DEL), _elm_colorselector_smart_del),

        EO_OP_FUNC(ELM_WIDGET_ID(ELM_WIDGET_SUB_ID_THEME), _elm_colorselector_smart_theme),
        EO_OP_FUNC(ELM_WIDGET_ID(ELM_WIDGET_SUB_ID_EVENT), _elm_colorselector_smart_event),
        EO_OP_FUNC(ELM_WIDGET_ID(ELM_WIDGET_SUB_ID_FOCUS_NEXT_MANAGER_IS), _elm_colorselector_smart_focus_next_manager_is),
        EO_OP_FUNC(ELM_WIDGET_ID(ELM_WIDGET_SUB_ID_FOCUS_DIRECTION_MANAGER_IS), _elm_colorselector_smart_focus_direction_manager_is),
        EO_OP_FUNC(ELM_WIDGET_ID(ELM_WIDGET_SUB_ID_FOCUS_NEXT), _elm_colorselector_smart_focus_next),
        EO_OP_FUNC(ELM_WIDGET_ID(ELM_WIDGET_SUB_ID_ACCESS), _elm_colorselector_smart_access),

        EO_OP_FUNC(ELM_OBJ_LAYOUT_ID(ELM_OBJ_LAYOUT_SUB_ID_SIZING_EVAL), _elm_colorselector_smart_sizing_eval),

        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_COLOR_SET), _color_set),
        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_COLOR_GET), _color_get),
        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_MODE_SET), _mode_set),
        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_MODE_GET), _mode_get),
        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_COLOR_ADD), _palette_color_add),
        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_CLEAR), _palette_clear),
        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_NAME_SET), _palette_name_set),
        EO_OP_FUNC(ELM_OBJ_COLORSELECTOR_ID(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_NAME_GET), _palette_name_get),
        EO_OP_FUNC_SENTINEL
   };
   eo_class_funcs_set(klass, func_desc);

   evas_smart_legacy_type_register(MY_CLASS_NAME, klass);

   if (_elm_config->access_mode == ELM_ACCESS_MODE_ON)
      _elm_colorselector_smart_focus_next_enable = EINA_TRUE;
}
static const Eo_Op_Description op_desc[] = {
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_COLOR_SET, "Set color to colorselector."),
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_COLOR_GET, "Get current color from colorselector."),
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_MODE_SET, "Set Colorselector's mode."),
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_MODE_GET, "Get Colorselector's mode."),
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_COLOR_ADD, "Add a new color item to palette."),
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_CLEAR, "Clear the palette items."),
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_NAME_SET, "Set current palette's name."),
     EO_OP_DESCRIPTION(ELM_OBJ_COLORSELECTOR_SUB_ID_PALETTE_NAME_GET, "Get current palette's name."),
     EO_OP_DESCRIPTION_SENTINEL
};
static const Eo_Class_Description class_desc = {
     EO_VERSION,
     MY_CLASS_NAME,
     EO_CLASS_TYPE_REGULAR,
     EO_CLASS_DESCRIPTION_OPS(&ELM_OBJ_COLORSELECTOR_BASE_ID, op_desc, ELM_OBJ_COLORSELECTOR_SUB_ID_LAST),
     NULL,
     sizeof(Elm_Colorselector_Smart_Data),
     _class_constructor,
     NULL
};
EO_DEFINE_CLASS(elm_obj_colorselector_class_get, &class_desc, ELM_OBJ_LAYOUT_CLASS, NULL);
