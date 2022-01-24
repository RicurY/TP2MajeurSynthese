#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h> 
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>

#define SIZE_MAX 256
#define PORT "69"

const char erreur[SIZE_MAX] = "Erreur, il faut 3 arguments\n";


int RRQ(char* filename, char* mode, int socket, struct addrinfo *result){
	
	char* cmd = malloc(2+strlen(filename)+1+strlen(mode)+1);
	cmd[0]=0;
	cmd[1]=1;
	strcpy(&cmd[2],filename);
	cmd[2+strlen(filename)]=0;
	
	strcpy(&cmd[2+strlen(filename)+1],mode);
	cmd[2+strlen(filename)+1+strlen(mode)+1]=0;
	
	
	int nbCharEnvoye = sendto(socket, cmd, 2+strlen(filename)+1+strlen(mode)+1, 0, result->ai_addr, result->ai_addrlen);
	
	free(cmd);
	
	return nbCharEnvoye;

}

/*int DATA(char* data, .......){
	
	char* cmd = malloc(2+2+strlen(data));
	cmd[0]=1;
	cmd[1]=1;
	cmd[2]=0;
	cmd[3]=1;
	strcpy(&cmd[4],data);
	
	
	int nbCharReçu = recvfrom(.........);
	
	free(cmd);
	
	return nbCharEnvoye;

}*/

int main(int argc, char* argv[]){
	
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	
	if (argc!=3){
		write(STDOUT_FILENO,erreur,sizeof(erreur));
		exit(EXIT_FAILURE);
	}
	
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_UDP;
	
	s = getaddrinfo(argv[1], PORT, &hints, &result);
	if(s!=0){
		fprintf(stderr,"getaddrinfo : %s\n",gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

       if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;

       close(sfd);
    }

   if (rp == NULL) { 
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }
	
	int nbCharEnvoye = RRQ(argv[2],"octet", sfd, result);
	char msg[SIZE_MAX];
	sprintf(msg,"%d",nbCharEnvoye);	
	
	write(STDOUT_FILENO, "C'est bon\n", 11);
	exit(EXIT_FAILURE);
	
}




