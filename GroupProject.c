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
  int price;
  struct Product *next;
};

struct Node //node with just a pointer
{
    struct Product *start;
};

struct Product *CreateListNode(char name[], char description[], char category[], int price); //struct statment for create list node

struct Node *CreateListNoNodes() //just has pointer
{
    struct Node *Node = (struct Node*)malloc(sizeof(struct Node));
    Node -> start = NULL;
    return Node;
}

struct Product *CreateListNode(char name[], char description[], char category[], int price) //has data points
{
    struct Product *temp = (struct Product*)malloc(sizeof(struct Product));
    strcpy(temp -> name, name);
    strcpy(temp -> description, description);
    strcpy(temp -> category, category);
    temp -> price = price;
    temp -> next = NULL;
    return temp;
}

void InsertProduct(struct Node *Node, char name[], char description[], char category[], int price) //https://stackoverflow.com/questions/36528584/c-linked-list-how-to-insert-node-at-front
{
    struct Product *current = NULL;

    if (Node->start == NULL)
    {
        Node->start = CreateListNode(name, description, category, price);
    }
    else
    {
        current = CreateListNode(name, description, category, price);
        current -> next = Node -> start;
        Node -> start = current; //sets what is inputted for creatlistnode to the current start node
    }
    printf("Saved product.\n"); //confirms this has been done
    return;
}

void Traverse(struct Node *Node)
{
    struct Product *CurrentNode = Node -> start;
    //If there is no data in the list
    if (CurrentNode == NULL)
    {
        printf("Create the data in list before searching\n");
        return;
    }
    else
    {
        while(CurrentNode -> next != NULL)
        {
            printf("Product Name: %s", CurrentNode -> name);
            printf("Description: %s", CurrentNode -> description);
            printf("Category: %s", CurrentNode -> category);
            printf("Price: %d\n", CurrentNode -> price);
            CurrentNode = CurrentNode -> next;
        }
        printf("Product Name: %s", CurrentNode -> name);
        printf("Description: %s", CurrentNode -> description);
        printf("Category: %s", CurrentNode -> category);
        printf("Price: %d\n", CurrentNode -> price);
    }
    return;
}

void TraverseProducts(struct Node *Node) //traverses list based on PUID
{
    struct Product *current = Node -> start;

    if (current == NULL)
    {
        printf("No data.\n");
        return;
    }
    else
    {
        while(current -> next != NULL)
        {
            printf("Product name: %s", current -> name);
            current = current -> next;
        }
        printf("Product name: %s", current -> name);
    }
    return;
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
  char name[50];
  char description[200];
  char category[100];
  char tempPrice[10];
  int price;

  struct Node *Node;
  Node = CreateListNoNodes();

    int choice = 0;
    char input[50];
    char search[100];

    while(choice != 5)
    {
        printf("\nWelcome to your wishlist\n");
        printf("------------------------\n");
        printf("Menu:\n");
        printf("------------------------\n");
        printf("1. Search for an item\n");
        printf("2. Add an item from recent searches\n");
        printf("3. Display only names of added items\n");
        printf("4. Display all information of added items\n");
        printf("5. Quit\n");
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
            printf("\nPlease enter the product information from your recent search\n");
            printf("------------------------\n");

              printf("Type in the name of the product: \n");
              fgets(name, 50, stdin);
              fflush(stdin);

              printf("Type in a short description of the product: \n");
              fgets(description, 200, stdin);
              fflush(stdin);

              printf("Type in the category of this product: \n");
              fgets(category, 100, stdin);
              fflush(stdin);

              printf("Type in the price of this product: \n");
              fgets(tempPrice, 30, stdin);
              fflush(stdin);

              while(!(price = atoi(tempPrice)))
              {
                printf("Age is an integer: ");
                fgets(tempPrice, 30, stdin);
                fflush(stdin);
              }

              InsertProduct(Node, name, description, category, price);

            break;

          case 3:
              printf("Here are the names of the items in your existing list: \n");
              printf("----------------------------------------- \n");
              TraverseProducts(Node);
              break;
          case 4:
            printf("Here are the items in your existing list: \n");
            printf("----------------------------------------- \n");
            Traverse(Node);
            break;
         case 5:
              break;
        }
    }
}
