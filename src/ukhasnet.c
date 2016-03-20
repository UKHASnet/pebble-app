// ukhasnet.c
#include <pebble.h>
#include <ukhasnet.h>

static Window *s_window;	
static TextLayer *s_top_line;
static TextLayer *s_middle_line;
static TextLayer *s_bottom_line;

static void window_init(Window *window) {
	// Get information about the Window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Create the TextLayer with specific bounds
	s_top_line = text_layer_create(GRect(0, 10, bounds.size.w, 50));
	s_middle_line = text_layer_create(GRect(0, 70, bounds.size.w, 50));
	s_bottom_line = text_layer_create(GRect(0, 130, bounds.size.w, 50));

	// Improve the layout to be more like a watchface
	setup_text_line(s_top_line);
	setup_text_line(s_middle_line);
	setup_text_line(s_bottom_line);

	text_layer_set_text(s_top_line, "TOP");
	text_layer_set_text(s_middle_line, "MIDDLE");
	text_layer_set_text(s_bottom_line, "MEGA LOLZ");

	layer_add_child(window_layer, text_layer_get_layer(s_top_line));
	layer_add_child(window_layer, text_layer_get_layer(s_middle_line));
	layer_add_child(window_layer, text_layer_get_layer(s_bottom_line));
}

static void setup_text_line(TextLayer *text_layer) {
	text_layer_set_background_color(text_layer, GColorClear);
	text_layer_set_text_color(text_layer, GColorBlack);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_40_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
}

static void window_deinit(Window *window) {
	text_layer_destroy(s_top_line);
	text_layer_destroy(s_middle_line);
	text_layer_destroy(s_bottom_line);
}


static void app_init(void) {
	s_window = window_create();
	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_window, (WindowHandlers) {
		.load = window_init,
		.unload = window_deinit
	});

	window_stack_push(s_window, true);
}

static void app_deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(s_window);
}

int main( void ) {
	app_init();
	app_event_loop();
	app_deinit();
}
