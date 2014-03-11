#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
char* getInfo(char str[], char *infoAbtFile);
void shortList(char str1[],char str2[], char message[], char *indexGetArray);
int checkShortList(char str1[] , char str2[] , char chk[]);
void execute(char **cmnd);
void indexGet(char *str);
void uploadFile(char *str, int connectit);
char* hash(char filename[1500], unsigned char hashst[1000]);
char* longList(char *indexGetArray);
char* longListRegEx(char regEx[], char *indexGetArray);
#endif
