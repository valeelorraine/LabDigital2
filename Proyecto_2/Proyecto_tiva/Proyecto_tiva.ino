char Message;
int pushB1 = PUSH1;                   // El push está attached con el PUSH 1
int VAL = 0;                          // Valor del contador del primer boton
int FLAG = 0;                         // Bandera para los botones
//******************************************************************************************
//                                     S E T   U P 
//******************************************************************************************

void setup() {
  Serial.begin(9600);                           // Inicializar C. serial a 9600 bits per second
  Serial3.begin(9600);                          // Inicializar C. serial a 9600 bits per second
  Serial3.setTimeout(50);                        // T para la transf. de datos
  delay(100);
  pinMode(PB_2, OUTPUT);                        // Pin para el led
  pinMode(PE_0, OUTPUT);                        // Pin para el led
  pinMode(PF_0, OUTPUT);                        // Pin para el led
  pinMode(pushB1, INPUT_PULLUP);      // Hacer el pin un input pull up
}

//*****************************************************************************************
//                               L O O P   P R I N C I P A L
//*****************************************************************************************
void loop() {
         
  if (Serial3.available()){                      
    Message = Serial3.read();                   // Guardar lo leído en Message
    Serial.write(Message);                     // Escribir lo que se recibe
 
  delay(4);
    if(Message == 1){                          // Recibe un 1
      digitalWrite(PB_2, HIGH);                 // Se enciende el Led
      digitalWrite(PF_0, LOW);
      digitalWrite(PE_0, LOW);
  }

    if(Message == 2){                          // Recibe 0
      digitalWrite(PE_0, HIGH);                  // Se apaga el LED
      digitalWrite(PF_0, LOW);
      digitalWrite(PB_2, LOW); 
  }

   if(Message == 3){                          // Recibe 0
      digitalWrite(PF_0, HIGH);                  // Se apaga el LED
      digitalWrite(PE_0, LOW);
      digitalWrite(PB_2, LOW); 
      
  }
   }

   
}


// PARTE DEL AARDUINOOOO
//Valerie Valdez
//Universidad el Valle de Guatemala

//                  Prueba

 /*  
char Message;                 // Variable a utilizar

//******************************************************************************************
//                                     S E T   U P 
//******************************************************************************************
void setup() {
  Serial.begin(9600);         // Inicializar C. serial a 9600 bits per second
  delay(100);
}

//*****************************************************************************************
//                               L O O P   P R I N C I P A L
//*****************************************************************************************
void loop() {
  Serial.write(49);
  //Serial.println(1);         
  delay(2500);
 // Serial.println(0);
  Serial.write(48);
  delay(2500);
}
*/
