 /* 
  Valerie Valdez                                                                  Carné: 19659
  Universidad del Valle de Guatemala                                              Sección 10
  
                                          Electrónica Digital
                                  Laboratorio07 Almacenamiento SD
 */

//******************************************************************************************
//                                  Librerías
//******************************************************************************************
#include <SD.h>                         // Libreria para la SD
#include <SPI.h>                        // Librería para la SPI

File Apple;
File Google;
File Superman;

//******************************************************************************************
//                                  V A R I A B L E S 
//******************************************************************************************

  int pushB1 = PUSH1;                   // El push está attached con el PUSH 1
  int VAL = 0;                          // Valor del contador del primer boton
  int FLAG = 0;                         // Bandera para los botones
  int OPCION;                           // Variable para leer lo que se ingresa
  #define LED0 RED_LED                  // Led roja
  #define LED2 GREEN_LED                // Led verde
  
//******************************************************************************************
//                                     S E T   U P 
//******************************************************************************************
  void setup() {                        // put your setup code here, to run once:
    SPI.setModule(0);                   // Utilizar configuración 0 PARA LA SD
    Serial.begin(9600);                 // Inicializar C. serial a 9600 bits per second
    pinMode(pushB1, INPUT_PULLUP);      // Hacer el pin un input pull up  
    pinMode(LED0, OUTPUT);              // Inicializar el pin digital como output
    pinMode(LED2,OUTPUT);               // Inicializar el pin digital como output  


//                       C O M U N I C A C I Ó N   S E R I A L

  pinMode(12, OUTPUT);

  if (!SD.begin(12)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

//*****************************************************************************************
//                               L O O P   P R I N C I P A L
//*****************************************************************************************
  void loop() {                         // put the main code here, to run repeatedly: 
    int BState1 = digitalRead(pushB1);  // Leer el estado del PUSH
 
    if (BState1 == LOW){                // Primer botón
        FLAG = 1;                       // Bandera para el antirrebote
          }
    else{
        if(FLAG == 1){                  // Si la FLAG está encendida entonces apgarla
           FLAG = 0;                    // Apagar bandera
           Serial.print("M E N Ú  de   S E L E C C I Ó N   de  A R C H I V O S \n");
           Serial.print("         A L M A C E N A M I E N T O    S D          \n");
           Serial.print("\n-Ingrese el número de la opción a escoger \n");
           Serial.print("1) Logo de Apple\n");
           Serial.print("2) Logo de LEGO\n");
           Serial.print("3) Logo de Superman\n\n");
           }
        }
        
    while(Serial.available()){           // Habilitar la lectura del serial
      int OPCION = (int)Serial.read();   // Leer lo que se ingrese

     if (OPCION >= 49 && OPCION <= 51 ){ // Solo ingresar un numero entre 1 y 
      switch(OPCION){                    // Switch case para el primer botón
         case 49: 
          digitalWrite(LED0, HIGH);      // Encender el rojo
          delay(1000);
          // re-open the file for reading:
          Apple = SD.open("Apple.txt");
          if ( Apple) {
             Serial.println("Apple.txt:");

          // read from the file until there's nothing else in it:
           while ( Apple.available()) {
           Serial.write( Apple.read());
           }
          // close the file:
           Apple.close();
          
          digitalWrite(LED0, LOW);       // Apagar rojo
          delay(250);
         break;


         case 50:
          digitalWrite(LED0, HIGH);      // Encender rojo y verde para hacer amarillo
          digitalWrite(LED2, HIGH);
          delay(1000);

          Superman = SD.open("Superman.txt");
          if (Superman) {
            Serial.println("Superman.txt:");
        
            // read from the file until there's nothing else in it:
            while (Superman.available()) {
              Serial.write(Superman.read());
            }
            // close the file:
            Superman.close();
          } 
          
          digitalWrite(LED0, LOW);       // Apagar rojo
          digitalWrite(LED2, LOW);       // Apagar verde
          delay(250);
         break;

  
         case 51:
          digitalWrite(LED2, HIGH);      // Encender el verde
          delay(1000);

          Google = SD.open("Google.txt");
            if (Google) {
              Serial.println("Google.txt:");
          
              // read from the file until there's nothing else in it:
              while (Google.available()) {
                Serial.write(Google.read());
              }
              // close the file:
              Google.close();
          
          digitalWrite(LED2, LOW);       // Apagar verde
         break;
        }
      }
     }
    }
     }
      } 
     
