#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10  //size of hashmap

//struct for key-data pairs
struct KDPair {
  char *key;
  int data;
};

//hashes key and gives index into the hashmap
int hashCode(char *key){
  int hash = 7;
  for (int i = 0; i < strlen(key); i++) {
      hash = hash*31 + key[i];
  }
  int h = hash % SIZE; //hash turned into index
  if(h < 0){
    h = h * (-1);
  }
  return h;
}

//function for duplicating a hashmap. not used currently
struct KDPair ** duplicateMap(struct KDPair **hashMap){
  static struct KDPair *newMap[SIZE];
  for(int i=0; i<SIZE; i++){
    if(hashMap[i] != NULL){
      struct KDPair *kd = hashMap[i];
      struct KDPair *pair = (struct KDPair*) malloc(sizeof(struct KDPair));
      pair->key = kd->key;
      pair->data = kd->data;
      newMap[i] = pair;
    }
    else{
      newMap[i] = NULL;
    }
  }
  return newMap;
}

//for printing a map
int disp_map(struct KDPair **hashMap){
  printf("---------------------------------------------------\n");
  for(int i=0; i<SIZE; i++){
    struct KDPair *pair = hashMap[i];
    if(pair != NULL){
      printf("Key=%s, Data=%d\n", pair->key, pair->data);
    }
  }
  printf("---------------------------------------------------\n");
}

struct KDPair * insert(char *key, struct KDPair **hashMap){
  int index = hashCode(key);
  //if key already present, data is incremented
  if(hashMap[index] != NULL){
    struct KDPair *kd = hashMap[index];
    kd->data++;
    return *hashMap;
  }
  else{ // otherwise a new pair is created with data 1 and put into hashmap
    struct KDPair *pair = (struct KDPair*) malloc(sizeof(struct KDPair));
    pair->data = 1;
    pair->key = key;
    hashMap[index] = pair;
    return *hashMap;
  }
}

//retrieve data from hashmap with key
int getData(char *key, struct KDPair **hashMap){
  int index = hashCode(key);
  if(hashMap[index] != NULL) {
    return hashMap[index]->data;
  }
  return -1; //-1 returned if key not found
}

//find biggest data val in hashmap, returns corresponding key
char* biggest(struct KDPair **hashMap){
  int bigInd = -1;
  int bigData = -1;
  for(int i=0; i<SIZE; i++){
    struct KDPair *pair = hashMap[i];
    if(pair != NULL){
      if(pair->data > bigData){
        bigData = pair->data;
        bigInd = i;
      }
    }
  }
  struct KDPair *big = hashMap[bigInd];
  return big->key;
}

//read names in csv file into hashmap for counting frequency
struct KDPair * read_file(FILE* stream, struct KDPair **hashMap){
  char line[1024];
  fgets(line, 1024, stream); //get first line from file
  char *ptr = strtok(line, ","); //get first column from first line
  //code below is to find which column in csv is for names. stored in col
  int count = 0;
  int col = 0;
  while(ptr != NULL){
    char *p;
    if ((p=strchr(ptr, '\n')) != NULL){
        ptr = strtok(ptr, "\n"); //remove newline char if present
    }
    int res = strcmp(ptr, "Name");
    if(res == 0){
      col = count;
    }
    count++;
    ptr = strtok(NULL, ",");
  }
  char *name = ""; //for finding each name in each line
  while(!feof(stream)){
    fgets(line, 1024, stream);
    char * newline = strdup(line); //get next line
    if(feof(stream)){
      return *hashMap; //return map when end of file
    }
    name = strtok(newline, ",");
    for(int i=0; i<=col; i++){
      char *pos;
      if ((pos=strchr(name, '\n')) != NULL){
          name = strtok(name, "\n");
      }
      if(i == col){ //when a name is found, insert it into the map
        insert(name, hashMap);
      }
      else{
        name = strtok(NULL, ","); //next column if necessary
      }
    }
  }
  return *hashMap;
}

//print csv file
int print_file(FILE* stream){

  char line[1024];

  while(!feof(stream)){

    fgets(line, 1024, stream);
    if(feof(stream)){
      return 0;
    }
    printf("%s", line);
  }
  return 0;
}

int main(){

  FILE *stream = fopen("file.csv", "r");

  if (!stream) {
        printf("Missing input file\n");
        exit(1);
  }

  //create hashmap. array of pointers to KDPair structs
  struct KDPair *hashMap[SIZE];
  for(int i=0; i<SIZE; i++){
    hashMap[i] = malloc(sizeof(struct KDPair));
    hashMap[i] = NULL;
  }

  read_file(stream, hashMap);

  disp_map(hashMap);

  // struct KDPair **newMap = duplicateMap(hashMap);
  // disp_map(newMap);

  printf("%s is the most frequent name\n", biggest(hashMap));

  return 0;
}
