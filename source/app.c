/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 18:40:56
 * @ Modified time: 2024-07-19 19:14:33
 * @ Description:
 * 
 * The main flow of the application.
 */

#ifndef APP_C
#define APP_C

#include "./model/model.c"

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

  // Init the model
  Model_init();

  // Read the data we want
  Model_readData("./data/Trinity100.txt");

  // Run the menu after the init
  App.appState = APP_MENU;
}

/**
 * Show the main menu.
*/
void App_menu() {
  
  // ! Remove this later
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