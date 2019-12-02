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

int main() {
  int choice;


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
    printf("search for an item successful\n");
    doSearch("test");
  }

  if (choice == 5){
    break;
  }


  }
}
