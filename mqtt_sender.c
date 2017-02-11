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
//char *host ="broker.hivemq.com";
//int *port = 1883;
int qos = 2;
//char topic[128] = "";

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", message->topic, message->payload);
	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

void onConnectCallback(struct mosquitto *mosq,void *obj,int rc) {
    printf("30000");
    if(!rc) {
        printf("Success connect to server!\n");

    }
    else {
        printf("Fail to connect to server.\n");
    }
    fflush(stdout);
}

void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;

	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

int main(int argc,char **argv) {

    char *host = "broker.mqttdashboard.com";
    //char* host = "140.112.41.157";
    int port = 1883;

    char* mqttClientId = "lens_UeH7aySXVo87dOnbOioUFW9L3tC";
    //char *mqttClientId = "clientId-wcd3z7o1Tv";
    char* mqttUsername = "shine";
    char* mqttPasswd = "shine0125951";

    char *message = "Hello world.";
    char *topic = "hello";

    struct mosquitto *mosq = NULL;
    char buf[BUF_SZ];

    int numServer = 1;
    int serverIndex = 0;
    //int k = 0 ;
    printf("Start to connect to server.\n");
    fflush(stdout);
    FILE *src = stdin;


    mosquitto_lib_init();    //initial the mosquitto function
    mosq = mosquitto_new(NULL, 1, NULL);  //create a new mosquitto client
    if(mosquitto_lib_init() == MOSQ_ERR_SUCCESS	)
    {
        printf("[1] host: %s, port: %d\n", host, port);
    }
    fflush(stdout);

    mosquitto_username_pw_set(mosq, mqttUsername, mqttPasswd);   //configure a username and passwd
    mosquitto_connect_callback_set(mosq,onConnectCallback);  //set the connect callback
    mosquitto_message_callback_set(mosq, my_message_callback);
    // if(mosquitto_connect_async(mosq, host, port, 60) == MOSQ_ERR_SUCCESS	){
    //     printf("success on connecting.\n");
    //     mosquitto_loop_start(mosq);
    // }  //start the loop
    if(mosquitto_connect(mosq, host, port, 60)){
    		fprintf(stderr, "Unable to connect.\n");
    		return 1;
    	}
    else if(mosquitto_connect(mosq, host, port, 60) == MOSQ_ERR_SUCCESS	){
        printf("success on connecting.\n");
        mosquitto_loop(mosq,-1,1);
  }

    //mosquitto_publish(mosq,NULL,topic,strlen(message),message,qos,0);
    if( mosquitto_publish(mosq,NULL,topic,strlen(message),message,qos,0) == MOSQ_ERR_SUCCESS) {
        printf("success on publish.\n");
        fflush(stdout);
    } else {
        printf("fail on publish.\n");
        fflush(stdout);
    }

    for(int k=0; k<5; k++) {
        char *top = "aaa";
        char *mes = "hello world.\n";
        mosquitto_publish(mosq,NULL,top,strlen(mes),mes,qos,0);
    }

    fclose(src);
    return 0;

}
