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
#include <mosquitto.h>
#include "mqtt_command.h"

#define CONNECT_RESPONSE_SUCCESS  0



#define BUF_SZ 1024

int qos = 2;


#define FALSE  -1
#define TRUE   0
#define SPEED B115200


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


void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    int i;
    printf("my_connect_callback\n");
    if(!result) {
        /* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
    } else {
        fprintf(stderr, "Connect failed\n");
    }
}


void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    int i;
    printf("my_subscribe_callback\n");
    printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
    for(i=1; i<qos_count; i++) {
        printf(", %d", granted_qos[i]);
    }
    printf("\n");
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
    int i;
    printf("my_publish_callback\n");
    printf("Published : %d\n", mid);
}

int main(int argc,char **argv) {
    int fd;
    int packet_count =0;
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

    uint8_t packet[31]= {0};
    int buff_count=0;
    int state=1;
    int status = -1;
    //int *id =  calloc(10 , sizeof(int));
    uint8_t id [10] = {0};
    while(1)while(read(fd,buff,sizeof(uint8_t)*buff_len))
        {
            //printf("hello");
            if(state==1)
            {
                if(buff[0] == 0x66)
                {
                    state=2;
                    buff_count++;
                    packet[buff_count-1]=buff[0];
                }
            }
            else if(state==2)
            {
                buff_count++;
                packet[buff_count-1]=buff[0];
                if(buff[0]==0x66 || buff[0]==0xE6)
                {
                    state=3;
                }
                else
                {
                    state=1;
                }
            }
            else if(state==3)
            {
                if(buff_count==30)
                {
                    state=1;
                    for(int i=0; i<30; i++)
                    {
                        printf("%X ",packet[i]);
                    }
                    packet_count++;
                    printf("\npacket_count: %d\n",packet_count);
                    if(packet[13]==0x0)
                    {
                        printf("id: %X %X\n",packet[18],packet[19]);
                        printf("status: positive\n\n");
                        id[0] = packet[18];
                        id[1] = packet[19];
                        status = 1;

                    }
                    else
                    {
                        printf("id: %X %X\n",packet[19],packet[20]);
                        printf("status: negitive\n\n");
                        id[0] = packet[19];
                        id[1] = packet[20];
                        status = 0;

                    }
                    for(int i=0; i<30; i++)
                    {
                        packet[i]=0;
                    }
                    buff_count=0;
                }
                else
                {
                    buff_count++;
                    packet[buff_count-1]=buff[0];
                }
            }



            char* host = "broker.mqttdashboard.com";
            int port = 1883;
            char* mqttClientId = NULL;
            char* mqttUsername = NULL;
            char* mqttPasswd = NULL;
            //char* name = "id";
            char *topic = "hello";
            struct mosquitto *mosq;
            char buf[BUF_SZ];

            int numServer = 1;
            int serverIndex = 0;

            uint8_t message [100] ;

            mosquitto_lib_init();    //initial the mosquitto function
            mosq = mosquitto_new(NULL, 1, NULL);  //create a new mosquitto client
            mosquitto_username_pw_set(mosq, mqttUsername, mqttPasswd);   //configure a username and passwd
            mosquitto_publish_callback_set(mosq, my_publish_callback);
            //mosquitto_connect_callback_set(mosq,my_connect_callback);  //set the connect callback
            mosquitto_subscribe(mosq,NULL,topic,qos);
            if (status != -1) {

                sprintf(message, "{" "\"device_id\": %X %X" "," "\"cmd\":\"update_status\"" "," "\"status\": %d" "}",id[0],id[1],status);


                printf("Start to connect to server.\n");
                printf( "Topic is %s\n", topic);
                fflush(stdout);



                if(mosquitto_connect(mosq, host, port, 60)) {
                    fprintf(stderr, "Unable to connect.\n");
                    return 1;
                }

                if( mosquitto_publish(mosq,NULL,topic,strlen(message),message,qos,0) == MOSQ_ERR_SUCCESS) {
                    printf("success on publish.\n");
                    uint8_t id [10] = {0};
                    fflush(stdout);
                } else {
                    printf("fail on publish.\n");
                    uint8_t id [10] = {0};
                    fflush(stdout);
                }

                mosquitto_loop(mosq, -1, 1);
                status = -1;
            }
        }
    close(fd);
    return 0;
    exit(0);
}
