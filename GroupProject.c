#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char[] sendRequest(char requestText[1024])
{
  struct hostent *server;
  struct sockaddr_in serv_addr;
  int sockfd, bytes, sent, received, total;
  char response[4096];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) printf("\nError opening socket.");

  server = gethostbyname("www.googleapis.com");
  if (server == NULL) printf("\nError resolving host.");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(80);
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nError connecting.");
  }

  total = strlen(requestText);
  sent = 0;
  do {
    bytes = write(sockfd, mesage+sent, total-sent);
    if (bytes < 0) printf("\nError writing message to socket.");
    if (bytes == 0) break;
    sent += bytes;
  } while (sent < total);

  memset(response, 0, sizeof(response));
  total = sizeof(response) - 1;
  received = 0;

  do {
    bytes = read(sockfd, response+receieved, total-received);
    if (bytes < 0) printf("\nError reading message to socket.");
    if (bytes == 0) break;
    received += bytes;
  } while (received < total);

  if (receieved == total) printf("\nError storing complete response.");

  close(sockfd);

  return response;
}

void parseResults(char response[4096])
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
    printf("\n------------------------------------------------------------------")
  }
}

void doSearch(char searchTerm[100])
{
  char googCX[33] = "015041558588304470797:lhyjceovlrd";
  char googAPIKey[39] = "AIzaSyC94Y5BUd9QQ24MlamiR7AS7gUbtkZDcjc";
  char httpRequest[1024];
  char baseURI[43] = "https://www.googleapis.com/customsearch/v1?";
  char requestURI[100];

  sprintf(requestURI, "%skey=%s&cx=%s&q=%s", baseURI, googAPIKey, googCX, searchTerm);

  sprintf(httpRequest, "GET %s", requestURI);

  parseResults(sendRequest(httpRequest));
}



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
