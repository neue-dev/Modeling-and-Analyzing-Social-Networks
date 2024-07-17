/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:36:51
 * @ Modified time: 2024-07-17 08:56:12
 * @ Description:
 * 
 * The main file of the project.
 */

#include "./model/hashmap.c"

int main() {

  HashMap *test = HashMap_new();

  HashMap_put(test, "Hello, World!", NULL);

  printf("\nHello world.");

  return 0;
}