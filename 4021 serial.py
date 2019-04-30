from time import sleep
import pyowm
import serial
ser = serial.Serial('com3', 9600) # Establish the connection on a specific port



def loopsend():
     counter = 32 # Below 32 everything in ASCII is gibberish
     while True:
          counter +=1
          print("op")
          ser.write(str.encode((chr(counter)))) # Convert the decimal number to ASCII then send it to the Arduino
          sleep(.1) # Delay for one tenth of a second
          if counter == 255:
             counter = 32
     
def sendword(word):
     print(word)
     word += '\n'
     for i in range(1):
          ser.write(str.encode(word))
          sleep(.1)
     if ser.in_waiting:
          print (ser.readline())      
     #for i in range(6):
     #     ser.write(str.encode("42"))

def sendweather():
     owm = pyowm.OWM('8b4a6ccf31ac07f598ca2f16f6195132')
     location = owm.weather_at_place('Cheltenham,GB')
     weather = location.get_weather()
     wth = str(weather.get_temperature('celsius')['temp'])
     sts = weather.get_detailed_status()
     sts +=' '+wth+'C\n'
     print(sts)
     for i in range(1):
          print(sts)
          ser.write(str.encode(sts))
          if ser.in_waiting:
               print (ser.readline()) 
          
while True:
     if ser.in_waiting:
          ino = ser.readline()
          ino = str(ino.strip())
          print(ino)
          if ino == "b'w'":
               sendweather()
          elif ino == "b'h'":
               sendword("horoscope")
          elif ino == "b'!'":
               sendword("time")
          elif ino == "b's'":
               sendword("settings")
#     word = input("Input word\n")
#     sendword(word)
#sendweather("42","ops")

#i don't want to distract myself
#i want to get this over with
#so i don't have to be stressed.





