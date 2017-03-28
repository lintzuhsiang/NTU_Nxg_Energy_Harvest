NXG-Lab Energy Harvest System User Guide
=======

## Preparation for setting the gateway
### Step 1 :
![](https://i.imgur.com/q3vjIBR.png)
Plug in the USB.
Click the botton in the bottom right corner of the screen 
*"EnOcean  GmbH EnOcean USB 300 DA [0600]"*
    
### Step 2 :
 Key the following text in the command window.
 *"git clone 'https://github.com/lintzuhsiang/nxg-energy-harvest.git'"*
## Parse enocean tag's info to the gateway
### Step 1 :
Key the following text in the command window.
    *"cd Desktop/nxg-energy-harvest/"*
    *"gcc parse_packet.c -o parse_packet"*
    *"sudo ./parse_packet"*
    This code is used for parsing from PTM330, the EnOcean tag, to the gateway.
### Step 2 :
Switch the magnet in the solenoid.
### Step 3 :
   You can see the following text on the command window.
   
![](https://i.imgur.com/ojkRpv8.png)

![](https://i.imgur.com/jlGO0Mj.png)

Red : If it shows" 0 0 ",it means positive
If it shows" 0 60 0 ",it means negative
Green : ID

## Send the message from the gateway to the server
### Step 1 :
   Key the following text in the command window.
    *"sudo apt-get install mosquitto"*
    *"sudo apt-get install mosquitto-client"*
    *"sudo apt-get install libmosquitto-dev"*
    *"gcc -o mqtt_sender mqtt_sender.c -lmosquitto"*
    *"sudo ./mqtt_sender"*
    
### Step 2 :
   You can see the following text on the command window.
    *"Start to connect to server.
Topic is hello
my_publish_callback
Published : 1
success on publish."*
    This code is for using mqtt to send the message from gateway to server. You can reset the host and the port if you need to. 
    
## Parse tag's info and start the gateway simultaneously
### Step 1 :
   Key the following text in the command window.
    *"gcc -o main main.c -lmosquitto"*
    *"sudo ./main"*

### (Step 2 :)
   You can see the results on the server or mqttlens.
    ![](https://i.imgur.com/8mt3zsY.png)
    The main code is used for collecting tags' ID and status to the server. By switching the magnet in the solenoid, the tag will send its ID and status to the gateway. After that, the gateway will send the received message to the server by using mosquitto in json format.
## Start the server (same as the smarthome)
### STEP 1 : OPEN Virtual Machine
(Choose "Smart-Home-Core-Server")
### STEP 2 : Key the password "CCTslabBL530"
 (Do not press "updated")
### STEP 3 : Key in the command window
 *"cd Desktop/smartxlab-cloud/app"*
 *"sudo gulp"*
### (STEP 4 : )
If you just restart the computer,you will have to remove an error(1883) by keying the string in the command window)
(1883: MQTT port conflicts with the port in mosquitto which will happen when you restart automatically
*"sudo service mosquitto stop"*
*"sudo gulp"*
### STEP 5 :  Check that the server is working
open the browser and go to "140.112.41.157:3000"

    
