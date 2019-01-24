from serial import Serial
import paho.mqtt.client as mqttClient
from time import sleep
import json

AllowedActions = ['both', 'publish', 'subscribe']

# Custom MQTT message callback
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to broker")
    else:
        print("Connection failed")


def on_publish(client, messageJson):
    if mode == 'publish':
        print('Published topic %s: %s\n' % (topic, messageJson))

broker = "MQTT broker ip" # [To DO]
port = MQTT broker port # [To DO]
clientId = "myRPi"
topic = "RPi/PM25"
mode = "publish"

client = mqttClient.Client(clientId)
client.on_connect = on_connect
client.on_publish = on_publish
print("try connect")
client.connect(broker,port)
sleep(2)

loopCount = 0
ser = Serial('/dev/ttyACM0', 9600, timeout=.5)

while True:
    if ser.inWaiting():
        str = ser.readline().decode('utf8')[:-2]
        end = str.find(' :')
        if end > 0:
            PM25 = str[:end]
            message = {}
            message['type'] = 'PM2.5'
            message['value'] = PM25
            message['device'] = 'RPi'
            message['sequence'] = loopCount
            messageJson = json.dumps(message)
            client.publish(topic, messageJson)
            sleep(2)
            loopCount += 1

