
#include "LedControl.h"
#include <avr/io.h>
#include <avr/interrupt.h>
int DIN = 4, DIN2 = 11;
int CS =  3, CS2 = 10;
int CLK = 2, CLK2 = 9;
char ca;
int distancia = 15, dis2 = 10;
int distancia2 = 15, dis3 = 10, distancia3 = 0, distancia5 = 0;
int tiempo, tiempo2;
int velocidad[2] = {3, 3};
char serial = 0;
int dirh = 1, dirv = 1, x = 3, y = 7, p = 0, i = 0, times = 10, def = 80;
int ind1 = 0, ind2 = 0;
unsigned  int  velo1 = 10, count = 0;
int trig1 = A0, trig2 = 6, echo1 = A1, echo2 = 5;
int punt[2] = {0, 0};
int pj[2];



byte paletas[2] =
{
  B11000000,
  B00000011
};
byte player[8] = {
  B11111111,
  B11111111,
  B00100011,
  B00100011,
  B00111111,
  B00111111,
  B00111111,
  B00000000
};
byte j1[8] = {
  B00000000,
  B11111001,
  B10011001,
  B10011001,
  B10011001,
  B10011001,
  B10011111,
  B10011111
};
byte j2[8] = {

  B00000000,
  B00000000,
  B11111110,
  B11111110,
  B11111110,
  B01100000,
  B00110000,
  B00000000
};




byte win[8] = {
  B01111110,
  B01111110,
  B00100000,
  B00011000,
  B00100000,
  B01111110,
  B01111110
};
byte win2[8] = {
  B01111110,
  B00001100,
  B00011000,
  B00110000,
  B01111110,
  B00000000,
  B01111110,
  B01111110
};

byte pelota = B00010000;
byte pelota2 = B00000000;
byte dirp[2] = {1, 1};
byte punto = B10000000;
byte c = 0;

LedControl lc = LedControl(DIN, CLK, CS, 1);
LedControl lc2 = LedControl(DIN2, CLK2, CS2, 1);

void setup() {
  pj[0] = A5;
  pj[1] = A4;
  Serial.begin(9600);
  lc.shutdown(0, false);      //The MAX72XX is in power-saving mode on startup
  lc.shutdown(1, false);
  lc.setIntensity(0, 15);     // Set the brightness to maximum value
  lc.clearDisplay(0);
  lc.setIntensity(1, 15);     // Set the brightness to maximum value
  lc.clearDisplay(1);
  lc2.shutdown(0, false);      //The MAX72XX is in power-saving mode on startup
  lc2.shutdown(1, false);
  lc2.setIntensity(0, 15);     // Set the brightness to maximum value
  lc2.clearDisplay(0);
  lc2.setIntensity(1, 15);     // Set the brightness to maximum value
  lc2.clearDisplay(1);/// and clear the display
  Serial.begin(9600);
  pinMode(trig1, OUTPUT);
  /*activación del pin 9 como salida: para el pulso ultrasónico*/
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(pj[0], OUTPUT);
  pinMode(pj[1], OUTPUT);
  /*activación del pin 9 como salida: para el pulso ultrasónico*/
  pinMode(echo2, INPUT);
  posicion();
  cli();
  TCCR1A = 0;
  OCR1A = 624;//valor para 10ms
  TCCR1B = 1 << WGM12;// divide entre 256 la frecuencia
  TCCR1B |= 1 << CS12;
  TIMSK1 |= 1 << OCIE1A;//modo ctc activado
  sei();
}

void loop() {
  


}




void printByte(byte character [], int a)
{
  if (a == 0) {
    lc.setRow(0, 0, character[0]);
    lc2.setRow(0, 0, character[1]);
  }
  if (a != 0) {
    if (p == 0) {
      lc.setRow(0, a, character[0]);
    }
    if (p == 1) {
      lc2.setRow(0, a, character[0]);

    }
  }
}
void printByte2(byte character [], int a)
{
  if (p == 0) {
    lc.setRow(0, a, character[0]);
  }
  if (p == 1) {
    lc2.setRow(0, a, character[0]);
  }


}
void Limpiar() {
  if (y >= 0) {
    if (dirv == 0) {
      if (y > 1) {
        printByte(&c, y - 1);
      }


    }
    if (dirv == 1)
      printByte(&c, y + 1);

  }

}



void posicion() {//se calcula la posicion de acuerdo a las lecturas del sensor ultra sonido
  int dist;

  digitalWrite(trig1, LOW);


  /* Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);
  digitalWrite(trig1, HIGH);

  /* envío del pulso ultrasónico*/
  delayMicroseconds(5);
  tiempo = pulseIn(echo1, HIGH, 10000);

  digitalWrite(trig2, LOW);

  /* Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);

  digitalWrite(trig2, HIGH);

  /* envío del pulso ultrasónico*/
  delayMicroseconds(5);

  tiempo2 = pulseIn(echo2, HIGH, 10000);

  /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío
  del pulso ultrasónico y cuando el sensor recibe el rebote, es decir: desde que el pin 12 empieza a recibir el rebote, HIGH, hasta que
  deja de hacerlo, LOW, la longitud del pulso entrante*/
  distancia = (0.017 * tiempo);
  distancia2 = (0.017 * tiempo2);
  /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío
  del pulso ultrasónico y cuando el sensor recibe el rebote, es decir: desde que el pin 12 empieza a recibir el rebote, HIGH, hasta que
  deja de hacerlo, LOW, la longitud del pulso entrante*/

  /*fórmula para calcular la distancia obteniendo un valor entero*/
  /*Monitorización en centímetros por el monitor serial*/

  if (distancia > dis2)//me determina la direccion de las paletas
    dirp[0] = 1;

  if (distancia2 < dis3)
    dirp[0] = 0;
  if (distancia2 == dis3)
    dirp[0] = 3;
  if (distancia2 > dis3)
    dirp[1] = 0;

  if (distancia2 < dis3)
    dirp[1] = 1;

  if (distancia2 == dis3)
    dirp[1] = 3;
  if (distancia > 11)
    distancia = 12;
  if (distancia2 > 11)
    distancia2 = 12;
  if (distancia < 6)
    distancia = 6;
  if (distancia2 < 6)
    distancia2 = 6;

  for (i = 0; i < 2; i++) {//me hace la paleta de acuerdo a la posicion
    if (i == 0)
      paletas[i] = (3 << (12 - distancia));
    if (i == 1)
      paletas[i] = (3 << (distancia2 - 6));
  }







  dis2 = distancia;//ref para direccion
  dis3 = distancia2;



}
void ppelota() {//en esta funcion se calcula la posicion de la pelota y se dibuja
  int i;
  if (y == 0) {//cuando la pelota este en cero me compara 

    
    if (dirv == 1) {

      if ((pelota & paletas[p]) == pelota) {//si es igual cambia la direccion de la pelota y hace la direccion hacia arriba.

        dirh = 1 - dirh;

        dirv = 0;

        velo();
      }
      if (dirv == 1) {
        if ((pelota & paletas[p]) == 0) {//no rebota 
          if ((pelota & punto) == pelota) {//entonces compara con la variable punto si es igual aumenta el puntaje y el marcador
            punt[p]++;

            digitalWrite(pj[1 - p], LOW);
            delay(100);
            digitalWrite(pj[1 - p], HIGH);

            if (punt[p] == 9)
              ind2 = 0;

            reiniciar();
            return;

          }
        }
      }


      pelota2 = pelota | paletas[p]; //me dibuja en y=0 la posicion de la pelota y la paleta a la ves
      printByte2(&pelota2, y);
      dirv = 0;




    }

  }

 
  

  if (y > 0) {//dibuja la pelota
    Limpiar();
    printByte(&pelota, y);
  }
  if (y == 0 || (y == 1 && dirv == 0)) {//en este caso en particular se dibuja la pelota debido a que la paleta se dibuja constantemente
    Limpiar();
    pelota2 = pelota | paletas[p];
    printByte2(&pelota2, 0);

  }
  if (y == 1) {

    if (dirv == 1 && y == 1) { //rebota cuando la pelota esta en la misma posicion que la paleta
      if ((pelota & paletas[p]) == pelota) {
        dirv = 0;
        def = 120;
        velo();
      }
    }
    if (dirh == 1 && dirv == 1) {
      if ((pelota & paletas[p]) == 0) {
        if ((pelota & (paletas[p] >> 1)) == pelota) {//mira las esquinas para determinar la direccion de la pelota
          if (dirp[p] == 1) {
            dirh = 0;
            def = 120;
            ind1 = 1;

            velo();
          }
          dirv = 0;
        }
      }
    }
    if (dirh == 0 && dirv == 1) {
      if ((pelota & paletas[p]) == 0) {
        if ((pelota & (paletas[p] << 1)) == pelota) {//mira la esquina para determinar la posicion de la segunda esquina y variar la direccion
          if (dirp[p] == 0) {
            dirh = 1;
            ind1 = 1;
            velo();
          }
          dirv = 0;

        }

      }
    }





  }

  if (y == 8) {//cambia la pantalla donde se muestra la pelota
    dirv = 1;
    dirh = 1 - dirh;//debido a que esan invertidas se cambia la direccion apenas cambia la pantalla
    p = 1 - p;
    x = 7 - x;//tambien la posicion en x se cambia
    pelota2 = 0;
    for (i = 0; 8 > i; i++) {
      if ((pelota >> i) == 1) {
        pelota2 = 1 << (7 - i);//se invierte la pelota por que las pantallas estan invertidas
      }
    }
    pelota = pelota2;
    printByte(&pelota, 7);
    y = 7;

  }







  //aqui empiesa la parte de determinar la posicion de la pelota
  if (x == 7)
    dirh = 1;
  if (x == 0)
    dirh = 0;
  if (dirv == 1) { //posicion en y si drv=0 sube si no baja

    y--;
  }
  else
  {

    y++;
  }
  if (dirh == 1) {
    x--;
    pelota = pelota << 1;//mueve la pelota a la derecha
  }
  else
  {
    x++;
    pelota = pelota >> 1;//mueve la pelota a la isquierda
  }
  if (y == 2) {
    distancia3 = dis2;//para calcular la velocidad

    distancia5 = dis3;

  }


  //end if (y!=8)
}


void velo() {//calcula la velocidad 
  if (p == 0)
    velocidad[0] = (distancia - distancia3);
  if (p == 1)
    velocidad[1] = (distancia2 - distancia5);

  if (velocidad[p] < 0)
    velocidad[p] = velocidad[p] * -1;

  times = (def / (velocidad[p] * 2 + 1));//ecuacion que me determina la velocidad con respecto a la interrupcion ISR


}
void reiniciar() {//vuelve a empesar el juego re dibuja la pelota
  y = 7;



  dirh = 1 - dirh;
  dirv = 1;
  p = 1 - p;
}


void Mostrar() {//muestra el jugador que gana en pantalla
  int i;
  ind2 = 0;
  for (i = 0; i < 8; i++) {
    if ((1 - p) == 1)
      lc.setRow(0, i, j1[i]);
    else {
      lc.setRow(0, i, j2[i]);
    }

    lc2.setRow(0, i, player[i]);




  }
  delay(2000);
  lc.clearDisplay(0);
  lc2.clearDisplay(0);
  for (i = 0; i < 8; i++) {
    lc.setRow(0, i, win2[i]);

    lc2.setRow(0, i, win[i]);
  }
  delay(2000);
  lc.clearDisplay(0);
  lc2.clearDisplay(0);
  ind2 = 1;


}
ISR(TIMER1_COMPA_vect) {//interrupcion interna en este caso  por desbordamiento
  if (ind2 == 1) {
    posicion();//calcula la posicion de la paletas 
    if (y == 0) {
      if (y == 0) {


        if (dirv == 1) {

          if ((pelota & paletas[p]) == pelota) {//empuja la pelota en y=0 si la toco la mover mi paleta
            if (dirp[p] == 1)
              dirh = dirh;
            if (dirp[p] == 0)
              dirh = 1 - dirh;
            dirv = 0;
            y++;

            ppelota();//dibujo y calculo la posocion de la pelota

            velo();
          }
          if (dirv == 1) {
            if ((pelota & paletas[p]) == 0) {//punto
              if ((pelota & punto) == pelota) {
                punt[p]++;

                digitalWrite(pj[1 - p], LOW);
                delay(100);
                digitalWrite(pj[1 - p], HIGH);

                if (punt[p] == 9)
                  ind2 = 0;

                reiniciar();
                return;

              }
            }
          }

          if(y==0){
          pelota2 = pelota | paletas[p];
          printByte2(&pelota2, y);
          }




        }

      }

      pelota2 = pelota | paletas[p];
      printByte2(&pelota2, 0);
      printByte(&c, 1);
    }
    else {


      printByte(paletas, 0);//dibuja las paletas
    }





    if (count == times) {//cuenta y entra hasta que la velocidad se halla cumplido
     



      if (y == 5)
        ind1 = 0;


      ppelota();
      count = 0;
      if (ind1 = 1)
        times = times + 1;//me hace mas lenta la velocidad

    }


    count++;

  }
  if (ind2 == 0)
    Mostrar();





}



