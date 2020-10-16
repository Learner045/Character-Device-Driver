#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
 
#define BUFFER_LENGTH 20000
static char user_buffer[BUFFER_LENGTH];
 
int main(){
  int ret, fd;

	fd = open("/dev/process_list", O_RDONLY);             // Open the device with read only access

	if (fd < 0){
		perror("Failed to open the device process_list!");
		return errno;
	}

	ret = read(fd, user_buffer, BUFFER_LENGTH);        // Read the response

	if (ret < 0){
		perror("Failed to read the message from the process_list");
		return errno;
	}
 
   int len = sizeof(user_buffer) / sizeof(char*);
   
  if(len>1)
	printf("\n%s\n", user_buffer);			// display the buffer contents

	close(fd);

	printf("End of the program\n");
	return 0;
}

