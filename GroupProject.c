#include <stdio.h>

int main() {

  printf("Welcome to your wishlist");
}

// Basic structure should be an array of structs 
// no need for linked list, we can just set a hard limit on the number of items that can be stored
// Structs should store useful info about items like name, keywords, etc.
// We should have support for creating multiple lists, use nested structs for this
// We should also have support for a favorites list. Any item from any list should be able to be marked as a favorite, making it appear on the unified favorites list.
// Basic menu design should be similar to the following:
/*
Menu:
1. Add a new list
2. Display an existing list (submenu to display available lists, lists and their comtents should be saved to a file which is read in on startup, should have an option somewhere to add an item as a favorite)
3. Display favorites list (should have an option to remove stuff from it)
4. Search for an item (select an item, then use Google API to search it)
5. Quit
8/