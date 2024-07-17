/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:36:51
 * @ Modified time: 2024-07-17 10:12:20
 * @ Description:
 * 
 * The main file of the project.
 */

#include <stdio.h>
#include "./model/hashmap.c"


int main() {

  HashMap *test = HashMap_new();
  char *string = "hello world";

  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!0", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!1", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!2", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!3", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!4", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!5", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!6", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!7", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!8", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!9", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!10", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!11", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!12", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!13", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!14", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!15", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!16", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!17", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!18", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!19", string));
  printf("\nsuccess: %d", HashMap_put(test, "Hello, World!20", string));

  printf("\nHello world. %s", HashMap_get(test, "Hello, World!20"));

  return 0;
}