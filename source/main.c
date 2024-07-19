/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:36:51
 * @ Modified time: 2024-07-19 11:59:29
 * @ Description:
 * 
 * The main file of the project.
 */

#include "./model/model.c"
#include "./model/hashmap.c"

#include <stdio.h>
#include <string.h>

int main() {

  // HashMap *test = HashMap_new();
  // char *string = "hello world";
  // char s[32];


  // for(int i = 0; i < 1000000; i++) {
  //   char *d = calloc(32, 1);

  //   snprintf(s, 16, "%d", i);
  //   snprintf(d, 16, "data: %d", i);

  //   HashMap_put(test, s, d);
  // }

  // strcpy(s, "12345");
  // printf("%s: %s", s, (char *) HashMap_get(test, s));
  // printf("success");

  Model_readData("./data/Harvard1.txt");

  return 0;
}