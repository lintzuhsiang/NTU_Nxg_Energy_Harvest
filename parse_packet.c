#include     <stdio.h>      /*標準輸入輸出定義*/
#include     <stdlib.h>     /*標準函數庫定義*/
#include     <unistd.h>     /*Unix標準函數定義*/
#include     <sys/types.h>  /**/
#include     <sys/stat.h>   /**/
#include     <fcntl.h>      /*檔控制定義*/
#include     <termios.h>    /*PPSIX終端控制定義*/
#include     <errno.h>      /*錯誤號定義*/
#include     <string.h>
#include     <stdbool.h>
#include     <stdint.h>

#define FALSE  -1
#define TRUE   0
#define SPEED B115200

//int aaa(uint8_t);
bool set_interface_attribs(int fd,int speed,int parity)
{
    struct termios tty;
    memset(&tty,0,sizeof(tty));
    tcflush(fd, TCIOFLUSH);
    cfsetispeed(&tty, SPEED);
    cfsetospeed(&tty, SPEED);
    int status = tcsetattr(fd, TCSANOW, &tty);

    if  (status != 0) {
        perror("tcsetattr fd1");
        return false;
    }

    tty.c_cflag = speed | CREAD | CLOCAL | CS8;
    tty.c_iflag = BRKINT | IGNPAR | IXON;
    tty.c_iflag &= ~(IXOFF | IXON);
    tty.c_cflag &= ~CSTOPB;
    tty.c_oflag = 0;
    tty.c_lflag = IEXTEN | ECHOE | ECHOK | ECHOCTL | ECHOKE;
    tty.c_cc[VINTR] = 003;
    tty.c_cc[VQUIT] = 034;
    tty.c_cc[VERASE] = 0177;
    tty.c_cc[VKILL] = 025;
    tty.c_cc[VEOF] = 004;
    tty.c_cc[VSTART] = 021;
    tty.c_cc[VSTOP] = 023;
    tty.c_cc[VSUSP] = 032;
    tty.c_cc[VREPRINT] = 0;
    tty.c_cc[VWERASE] = 027;
    tty.c_cc[VLNEXT] = 026;
    tty.c_cc[VMIN] = 100;
    tty.c_cc[VTIME] = 2;
    return true;
}

int main(int argc, char **argv)
{
    int fd;
    int packet_count =0;
    bool flag=0;
    int buff_len=1;
    uint8_t buff[1];
    char *dev = "/dev/ttyUSB0";
    uint8_t counter=0;

    fd = open(dev,O_RDWR|O_NOCTTY|O_SYNC);
    if (fd>0)
    {
        set_interface_attribs(fd,SPEED,0);
    }
    else
    {
        printf("Can't Open Serial Port!\n");
        exit(0);
    }
    //
    // if (!set_interface_attribs(fd, SPEED, 0)) {
    //  printf("Error with set_interface_attribs function.\n");
    //  exit(1);
    // }
    uint8_t packet[31]= {0};
    int buff_count=0;
    int temp;
    bool ano_flag=0;
    int state=1;
    while(1)while(read(fd,buff,sizeof(uint8_t)*buff_len))
        {

            //printf("%X     ",buff[0]);
            //printf("(%d)",temp);
            if(state==1)
            {
                if(buff[0] == 0x66)
                {
                    state=2;
                }
                buff_count++;
                packet[buff_count-1]=buff[0];
            }
            if(state==2)
            {
                if(buff_count==2)
                {
                    state=3;
                }
                else
                {
                    state=1;
                }
                buff_count++;
                packet[buff_count-1]=buff[0];
            }
            if(state==3)
            {
                if(buff_count==30)
                {
                    state=1;
                    for(int i=0; i<30; i++)
                    {
                        printf("%X ",packet[i]);
                    }
                    packet_count++;
                    printf("\ncount:l %d\n",packet_count);
                    if(packet[13]==0x0)
                    {
                        printf("id: %X %X\n",packet[18],packet[19]);
                        printf("status: positive\n\n");
                    }
                    else
                    {
                        printf("id: %X %X\n",packet[19],packet[20]);
                        printf("status: negitive\n\n");
                    }
                    for(int i=0; i<30; i++)
                    {
                        packet[i]=0;
                    } t
                    buff_count=0;
                }
                else
                {
                    buff_count++;
                    packet[buff_count-1]=buff[0];
                }
            }
        }
    close(fd);
    exit(0);
}
/*int aaa(uint8_t buff[0])
{
    //printf("%X ", buff[0]);
    if(flag==1)
    {
        if(buff[0]==0x66 || buff[0]==0xE6)
        {
            paccount++;
            printf("paccount: %d",paccount);
            flag=0;
            return 0;
        }
    }
    if(buff[0] == 0x66)
    {
        flag=1;
        return 0;
    }
    return 0;
}*/





