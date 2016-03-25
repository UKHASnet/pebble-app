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
static char t_buffer[64];
static char m_buffer[64];
static char b_buffer[64];


static void window_init(Window *window) {
	// Get information about the Window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Create the TextLayer with specific bounds
	s_top_line = text_layer_create(GRect(0, 0, bounds.size.w, 30));
	s_middle_line = text_layer_create(GRect(0, 30, bounds.size.w, 110));
	s_bottom_line = text_layer_create(GRect(0, 110, bounds.size.w, 144));

	// Improve the layout to be more like a watchface
	setup_text_line(s_top_line);
	setup_text_line(s_middle_line);
	setup_text_line(s_bottom_line);

	text_layer_set_background_color(s_top_line, GColorPictonBlue);
	text_layer_set_text_color(s_top_line, GColorWhite);
	text_layer_set_background_color(s_middle_line, GColorRajah);
	//text_layer_set_text_color(s_middle_line, GColorWhite);
	text_layer_set_background_color(s_bottom_line, GColorOxfordBlue);
	text_layer_set_text_color(s_bottom_line, GColorWhite);
	

	text_layer_set_text(s_top_line, "UKHASnet");
	text_layer_set_text(s_middle_line, "Node");
	text_layer_set_text(s_bottom_line, "Watcher");

	layer_add_child(window_layer, text_layer_get_layer(s_top_line));
	layer_add_child(window_layer, text_layer_get_layer(s_middle_line));
	layer_add_child(window_layer, text_layer_get_layer(s_bottom_line));
}

static void inbox_recieved_callback(DictionaryIterator *iter, void *context) {
	text_layer_set_text(s_top_line, get_message_from_key(iter,AppKeyNode, "Node: %s",t_buffer));
	text_layer_set_text(s_middle_line, get_message_from_key(iter,AppKeyPacket, "Packet: %s",m_buffer));
	text_layer_set_text(s_bottom_line, get_message_from_key(iter,AppKeyTime, "Time: %s",b_buffer));
	//vibes_short_pulse();
}

static char* get_message_from_key(DictionaryIterator *iter, AppKeys key,const char * format, char * buffer){
	Tuple *message_tuple = dict_find(iter, key);
	if(message_tuple) {
		printf("Found message string\n");
		// This value was stored as JS Number, which is stored here as int32_t
		char *message_name = message_tuple->value->cstring;
	    snprintf(buffer, 64, format, message_name);
	    return buffer;
	}
	return "";
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  // A message was received, but had to be dropped
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped. Reason: %d", (int)reason);
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
	// Register to be notified about inbox dropped events
	app_message_register_inbox_dropped(inbox_dropped_callback);
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
