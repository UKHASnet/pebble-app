// ukhasnet.c
#include <pebble.h>
#include <ukhasnet.h>

static Window *s_window;	
static TextLayer *s_top_line;
static TextLayer *s_middle_line;
static TextLayer *s_bottom_line;
// Largest expected inbox and outbox message sizes
const uint32_t inbox_size = 256;
const uint32_t outbox_size = 10;



static void window_init(Window *window) {
	// Get information about the Window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Create the TextLayer with specific bounds
	s_top_line = text_layer_create(GRect(0, 0, bounds.size.w, 50));
	s_middle_line = text_layer_create(GRect(0, 50, bounds.size.w, 50));
	s_bottom_line = text_layer_create(GRect(0, 100, bounds.size.w, 50));

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

static void inbox_recieved_callback(DictionaryIterator *iter, void *context) {
	static char str[60];
	uint32_t size = dict_size(iter);
	snprintf(str, "Receiving Message from dictionary of size %d\n\0", &size);
	printf(str);
		// Does this message contain a temperature value?
	Tuple *message_tuple = dict_find(iter, message);
	uint8_t buffer[256];
	Tuple *tuple = dict_read_begin_from_buffer(iter, buffer, 256);
	printf("Printing messages...\n");
	while (tuple) {

	  printf(tuple->value->cstring);
	  tuple = dict_read_next(iter);
	}

	if(message_tuple) {
		printf("Found message string\n");
		// This value was stored as JS Number, which is stored here as int32_t
		char *message_name = message_tuple->value->cstring;
			static char s_buffer[256];
    snprintf(s_buffer, sizeof(s_buffer), "Location: %s", message_name);
    text_layer_set_text(s_middle_line, s_buffer);
	}

}

static void setup_text_line(TextLayer *text_layer) {
	text_layer_set_background_color(text_layer, GColorClear);
	text_layer_set_text_color(text_layer, GColorBlack);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
	text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
}

static void window_deinit(Window *window) {
	text_layer_destroy(s_top_line);
	text_layer_destroy(s_middle_line);
	text_layer_destroy(s_bottom_line);
}

static void register_callbacks() {
	app_message_register_inbox_received(inbox_recieved_callback);
}



static void app_init(void) {
	// Open AppMessage
	app_message_open(inbox_size, outbox_size);
	s_window = window_create();

	window_set_window_handlers(s_window, (WindowHandlers) {
		.load = window_init,
		.unload = window_deinit
	});

	register_callbacks();

	window_stack_push(s_window, true);
}

static void app_deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(s_window);
}

int main( void ) {
	printf("This is the WATCH RAWWRRRRR");
	app_init();
	app_event_loop();
	app_deinit();
}
