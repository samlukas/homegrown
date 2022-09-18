# we import the Twilio client from the dependency we just installed

from twilio.rest import Client
import json
import configparser
# set up serial port reader to interpret arduino data
import serial.tools.list_ports

config = configparser.ConfigParser()
config.read('config.ini')

#ports = serial.tools.list_ports.comports()
#print(ports)
serialInst = serial.Serial()
serialInst.baudrate = 9600
serialInst.port = config['DEFAULT']['plants']

def poll(s):
    s.open()
    s.write(b'poll\n')
    packet = s.readline()
    s.close()
    str_data = packet.decode()
    print(str_data)
    return json.loads(str_data)

data = poll(serialInst)

    # the following line needs your Twilio Account SID and Auth Token
client = Client("AC337e1f870f1ce671c5a17f77cdec2f94", "4b028e7d594418d5e74192918398281f")

# change the "from_" number to your Twilio number and the "to" number
# to the phone number you signed up for Twilio with, or upgrade your
# account to send SMS to any phone number

# this function will run every 4 hours (the arduino code is timing this)
def send_watering_msg(moisture):

    text = "Your plant needs watering: the moisture level is " + str(moisture) + "%, and the soil is "

    # display the appropriate message based on soil moisture
    # note: moisture levels for each need to be changed at the end to reflect sensor data
    if moisture > 50:
        text += "mildly dry."
    elif moisture > 25:
        text += "moderately dry."
    else:
        text += "very dry."

    client.messages.create(to="+16047720899",
                           from_="+19014459525",
                           body=text)
