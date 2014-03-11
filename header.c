#include "header.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fnmatch.h>

char* longListRegEx(char regEx[], char *indexGetArray)
{
	indexGetArray = (char *)malloc(sizeof(char )*10000);
	memset(indexGetArray,'\0',sizeof(indexGetArray));
	char *dir;
	dir  = (char *)malloc(sizeof(char )*100);
	memset(indexGetArray,'\0',sizeof(indexGetArray));
	strcpy(dir,".");
	DIR *d = opendir(dir);
	if(d==NULL)
	{
		printf("error\n");
		return;
	}
	else
	{
		chdir(dir);
		while(1)
		{
			struct dirent *directry;
			directry = readdir(d);
			if(directry==NULL)
			{
				//printf(" Empty\n");
				break;
			}
			else
			{
				/* Calculation */
				struct stat info;
				lstat(directry->d_name,&info);
				int size = info.st_size;
				long int tme = info.st_mtime;
				char t[100]="";
				strftime(t, 100, "%d-%m-%Y %H:%M:%S", localtime( &info.st_mtime));

				//printf("regEx is %s\n",regEx);
				int flagReg = fnmatch(regEx,directry->d_name,FNM_NOESCAPE);
				if(flagReg==0)
				{
					strcat(indexGetArray,"Name: ");
					strcat(indexGetArray,directry->d_name);
					strcat(indexGetArray,"\nSize: ");
					char tempS[10] = "";
					sprintf(tempS,"%d",size);
					strcat(indexGetArray,tempS);
					strcat(indexGetArray,"bytes\n Last Modified Time Stamp:");
					strcat(indexGetArray,t);
					strcat(indexGetArray,"\nType: ");
					//printf("%ld ",tme);
					//printf("%s ",t);
					if(S_ISDIR(info.st_mode))
						strcat(indexGetArray,"Directory\n\n");
					else if(S_ISREG(info.st_mode))
						strcat(indexGetArray,"Regular File\n\n");
					else if(S_ISBLK(info.st_mode))
						strcat(indexGetArray,"Block Device\n\n");
					else if(S_ISLNK(info.st_mode))
						strcat(indexGetArray,"Symbolic Link\n\n");
				}
			}
		}
		//chdir("..");
	}
	//printf("%s\n",indexGetArray);
	return indexGetArray;
}

char* longList(char *indexGetArray)
{
	indexGetArray = (char *)malloc(sizeof(char )*10000);
	memset(indexGetArray,'\0',sizeof(indexGetArray));
	char *dir;
	dir  = (char *)malloc(sizeof(char )*100);
	memset(indexGetArray,'\0',sizeof(indexGetArray));
	strcpy(dir,".");
	DIR *d = opendir(dir);
	if(d==NULL)
	{
		printf("error\n");
		return;
	}
	else
	{
		chdir(dir);
		while(1)
		{
			struct dirent *directry;
			directry = readdir(d);
			if(directry==NULL)
			{
				//printf(" Empty\n");
				break;
			}
			else
			{
				struct stat info;
				lstat(directry->d_name,&info);
				int size = info.st_size;
				long int tme = info.st_mtime;
				char t[100]="";
				strftime(t, 100, "%d-%m-%Y %H:%M:%S", localtime( &info.st_mtime));

				strcat(indexGetArray,"Name: ");
				strcat(indexGetArray,directry->d_name);
				strcat(indexGetArray,"\nSize: ");
				char tempS[10] = "";
				sprintf(tempS,"%d",size);
				strcat(indexGetArray,tempS);
				strcat(indexGetArray,"bytes\n Last Modified Time Stamp:");
				strcat(indexGetArray,t);
				strcat(indexGetArray,"\nType: ");
				//printf("%ld ",tme);
				//printf("%s ",t);
				if(S_ISDIR(info.st_mode))
					strcat(indexGetArray,"Directory\n\n");
				else if(S_ISREG(info.st_mode))
					strcat(indexGetArray,"Regular File\n\n");
				else if(S_ISBLK(info.st_mode))
					strcat(indexGetArray,"Block Device\n\n");
				else if(S_ISLNK(info.st_mode))
					strcat(indexGetArray,"Symbolic Link\n\n");
			}
		}
		//chdir("..");
	}
	//printf("%s\n",indexGetArray);
	return indexGetArray;
}

char* getInfo(char str11[], char *infoAbtFile)
{
	infoAbtFile = (char *)malloc(sizeof(char)*1000);
	memset(infoAbtFile,'\0',sizeof(infoAbtFile));
	struct stat sb;
	//printf("Hiiiiiiiiiiiiiiiiiiiiiiii\n");
	if (stat(str11, &sb) == -1)
	{
		perror("stat");
		exit(EXIT_FAILURE);
	}
	/*
	   printf("File name:                %s\n",str11);
	   printf("File size:                %lld bytes\n",(long long) sb.st_size);
	   printf("Last file modification:   %s", ctime(&sb.st_mtime));
	 */
	char st1[100]="" , st2[100]="" , st3[100]="";
	sprintf(st1,"File name:                %s\n",str11);
	sprintf(st2,"File size:                %lld bytes\n",(long long) sb.st_size);
	sprintf(st3,"Last file modification:   %s\n", ctime(&sb.st_mtime));
	strcat(infoAbtFile,st1);
	strcat(infoAbtFile,st2);
	strcat(infoAbtFile,st3);
	//printf("%s\n",infoAbtFile);
	return infoAbtFile;
}

void shortList(char str1[],char str2[], char message[], char *indexGetArray)
{
	indexGetArray = (char *)malloc(sizeof(char )*10000);
	memset(indexGetArray,'\0',sizeof(indexGetArray));
	char *dir;
	dir  = (char *)malloc(sizeof(char )*100);
	strcpy(dir,".");
	DIR *d = opendir(dir);
	if(d==NULL)
	{
		printf("%s\n", message);
		return;
	}
	else
	{
		chdir(dir);
		while(1)
		{
			struct dirent *directry;
			directry = readdir(d);
			if(directry==NULL)
			{
				//printf(" Empty\n");
				break;
			}
			else
			{
				struct stat info;
				lstat(directry->d_name,&info);  
				int size = info.st_size;// Size
				long int tme = info.st_mtime;  // TimeStamp
				char t[100]="";
				strftime(t, 100, "%d-%m-%Y %H:%M:%S", localtime( &info.st_mtime));

				if(checkShortList(str1,str2,t))
				{
					strcat(indexGetArray,"Name: ");
					strcat(indexGetArray,directry->d_name);
					strcat(indexGetArray,"\nSize: ");
					char tempS[10] = "";
					sprintf(tempS,"%d",size);
					strcat(indexGetArray,tempS);
					strcat(indexGetArray,"bytes\n Last Modified Time Stamp:");
					strcat(indexGetArray,t);
					strcat(indexGetArray,"\nType: ");
					//printf("%ld ",tme);
					//printf("%s ",t);
					if(S_ISDIR(info.st_mode))
						strcat(indexGetArray,"Directory\n\n");
					else if(S_ISREG(info.st_mode))
						strcat(indexGetArray,"Regular File\n\n");
					else if(S_ISBLK(info.st_mode))
						strcat(indexGetArray,"Block Device\n\n");
					else if(S_ISLNK(info.st_mode))
						strcat(indexGetArray,"Symbolic Link\n\n");
				}
			}
		}
		//chdir("..");
	}
}

int checkShortList(char str1[] , char str2[] , char chk[])
{
	struct tm time1,time2,time3;
	strptime(str1, "%d-%m-%Y %H:%M:%S", &time1);
	strptime(str2, "%d-%m-%Y %H:%M:%S", &time2);
	strptime(chk, "%d-%m-%Y %H:%M:%S", &time3);


	time1.tm_isdst = -1;	time2.tm_isdst = -1;	time3.tm_isdst = -1;
	time_t timestamp1 = mktime(&time1);	time_t timestamp2 = mktime(&time2);	time_t timestamp3 = mktime(&time3);
	if(timestamp3<timestamp2)
	{
		if(timestamp3>timestamp1) 
			return 1;
	}
	return 0;
}

void  execute(char **cmnd)
{
	pid_t pid;
	int status;
	if( (pid = fork()) < 0)
	{     	
		printf("forking child process failed\n");
		_exit(1);
	}
	else if(pid == 0)
	{
		if (execvp(cmnd[0], cmnd) < 0)
		{
			printf("exec failed\n");
			_exit(1);
		}
	}
	else 
	{       /* for the parent:  */
		while (wait(&status) != pid);       /* wait for completion  */			
	}
}

void indexGet(char *str)
{
	// Sample Query - IndexGet ShortList <starting-time-stamp> <ending-time-stamp>
	// find . -type f -newermt "2013-03-20" ! -newermt "2013-03-23" -ls  for ShortList
	// ls -lh for LongList
	int len=0,flagStyle=0;
	char startTime[20],endTime[20];
	memset(startTime,'\0',sizeof(startTime));
	memset(endTime,'\0',sizeof(endTime));
	char *token;
	token = strtok(str," ");
	while(token != NULL)
	{
		printf("token:%s\n",token);
		len++;
		if(len==2)
		{
			if(strcmp(token,"LongList")==0)
			{
				flagStyle=0;
				break;
			}
			else if(strcmp(token,"ShortList")==0)
				flagStyle=1;
		}
		if(flagStyle==1)
		{
			if(len==3)
				strcpy(startTime,token);
			if(len==4)
				strcpy(endTime,token);
		}
		token = strtok(NULL," ");
	}
	char **cmnd;
	if(flagStyle==0) // LongList - ls -lh
	{
		cmnd = (char **)malloc(sizeof(char *)*2);
		int i=0;
		for(i=0;i<2;i++)
			cmnd[i] = (char *)malloc(sizeof(char)*5);
		strcpy(cmnd[0],"ls");
		strcpy(cmnd[1],"-lh");
		execute(cmnd);
	}
	else if(flagStyle==1)  // find . -type f -newermt "2013-03-20" ! -newermt "2013-03-23" -ls
	{
		cmnd = (char **)malloc(sizeof(char *)*10);
		int i=0;
		for(i=0;i<10;i++)
		{
			cmnd[i] = (char *)malloc(sizeof(char)*20);
			memset(cmnd[i],'\0',sizeof(cmnd[i]));
		}
		strcpy(cmnd[0],"find");
		strcpy(cmnd[1],".");  // have to change this
		strcpy(cmnd[2],"-type");
		strcpy(cmnd[3],"f");
		strcpy(cmnd[4],"-newermt");
		strcpy(cmnd[5],startTime);
		strcpy(cmnd[6],"!");
		strcpy(cmnd[7],"-newermt");
		strcpy(cmnd[8],endTime);
		strcpy(cmnd[9],"-ls");
		for(i=0;i<10;i++)
			printf("%s ",cmnd[i]);
		printf("\n");
		execute(cmnd);
	}
}

void uploadFile(char *str, int connectit)
{
	char filename[100];
	char send_data[1025];
	memset(filename,'\0',sizeof(filename));
	char *token = NULL;
	int len=0;
	token = strtok(str," ");
	while(token!=NULL)
	{
		len++;
		if(len==2)
		{
			strcpy(filename,token);
			break;
		}
		token = strtok(NULL," ");
	}
	int fd = open(filename,O_RDONLY);
	if(fd <=0)printf("cannot open file\n");
	send(connectit,filename,100,0);
	int readVar = read(fd,send_data,1024);
	while(readVar > 0)
	{
		send_data[readVar]='\0';
		send(connectit,send_data,strlen(send_data), 0);
		readVar = read(fd,send_data,1024);
		if(readVar<=0)
			break;
	}
	close(fd);
}

char* hash(char filename[1500], unsigned char hashst[])
{
	memset(hashst,'\0',sizeof(hashst));
	unsigned char c[MD5_DIGEST_LENGTH];
	//char *filename="hello";
	int i;
	FILE *inFile = fopen (filename, "rb");
	//FILE *outFile = fopen("out.txt","w+");
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];

	if (inFile == NULL) {
		printf ("%s can't be opened.\n", filename);
		return 0;
	}

	MD5_Init (&mdContext);
	while ((bytes = fread (data, 1, 1024, inFile)) != 0)
		MD5_Update (&mdContext, data, bytes);
	MD5_Final (c,&mdContext);
	for(i = 0; i < MD5_DIGEST_LENGTH; i++)
	{
		printf("%02x",c[i]);
		hashst[i]=c[i];
		sprintf(hashst,"%02x", c[i]);
	}
	printf("\n");
	strcpy(hashst,c);
	fclose (inFile);
	return filename;
}
