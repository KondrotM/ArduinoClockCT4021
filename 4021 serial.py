from time import sleep
import pyaztro
import pyowm
import serial
ser = serial.Serial('com3', 9600) # Establish the connection on a specific port

     
def sendword(word): #sends word over serial
     print(word) 
     word += '\n' #appends line break
     for i in range(1): #leftover from testing
          ser.write(str.encode(word)) #sends encoded text
          sleep(.1) #waits for processing
     if ser.in_waiting: 
          print (ser.readline()) #reads any data sent back


def sendweather():
     owm = pyowm.OWM('8b4a6ccf31ac07f598ca2f16f6195132') #API key
     location = owm.weather_at_place('Cheltenham,GB')
     weather = location.get_weather()
     wth = str(weather.get_temperature('celsius')['temp'])
     sts = weather.get_status() #describes weather
     sts +=' '+wth+'C\n' #appends deg.C and line break
     for i in range(1):
          print(sts)
          ser.write(str.encode(sts))
          if ser.in_waiting:
               print (ser.readline())

def sendhoroscope(ino):
     signs = dict() #creates dictionary for each serial input and corresponding output
     signs["b'a'"] = 'aries'
     signs["b'b'"] = 'taurus'
     signs["b'c'"] = 'gemini'
     signs["b'd'"] = 'cancer'
     signs["b'e'"] = 'leo'
     signs["b'f'"] = 'virgo'
     signs["b'g'"] = 'libra'
     signs["b'h'"] = 'scorpio'
     signs["b'i'"] = 'sagittarius'
     signs["b'j'"] = 'capricorn'
     signs["b'k'"] = 'aquarius'
     signs["b'l'"] = 'pisces'
     
     horoscope = pyaztro.Aztro(signs[ino]) #star-sign horoscope set

     horoln = horoscope.mood + ', ' + str(horoscope.lucky_number) + '\n' #creates full string to send
     print(horoln)
     ser.write(str.encode(horoln)) #sends full string to arduino

          
while True:
     if ser.in_waiting: #if there is serial data
          ino = ser.readline() #reads information from serial
          ino = str(ino.strip()) #removes line ends, converts byte into string
          print(ino)
          if ino == "b'w'":
               sendweather()
          elif ino in ["b'a'", "b'b'", "b'c'", "b'd'", "b'e'", "b'f'", "b'g'", "b'h'", "b'i'", "b'j'", "b'k'", "b'l'"]:
               sendhoroscope(ino)




