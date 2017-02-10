#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
//#include "util.h"
//#include "debug.h"
//#include "aes_lib.h"
#include "mqtt_command.h"

//CONNECT_RESPONSE_SUCCESS = 0,

// typedef enum {
// 	MQTT_INPUT_INDEX_MAIN = 0,
// 	MQTT_INPUT_INDEX_SERVER_ADDR,
// 	MQTT_INPUT_INDEX_SERVER_PORT,
// 	MQTT_INPUT_INDEX_QOS,
// 	MQTT_INPUT_INDEX_SERVER_COUNT,
// 	MQTT_INPUT_INDEX_SERVER_LIST_START
// } MqttInputArgumentIndex;

#define BUF_SZ 1024
//char *host ="broker.hivemq.com";
//int *port = 1883;
int qos = 0;
//char topic[128] = "";

// int connectServer(struct mosquitto *mosq,const char* host,int port){
// 	if(mosquitto_connect(mosq,host,port,600)!=MOSQ_ERR_SUCCESS){
// 		printf("Fail connect to %s: %d\n",host,port );
// 		fflush(stdout);
// 		return 0;
// 	}
// 	else
// 		printf("Success connect to %s:%d\n",host,port);
// 		fflush(stdout);
// 		return 1;
// }

/*int SendMessageToTopic(struct mosquitto *mosq,char *topic,char *message){
	//#ifndef ENCRYPTION
	return mosquitto_publish(mosq,NULL,topic,strlen(message),message,qos,0);
	//else
}*/

void onConnectCallback(struct mosquitto *mosq,void *obj,int rc){
	if(rc == CONNECT_RESPONSE_SUCCESS ){
		printf("Success connect to server!\n");

	}
	else{
		printf("Fail to connect to server.\n");
	}
	fflush(stdout);
}

void callback(struct mosquitto *mosq, void *obj, int rc){}

int main(int argc,char **argv){

	//char* host = "broker.hivemq.com";
  char* host = "broker.mqttdashboard.com";
	int port = 8000;

	//char *mqttClientId = "lens_UeH7aySXVo87dOnbOioUFW9L3tC";
	char *mqttClientId = "clientId-wcd3z7o1Tv";
	char *mqttUsername = "shine";
	char *mqttPasswd = "shine0125951";

	char *message = "Hello world.";
	char *topic = "hello";

	struct mosquitto *mosq;
	char buf[BUF_SZ];

	int numServer = 1;
	int serverIndex = 0;
  //int k = 0 ;
	printf("Start to connect to server.\n");
	fflush(stdout);
	FILE *src = stdin;


	mosquitto_lib_init();    //initial the mosquitto function
  mosq = mosquitto_new(mqttClientId, 1, NULL);  //create a new mosquitto client
	if(mosquitto_lib_init() == MOSQ_ERR_SUCCESS	)
	{
		printf("[1] host: %s, port: %d\n", host, port);
	}
	fflush(stdout);

  mosquitto_username_pw_set(mosq, mqttUsername, mqttPasswd);   //configure a username and passwd
  mosquitto_connect_callback_set(mosq,callback);  //set the connect callback
  mosquitto_connect_async(mosq, host, port, 60);  //connect to a mqtt broker
	if(mosquitto_connect_async(mosq, host, port, 60) == MOSQ_ERR_SUCCESS	)
	mosquitto_loop_start(mosq);  //start the loop

	mosquitto_connect(mosq, host, port, 60);  //connect to a mqtt broker
  mosquitto_loop(mosq);  //start the loop

	mosquitto_publish(mosq,NULL,topic,strlen(message),message,qos,0);
	if(mosquitto_publish(mosq,NULL,topic,strlen(message),message,qos,0) == MOSQ_ERR_SUCCESS){
		printf("success on publish.\n");
		fflush(stdout);
	}else{
		printf("fail on publish.\n");
		fflush(stdout);
	}

	for(int k=0;k<5;k++){
			char *top = "aaa";
			char *mes = "hello world.\n";
			mosquitto_publish(mosq,NULL,top,strlen(mes),mes,qos,0);
	}

  fclose(src);
  return 0;

}
