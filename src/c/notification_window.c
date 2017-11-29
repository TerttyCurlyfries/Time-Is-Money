#include <pebble.h>
#include "notification_window.h"

//Splash Vars
static Window *s_note_window;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static TextLayer *s_note_text;
static TextLayer *s_subnote_text;

char* condition;
char* descrption;

void noteCrash(void * data){

  window_stack_remove(s_note_window, true);

}

static void note_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_note_window, GColorGreen);
  
  s_background_layer = bitmap_layer_create(bounds);

  s_note_text = text_layer_create(GRect(0, 65, 144, 30));
  text_layer_set_text_color(s_note_text, GColorBlack);
  text_layer_set_background_color(s_note_text, GColorClear);
  text_layer_set_text(s_note_text, condition);  
  text_layer_set_text_alignment(s_note_text, GTextAlignmentCenter);
  text_layer_set_font(s_note_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  // Add to Window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_note_text));
  
  s_subnote_text = text_layer_create(GRect(0, 100, 144, 30));
  text_layer_set_text_color(s_subnote_text, GColorBlack);
  text_layer_set_background_color(s_subnote_text, GColorClear);
  text_layer_set_text(s_subnote_text, descrption);  
  text_layer_set_text_alignment(s_subnote_text, GTextAlignmentCenter);
  text_layer_set_font(s_subnote_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  // Add to Window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_subnote_text));
  
}

static void note_window_unload(Window *window) {

}

void show_note_menu(char* title, char* desc){
  
  condition = title;
  descrption = desc;
  s_note_window = window_create();
  
  window_set_window_handlers(s_note_window, (WindowHandlers) {
    .load = note_window_load,
    .unload = note_window_unload,
  });
  
  vibes_short_pulse();
  window_stack_push(s_note_window, true);
  
    //Hi Ho timer
  app_timer_register(2000, noteCrash, NULL);
}