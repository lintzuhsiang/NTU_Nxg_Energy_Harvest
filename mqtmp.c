#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
//#include "util.h"
//#include "debug.h"
//#include "aes_lib.h"
#include "mqtt_command.h"

//#define ENCRYPTION
#define NON_BLOCKING_CONNECTION
#define BUF_SZ 1024

const char* TAG = "COMMAND_SENDER";
unsigned char pKey[128];
int qos = 0;
char topic[128] = "";

int connectServer(struct mosquitto *mosq, const char* host, int port) {
    if(mosquitto_connect(mosq, host, port, 600) != MOSQ_ERR_SUCCESS) {
        printf("Connect Fail to %s:%d\n", host, port);
        fflush(stdout);
        return 0;
    }
    printf("Connect Success to %s:%d\n", host, port);
    fflush(stdout);
    return 1;
}

int sendMessageToTopic(struct mosquitto* mosq, const char* topic, char* message) {
	//#ifndef ENCRYPTION
  return mosquitto_publish(mosq, NULL, topic, strlen(message), message, qos, 0);
	//#else
	unsigned char pResult[BUF_SZ];
	char resultHexStr[BUF_SZ * 2];
	int encryptedLen = 0;
	//char* msg = trim(message);

	//debug(TAG, "Original Message [%s]", msg);
	//debug(TAG, "Encryption Key [%s]", pKey);
	//aesEncrypt192Multi(trim(message), pKey, pResult, strlen(msg));

	//if(strlen(msg) % 16 == 0) {
	// 	encryptedLen = strlen(msg);
	// }
	// else {
	// 	encryptedLen = (strlen(msg) / 16 + 1) * 16;
	// }
  //
	// convert2HexChar(resultHexStr, pResult, encryptedLen, &encryptedLen);
  //
	// resultHexStr[encryptedLen] = '\0';
	// debug(TAG, "Encrypted Text [%s]", resultHexStr);
  //   return mosquitto_publish(mosq, NULL, topic, encryptedLen, resultHexStr, qos, 0);
  //
	// #endif
}

void onConnectCallback(struct mosquitto *mosq, void *obj, int rc) {
    if(rc == CONNECT_RESPONSE_SUCCESS) {
        printf("Connected to MQTT server\n");
    }
    else {
        printf("ERR!!! Fail to connect to MQTT server\n");
    }
    fflush(stdout);
}

// bool preparePKey(char* key, int sz) {
//     FILE* f = fopen("/home/key.config", "r");
//     if (!f) return false;
//     key = trim(fgets(key, sz, f));
//     fclose(f);
//     return strlen(key) > 0;
// }
//
// bool prepareTopic(char* topic, int sz) {
//     FILE* f = fopen("/home/topic.config", "r");
//     if (!f) return false;
//     topic = trim(fgets(topic, sz, f));
//     fclose(f);
//     return strlen(topic) > 0;
// }

int main(int argc, char** argv) {

    char** host = NULL;

    int* port = NULL;

    char* mqttClientId = NULL;
    char* mqttUsername = NULL;

    struct mosquitto *mosq[10];


    char buf[BUF_SZ];

    // Multi-server -------------

    int numServer = 1;
    int serverIndex = 0;

    //---------------------------

    // Initial parameters -------
    FILE *src = stdin;


    // simplistic way of parsing params
    // 1st param is host (using default topic in topic.config)
    // 2nd param is port
    // 3rd param is qos
    // 4th param is total number of servers (default value: 1)
    // (5th, 6th) => (server ip, server port)

    // Allocate memory here
    // if (argc >= MQTT_INPUT_INDEX_SERVER_COUNT + 1)
    //     numServer = atoi(argv[MQTT_INPUT_INDEX_SERVER_COUNT]);
    // printf("Total number of server: %d\n", numServer);
    // fflush(stdout);
    //
    // host = calloc(numServer, sizeof(char*));
    // port = calloc(numServer, sizeof(int));
    //
    // if (argc >= MQTT_INPUT_INDEX_SERVER_ADDR + 1)
    //     host[0] = argv[MQTT_INPUT_INDEX_SERVER_ADDR];
    // if (argc >= MQTT_INPUT_INDEX_SERVER_PORT + 1)
    //     port[0] = atoi(argv[MQTT_INPUT_INDEX_SERVER_PORT]);
    // if (argc >= MQTT_INPUT_INDEX_QOS + 1)
    //     qos  = atoi(argv[MQTT_INPUT_INDEX_QOS]);
    //
    // if((argc < MQTT_INPUT_INDEX_SERVER_COUNT + SERVER_INFO_INDEX_TOTAL_SIZE * (numServer - 1) + 1) && (argc >= (MQTT_INPUT_INDEX_QOS + 1))) {
    //     printf("Wrong number of parameters input!\n");
    //     exit(2);
    // }
    //
    // if(argc > MQTT_INPUT_INDEX_SERVER_COUNT + SERVER_INFO_INDEX_TOTAL_SIZE * (numServer - 1) + MQTT_INPUT_After_Server_INDEX_CLIENTID + 1) {
    //     mqttClientId = argv[MQTT_INPUT_INDEX_SERVER_COUNT + SERVER_INFO_INDEX_TOTAL_SIZE * (numServer - 1) + MQTT_INPUT_After_Server_INDEX_CLIENTID + 1];
    //     printf("Client ID: %s\n", mqttClientId);
    //     fflush(stdout);
    // }
    //
    // if(argc > MQTT_INPUT_INDEX_SERVER_COUNT + SERVER_INFO_INDEX_TOTAL_SIZE * (numServer - 1) + MQTT_INPUT_After_Server_INDEX_USERNAME + 1) {
    //     mqttUsername = argv[MQTT_INPUT_INDEX_SERVER_COUNT + SERVER_INFO_INDEX_TOTAL_SIZE * (numServer - 1) + MQTT_INPUT_After_Server_INDEX_USERNAME + 1];
    //     printf("Mqtt Username: %s\n", mqttUsername);
    //     fflush(stdout);
    // }
    //
    // #ifdef ENCRYPTION
    // if (!preparePKey(pKey, sizeof(pKey))) {
    //     printf("Please put a key string in key.config file");
    //     exit(2);
    // }
    // debug(TAG, "Key is %s", pKey);
    // #endif

    // if (!prepareTopic(topic, sizeof(topic))) {
    //     printf("Please put a topic string in topic.config file");
    //     exit(2);
    // }
    printf( "Topic is %s\n", topic);
    fflush(stdout);

    mosquitto_lib_init();
    mosq[0] = mosquitto_new(mqttClientId, 1, NULL);
    printf("[1] host: %s, port: %d\n", host[0], port[0]);
    fflush(stdout);

    mosquitto_username_pw_set(mosq[0], mqttUsername, NULL);
    mosquitto_connect_callback_set(mosq[0], onConnectCallback);
    mosquitto_connect_async(mosq[0], host[0], port[0], 10);
    mosquitto_loop_start(mosq[0]);

    for(serverIndex = 1; serverIndex < numServer; serverIndex++) {
        mosq[serverIndex] = mosquitto_new(mqttClientId, 1, NULL);
        host[serverIndex] = argv[4 + 2 * (serverIndex - 1) + 1];
        port[serverIndex] = atoi(argv[4 + 2 * (serverIndex - 1) + 2]);
        printf("[%d] host: %s, port: %d\n", serverIndex + 1, host[serverIndex], port[serverIndex]);
        fflush(stdout);

        mosquitto_username_pw_set(mosq[serverIndex], mqttUsername, NULL);
        mosquitto_connect_async(mosq[serverIndex], host[serverIndex], port[serverIndex], 10);
        mosquitto_loop_start(mosq[serverIndex]);
    }

    // --------------------------

    do {
        if (!fgets(buf, BUF_SZ, src))
            continue;

        char* str = buf;
        if (!strlen(str)) continue;

        printf( "%s\n", str);
        fflush(stdout);
        for(serverIndex = 0; serverIndex < numServer; serverIndex++) {
            if(sendMessageToTopic(mosq[serverIndex], topic, str) != MOSQ_ERR_SUCCESS)
                mosquitto_reconnect_async(mosq[serverIndex]);
        }
    }  while (!feof(src));
    //debug(TAG, "Source stream terminated");
    return 0;
}
