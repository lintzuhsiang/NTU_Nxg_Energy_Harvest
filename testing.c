#include     <stdio.h>      /*標準輸入輸出定義*/
#include     <stdlib.h>     /*標準函數庫定義*/
#include     <unistd.h>     /*Unix標準函數定義*/
#include     <sys/types.h>  /**/
#include     <sys/stat.h>   /**/
#include     <fcntl.h>      /*檔控制定義*/
#include     <termios.h>    /*PPSIX終端控制定義*/
#include     <errno.h>      /*錯誤號定義*/
#include     <string.h>

#define FALSE  -1
#define TRUE   0
#define SPEED 115200

void set_speed(int fd)
{
  int status;
  struct termios   Opt;
  tcgetattr(fd, &Opt);
  tcflush(fd, TCIOFLUSH);
	cfsetispeed(&Opt, SPEED);
	cfsetospeed(&Opt, SPEED);
  status = tcsetattr(fd, TCSANOW, &Opt);
  if  (status != 0)
    perror("tcsetattr fd1");
  return;
}

int OpenDev(char *Dev)
{
  int fd = open( Dev, O_RDWR );   //| O_NOCTTY | O_NDELAY
  if (-1 == fd)
  {
   perror("Can't Open Serial Port");
   return -1;
  }
  else
    return fd;
}

int main(int argc, char **argv)
{

  int fd;
  int buff_len = 20;
	char buff[buff_len];
  char *dev ="/dev/ttyUSB0";

  fd = OpenDev(dev);
  if (fd>0)
      set_speed(fd);
  else
  {
   printf("Can't Open Serial Port!\n");
   exit(0);
  }

  while(1)
  {
   while(read(fd,buff,sizeof buff))
   {
    int len = sizeof buff;
    char str[buff_len+1];

    for ( int i = 0; i < len; i++ ) {
            sprintf(&str[i],"%x",buff[i]);  //Dec => Hex
    }

    str[len] = '\0';
    printf("%s\n",str);
	  }
  }
    close(fd);
    exit(0);
}
