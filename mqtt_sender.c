#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
//#include "util.h"
//#include "debug.h"
//#include "aes_lib.h"
#include "mqtt_command.h"

#define CONNECT_RESPONSE_SUCCESS  0



#define BUF_SZ 1024

int qos = 0;



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

    char* host = "140.112.41.157";
    int port = 1883;

    char* mqttClientId = NULL;
    char* mqttUsername = NULL;
    char* mqttPasswd = NULL;

    char *message = "Hello world.";
    char *topic = "hello";

    struct mosquitto *mosq;
    char buf[BUF_SZ];

    int numServer = 1;
    int serverIndex = 0;

    printf("Start to connect to server.\n");
    printf( "Topic is %s\n", topic);
    fflush(stdout);

    mosquitto_lib_init();    //initial the mosquitto function
    mosq = mosquitto_new(NULL, 1, NULL);  //create a new mosquitto client


    mosquitto_username_pw_set(mosq, mqttUsername, mqttPasswd);   //configure a username and passwd
    mosquitto_publish_callback_set(mosq, my_publish_callback);
    //mosquitto_connect_callback_set(mosq,my_connect_callback);  //set the connect callback


    if(mosquitto_connect(mosq, host, port, 60)) {
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }

    if( mosquitto_publish(mosq,NULL,topic,strlen(message),message,qos,0) == MOSQ_ERR_SUCCESS) {
        printf("success on publish.\n");
        fflush(stdout);
    } else {
        printf("fail on publish.\n");
        fflush(stdout);
    }

    mosquitto_loop_forever(mosq, -1, 1);
    return 0;
}
