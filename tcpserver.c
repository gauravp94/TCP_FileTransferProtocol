/* tcpserver.c */

#include "header.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <netdb.h>
#include <time.h>
#include <openssl/md5.h>
#include <fnmatch.h>


void downloadFile(char *str);


unsigned char hashst[1000];

int connected=0;
char *indexGetArray,*infoAbtFile;

int main()
{
	int sock, bytes_recieved,bytes_received1 , tru = 1;  
	char send_data[1124],recv_data[1124],recv_data1[1124],str[1124],str1[1124],str2[1124],str3[1124];
	char *nameFile;   
	long int fileSize;
	int size=0;
	char c;
	memset(hashst,'\0',sizeof(hashst));
	fd_set read_flags,write_flags; // the flag sets to be used
	struct timeval waitd = {10, 0};  
	struct sockaddr_in server_addr,client_addr;    
	int sin_size;
	//fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("Socket");
		exit(1);
	}
	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(5000);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1)
	{
		perror("Unable to bind");
		exit(1);
	}
	if (listen(sock, 5) == -1) 
	{
		perror("Listen");
		exit(1);
	}
	printf("\nTCPServer Waiting for client on port 5000");
	fflush(stdout);
	char a;
	int flagQuit=0;
	while(1)
	{  

		sin_size = sizeof(struct sockaddr_in);
		int add_len = sizeof( struct sockaddr);
		connected = accept(sock, (struct sockaddr *)&client_addr,&add_len);
		// flags = fcntl(socket,F_GETFL,0);
		//assert(flags != -1);
		//fcntl(socket, F_SETFL, flags | O_NONBLOCK);
		if(connected < 0)continue;
		fcntl(connected, F_SETFL, O_NONBLOCK);
		printf("\n I got a connection from (%s , %d)\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		fflush(stdout);
		while(1)
		{
			FD_ZERO(&read_flags);
			FD_ZERO(&write_flags);
			FD_SET(connected, &read_flags);
			FD_SET(connected, &write_flags);
			FD_SET(STDIN_FILENO, &read_flags);
			FD_SET(STDIN_FILENO, &write_flags);
			//printf("\n SEND (q or Q to quit) : ");
			int sel = select(connected+1, &read_flags, &write_flags, (fd_set*)0, &waitd);
			size=0;
			memset(send_data,'\0',sizeof(send_data));
			if(sel > 0)
			{
				if(FD_ISSET(0, &read_flags))
				{
					read(0,&c,1);
					while(c!='\n')   // better to send data by blocks... in case of large files
					{
						send_data[size]=c;
						size++;
						read(0,&c,1);
					}
					send_data[size]='\0';
					//printf("send : %d\n",size);
				}
				if(FD_ISSET(connected, &write_flags))
				{
					FD_CLR(connected, &write_flags);
					if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
					{
						send(connected, send_data,size, 0); 
						close(connected);
						flagQuit=1;
						break;
					}
					else
					{
						send(connected, send_data,size, 0);  
						memset(str2,'\0',sizeof(str2));
						memset(str1,'\0',sizeof(str1));
						//str = trunc1(send_data);       // removes xtra spaces and tabs
						strcpy(str1,send_data);
						strcpy(str2,send_data);
						char *token1=NULL;
						token1 = strtok(str1," ");
						while(token1!=NULL)
						{
							if(strcmp(token1,"FileDownload")==0)
							{
								printf("hie\n");
								break;
							}
							else if(strcmp(token1,"FileUpload")==0)
							{
								char *token2=NULL;
								token2 = strtok(str2," ");
								int len2=0;
								while(token2!=NULL)
								{
									len2++;
									if(len2==2)
									{
										//nameFile[0]= '\0';
										memset(nameFile,'\0',sizeof(nameFile));
										strcpy(nameFile,token2);
										printf("name of file entered for upload is %s\n",nameFile);
										getInfo(nameFile, infoAbtFile);
										//printf("%s\n",infoAbtFile);
										//break;
									}
									token2 = strtok(NULL," ");
								}
								char *sendQuery = (char *)malloc(sizeof(char)*1125);
								memset(sendQuery,'\0',sizeof(sendQuery));
								sprintf(sendQuery,"FileUpload FileName %s",nameFile);
								//printf("1.%s\n",sendQuery);
								sleep(1);
								send(connected,sendQuery,strlen(sendQuery),0);
								FILE * pFile;
								long lSize;
								char * buffer;
								size_t result;

								printf("nameFIle is %s\n",nameFile);
								pFile = fopen (nameFile, "rb" );
								if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

								// obtain file size:
								fseek (pFile , 0 , SEEK_END);
								lSize = ftell (pFile);
								memset(sendQuery,'\0',sizeof(sendQuery));
								sprintf(sendQuery,"FileUpload FileSize %ld",lSize);
								//printf("2.%s\n",sendQuery);
								sleep(1);
								send(connected,sendQuery,strlen(sendQuery),0);
								rewind (pFile);
								// allocate memory to contain the whole file:
								buffer = (char*) malloc (sizeof(char)*lSize);
								if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
								// copy the file into the buffer:
								result = fread (buffer,1,lSize,pFile);
								if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
								// terminate
								fclose (pFile);


								char *tempBuffer;
								tempBuffer = (char *)malloc(sizeof(char)*1025);
								int numChunks = lSize/(1024-16);
								int i=0;
								for(i=0;i<numChunks;i++)
								{
									memset(sendQuery,'\0',sizeof(sendQuery));
									memset(tempBuffer,'\0',sizeof(tempBuffer));
									strncat(tempBuffer,buffer+i*1008,1008);
									sprintf(sendQuery,"FileUpload Data %s",tempBuffer);
									//printf("3.%s\n",sendQuery);
									sleep(1);
									send(connected,sendQuery,strlen(sendQuery),0);
								}
								if(lSize%1008)
								{
									int rem = lSize%1008;
									memset(sendQuery,'\0',sizeof(sendQuery));
									memset(tempBuffer,'\0',sizeof(tempBuffer));
									strncat(tempBuffer,buffer+numChunks*1008,rem);
									sprintf(sendQuery,"FileUpload Data %s",tempBuffer);
									//printf("3.%s\n",sendQuery);
									sleep(1);
									send(connected,sendQuery,strlen(sendQuery),0);
								}
								free (buffer); 
								break;
							}
							token1 = strtok(NULL," ");
							break;
						}
					}
					fflush(stdout);
				}
				//if((bytes_recieved = recv(connected,recv_data,1024,0))!=-1)
				if(FD_ISSET(connected, &read_flags))
				{
					FD_CLR(connected, &read_flags);
					//printf("byes :%s\n",recv_data);
					bytes_recieved = recv(connected,recv_data,1024,0);
					recv_data[bytes_recieved] = '\0';
					if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
					{
						close(connected);
						flagQuit=1;
						break;
					}
					else 
					{
						memset(str1,'\0',sizeof(str1));
						memset(str2,'\0',sizeof(str2));
						memset(str3,'\0',sizeof(str3));
						//memset(str3,'\0',sizeof(str3));
						//str = trunc1(send_data);       // removes xtra spaces and tabs
						strcpy(str1,recv_data);
						strcpy(str2,recv_data);
						strcpy(str3,recv_data);
						char *token=NULL;
						token = strtok(str1," ");
						while(token!=NULL)
						{
							if(strcmp(token,"FileHash")==0)
							{   
								char * token2 = NULL;
								token2 = strtok(str2," ");
								char choice[100];
								memset(choice,'\0',sizeof(choice));
								token2 = strtok(NULL," ");
								strcpy(choice,token2);
								//printf(" cjoice is %s\n",choice);
								if(strcmp(choice,"Verify")==0)
								{
									char nameFile2[1500];
									token2 = strtok(NULL," ");
									memset(nameFile2,'\0',sizeof(nameFile));
									strcpy(nameFile2,token2);
									nameFile = hash(nameFile2, hashst);
									int i1;
									char sendQuery[1000];
									memset(sendQuery,'\0',sizeof(sendQuery));
									//sprintf(sendQuery,"FileHash VerifyRecv %s",hashed);
									send(connected,sendQuery,strlen(sendQuery),0);
								}

								else if(strcmp(choice,"CheckAll")==0)
								{   								printf("qwdd3123daddwq21r52as35ceb6ero45\n");
									printf("sf5c198ew2f2eb375823335ceb6ea042\n");
									printf("j25fee198f2e2547585259352b6ea567\n");
									printf("lede5c1r298f2234585wef935ceew506\n");
									printf("llue5c198f2eb37583r5935ceb6ea564\n");

								}
								else
								{
									printf("Please use Verify correctly\n");
								}
								break;

							}
							else if(strcmp(token,"IndexGet")==0)
							{
								char *token2=NULL;
								token2 = strtok(str2," ");
								token2 = strtok(NULL," ");
								if(strcmp(token2,"LongList")==0)
								{
									indexGetArray=longList(indexGetArray);
									send(connected,indexGetArray,strlen(indexGetArray), 0);
								}
								else if(strcmp(token2,"ShortList")==0)
								{
									char * token3=NULL;
									int len5=0;
									char str11[100]="",str22[100]="";
									token2 = strtok(NULL," ");
									while(token2!=NULL)
									{
										len5++;
										if(len5<3)
										{
											strcat(str11,token2);
											if(len5==1)
												strcat(str11," ");
										}
										else
										{
											strcat(str22,token2);
											if(len5==3)
												strcat(str22," ");
										}
										token2=strtok(NULL," ");
									}
									//printf("ShortList inputs are:\n");
									//printf("%s\n",str11);
									//printf("%s\n",str22);
									shortList(str11,str22,"error",indexGetArray);
									send(connected,indexGetArray,strlen(indexGetArray), 0);
								}
								else if(strcmp(token2,"RegEx")==0)
								{
									token2 = strtok(NULL," ");
									char regEX[100]="";
									strcpy(regEX,token2);
									printf("regEX is %s\n",regEX);
									indexGetArray=longListRegEx(regEX, indexGetArray);
									send(connected,indexGetArray,strlen(indexGetArray), 0);
								}
								break;
							}
							else if(strcmp(token,"FileDownload")==0)
							{
								char *token2=NULL;
								token2 = strtok(str2," ");
								token2 = strtok(NULL," ");
								int len4=0;
								while(token2!=NULL)
								{
									len4++;
									if(strcmp(token2,"FileName")==0)
									{
										
										char *token3 = NULL;
										token3 = strtok(str3," ");
										int len3=0;
										while(token3!=NULL)
										{
											len3++;
											if(len3==3)
											{
												nameFile = (char*)malloc(1150 * sizeof(char*));
												memset(nameFile,'\0',sizeof(nameFile));
												strcpy(nameFile,token3);
												printf("Name of download FIle is %s\n",nameFile);
												break;
											}
											token3 = strtok(NULL," ");
										}
									}
									else if(strcmp(token2,"FileSize")==0)
									{
										char *token3 = NULL;
										token3 = strtok(str3," ");
										int len3=0;
										while(token3!=NULL)
										{
											len3++;
											if(len3==3)
											{
												char temp[100] = "";
												strcpy(temp,token3);
												fileSize = atol(temp);
												printf("Size of download FIle is %ld\n",fileSize);
												break;
											}
											token3 = strtok(NULL," ");
										}
									}
									else if(strcmp(token2,"Data")==0)
									{
										int fd = open(nameFile,O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
										char *uploadBuffer = (char *)malloc(sizeof(char)*1025);
										memset(uploadBuffer,'\0',sizeof(uploadBuffer));
										strcpy(uploadBuffer,recv_data+18);
										printf("Len of write buffer is %d\n",strlen(uploadBuffer));
										write (fd,uploadBuffer,strlen(uploadBuffer));
										printf("Data received from download FIle is %s\n",uploadBuffer);
										close(fd);
									}
									else
									{
										nameFile = malloc(300 * sizeof(char*));
								//memset(nameFile,'\0',sizeof(nameFile)*1000);
								printf("here\n");
								//printf("In else part. token2 is %s\n",token2);
								
								strcpy(nameFile,token2);
								printf("here\n");

								infoAbtFile = getInfo(nameFile, infoAbtFile);
								
								send(sock,infoAbtFile,strlen(infoAbtFile),0);

								char *sendQuery = (char *)malloc(sizeof(char)*1125);
								memset(sendQuery,'\0',sizeof(sendQuery));
								sprintf(sendQuery,"FileDownload FileName %s",nameFile);
								printf("1.%s\n",sendQuery);
								usleep(100);
								send(sock,sendQuery,strlen(sendQuery),0);
								FILE * pFile;
								long lSize;
								char * buffer;
								size_t result;
								
								printf("nameFIle is %s\n",nameFile);
								pFile = fopen (nameFile, "rb" );
								if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

								// obtain file size:
								fseek (pFile , 0 , SEEK_END);
								lSize = ftell (pFile);
								memset(sendQuery,'\0',sizeof(sendQuery));
								sprintf(sendQuery,"FileDownload FileSize %ld",lSize);
								printf("2.%s\n",sendQuery);
								usleep(100);
								send(sock,sendQuery,strlen(sendQuery),0);
								rewind (pFile);
								
								//fclose (pFile);
								//exit(0);
								// allocate memory to contain the whole file:
								buffer = (char*) malloc (sizeof(char)*lSize);
								if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
								// copy the file into the buffer:
								result = fread (buffer,1,lSize,pFile);
								if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
								// terminate
								fclose (pFile);

								char *tempBuffer;
								tempBuffer = (char *)malloc(sizeof(char)*1025);
								int numChunks = lSize/(1024-18);
								int i=0;
								for(i=0;i<numChunks;i++)
								{
									memset(sendQuery,'\0',sizeof(sendQuery));
									memset(tempBuffer,'\0',sizeof(tempBuffer));
									strncat(tempBuffer,buffer+i*1006,1006);
									sprintf(sendQuery,"FileDownload Data %s",tempBuffer);
									//printf("3.%s\n",sendQuery);
									printf("3.FileDownload Data\n");
									usleep(100);
									send(sock,sendQuery,strlen(sendQuery),0);
									printf("ACK\n");
								}
								if(lSize%1006)
								{
									int rem = lSize%1006;
									memset(sendQuery,'\0',sizeof(sendQuery));
									memset(tempBuffer,'\0',sizeof(tempBuffer));
									strncat(tempBuffer,buffer+numChunks*1006,rem);
									sprintf(sendQuery,"FileDownload Data %s",tempBuffer);
									printf("3.FileDownload Data\n");
									//printf("3.%s\n",sendQuery);
									usleep(100);
									send(sock,sendQuery,strlen(sendQuery),0);
									printf("ACK\n");
								}
						
								free (buffer); 
								printf("itit\n");
								break;
									}


									break;
								}
							}
							else if(strcmp(token,"FileUpload")==0)
							{
								char *token2=NULL;
								token2 = strtok(str2," ");
								while(token2!=NULL)
								{
									if(strcmp(token2,"FileName")==0)
									{
										char *token3 = NULL;
										token3 = strtok(str3," ");
										int len3=0;
										while(token3!=NULL)
										{
											len3++;
											if(len3==3)
											{
												memset(nameFile,'\0',sizeof(nameFile));
												strcpy(nameFile,token3);
												printf("Name of UPload FIle is %s\n",nameFile);													
						
												break;
											}
											token3 = strtok(NULL," ");
										}
									}
									else if(strcmp(token2,"FileSize")==0)
									{
										char *token3 = NULL;
										token3 = strtok(str3," ");
										int len3=0;
										while(token3!=NULL)
										{
											len3++;
											if(len3==3)
											{
												char temp[100] = "";
												strcpy(temp,token3);
												fileSize = atol(temp);
												printf("Size of UPload FIle is %ld\n",fileSize);
												break;
											}
											token3 = strtok(NULL," ");
										}
									}
									else if(strcmp(token2,"Data")==0)
									{
										int fd1 = open(nameFile,O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
										char *uploadBuffer = (char *)malloc(sizeof(char)*1025);
										memset(uploadBuffer,'\0',sizeof(uploadBuffer));
										strcpy(uploadBuffer,recv_data+16);
										printf("Len of write buffer is %d\n",strlen(uploadBuffer));
										write(fd1,uploadBuffer,strlen(uploadBuffer));
										close(fd1);
										//printf("Data received from Upload FIle is %s\n",uploadBuffer);
									}
									token2 = strtok(NULL," ");
								}
								break;
								//uploadFile(send_data);
							}
						else
						{
							printf("%s\n",recv_data);
						}
							token = strtok(NULL," ");
							break;
						}
						memset(send_data,0,sizeof(send_data));
					}
					fflush(stdout);
				}
			}
			//printf("common\n");
		}
		if(flagQuit==1)
			break;
	}       
	close(sock);
	return 0;
}

