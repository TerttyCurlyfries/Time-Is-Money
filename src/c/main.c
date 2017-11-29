#include <pebble.h>
#include "action_menu.h"
#include "main.h"
#include "settings_menu.h"

//Splash Vars
static Window *s_splash_window;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static TextLayer *s_splash_text;
//First Time Splash, I'm putting this under Splash Vars

#define HASOPENED_PKEY 0

static int hasOpened = 0;
static int debug = HASOPENED_PKEY;
static char debug_char[18];
//snprintf(debug_char, sizeof(debug_char), "%u Fucks given", debug);

//Hub Vars
static Window *s_hub_window;
static BitmapLayer *w_background_layer;
static GBitmap *w_background_bitmap;
static GFont w_time_font, w_clock_font;
static TextLayer *w_time_layer, *w_date_layer, *w_mph_layer;
static TextLayer *w_IN_layer, *w_OUT_layer;
static TextLayer *w_INTIME_layer, *w_OUTTIME_layer;

//IN and OUT Vars
static char inbuffer[] = "00:00";
static char outbuffer[] = "00:00";

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char buffer[] = "00:00";
  if(clock_is_24h_style()) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof(buffer), "%I:%M", tick_time);
  if('0' == buffer[0]) {
    memmove(buffer, &buffer[1], sizeof(buffer)-1);
  }
  //strftime(ampm_buffer, sizeof(ampm_buffer), "%p", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(w_time_layer, buffer);
  
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%b %d", tick_time);
  // Show the date
  text_layer_set_text(w_date_layer, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void getTimeIn(void){
  
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *statictick_time = localtime(&temp);
  
  strftime(inbuffer, sizeof(inbuffer), "%I:%M", statictick_time);
  text_layer_set_text(w_INTIME_layer, inbuffer);
  
}

void getTimeOut(void){

  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *statictick_time = localtime(&temp);
  
  strftime(outbuffer, sizeof(outbuffer), "%I:%M", statictick_time);
  text_layer_set_text(w_OUTTIME_layer, outbuffer);
  
}
void LETSBREAKITDOWN(void * data){
  
  window_stack_push(s_hub_window, true);
  window_stack_remove(s_splash_window, false);
  
  hasOpened = 1;
  
    // Write the data structure
  persist_write_int(HASOPENED_PKEY, hasOpened);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "Data WROTED, user won't be seeing that splash no more no sir!");
//  APP_LOG(APP_LOG_LEVEL_INFO, debug);

}

void MONEY(void * data){
  text_layer_set_text(s_splash_text, "MONEY");
  app_timer_register(1000, LETSBREAKITDOWN, NULL);
}

void TIME(void * data){
  text_layer_set_text(s_splash_text, "IS");
  app_timer_register(1000, MONEY, NULL);
}

//Thing for timer to do once it runs out? Idk it works though
void test(void * data){

  if(persist_read_int(HASOPENED_PKEY) == 0){
  text_layer_set_text(s_splash_text, "TIME");
  app_timer_register(1000, TIME, NULL);
  }else{
     // Register with TickTimerService
  window_stack_push(s_hub_window, true);
  window_stack_remove(s_splash_window, false);
  }
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_splash_window, GColorGreen);
  
  s_background_layer = bitmap_layer_create(bounds);

  s_splash_text = text_layer_create(GRect(0, 65, 144, 30));
  text_layer_set_text_color(s_splash_text, GColorBlack);
  text_layer_set_background_color(s_splash_text, GColorClear);
  text_layer_set_text(s_splash_text, "$$$");
  text_layer_set_text_alignment(s_splash_text, GTextAlignmentCenter);
  text_layer_set_font(s_splash_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  // Add to Window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_splash_text));
  
}

void hub_select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
 show_THEmenu();
}

void hub_down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
 show_settings_menu();
}

void hub_config_provider(Window *window) {
// window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
 window_single_click_subscribe(BUTTON_ID_SELECT, hub_select_single_click_handler);
 window_single_click_subscribe(BUTTON_ID_DOWN, hub_down_single_click_handler);
}

static void hub_window_load(Window *window) {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_hub_window, GColorFromRGB(164,189,153));
  
  s_background_layer = bitmap_layer_create(bounds);
  
  // Create date TextLayer
  w_date_layer = text_layer_create(GRect(0, 45, 144, 30));
  text_layer_set_text_color(w_date_layer, GColorBlack);
  text_layer_set_background_color(w_date_layer, GColorClear);
  text_layer_set_text_alignment(w_date_layer, GTextAlignmentCenter);
  w_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_CONDENSED_16));
  text_layer_set_font(w_date_layer, w_time_font);
  // Add to Window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(w_date_layer));

  w_mph_layer = text_layer_create(GRect(0, 75, 144, 30));
  text_layer_set_text_color(w_mph_layer, GColorBlack);
  text_layer_set_background_color(w_mph_layer, GColorClear);
  text_layer_set_text(w_mph_layer, "$pH:");
  text_layer_set_text_alignment(w_mph_layer, GTextAlignmentCenter);
  text_layer_set_font(w_mph_layer, w_time_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(w_mph_layer));
  
  w_IN_layer = text_layer_create(GRect(-10, 110, 80, 50));
  text_layer_set_text_color(w_IN_layer, GColorBlack);
  text_layer_set_background_color(w_IN_layer, GColorClear);
  text_layer_set_text(w_IN_layer, "IN:");
  text_layer_set_text_alignment(w_IN_layer, GTextAlignmentCenter);
  text_layer_set_font(w_IN_layer, w_time_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(w_IN_layer));
  
  w_INTIME_layer = text_layer_create(GRect(-10, 130, 80, 50));
  text_layer_set_text_color(w_INTIME_layer, GColorBlack);
  text_layer_set_background_color(w_INTIME_layer, GColorClear);
  text_layer_set_text(w_INTIME_layer, "4:20");
  text_layer_set_text_alignment(w_INTIME_layer, GTextAlignmentCenter);
  text_layer_set_font(w_INTIME_layer, w_time_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(w_INTIME_layer));
  
  w_OUT_layer = text_layer_create(GRect(70, 110, 80, 50));
  text_layer_set_text_color(w_OUT_layer, GColorBlack);
  text_layer_set_background_color(w_OUT_layer, GColorClear);
  text_layer_set_text(w_OUT_layer, "OUT:");
  text_layer_set_text_alignment(w_OUT_layer, GTextAlignmentCenter);
  text_layer_set_font(w_OUT_layer, w_time_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(w_OUT_layer));
  
  w_OUTTIME_layer = text_layer_create(GRect(70, 130, 80, 50));
  text_layer_set_text_color(w_OUTTIME_layer, GColorBlack);
  text_layer_set_background_color(w_OUTTIME_layer, GColorClear);
  text_layer_set_text(w_OUTTIME_layer, "6:90");
  text_layer_set_text_alignment(w_OUTTIME_layer, GTextAlignmentCenter);
  text_layer_set_font(w_OUTTIME_layer, w_time_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(w_OUTTIME_layer));
  
  // Create the TextLayer with specific bounds
  w_time_layer = text_layer_create(GRect(0, 15, 144, 30));
  text_layer_set_background_color(w_time_layer, GColorClear);
  text_layer_set_text_color(w_time_layer, GColorBlack);
  text_layer_set_text(w_time_layer, "00:00");
  text_layer_set_text_alignment(w_time_layer, GTextAlignmentCenter);
  //text_layer_set_size(w_time_layer, );
  w_clock_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_CONDENSED_24));
  text_layer_set_font(w_time_layer, w_clock_font);
  layer_add_child(window_layer, text_layer_get_layer(w_time_layer));
  }

static void hub_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(w_time_layer);
  text_layer_destroy(w_date_layer);
  text_layer_destroy(w_IN_layer);
  text_layer_destroy(w_OUT_layer);
  fonts_unload_custom_font(w_time_font);
  
  gbitmap_destroy(w_background_bitmap);
  bitmap_layer_destroy(w_background_layer);

}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_splash_text);
  bitmap_layer_destroy(s_background_layer);
  gbitmap_destroy(s_background_bitmap);
}

static void init() {
  
  // Create main Window element and assign to pointer
  s_splash_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_splash_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_splash_window, true);
  
  //Creating hub
  s_hub_window = window_create();
    
  window_set_click_config_provider(s_hub_window, (ClickConfigProvider) hub_config_provider);

  window_set_window_handlers(s_hub_window, (WindowHandlers) {
    .load = hub_window_load,
    .unload = hub_window_unload
  });
  
  //Hi Ho timer
  app_timer_register(1000, test, NULL);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_splash_window);
  window_destroy(s_hub_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}