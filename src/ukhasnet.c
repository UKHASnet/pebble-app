// ukhasnet.c
#include <pebble.h>
#include <ukhasnet.h>

static Window *s_window;	
static TextLayer *s_top_line;

static void window_init(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_top_line = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_top_line, GColorClear);
  text_layer_set_text_color(s_top_line, GColorBlack);
  text_layer_set_text(s_top_line, "00:00");
  text_layer_set_font(s_top_line, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_top_line, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_top_line));
}

static void window_deinit(Window *window) {
	text_layer_destroy(s_top_line);
}


static void init(void) {
	s_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_init,
    .unload = window_deinit
  });

	window_stack_push(s_window, true);
}

static void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(s_window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}