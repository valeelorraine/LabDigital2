# Universidad del Valle de Guatemala
# IE3027 - Electronica Digital 2
# Valerie Valdez
# Adafruit IO
# https://io.adafruit.com/

# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed
import serial as ser1                       # Para el serial PORT
import time

ADAFRUIT_IO_KEY = "aio_ambl30U6qvqJbkLg4WDfxYIXN1r3"
ADAFRUIT_IO_USERNAME = "valeelorraine"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
ser = ser1.Serial('COM5', baudrate = 9600, timeout = 1)
time.sleep(1)                                # Delay

# Sensor de Temperatura
digital_feed = aio.feeds('temp')             # Temperatura
aio.send_data(digital_feed.key, 50)          # Valor a enviar
digital_data1 = aio.receive(digital_feed.key)
print(f'Magnitud de temperatura: {digital_data1.value}')

# Sensor de Humedad
digital_feed1 = aio.feeds('lab05')
aio.send_data(digital_feed1.key, 100)
digital_data1 = aio.receive(digital_feed1.key)
print(f'Porcentaje de humedad: {digital_data1.value}')

# Enviar valor del contador
def send():
    command = b'\xff'                        # Valor para empezar a recibir
    ser.write(command)  
    time.sleep(0.01)
    ser.write(cen1)                          # Enviar centenas
    time.sleep(0.01)
    ser.write(dec1)                          # Enviar decenas
    time.sleep(0.01)
    ser.write(un1)                           # Enviar unidades

# Recibir valor del contador
def obtvalor():
    command = b'\x00'                        # Valor para empezar a recibir
    ser.write(command)                       # Enviar comando de lectura del cont
    cen = ser.readline().decode('ascii')     # Recibir centenas en ASCII
    dec = ser.readline().decode('ascii')     # Recibir decenas en ASCII
    un = ser.readline().decode('ascii')      # Recibir unidades en ASCII
    time.sleep(0.01)
    CONTADOR = cen + dec + un                # Unir numeros y enviar a Adafruit
    return CONTADOR

# Estoooo para que es
digitalfeed2 = aio.feeds("interno")
digitalfeed3 = aio.feeds("contador")

while(1):
    data = obtvalor()
    aio.send_data(digitalfeed2.key, data)
    cont1data = aio.receive(digitalfeed3.key) # Recibir valor de Adafruit

    numcont = cont1data.value                 # Separar valor de adafruit en carct.
    cen1 = numcont[0:1].encode('ascii', 'ignore') # Tomar en cuenta los ASCII
    dec1 = numcont[1:2].encode('ascii', 'ignore')
    un1 = numcont[2:3].encode('ascii', 'ignore')
    send()                                   # Enviar valor del contador de Adafruit
    print(numcont)
    print(cen1)
    print(dec1)
    print(un1)
    print(data)
    
    time.sleep(0.01)
    




















