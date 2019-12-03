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

struct curl_response{
  char *text;
  size_t size;
};

size_t curl_callback (void *conents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct curl_response *p = (struct curl_response *)userp;

  p->text = (char*)realloc(p->text, p->size + realsize + 1);

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

  response->text = (char*)calloc(1, sizeof(response->text));

  if (response->text == NULL)
  {
    printf("\nFailed to allocate text in curl_fetch.");
    return CURLE_FAILED_INIT;
  }

  response->size = 0;

  curl_easy_setopt(c, CURLOPT_URL, url);
  curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, curl_callback);
  curl_easy_setopt(c, CURLOPT_WRITEDATA, (void*)response);
  
  result = curl_easy_perform(c);

  return result;
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

int doSearch(char *searchTerm)
{
  char* googCX = "015041558588304470797:lhyjceovlrd";
  char* googAPIKey = "AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc";
  char* httpRequest;
  char* baseURI = "https://www.googleapis.com/customsearch/v1?key=AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc&cx=015041558588304470797:lhyjceovlrd&q=";
  char requestURI[1000] = {0};
  char q[100] = {0};

  snprintf(q, 99, "%s", searchTerm);
  snprintf(requestURI, 999, "%s%s", baseURI, q);

  char* testURI = "https://www.googleapis.com/customsearch/v1?key=AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc&cx=015041558588304470797:lhyjceovlrd&q=test";

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

int main() {
  int choice;
  char* searchString;


  while(1){

  printf("\nWelcome to your wishlist\n");
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
    // Whoever does this needs to pass searchString to doSearch()
    // searchString should be the name of an item the user chooses
    doSearch(searchString);
  }

  if (choice == 5){
    break;
  }


  }
}
