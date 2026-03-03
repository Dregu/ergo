#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include "state.h"
#include "render.h"


static void
cairo_set_source_u32(cairo_t *cairo, uint32_t color) {
	cairo_set_source_rgba(
		cairo,
		(color >> 24 & 0xff) / 255.,
		(color >> 16 & 0xff) / 255.,
		(color >> 8 & 0xff) / 255.,
		(color >> 0 & 0xff) / 255.
	);
}


void
render(void *data, struct state *state)
{
	int stride = state->width * 4;

	cairo_surface_t *surface = cairo_image_surface_create_for_data(
	  data,
		CAIRO_FORMAT_ARGB32,
		state->width,
		state->height,
		stride
	);
	cairo_t *cairo = cairo_create(surface);

	cairo_set_antialias(cairo, CAIRO_ANTIALIAS_BEST);
	cairo_set_source_u32(cairo, state->normal_bg);
	cairo_paint(cairo);
    cairo_set_source_u32(cairo, state->normal_fg);

	PangoLayout *layout = pango_cairo_create_layout(cairo);
	PangoFontDescription *desc = pango_font_description_from_string(state->font);
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

    for (int side = 0; side <= 1; side++) {
        int width = 0;
        int height;
        char* plain_text = NULL;
        PangoAttrList* attr_list = NULL;
        pango_parse_markup(state->items[side], -1, 0, &attr_list, &plain_text, NULL, NULL);
        pango_layout_set_attributes(layout, attr_list);
        pango_layout_set_text(layout, plain_text, -1);
        pango_layout_get_pixel_size(layout, &width, &height);
        
        int x;
        if (side)
            x = state->width - width;
        else
            x = 0;
        cairo_move_to(cairo, x, 0);
        pango_cairo_show_layout(cairo, layout);
    }

	g_object_unref(layout);
}

