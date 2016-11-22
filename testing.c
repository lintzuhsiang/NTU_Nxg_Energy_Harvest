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


/***@brief  設置串口通信速率
*@param  fd     類型 int  打開串口的文件控制碼
*@param  speed  類型 int  串口速度
*@return  void*/

void set_speed(int fd)
{
  int   i;
  int   status;
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

/**
*@brief   設置串口資料位元，停止位元和效驗位
*@param  fd     類型  int  打開的串口文件控制碼*
*@param  databits 類型  int 資料位元   取值 為 7 或者8*
*@param  stopbits 類型  int 停止位   取值為 1 或者2*
*@param  parity  類型  int  效驗類型 取值為N,E,O,,S
*/

int set_Parity(int fd,int databits,int stopbits,int parity)
{

 struct termios options;

 if  ( tcgetattr( fd,&options)  !=  0)
  {
        perror("SetupSerial 1");
        return(FALSE);
  }

  options.c_cflag &= ~CSIZE;

  switch (databits) /*設置數據位元數*/
  {
        case 7:
                 options.c_cflag |= CS7;
                 break;

        case 8:
                 options.c_cflag |= CS8;
                 break;

        default:
                 fprintf(stderr,"Unsupported data size\n");
                 return (FALSE);
        }

  switch (parity)
        {
        case 'n':
        case 'N':
                 options.c_cflag &= ~PARENB;   /* Clear parity enable */
                 options.c_iflag &= ~INPCK;     /* Enable parity checking */
                 break;
        case 'o':
        case 'O':
                 options.c_cflag |= (PARODD | PARENB);  /* 設置為奇效驗*/
                 options.c_iflag |= INPCK;             /* Disnable parity checking */
                 break;
        case 'e':
        case 'E':
                 options.c_cflag |= PARENB;     /* Enable parity */
                 options.c_cflag &= ~PARODD;   /* 轉換為偶效驗*/
                 options.c_iflag |= INPCK;       /* Disnable parity checking */
                 break;
        case 'S':
        case 's':  /*as no parity*/
                 options.c_cflag &= ~PARENB;
                 options.c_cflag &= ~CSTOPB;
                 break;
        default:
                 fprintf(stderr,"Unsupported parity\n");
                 return (FALSE);
                 }

  /* 設置停止位*/

  switch (stopbits)
        {
        case 1:
                 options.c_cflag &= ~CSTOPB;

                 break;

        case 2:

                 options.c_cflag |= CSTOPB;

                 break;

        default:

                 fprintf(stderr,"Unsupported stop bits\n");

                 return (FALSE);

        }

  /* Set input parity option */

  if (parity != 'n')

                 options.c_iflag |= INPCK;

    options.c_cc[VTIME] = 150; // 15 seconds

    options.c_cc[VMIN] = 0;



  tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */

  if (tcsetattr(fd,TCSANOW,&options) != 0)

        {

                 perror("SetupSerial 3");

                 return (FALSE);

        }

  return (TRUE);

 }

/**

*@breif 打開串口

*/

int OpenDev(char *Dev)

{

int     fd = open( Dev, O_RDWR );         //| O_NOCTTY | O_NDELAY

        if (-1 == fd)

                 { /*設置數據位元數*/

                         perror("Can't Open Serial Port");

                         return -1;

                 }

        else

        return fd;



}

/**

*@breif  main()

*/

void StringToHex(unsigned char* string, unsigned char* hexstring)
{
       unsigned char ch,i,j,len;

       len = strlen(string);

       for(i=0,j=0;i<len;i++,j+=2)
       {
            ch = string[i];

            if( ch >= 0 && ch <= 0x0F)
            {
                hexstring[j] = 0x30;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
            else  if( ch >= 0x10 && ch <= 0x1F)
            {
                hexstring[j] = 0x31;
                ch -= 0x10;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
            else  if( ch >= 0x20 && ch <= 0x2F)
            {
                hexstring[j] = 0x32;
                ch -= 0x20;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
            else  if( ch >= 0x30 && ch <= 0x3F)
            {
                hexstring[j] = 0x33;
                ch -= 0x30;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
            else  if( ch >= 0x40 && ch <= 0x4F)
            {
                hexstring[j] = 0x34;
                ch -= 0x40;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
            else  if( ch >= 0x50 && ch <= 0x5F)
            {
                hexstring[j] = 0x35;
                ch -= 0x50;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
            else  if( ch >= 0x60 && ch <= 0x6F)
            {
                hexstring[j] = 0x36;
                ch -= 0x60;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
            else  if( ch >= 0x70 && ch <= 0x7F)
            {
                hexstring[j] = 0x37;
                ch -= 0x70;

                if(ch >= 0 && ch <= 9)
                    hexstring[j+1] = 0x30 + ch;
                else
                    hexstring[j+1] = 0x37 + ch;
            }
       }
       hexstring[j] = 0x00;
}

void StringToHexAscii(char *in, char *out){
     char *buf;
     char *str;
     for ( int i = 0; i < sizeof(&in); i++ ) {
        char c = in[i];
        sprintf(buf,"%X",c);  //Dec => Hex
        strcat(str,buf);
    }
    strcpy (out, str);
}

int main(int argc, char **argv)
{

  int fd;
  int nread;
	int nnread;
  char buff[512];
  char *dev ="/dev/ttyUSB0";
	char* hexout;
	char* string;
  fd = OpenDev(dev);
  if (fd>0)
      set_speed(fd);

  else
  {
   printf("Can't Open Serial Port!\n");
   exit(0);
  }

  if (set_Parity(fd,8,1,'N')== FALSE)
  {
   printf("Set Parity Error\n");
   exit(1);
  }

  while(1)
  {
   while((nread = read(fd,buff,512))>0)
   {
	  printf("\nLen %x\n",nread);
    buff[nread+1]='\0';

	  string = &nread;
    //StringToHex(string, hexout);
    StringToHexAscii(string,hexout);
	  printf(hexout);

	  }
  }
    close(fd);
    exit(0);
}
