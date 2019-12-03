#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON.c"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <curl/curl.h>

struct Product
{
  char name[50];
  char description[50];
  char category[100];
  double price;
};

void displayProduct(struct Product *product)
{
  printf("Name: %s\n,    Description: %s\n,    Category: %s\n,    Price: %.2lf\n\n",product->name, product->description, product->category, product->price);
}


char *httpResponse;

struct curl_response
{
  char *text;
  size_t size;
};

size_t curl_callback(void *conents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct curl_response *p = (struct curl_response *)userp;

  p->text = (char *)realloc(p->text, p->size + realsize + 1);

  if (p->text == NULL)
  {
    printf("\nCouldn't expand buffer in curl_callback.");
    free(p->text);
    return -1;
  }

  memcpy(&(p->text[p->size]), conents, realsize);

  p->size += realsize;
  p->text[p->size] = 0;

  return realsize;
}

CURLcode curl_fetch(CURL *c, char *url, struct curl_response *response)
{
  CURLcode result;

  response->text = (char *)calloc(1, sizeof(response->text));

  if (response->text == NULL)
  {
    printf("\nFailed to allocate text in curl_fetch.");
    return CURLE_FAILED_INIT;
  }

  response->size = 0;

  curl_easy_setopt(c, CURLOPT_URL, url);
  curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, curl_callback);
  curl_easy_setopt(c, CURLOPT_WRITEDATA, (void *)response);

  result = curl_easy_perform(c);

  return result;
}

void parseResults(char *response)
{
  cJSON *items = NULL;
  cJSON *item = NULL;
  cJSON *responseJSON = cJSON_Parse(response);
  int resultCtr = 0;

  items = cJSON_GetObjectItemCaseSensitive(responseJSON, "items");
  cJSON_ArrayForEach(item, items)
  {
    ++resultCtr;

    cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "title");
    cJSON *link = cJSON_GetObjectItemCaseSensitive(item, "link");
    cJSON *snippet = cJSON_GetObjectItemCaseSensitive(item, "snippet");

    printf("\n----------------------------Result %d-----------------------------", resultCtr);
    printf("\nTitle:\t%s", title->valuestring);
    printf("\nLink:\t%s", link->valuestring);
    printf("\nSnippet:\t %s", snippet->valuestring);
    printf("\n------------------------------------------------------------------");
  }
}

int doSearch(char *searchTerm)
{
  char *googCX = "015041558588304470797:lhyjceovlrd";
  char *googAPIKey = "AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc";
  char *httpRequest;
  char *baseURI = "https://www.googleapis.com/customsearch/v1?key=AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc&cx=015041558588304470797:lhyjceovlrd&q=";
  char requestURI[1000] = {0};
  char q[100] = {0};

  snprintf(q, 99, "%s", searchTerm);
  snprintf(requestURI, 999, "%s%s", baseURI, q);

  char *testURI = "https://www.googleapis.com/customsearch/v1?key=AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc&cx=015041558588304470797:lhyjceovlrd&q=test";

  //printf("\nURL Constructed: %s", requestURI);
  CURL *c;
  CURLcode result;

  struct curl_response curl_response;
  struct curl_response *cr = &curl_response;

  struct curl_slist *headers = NULL;

  if ((c = curl_easy_init()) == NULL)
  {
    printf("\nError initializing curl in doSearch.");
    return -1;
  }

  headers = curl_slist_append(headers, "Accept: application/json");

  curl_easy_setopt(c, CURLOPT_CUSTOMREQUEST, "GET");
  curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);

  result = curl_fetch(c, requestURI, cr);

  if (result != CURLE_OK || cr->size < 1)
  {
    printf("\nError fetching url %s, curl said %s", requestURI, curl_easy_strerror(result));
    return -2;
  }

  if (cr->text != NULL)
  {
    parseResults(cr->text);
    free(cr->text);
    return 0;
  }
  else
  {
    printf("\nReturned response was null.");
    free(cr->text);
    return -3;
  }
}

int main()
{
    struct Product macbook;
    struct Product hairbrush;
    // struct Product ps4;
    // struct Product hydroflask;
    // struct Product lipstick;

    strcpy(macbook.name, "MacBook");
    strcpy(macbook.description, "13 inch 2019 refurbished MacBook Pro");
    strcpy(macbook.category, "Laptop");
    macbook.price = 1099.99;

    strcpy(hairbrush.name, "Hairbrush");
    strcpy(hairbrush.description, "Brush from Rihanna's beauty line Fenty");
    strcpy(hairbrush.category, "Beauty");
    hairbrush.price= 45.99;

    // strcpy(ps4.name, "PS4");
    // strcpy(ps4.description, "The newest sony gaming console with Uncharted 4 bundle pack");
    // strcpy(ps4.category, "Gaming");
    // ps4.price = 199.99;
    //
    // strcpy(hydroflask.name, "Hydroflask");
    // strcpy(hydroflask.description, "Waterbottle that keeps you water cold all day.");
    // strcpy(hydroflask.category, "Health");
    // hydroflask.price = 45.99;
    //
    // strcpy(lipstick.name, "Lipstick");
    // strcpy(lipstick.description, "Versace lipstick on sale right now!");
    // strcpy(lipstick.category, "Beauty");
    // lipstick.price = 99.99;




    struct Product shoppingListOne[5];
    // struct Product shoppingListTwo[10];
    // struct Product shoppingListThree[10];
    // struct Product shoppingListFour[10];
    // struct Product shoppingListFive[10];


    int choice;
    char pSelection[10];
    int listCounterOne = 0;

    while(1)
    {

        printf("Welcome to your wishlist\n");
        printf("------------------------\n");
        printf("Menu:\n");
        printf("------------------------\n");
        printf("1. Search for an item\n");
        printf("2. Add an item from recent searches\n");
        printf("3. Display an existing list\n");
        printf("4. Display favorites list\n");
        printf("5. Quit\n");
        printf("------------------------\n");
        printf("Selection: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
          printf("search for an item successful\n");
          doSearch("test");

           //      printf("Please select one of the following products by typing it in.\n");
           //      printf("------------------------\n");
           //      displayProduct(&macbook);
           //      displayProduct(&hairbrush);
           //      // displayProduct(&ps4);
           //      // displayProduct(&lipstick);
           //      // displayProduct(&hydroflask);
           //      printf("------------------------\n");
           //      printf("If you are done adding to the shopping list please type in Exit.\n");
           //      printf("------------------------\n");
           //
           //
           // for(int i=0; i <=5; i++)
           // {
           //
           //      printf("Type in the name of the product in exactly how it is displayed ");
           //      scanf("%s",pSelection);
           //
           //          if(strcmp(pSelection, "Macbook") == 0){
           //              shoppingListOne[i]=macbook;
           //              printf("Added Succesfully\n");
           //              listCounterOne++;
           //
           //          }
           //          // else if (strcmp(pSelection, "Lipstick") == 0){
           //          //      shoppingListOne[i]=lipstick;
           //          //      printf("Added Succesfully\n");
           //          //      listCounterOne++;
           //          // }
           //          // else if (strcmp(pSelection, "PS4") == 0){
           //          //     shoppingListOne[i]=ps4;
           //          //     printf("Added Succesfully\n");
           //          //     listCounterOne++;
           //          // }
           //          // else if (strcmp(pSelection, "Hydroflask") == 0){
           //          //      shoppingListOne[i]=hydroflask;
           //          //      printf("Added Succesfully\n");
           //          //      listCounterOne++;
           //          // }
           //          else if (strcmp(pSelection, "Hairbrush") == 0){
           //               shoppingListOne[i]=hairbrush;
           //               printf("Added Succesfully \n");
           //               listCounterOne++;
           //          }
           //          else if (strcmp(pSelection, "Exit") == 0){
           //              printf("Exiting");
           //               break;
           //          }
           //
           //          else {
           //              printf("Please type in a proper product name to add to the list \n");
           //              i--;
           //          }
           //  }
          }

          if (choice == 2)
          {
            printf("Please select one of the following products by typing it in.\n");
            printf("------------------------\n");
            displayProduct(&macbook);
            displayProduct(&hairbrush);
            printf("------------------------\n");
            printf("If you are done adding to the shopping list please type in Exit.\n");
            printf("------------------------\n");
            for(int i=0; i <=5; i++)
            {
              printf("Type in the name of the product in exactly how it is displayed ");
              scanf("%s",pSelection);

                  if(strcmp(pSelection, "Macbook") == 0)
                  {
                      shoppingListOne[i]=macbook;
                      printf("Added Succesfully\n");
                      listCounterOne++;
                  }
                  // else if (strcmp(pSelection, "Lipstick") == 0){
                  //      shoppingListOne[i]=lipstick;
                  //      printf("Added Succesfully\n");
                  //      listCounterOne++;
                  // }
                  // else if (strcmp(pSelection, "PS4") == 0){
                  //     shoppingListOne[i]=ps4;
                  //     printf("Added Succesfully\n");
                  //     listCounterOne++;
                  // }
                  // else if (strcmp(pSelection, "Hydroflask") == 0){
                  //      shoppingListOne[i]=hydroflask;
                  //      printf("Added Succesfully\n");
                  //      listCounterOne++;
                  // }
                  else if (strcmp(pSelection, "Hairbrush") == 0){
                       shoppingListOne[i]=hairbrush;
                       printf("Added Succesfully \n");
                       listCounterOne++;
                  }
                  else if (strcmp(pSelection, "Exit") == 0){
                      printf("Exiting");
                       break;
                  }

                  else {
                      printf("Please type in a proper product name to add to the list \n");
                      i--;
                  }
                }
              }

            if (choice == 3)
            {
              printf("Here are the items in your existing lists: ");
              printf("----------------------------------------- \n");
              for(int i = 0; i<listCounterOne; i++)
              {
                printf("Name: %s\n,    Description: %s\n,    Category: %s\n,    Price: %.2lf\n\n",shoppingListOne[i].name, shoppingListOne[i].description, shoppingListOne[i].category, shoppingListOne[i].price);
              }
              printf("----------------------------------------- \n");
              printf("display an exisiting list successful\n");
            }

            if (choice == 4)
            {
              printf("display favorites list successful\n");
                // printf("search for an item successful\n");
                // doSearch("test");
            }

            if (choice == 5)
            {
                break;
            }

    }

}
