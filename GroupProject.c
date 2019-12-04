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
  char description[200];
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

    strcpy(macbook.name, "Apple Macbook");
    strcpy(macbook.description, "Buy Apple MacBook ");
    strcpy(macbook.category, "Computer");
    macbook.price = 1099.99;

    strcpy(hairbrush.name, "Diversion Safe Hair Brush by Stash-it");
    strcpy(hairbrush.description, "Diversion Safe Hair Brush by Stash-it");
    strcpy(hairbrush.category, "Beauty");
    hairbrush.price= 45.99;




    struct Product shoppingListOne[5];


    int choice = 0;
    char input[50];
    char pSelection[50];
    int listCounterOne = 0;
    char search[100];

    while(choice != 4)
    {

        printf("\nWelcome to your wishlist\n");
        printf("------------------------\n");
        printf("Menu:\n");
        printf("------------------------\n");
        printf("1. Search for an item\n");
        printf("2. Add an item from recent searches\n");
        printf("3. Display an existing list\n");
        printf("4. Quit\n");
        printf("------------------------\n");
        printf("Selection: ");

        fgets(input, 50, stdin);
        fflush(stdin);
        choice = atoi(input);

        switch (choice){
          case 1:
            printf("Enter what you would like to search: \n");
            fgets(search, 100, stdin);
            fflush(stdin);
            strtok(search, "\n");
            doSearch(search);
            break;

          case 2:
            printf("\nPlease select one of the following products by typing it in.\n");
            printf("------------------------\n");
            displayProduct(&macbook);
            displayProduct(&hairbrush);
            printf("------------------------\n");
            printf("If you are done adding to the shopping list please type in Exit.\n");
            printf("------------------------\n");
            for(int i=0; i <=5; i++)
            {
              printf("Type in the first word of the product: \n");
              fgets(pSelection, 50, stdin);
              fflush(stdin);
              strtok(pSelection, "\n");

                  if(strcmp(pSelection, "Apple") == 0)
                  {
                      shoppingListOne[i]=macbook;
                      printf("Added Succesfully\n");
                      listCounterOne++;
                  }
                  else if (strcmp(pSelection, "Diversion") == 0)
                  {
                       shoppingListOne[i]=hairbrush;
                       printf("Added Succesfully \n");
                       listCounterOne++;
                  }
                  else if (strcmp(pSelection, "Exit") == 0)
                  {
                       printf("\nExiting\n");
                       break;
                  }

                  else
                  {
                      printf("\nThe word you typed was not in the search results. Try again.\n");
                      i--;
                  }
            }
            break;

          case 3:
              printf("Here are the items in your existing list: \n");
              printf("----------------------------------------- \n");
              printf("Apple Macbook\n");
              printf("Diversion Safe Hair Brush by Stash-it\n");
              break;

         case 4:
              break;
        }
    }
}
