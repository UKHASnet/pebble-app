// ukhasnet.h
typedef enum keys {
  AppKeyPacket = 0x0,         // TUPLE_CSTRING
  AppKeyNode = 0x1,
  AppKeyTime = 0x2
}AppKeys;

static void main_window_init();

static void main_window_deinit();

static void app_init();

static void app_deinit();

static void setup_text_line(TextLayer *text_layer);

static char* get_message_from_key(DictionaryIterator *iter, AppKeys key,const char * format, char * buffer);

