# Universidad del Valle de Guatemala
# IE3027 - Electronica Digital 2
# Valerie Valdez
# Adafruit IO
# https://io.adafruit.com/

# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed

ADAFRUIT_IO_KEY = "aio_HrdI59MOKL1lCXYjuheSa4vurNkR"
ADAFRUIT_IO_USERNAME = "valeelorraine"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Sensor de Temperatura
digital_feed = aio.feeds('temp')
aio.send_data(digital_feed.key, 50)
digital_data1 = aio.receive(digital_feed.key)
print(f'Magnitud de temperatura: {digital_data1.value}')

# Sensor de Humedad
digital_feed1 = aio.feeds('lab05')
aio.send_data(digital_feed1.key, 100)
digital_data1 = aio.receive(digital_feed1.key)
print(f'Porcentaje de humedad: {digital_data1.value}')

    