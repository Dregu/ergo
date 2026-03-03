#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include "render.h"
#include "state.h"

static void cairo_set_source_u32(cairo_t* cairo, uint32_t color)
{
    cairo_set_source_rgba(cairo, (color >> 24 & 0xff) / 255., (color >> 16 & 0xff) / 255., (color >> 8 & 0xff) / 255., (color >> 0 & 0xff) / 255.);
}

gboolean no_color(PangoAttribute* a, void*)
{
    if (a->klass->type == PANGO_ATTR_FOREGROUND || a->klass->type == PANGO_ATTR_FOREGROUND_ALPHA)
        return false;
    return true;
}

void render(void* data, struct state* state)
{
    int stride = state->width * 4;

    cairo_surface_t* surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32, state->width, state->height, stride);
    cairo_t* cairo = cairo_create(surface);

    cairo_set_antialias(cairo, CAIRO_ANTIALIAS_BEST);
    cairo_set_source_u32(cairo, state->normal_bg);
    cairo_paint(cairo);
    cairo_set_source_u32(cairo, state->normal_fg);

    PangoLayout* layout = pango_cairo_create_layout(cairo);
    PangoFontDescription* desc = pango_font_description_from_string(state->font);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    for (int col = 0; col < state->cols; col++)
    {
        int width = 0;
        int height;
        char* plain_text = NULL;
        PangoAttrList* attr_list = NULL;
        pango_parse_markup(state->items[col], -1, 0, &attr_list, &plain_text, NULL, NULL);
        pango_layout_set_attributes(layout, attr_list);
        pango_layout_set_text(layout, plain_text, -1);
        pango_layout_get_pixel_size(layout, &width, &height);
        int x = 4;
        if (col >= 1 && col < state->cols - 1)
            x = (state->width - width) / (state->cols - 1);
        else if (col == state->cols - 1)
            x = state->width - width - x;
        cairo_move_to(cairo, x, (state->height - height) / 2);
        pango_cairo_show_layout(cairo, layout);
    }

    g_object_unref(layout);
}
