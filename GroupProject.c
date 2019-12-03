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

char* httpResponse;

struct string
{
  char *text;
  size_t length;
};

void init_string(struct string *s) {
  s->length = 0;
  s->text = malloc(s->length+1);
  if (s->text == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->text[0] = '\0';
  printf("\ninit_string is chill.");
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->length + size*nmemb;
  s->text = realloc(s->text, new_len+1);
  if (s->text == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->text+s->length, ptr, size*nmemb);
  s->text[new_len] = '\0';
  s->length = new_len;

  printf("\nwritefunc is chill.");

  return size*nmemb;
}

char* sendRequest(char* requestURI)
{
  CURL *curl;
  CURLcode result;

  curl = curl_easy_init();
  if (curl)
  {
    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_URL, requestURI);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    printf("\ncurl options set.");
    result = curl_easy_perform(curl);

    printf("\nResponse: %s\nResult: %d", s.text, result);
    
    strcpy(httpResponse, s.text);

    free(s.text);

    curl_easy_cleanup(curl);

    return httpResponse;
  }
}

void parseResults(char* response)
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

void doSearch(char searchTerm[100])
{
  char* googCX = "015041558588304470797:lhyjceovlrd";
  char* googAPIKey = "AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc";
  char* httpRequest;
  char* baseURI = "https://www.googleapis.com/customsearch/v1?";
  char* requestURI;

  sprintf(requestURI, "%skey=%s&cx=%s&q=%s", baseURI, googAPIKey, googCX, searchTerm);

  //sprintf(httpRequest, "GET %s", requestURI);

  printf("\n%s", httpRequest);

  parseResults(sendRequest(requestURI));
}

struct Product
{
    char  *name[50];
    char  *description[50];
    char  *category[100];
    double price;
};


void displayProduct(struct Product *product)
{

    printf("Name: %s\n,    Description: %s\n,    Category: %s\n,    Price: %.2lf\n\n",product->name, product->description, product->category, product->price);

}

void addToList(struct Product p, char pName, char pDesc, char pCat, double price)
{

    strcpy(p.name,pName);
    strcpy(p.description,pDesc);
    strcpy(p.category,pCat);


}


 void displayList(struct Product *arr[] ) {

    for(int i=0;i<=5;i++){
        printf("Name: %s\n,    Description: %s\n,    Category: %s\n,    Price: %.2lf\n\n",arr[i]->name, arr[i]->description, arr[i]->category, arr[i]->price);
    }
}




int main()
{

    struct Product macbook;
    struct Product hairbrush;
    struct Product ps4;
    struct Product hydroflask;
    struct Product lipstick;

    strcpy(macbook.name, "MacBook");
    strcpy(macbook.description, "13 inch 2019 refurbished MacBook Pro");
    strcpy(macbook.category, "Laptop");
    macbook.price = 1099.99;

    strcpy(hairbrush.name, "Hairbrush");
    strcpy(hairbrush.description, "Brush from Rihanna's beauty line Fenty");
    strcpy(hairbrush.category, "Beauty");
    hairbrush.price= 45.99;

    strcpy(ps4.name, "PS4");
    strcpy(ps4.description, "The newest sony gaming console with Uncharted 4 bundle pack");
    strcpy(ps4.category, "Gaming");
    ps4.price = 199.99;

    strcpy(hydroflask.name, "Hydroflask");
    strcpy(hydroflask.description, "Waterbottle that keeps you water cold all day.");
    strcpy(hydroflask.category, "Health");
    hydroflask.price = 45.99;

    strcpy(lipstick.name, "Lipstick");
    strcpy(lipstick.description, "Versace lipstick on sale right now!");
    strcpy(lipstick.category, "Beauty");
    lipstick.price = 99.99;




    struct Product shoppingListOne[5];
    struct Product shoppingListTwo[10];
    struct Product shoppingListThree[10];
    struct Product shoppingListFour[10];
    struct Product shoppingListFive[10];


    int choice;
    char pSelection[10];
    int listCounterOne = 0;
    while(1)
    {

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

        if (choice == 1)
        {

            printf("Please select one of the following products by typing it in.\n");
                printf("------------------------\n");
                displayProduct(&macbook);
                displayProduct(&hairbrush);
                displayProduct(&ps4);
                displayProduct(&lipstick);
                displayProduct(&hydroflask);
                printf("------------------------\n");
                printf("If you are done adding to the shopping list please type in Exit.\n");
                printf("------------------------\n");


           for(int i=0; i <=5; i++){

                printf("Type in the name of the product in exactly how it is displayed ");
                scanf("%s",pSelection);

                    if(strcmp(pSelection, "Macbook") == 0){
                        shoppingListOne[i]=macbook;
                        printf("Added Succesfully\n");
                        listCounterOne++;

                    }
                    else if (strcmp(pSelection, "Lipstick") == 0){
                         shoppingListOne[i]=lipstick;
                         printf("Added Succesfully\n");
                         listCounterOne++;
                    }
                    else if (strcmp(pSelection, "PS4") == 0){
                        shoppingListOne[i]=ps4;
                        printf("Added Succesfully\n");
                        listCounterOne++;
                    }
                    else if (strcmp(pSelection, "Hydroflask") == 0){
                         shoppingListOne[i]=hydroflask;
                         printf("Added Succesfully\n");
                         listCounterOne++;
                    }
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








            if (choice == 2)
            {
                printf("Here are the items in your existing lists: ");
                printf("----------------------------------------- \n");
                for(int i = 0; i<listCounterOne; i++){
            printf("Name: %s\n,    Description: %s\n,    Category: %s\n,    Price: %.2lf\n\n",shoppingListOne[i].name, shoppingListOne[i].description, shoppingListOne[i].category, shoppingListOne[i].price);
                }
                printf("----------------------------------------- \n");
                printf("display an exisiting list successful\n");
            }

            if (choice == 3)
            {
                printf("display favorites list successful\n");
            }

            if (choice == 4)
            {
                printf("search for an item successful\n");
            }

            if (choice == 5)
            {
                break;
            }
        }

    }

