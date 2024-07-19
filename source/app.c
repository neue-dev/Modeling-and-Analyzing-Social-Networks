/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 18:40:56
 * @ Modified time: 2024-07-19 21:07:32
 * @ Description:
 * 
 * The main flow of the application.
 */

#ifndef APP_C
#define APP_C

#include "./io/ui.c"
#include "./model/model.c"

#define APP_DEFAULT_DATASET "./data/Caltech36.txt"

#define APP_INDENT_INFO "[] "
#define APP_INDENT_EMPTY "   "
#define APP_INDENT_PROMPT "[> "
#define APP_INDENT_SUBINFO "[| "
#define APP_INDENT_SUCCESS " ! "
#define APP_INDENT_FAILURE " X "

typedef enum AppState AppState;

enum AppState {
  APPSTATE_INIT,
  APPSTATE_MENU,
  APPSTATE_LOAD,
  APPSTATE_FRIENDS,
  APPSTATE_CONNECTIONS,
  APPSTATE_EXIT,
};

struct App {

  // The current state of the app
  // In other words, what page we're on
  AppState appState;

  // The path to the active dataset
  char activeDataset[256];

} App;

/**
 * Prints the app header.
*/
void App_header() {
  
  // Print the header  
  UI_clear(); UI__();
  UI_indent(""); UI_s("Modeling and Analyzing Networks"); UI__(); 
  UI_indent(""); UI_s("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""); UI__();
  UI__();
}

/**
 * Initialize the app state.
*/
void App_init() {

  // Init the UI object first
  UI_init();

  // Print init state
  UI_s("Initializing the app..."); UI__();

  // Init the model
  Model_init();

  // Read the data we want
  Model_loadData(APP_DEFAULT_DATASET);

  // Set the active dataset
  strcpy(App.activeDataset, APP_DEFAULT_DATASET);

  // Run the menu after the init
  App.appState = APPSTATE_MENU;
}

/**
 * Show the main menu.
*/
void App_menu() {

  // Print the options
  UI_indent(APP_INDENT_INFO); UI_s("Select what to do next."); UI__();
  UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_indent("1. "); UI_s("Load another dataset."); UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_indent("2. "); UI_s("Display friend list."); UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_indent("3. "); UI_s("Display connections."); UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_indent("0. "); UI_s("Exit the app."); UI__();
  UI__();
  
  // Wait for user input
  int option = 0;
  scanf("%d", &option);

  // Go to the next page
  switch(option) {
    case 0: App.appState = APPSTATE_EXIT; break;
    case 1: App.appState = APPSTATE_LOAD; break;
    case 2: App.appState = APPSTATE_FRIENDS; break;
    case 3: App.appState = APPSTATE_CONNECTIONS; break;
    default: App.appState = APPSTATE_EXIT; break;
  }
}

/**
 * Load another dataset.
*/
void App_load() {

  // print the prompt
  UI_indent(APP_INDENT_INFO); UI_s("Specify a dataset to load."); UI__(); 
  UI__();
  UI_indent(APP_INDENT_PROMPT);

  // Wait for user input
  char filepath[256];
  scanf("%s", filepath);
  UI__();

  // ! check if filepath valid first

  // Clear the model first
  UI_indent(APP_INDENT_SUBINFO); UI_s("Clearing previous model data...."); UI__();
  Model_clearData();

  // Load the selected dataset
  UI_indent(APP_INDENT_SUBINFO); UI_s("Loading specified dataset...."); UI__();
  Model_loadData(filepath);

  // Change the active dataset
  strcpy(App.activeDataset, filepath);
  
  // Go to menu
  App.appState = APPSTATE_MENU;
}

/**
 * Runs the model and displays the friends of a given node.
*/
void App_friends() {
  App.appState = APPSTATE_MENU;
}

/**
 * Runs the model and looks for paths within the data.
*/
void App_connections() {
  App.appState = APPSTATE_MENU;
}

/**
 * The main process of the app.
 * Switches between the different pages.
*/
void App_main() {

  do {

    // Print the header each time after init
    if(App.appState != APPSTATE_INIT)
      App_header();

    // Execute the correct oage
    switch(App.appState) {
      
      // Init the app
      case APPSTATE_INIT: App_init(); break;

      // Load another dataset
      case APPSTATE_LOAD: App_load(); break;

      // Load another dataset
      case APPSTATE_FRIENDS: App_friends(); break;

      // Load another dataset
      case APPSTATE_CONNECTIONS: App_connections(); break;

      // Run the main menu of the app
      case APPSTATE_MENU: App_menu(); break;

      // Exit the app
      case APPSTATE_EXIT:
      default: break;
    }

  // While the app isn't done running
  } while(App.appState != APPSTATE_EXIT);
}

#endif