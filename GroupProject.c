#include <stdio.h>

int main() {
  int choice;


  while(1){

  printf("Welcome to your wishlist\n");
  printf("------------------------\n");
  printf("Menu:\n");
  printf("------------------------\n");
  printf("1. Add a new list\n");
  printf("2. Display an existing list\n");
  printf("3. Display favorites list\n");
  printf("4. Search for an item\n");
  printf("5. Quit\n");
  printf("------------------------\n");
  printf("Selection: ");
  scanf("%d", &choice);

  if (choice == 1){
    printf("add successful\n");
  }

  if (choice == 2){
    printf("display an exisiting list successful\n");
  }

  if (choice == 3){
    printf("display favorites list successful\n");
  }

  if (choice == 4){
    printf("search for an item successful\n");
  }

  if (choice == 5){
    break;
  }


  }
}
