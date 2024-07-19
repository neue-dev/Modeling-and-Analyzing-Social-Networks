/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:36:51
 * @ Modified time: 2024-07-19 10:12:27
 * @ Description:
 * 
 * The main file of the project.
 */

#include <stdio.h>
#include <string.h>

#include "./model/hashmap.c"


int main() {

  HashMap *test = HashMap_new();
  char *string = "hello world";

  for(int i = 0; i < 1000000; i++) {
    char s[32];
    snprintf(s, 16, "%d", i);

    HashMap_put(test, s, NULL);
  }

  printf("success");

  return 0;
}