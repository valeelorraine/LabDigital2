#Valerie Valdez 19659
#Univerisidad del Valle de Guatemala

#                            Electrónica analógica

from time import sleep
from tkinter import *
import time
import sys
import serial

serEnable = False         #habilitar comunicacion serial
P = ["",""]               #array para guardar los valores
puerto = serial.Serial()
                          
puerto.baudrate=9600      #Baudrate igual que el del PIC
puerto.timeout=3          # t. espera - recibir un dato cualquiera

puerto.port='COM5'        #Puerto usado para el serial FTDI

def serialInit():         #Abrir puerto e indicarlo en consola
    puerto.open()
    print('Puerto activado')
    global serEnable      #global, modificar valor en funcion
    serEnable = True      

def sendDato():           #Para enviar los datos
    ent = 0
    ent = int(Entrada.get())
    if(ent<0 or ent>255):     #Restringir el valor de entrada
        val3.set("El valor no está en el rango")
        Entrada.delete(0,10)
    else:
        puerto.close()    # El valor si se encuentra dentro del rango
        val3.set("Valor " + str(Entrada.get()) + " enviado exitosamente")
        Entrada.delete(0,10)
        puerto.open()
        puerto.write(chr(ent).encode())


#Ventana iterable creada con TK
vent=Tk()
vent.title("PIC16F887: Comunicación serial") #titulo
vent.geometry("490x350")                      #dimension

#Elementos de la ventana
val1 = StringVar()         #variables temporales
val2 = StringVar()
val3 = StringVar()         # Las variables de abajo son los cuadros y sus caract.
tex= Label(vent,textvariable= val1,bg = "Silver",fg = "White",height = 3, width = 14)
tex2= Label(vent,textvariable= val2,bg = "Silver",fg = "White",height = 3, width = 14)
texWarn = Label(vent,textvariable = val3,fg= "Black")
bot1 = Button(vent,text = "Enviar",bg = "Steelblue",fg = "White",command = sendDato)
bot2 = Button(vent,text = "Conectar",bg = "Yellowgreen",fg = "White",command = serialInit)
ident1 = Label(vent,text = " Potenciómetro 1 :",bg = "Gray",fg = "White",width = 14)
ident2 = Label(vent,text = " Potenciómetro 2 :",bg = "Gray",fg = "White",width = 14)
ident3 = Label(vent,text = "CONTADOR : \n (Ingrese un valor entre 0 - 255)",fg = "Black")
Entrada = Entry(vent)

#Configuración de la posición de los elementos en la ventana
Entrada.place(x=170,y=130)  #En y, entre mayor el No. más abajo está
texWarn.place(x=100,y=155)
tex.place(x=118,y=203)
tex2.place(x=268,y=203)
ident1.place(x=118,y=203)
ident2.place(x=268,y=203)
ident3.place(x=150,y=92)
bot1.place(x=350,y=125)
bot2.place(x=40,y=40)

while (serEnable==False):      #Crear ventana, esperar a que se active para la
    vent.update()              #comuicación

while True:
    with puerto:               #Tomar la com serial
        for i in range(2):
            P[i]=puerto.readline().strip() #divide los dos datos enviados
        c = round((float(P[0])*5.00/255),2)#asigna los valores y los convierte en equivalente de
        d = round((float(P[1])*5.00/255),2)#voltaje a 2 décimas
        val1.set(str(c)+"V")  #imprimir el valor
        val2.set(str(d)+"V")
        time.sleep(0.01)          #0.01s de espera entre lectura
    vent.update()                 #actualiza los componentes de la pantalla

#Al cerrarse la pantalla, se cierra la COM también.
puerto.close()
print('Puerto apagado')
sys.exit(0)
