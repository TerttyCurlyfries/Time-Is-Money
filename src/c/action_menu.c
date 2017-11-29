#include <pebble.h>
#include "action_menu.h"
#include "notification_window.h"
#include "main.h"

//Title
static char* title;
static char* desc;

//Action Menu
static ActionMenu *s_action_menu;
static ActionMenuLevel *action_menu;

typedef enum {
  carChoice,
  houseChoice,
  workChoice
} helloType;

typedef struct {
  helloType type;
} Context;

static helloType s_current_type;

static void hello_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {

  s_current_type = (helloType)action_menu_item_get_action_data(action);
  
  // Play this vibration
  switch(s_current_type) {
    case carChoice: title = "Clock In"; desc = "Another day, another dollar!"; show_note_menu(title, desc); getTimeIn(); break;
    case houseChoice: title = "Clock Out"; desc = "Good work entrepreneur!"; show_note_menu(title, desc); getTimeOut(); break;
    case workChoice: title = "Work"; break;
  }
}

void show_THEmenu(void){
  action_menu = action_menu_level_create(3);

  action_menu_level_add_action(action_menu, "Clock In", hello_performed_callback, (void *)carChoice);
  action_menu_level_add_action(action_menu, "Clock Out", hello_performed_callback, (void *)houseChoice);
  action_menu_level_add_action(action_menu, "Work", hello_performed_callback, (void *)workChoice);
  
  // Configure the ActionMenu Window about to be shown
  ActionMenuConfig config = (ActionMenuConfig) {
    .root_level = action_menu,
    .colors = {
      .background = GColorGreen,
      .foreground = GColorBlack,
    },
    .align = ActionMenuAlignCenter
  };
  
   // Show the ActionMenu
  s_action_menu = action_menu_open(&config);
}