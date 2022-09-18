# we import the Twilio client from the dependency we just installed

from twilio.rest import Client
#import configparser
#configparser.Config
# set up serial port reader to interpret arduino data
import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
print(ports)
serialInst = serial.Serial()

serialInst.baudrate = 9600
serialInst.port = ""

if serialInst.port != "":
    serialInst.open()
    packet = serialInst.readLine()
    print(packet.decode('utf'))


    # the following line needs your Twilio Account SID and Auth Token
client = Client("AC337e1f870f1ce671c5a17f77cdec2f94", "4b028e7d594418d5e74192918398281f")

# change the "from_" number to your Twilio number and the "to" number
# to the phone number you signed up for Twilio with, or upgrade your
# account to send SMS to any phone number

# this function will run every 4 hours (the arduino code is timing this)
def send_watering_msg(moisture):

    text = "Your plant needs watering: the soil is "

    # display the appropriate message based on soil moisture
    # note: moisture levels for each need to be changed at the end to reflect sensor data
    if moisture == 1:
        text += "mildly dry."
    elif moisture == 2:
        text += "moderately dry."
    else:
        text += "very dry."

    client.messages.create(to="+16047720899",
                           from_="+19014459525",
                           body=text)

