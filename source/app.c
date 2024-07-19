/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 18:40:56
 * @ Modified time: 2024-07-19 20:14:31
 * @ Description:
 * 
 * The main flow of the application.
 */

#ifndef APP_C
#define APP_C

#include "./io/ui.c"
#include "./model/model.c"

#define APP_DEFAULT_DATASET "./data/Caltech36.txt"
#define APP_INDENT_INFO "[*]"
#define APP_INDENT_EMPTY "[ ]"
#define APP_INDENT_SUBINFO " | "
#define APP_INDENT_SUCCESS " ! "
#define APP_INDENT_FAILURE " X "

typedef enum AppState AppState;

enum AppState {
  APP_INIT,
  APP_MENU,
  APP_EXIT,
};

struct App {

  // The current state of the app
  // In other words, what page we're on
  AppState appState;

} App;

/**
 * Initialize the app state.
*/
void App_init() {

  // Init the UI object first
  UI_init();

  // Print init state
  UI_indent(APP_INDENT_INFO); UI_s("Initializing the app..."); UI__();

  // Init the model
  Model_init();

  // Read the data we want
  Model_readData(APP_DEFAULT_DATASET);

  // Say that we're done initting
  UI_indent(APP_INDENT_SUCCESS); UI_s("Loaded default dataset: "); UI_s(APP_DEFAULT_DATASET); UI__();
  UI_indent(APP_INDENT_SUCCESS); UI_s("App initialized."); UI__(); 
  UI__();

  // Run the menu after the init
  App.appState = APP_MENU;
}

/**
 * Show the main menu.
*/
void App_menu() {
  
  // Print the options
  UI_indent(APP_INDENT_INFO); UI_s("Select what to do next."); UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_indent("-0-"); UI_s("Load another dataset."); UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_indent("-1-"); UI_s("Display friend list."); UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_indent("-2-"); UI_s("Display connections."); UI__();
  UI__();
  
  App.appState = APP_EXIT;
}

/**
 * The main process of the app.
 * Switches between the different pages.
*/
void App_main() {

  do {

    // Execute the correct oage
    switch(App.appState) {
      
      // Init the app
      case APP_INIT: App_init(); break;

      // Run the main menu of the app
      case APP_MENU: App_menu(); break;

      // Exit the app
      case APP_EXIT:
      default: break;
    }

  // While the app isn't done running
  } while(App.appState != APP_EXIT);
}

#endif