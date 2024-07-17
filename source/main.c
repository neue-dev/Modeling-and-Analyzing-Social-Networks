/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:36:51
 * @ Modified time: 2024-07-17 09:51:11
 * @ Description:
 * 
 * The main file of the project.
 */

#include "./model/hashmap.c"

int main() {

  HashMap *test = HashMap_new();
  char *string = "hello world";

  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!", string));

  printf("\nHello world. %s", HashMap_get(test, "Hello, World!"));

  return 0;
}