#include "networking.h"


int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  char name[100];
  printf("Enter name : ");
  fgets(name, sizeof(name), stdin);
  *strchr(name, '\n') = 0;

  char text[255];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );
  
  //pipes
  int fds[2];
  pipe(fds);

  //Tells server their name
  char name_signal[100] = "!setname ";
  strcat(name_signal, name);
  write(server_socket, name_signal, sizeof(name_signal));
  strcat(name, " : ");

  //fds[0] is stdin, fds[1] is stdout
  if(fork() == 0){
    close(fds[1]);
    dup2(fds[0], STDIN_FILENO);
    while(1){
      read(server_socket, buffer, sizeof(buffer));
      printf("%s\n", buffer);
    }
  }
  else{
    close(fds[0]);
    dup2(fds[1], STDOUT_FILENO);
    while (1) {
      fgets(buffer, sizeof(buffer), stdin);	
      *strchr(buffer, '\n') = 0;
      /*  Clean up after ourselves  */

      //Checks if it is a command
      if(buffer[0] == '!'){
	write(server_socket, buffer, sizeof(buffer));	     
      }else{
	
	write(server_socket, buffer, sizeof(buffer));	     
	printf("writing : %s\n", buffer);
	
	//refresh();
      }
    }
   
  }
}
