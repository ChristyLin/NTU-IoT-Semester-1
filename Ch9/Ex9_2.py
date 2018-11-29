import time
import paho.mqtt.client as paho
broker = "192.168.50.138"
port = 31275

def on_message(client, userdata, message):
    time.sleep(1)
    print("received message = ", str(message.payload.decode("utf8")))

client = paho.Client("client_vm")
client.on_message = on_message
print("connect to broker",broker)
client.connect(broker,port)
client.loop_start()

print("subscribing ")
client.subscribe("RPi/PM25")
while True:
    time.sleep(1)
