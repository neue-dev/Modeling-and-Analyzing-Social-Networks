/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 18:40:56
 * @ Modified time: 2024-07-29 03:55:23
 * @ Description:
 * 
 * The main flow of the application.
 */

#ifndef APP_C
#define APP_C

#include "./io/ui.c"
#include "./model/model.c"

#define APP_DEFAULT_DATASET "./data/Caltech36.txt"
#define APP_DEFAULT_COLS 7

#define APP_INDENT_INFO "[] "
#define APP_INDENT_EMPTY "   "
#define APP_INDENT_PROMPT "[> "
#define APP_INDENT_SUBINFO "[| "
#define APP_INDENT_SUCCESS "[! "
#define APP_INDENT_FAILURE "[X "

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

} App;

/**
 * Prints the app header.
*/
void App_header() {
  
  // Print the header  
  UI_clear(); UI__();
  UI_indent(""); UI_s("Modeling and Analyzing Networks"); UI__(); 
  UI_indent(""); UI_s("\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""); UI__();
  UI_indent(""); UI_s("[] Loaded: "); UI_s(Model.activeDataset); UI__();
  UI_indent(""); UI_s("_______________________________"); UI__();
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
  char option[1024];
  scanf(" %s", option);
  
  // Go to the next page
  switch(option[0] - 48) {
    case 0: App.appState = APPSTATE_EXIT; break;
    case 1: App.appState = APPSTATE_LOAD; break;
    case 2: App.appState = APPSTATE_FRIENDS; break;
    case 3: App.appState = APPSTATE_CONNECTIONS; break;

    // Do nothing and just remprompt
    default: App.appState = APPSTATE_MENU; break;
  }
}

/**
 * Load another dataset.
*/
void App_load() {

  // The user input
  char filepath[256];

  // Print the prompt
  UI_indent(APP_INDENT_INFO); UI_s("Specify a dataset to load."); UI__(); 
  UI_input(APP_INDENT_PROMPT, filepath);

  // Check if the name is valid first
  if(!Model_checkValidFile(filepath)) {
    
    // Print the prompt
    UI__();
    UI_indent(APP_INDENT_FAILURE); UI_s("Datasets can only be text files."); UI__();
    UI_indent(APP_INDENT_SUBINFO); UI_s("Load another dataset? (y/n)"); UI__();
    
    // Go to menu if no
    if(!UI_response(APP_INDENT_PROMPT))
      App.appState = APPSTATE_MENU;

    // Don't do the rest of the function
    return;
  }

  // Clear the model first
  UI_indent(APP_INDENT_SUBINFO); UI_s("Clearing previous model data...."); UI__();
  Model_clearData();

  // Load the selected dataset
  UI_indent(APP_INDENT_SUBINFO); UI_s("Loading specified dataset...."); UI__();
  
  // If the model doesn't exist or couldn't be found, stay on this page
  if(!Model_loadData(filepath)) {

    // Print the prompt
    UI__();
    UI_indent(APP_INDENT_FAILURE); UI_s("Could not find the specified dataset."); UI__();
    UI_indent(APP_INDENT_SUBINFO); UI_s("Input another dataset? (y/n)"); UI__();

    // Stay in the page
    if(UI_response(APP_INDENT_PROMPT))
      return;
  }
  
  // Go to menu
  App.appState = APPSTATE_MENU;
}

/**
 * A warning to place when invalid dataset was loaded.
 * 
 * @return  { int }   Whether or not no dataset is loaded.
 */
int App_hasNoDataset() {

  // A dataset was loaded
  if(strcmp(Model.activeDataset, MODEL_EMPTY))
    return 0;
  
  // Warning
  UI__();
  UI_indent(APP_INDENT_INFO); UI_s("No dataset is currently loaded."); UI__();
  UI_indent(APP_INDENT_SUBINFO); UI_s("Press any key to continue."); UI__();
  UI_response(APP_INDENT_PROMPT);

  // Go to menu
  App.appState = APPSTATE_MENU;

  // No dataset was loaded
  return 1;
}

/**
 * Runs the model and displays the friends of a given node.
*/
void App_friends() {

  // The user input
  char id[256];

  // No dataset loaded
  if(App_hasNoDataset())
    return;

  // Print the prompt
  UI_indent(APP_INDENT_INFO); UI_s("You are now viewing the friend count for a given node."); UI__();
  UI_indent(APP_INDENT_INFO); UI_s("Specify a node to inspect."); UI__(); 
  UI_input(APP_INDENT_PROMPT, id);

  // Print the adj list with APP_DEFAULT_COLS number of columns
  Model_printFriendList(id, APP_DEFAULT_COLS);

  // Type any key to continue
  UI__();
  UI_indent(APP_INDENT_INFO); UI_s("Inspect another node? (y/n)"); UI__();
  
  // Stay on page if yes
  if(UI_response(APP_INDENT_PROMPT))
    return;

  // Go to menu
  App.appState = APPSTATE_MENU;
}

/**
 * Runs the model and looks for paths within the data.
*/
void App_connections() {

  // The source and target ids
  char sourceId[256];
  char targetId[256];

  // No dataset loaded
  if(App_hasNoDataset())
    return;

  // Prompt for both ids
  UI_indent(APP_INDENT_INFO); UI_s("You are now viewing connections between two nodes."); UI__();
  UI_indent(APP_INDENT_INFO); UI_s("Specify a node 1."); UI__(); 
  UI_input(APP_INDENT_PROMPT, sourceId);
  UI_indent(APP_INDENT_INFO); UI_s("Specify a node 2."); UI__(); 
  UI_input(APP_INDENT_PROMPT, targetId);

  // Print the connections with the right number of cols
  Model_printConnection(sourceId, targetId, APP_DEFAULT_COLS);

  // Type any key to continue
  UI__();
  UI_indent(APP_INDENT_INFO); UI_s("View another connection? (y/n)"); UI__();
  
  // Stay on page if yes
  if(UI_response(APP_INDENT_PROMPT))
    return;

  // Go to menu
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