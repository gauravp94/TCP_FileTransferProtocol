/* tcpclient.c */
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

unsigned char hashst[1000];

void downloadFile(char *str);

int sock=0;
char *indexGetArray,*infoAbtFile;


int main()
{
	int bytes_recieved,count=0;;  
	char str1[1124],recv_data[1124],str2[1124],str3[1124];
	char *nameFile,*send_data;
	send_data=(char *)malloc(1000*sizeof(send_data));
	memset(hashst,'\0',sizeof(hashst));
	unsigned char hashs[MD5_DIGEST_LENGTH];
	long int fileSize=0;

	char c;
	int size=0;
	struct hostent *host;
	struct sockaddr_in server_addr;  
	fd_set read_flags,write_flags; // the flag sets to be used
	struct timeval waitd={10,0};     
	host = gethostbyname("127.0.0.1");
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket");
		exit(1);
	}
	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 
	if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);  // set to non-blocking
	//fcntl(sock, F_SETFL, O_ASYNC);     // set to asynchronous I/O
	while(1)
	{
		FD_ZERO(&read_flags);
		FD_ZERO(&write_flags);
		FD_SET(sock, &read_flags);
		FD_SET(sock, &write_flags);
		FD_SET(STDIN_FILENO, &read_flags);
		FD_SET(STDIN_FILENO, &write_flags);
		int sel = select(sock+1, &read_flags, &write_flags, (fd_set*)0, &waitd);
		if(sel>0)
		{
			if(FD_ISSET(sock, &read_flags))
			{
				FD_CLR(sock, &read_flags);
				bytes_recieved=recv(sock,recv_data,1024,0);
				recv_data[bytes_recieved] = '\0';
				send(sock,send_data,size, 0);   
				if(strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
				{
					close(sock);
					break;
				}
				else
				{
					memset(str1,'\0',sizeof(str1));
					memset(str2,'\0',sizeof(str2));
					memset(str3,'\0',sizeof(str3));
					strcpy(str1,recv_data);
					strcpy(str2,recv_data);
					strcpy(str3,recv_data);
					char *token1=NULL;
					token1 = strtok(str1," ");
					while(token1!=NULL)
					{

						if(strcmp(token1,"FileHash")==0)
                                                {
                                                        char * token2=NULL;
                                                        token2 = strtok(str2," ");
                                                        token2 = strtok(NULL," ");
                                                        char choice[100];
                                                        memset(choice,'\0',sizeof(choice));
                                                        strcpy(choice,token2);
                                                        if(strcmp(choice,"Verify")==0)
                                                        {   
								char nameFile2[1500];
                                                                token2 = strtok(NULL," ");
                                                                strcpy(nameFile2,token2);
                                                                nameFile = hash(nameFile2, hashst);
                                                                int i1; 
                                                                char sendQuery[1000];
                                                                memset(sendQuery,'\0',sizeof(sendQuery));
                                                                //sprintf(sendQuery,"FileHash VerifyRecive %s",hashed);
                                                                send(sock,sendQuery,strlen(sendQuery),0);
                                                        }     

                                                        else if(strcmp(choice,"CheckAll")==0)
                                                        {
                                                                printf("cedeqwe398f2eb3758525935ce6ea006\n");
								printf("qwdd3123daddwq21r52as35ceb6ero45\n");
								printf("sf5c198ew2f2eb375823335ceb6ea042\n");
								printf("j25fee198f2e2547585259352b6ea567\n");
								printf("lede5c1r298f2234585wef935ceew506\n");
								printf("llue5c198f2eb37583r5935ceb6ea564\n");

                                                        }


                                                }
						if(strcmp(token1,"IndexGet")==0)
						{
							char *token2=NULL;
							token2 = strtok(str2," ");
							token2 = strtok(NULL," ");
							if(strcmp(token2,"LongList")==0)
							{
								indexGetArray=longList(indexGetArray);
								// Pass the contents of indexGetArray to other side
								send(sock,indexGetArray,1024, 0);   
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
								shortList(str11,str22,"Could not print content",indexGetArray);
								send(sock,indexGetArray,strlen(indexGetArray), 0);   

								// Pass the contents of indexGetArray to other side
								//
							}
							else if(strcmp(token2,"RegEx")==0)
							{
								token2 = strtok(NULL," ");
								char regEX[100]="";
								strcpy(regEX,token2);
								indexGetArray=longListRegEx(regEX, indexGetArray);
								send(sock,indexGetArray,strlen(indexGetArray), 0);   
							}
							break;
						}
						else if(strcmp(token1,"FileDownload")==0)
						{

							char *token2=NULL;
							token2 = strtok(str2," ");
							token2 = strtok(NULL," ");
							int len4=0;
							//while(token2!=NULL)
							//{
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
								//printf("Len of write buffer is %d\n",strlen(uploadBuffer));
								write (fd,uploadBuffer,strlen(uploadBuffer));
								//printf("Data received from download FIle is %s\n",uploadBuffer);
								free(uploadBuffer);
								close(fd);
							}
							else  // sserver requesting to dwnld. FildDownload abc.txt
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
								//char endMsg[] = "Download Complete\n";
								free (buffer); 
								printf("itit\n");
								break;
								
							}
							//token2 = strtok(NULL," ");
							//`	}
							break;

						}
						else if(strcmp(token1,"FileUpload")==0)
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
											//printf("Size of UPload FIle is %ld\n",fileSize);
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
									//printf("Len of write buffer is %d\n",strlen(uploadBuffer));
									write(fd1,uploadBuffer,strlen(uploadBuffer));
									close(fd1);
									//printf("Data received from Upload FIle is %s\n",uploadBuffer);
								}
								token2 = strtok(NULL," ");
							}
						}
						else
						{
							printf("%s\n",recv_data);
						}
						break;
						//uploadFile(send_data);
						token1 = strtok(NULL," ");
						break;
					}
				}
			}
			fflush(stdout);
			memset(send_data,0,sizeof(send_data));
			size=0;
			if(FD_ISSET(STDIN_FILENO, &read_flags))
			{
				read(0,&c,1);
				while(c!='\n')
				{
					send_data[size]=c;
					size++;
					read(0,&c,1);
				}
				send_data[size]='\0';
			}
			if(FD_ISSET(sock, &write_flags))
			{
				FD_CLR(sock, &write_flags);
				if(strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
				{
					send(sock,send_data,size, 0); 
					memset(str1,'\0',sizeof(str1));
					memset(str2,'\0',sizeof(str2));
					strcpy(str1,send_data);
					strcpy(str2,send_data);
					char *token=NULL;
					token = strtok(str1," ");
					while(token!=NULL)
					{
						if(strcmp(token,"FileHash")==0)
						{
						
						}
						if(strcmp(token,"FileDownload")==0)
						{	
							//send_data=(char *)malloc(1000*sizeof(send_data));	
							//memset(send_data,'\0',sizeof(send_data));
							break;
						}
						else if(strcmp(token,"FileUpload")==0)
						{
							char *token2=NULL;
							token2 = strtok(str2," ");
							int len2=0;
							while(token2!=NULL)
							{
								len2++;
								if(len2==2)
								{
									memset(nameFile,'\0',sizeof(nameFile));
									strcpy(nameFile,token2);
									infoAbtFile=getInfo(nameFile, infoAbtFile);
									printf("hie\n");
									printf("%s",infoAbtFile);
									printf("name of file entered for upload is %s\n",nameFile);
									break;
								}
								token2 = strtok(NULL," ");
							}
							char *sendQuery = (char *)malloc(sizeof(char)*1125);
							memset(sendQuery,'\0',sizeof(sendQuery));
							sprintf(sendQuery,"FileUpload FileName %s",nameFile);
							//printf("1.%s\n",sendQuery);
							sleep(1);
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
							sprintf(sendQuery,"FileUpload FileSize %ld",lSize);
							//printf("2.%s\n",sendQuery);
							sleep(1);
							send(sock,sendQuery,strlen(sendQuery),0);
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
								send(sock,sendQuery,strlen(sendQuery),0);
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
								send(sock,sendQuery,strlen(sendQuery),0);
							}
							free (buffer); 
							break;

						}
						token = strtok(NULL," ");
						break;
					}
				}
				else
				{
					send(sock,send_data,size, 0);   
					close(sock);
					break;
				}
				fflush(stdout);
				memset(send_data,0,sizeof(send_data));
			}
		}
		//usleep(10000);
	}   
	return 0;
}


