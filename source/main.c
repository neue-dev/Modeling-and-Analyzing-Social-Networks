/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:36:51
 * @ Modified time: 2024-07-17 10:27:32
 * @ Description:
 * 
 * The main file of the project.
 */

#include <stdio.h>
#include "./model/hashmap.c"


int main() {

  HashMap *test = HashMap_new();
  char *string = "hello world";

  for(int i = 0; i < 10000000; i++) {
    char s[32];
    HashMap_put(test, itoa(i, s, 10), NULL);
  }

  printf("success");

  return 0;
}