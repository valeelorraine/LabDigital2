 /* 
  Valerie Valdez                                                                  Carné: 19659
  Universidad del Valle de Guatemala                                              Sección 10
  
                                          Electrónica Digital
                         Laboratorio06 Juego de carrera con TIVA C + Energia
 */

//******************************************************************************************
//                                  V A R I A B L E S 
//******************************************************************************************

  int pushB1 = PUSH1;                   // El push está attached con el PUSH 1
  int pushB2 = PUSH2;                   // El push está attached con el PUSH 2
  int VAL = 0;                          // Valor del contador del primer boton
  int VAL2 = 0;                         // Valor del contador del segundo botón
  int FLAG = 0;                         // Bandera para los botones
  int FLAG2 = 0; 
  #define LED0 RED_LED                  // Led roja
  #define LED2 GREEN_LED                // Led verde
  int INICIO = 1;                       // Variable para el func. del semáforo
 

//******************************************************************************************
//                                     S E T   U P 
//******************************************************************************************
  
  void setup() {                        // put your setup code here, to run once:
    Serial.begin(9600);                 // Inicializar C. serial a 9600 bits per second
    pinMode(pushB1, INPUT_PULLUP);      // Hacer el pin un input pull up
    pinMode(pushB2, INPUT_PULLUP);      // Hacer el pin un input pull up
     
    pinMode(LED0, OUTPUT);              // Inicializar el pin digital como output
    pinMode(LED2,OUTPUT);               // Inicializar el pin digital como output  
    
 // LEDs para el primer push
    pinMode(PB_5, OUTPUT);
    pinMode(PB_0, OUTPUT);
    pinMode(PB_1, OUTPUT);  
    pinMode(PE_4, OUTPUT);  
    pinMode(PE_5, OUTPUT);  
    pinMode(PB_4, OUTPUT);  
    pinMode(PA_5, OUTPUT);  
    pinMode(PA_6, OUTPUT);  
    pinMode(PA_7, OUTPUT);           

// LEDs para el segundo push 
    pinMode(PB_3, OUTPUT);  
    pinMode(PC_4, OUTPUT);  
    pinMode(PC_5, OUTPUT);  
    pinMode(PC_6, OUTPUT);  
    pinMode(PC_7, OUTPUT);  
    pinMode(PD_6, OUTPUT);  
    pinMode(PD_7, OUTPUT);  
    pinMode(PA_3, OUTPUT);  
    pinMode(PA_2, OUTPUT);  
}

//*****************************************************************************************
//                               L O O P   P R I N C I P A L
//*****************************************************************************************

  void loop() {                         // put the main code here, to run repeatedly: 
  // Para los botones
    int BState1 = digitalRead(pushB1);  // Leer el estado del PUSH
    int BState2 = digitalRead(pushB2);  // Leer el estado del PUSH
    Serial.println(BState1);            // Ver estado del Push 1
    delay(1);
 // Serial.println(BState2);            // Ver estado del Push 2
 // delay(1);
 
 
 //******************************   S E M Á F O R O    *************************************
 if (INICIO == 1){ 
   digitalWrite(PA_2, LOW);
   digitalWrite(PA_7, LOW);
  if(BState1 == LOW || BState2 == LOW){ // Si se presionó el PUSH y se soltó FLAG = 1  
    INICIO = 0;                       
    digitalWrite(LED0, HIGH);           // Encender el rojo
    delay(1000);
    digitalWrite(LED0, LOW);            // Apagar rojo
    delay(250);
    digitalWrite(LED0, HIGH);           // Encender rojo y verde para hacer amarillo
    digitalWrite(LED2, HIGH);
    delay(1000);
    digitalWrite(LED0, LOW);            // Apagar rojo
    digitalWrite(LED2, LOW);            // Apagar verde
    delay(250);
    digitalWrite(LED2, HIGH);           // Encender el verde
    delay(1000);
    digitalWrite(LED2, LOW);            // Apagar verde
    delay(250);
    if ( BState1 == LOW){ 
    VAL2 = 1;
    }
    if (BState2 == LOW){ 
        VAL = 1;
    }
    }
        }
        
        
//**********   C O N T A D O R E S    p a r a     l o s     B O T O N E S   *******************
//                           P R I M E R     B O T Ó N 
     if (digitalRead(LED2) == LOW){     // Revisar la LED verde
        if (BState1 == LOW){            // Primer botón
           FLAG = 1;                    // Bandera para el antirrebote
           }
        else{
         if (FLAG == 1){                // Si la FLAG está encendida entonces apgarla
           FLAG = 0;                    // Apagar bandera
           VAL++; 
           }
         
       switch(VAL){                     // Switch case para el primer botón
       case 1:                          // Todos los LEDS apagados
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;
       
       case 2:                          // Encender el primer LED
        digitalWrite(PB_5, HIGH);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;

       case 3:                          // Encender el segundo LED
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, HIGH);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;

       case 4:                          // Encender el tercer LED
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, HIGH);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;

       case 5:                          // Encender el cuarto LED
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, HIGH);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;

       case 6:                          // Encender el quinto LED
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, HIGH);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;
       
       case 7:                          // Encender el sexto LED
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, HIGH);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;
       
       case 8:                          // Encender el séptimo LED
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, HIGH);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);
       break;
       
       case 9:                          // Encender el octavo LED y el ganador                           
        digitalWrite(PB_5, LOW);
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, HIGH);
        digitalWrite(PA_7, HIGH);
        FLAG = 0; 
        FLAG2 = 0; 
       break;

       case 10:                         // Caso para inicializar todo
         VAL = 0;                       // Inicializar contador primer botón
         VAL2 = 0;                      // Inicializar contador segundo botón
         INICIO = 1;                    // Inicializar Semáforo

//      LEDs primer botón
        digitalWrite(PB_5, LOW);        // Apagar todas las LEDs 
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);


  //    LEDS segundo botón
        digitalWrite(PB_3, LOW);        // Apagar todas las LEDs
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break; }
       delay(1);                        // Delay entre lecturas para estabilidad
       }





//*************************   S E G U N D O     B O T Ó N   *****************************

 if (BState2 == LOW){                   // Segundo botón
           FLAG2 = 1; 
           }
        else{
         if (FLAG2 == 1){               // Si la FLAG está encendida entonces apgarla
           FLAG2 = 0;  
           VAL2++; 
           }
    
       switch(VAL2){                    // Switch case para el segundo botón
       case 1:                          // Todos los LEDs apagados
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;
       
       case 2:                          // Encender el primer LED
        digitalWrite(PB_3, HIGH);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;

       case 3:                          // Encender el segundo LED
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, HIGH);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;

       case 4:                          // Encender el tercer LED
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, HIGH);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;

       case 5:                          // Encender el cuarto LED
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, HIGH);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;

       case 6:                          // Encender el quinto LED
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, HIGH);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;
       
       case 7:                          // Encender el sexto LED
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, HIGH);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;
       
       case 8:                          // Encender el séptimo LED
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, HIGH);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break;
       
       case 9:                          // Encender el octavo LED
        digitalWrite(PB_3, LOW);
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, HIGH);
        digitalWrite(PA_2, HIGH);
        FLAG = 0;                       // Apagar banderas para que el botón no funcione
        FLAG2 = 0;  
       break;

       case 10:                         // Case para inicializar todo
         VAL = 0;                       // Inicializar contador del 1er botón 
         VAL2 = 0;                      // Inicializar contador del 2do botón
         INICIO = 1;                    // Inicializar semáforo

//      LEDs primer botón
        digitalWrite(PB_5, LOW);        // Todos los LEDs apagados
        digitalWrite(PB_0, LOW);
        digitalWrite(PB_1, LOW);
        digitalWrite(PE_4, LOW);
        digitalWrite(PE_5, LOW);
        digitalWrite(PB_4, LOW);
        digitalWrite(PA_5, LOW);
        digitalWrite(PA_6, LOW);
        digitalWrite(PA_7, LOW);


  //    LEDS segundo botón
        digitalWrite(PB_3, LOW);         // Todos los LEDs apagados
        digitalWrite(PC_4, LOW);
        digitalWrite(PC_5, LOW);
        digitalWrite(PC_6, LOW);
        digitalWrite(PC_7, LOW);
        digitalWrite(PD_6, LOW);
        digitalWrite(PD_7, LOW);
        digitalWrite(PA_3, LOW);
        digitalWrite(PA_2, LOW);
       break; 
       }
       delay(1);                         // Delay entre lecturas para estabilidad
       }
         }
           }


  
