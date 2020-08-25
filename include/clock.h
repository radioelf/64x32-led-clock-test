/*
 Reloj digitos de 15x8 pixeles, para panel leds 64X32

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLOCK_H
#define CLOCK_H

#define NEGRO           {0x00,0x00,0x00}
#define BLANCO          {0xFF,0xFF,0xFF}
#define VERDE           {0x00,0xFF,0x00}

uint8_t dozen_hh;
uint8_t units_hh;
uint8_t dozen_mm;
uint8_t units_mm;
uint8_t dozen_ss;
uint8_t units_ss;

uint8_t prevDhh;
uint8_t prevUhh;
uint8_t prevDmm;
uint8_t prevUmm;
uint8_t prevDss;
uint8_t prevUss;

uint8_t animSpeed = 30;

/*------------------------------------------------------------------------------------
Iniciamos reloj
------------------------------------------------------------------------------------*/
void clockIni() {
  backgroundLayer.fillScreen(NEGRO);                                                  // Borramos
  
  backgroundLayer.drawPixel(21, 13, VERDE);                                           // Puntos separación reloj
  backgroundLayer.drawPixel(21, 14, VERDE);
  backgroundLayer.drawPixel(21, 18, VERDE);
  backgroundLayer.drawPixel(21, 19, VERDE);
  
  backgroundLayer.drawPixel(22, 13, VERDE);
  backgroundLayer.drawPixel(22, 14, VERDE);
  backgroundLayer.drawPixel(22, 18, VERDE);
  backgroundLayer.drawPixel(22, 19, VERDE);
  
  backgroundLayer.drawPixel(42, 13, VERDE);
  backgroundLayer.drawPixel(42, 14, VERDE);
  backgroundLayer.drawPixel(42, 18, VERDE);
  backgroundLayer.drawPixel(42, 19, VERDE);
  
  backgroundLayer.drawPixel(43, 13, VERDE);
  backgroundLayer.drawPixel(43, 14, VERDE);
  backgroundLayer.drawPixel(43, 18, VERDE);
  backgroundLayer.drawPixel(43, 19, VERDE);
  backgroundLayer.swapBuffers();                                                      // Escribimos
}

/*------------------------------------------------------------------------------------
Obtenemos la posición inicial (abajo-derecha) del digito según su posición en el reloj 
------------------------------------------------------------------------------------*/
uint8_t posDigit(uint8_t digit) {
  switch (digit) {
    case 1:                                                                           // Unidades segundos
      return 61;
    case 2:                                                                           // Decenas segundos
      return 52;
    case 3:                                                                           // Unidades minutos
      return 40;
    case 4:                                                                           // Decenas minutos
      return 31;
    case 5:                                                                           // Unidades horas
      return 19;
    case 6:                                                                           // Decenas horas
      return 10;
  }
  return 61;
}
/*------------------------------------------------------------------------------------
Borramos toda la area (15x8) del digito indicado
------------------------------------------------------------------------------------*/
void clearDigit(uint8_t digit) {
  for (uint8_t z = 0; z < 8; z++) {
    for (uint8_t i = 0; i < 15; i++) {
      backgroundLayer.drawPixel(digit, 23 - i, NEGRO);                                  // Borramos columna
    }
    digit--;
  }
} 
/*------------------------------------------------------------------------------------
Digito 1, paso del 1 a 2
------------------------------------------------------------------------------------*/
void one (uint8_t digit, boolean morph) {
  uint8_t i;
  if (morph){
    for (uint8_t z = 0; z < 7; z++) {
      for (uint8_t i = 0; i < 15; i++) {
        if (z == 6) {
          if (i < 6) backgroundLayer.drawPixel(digit - 7, 22 - i, BLANCO);
        }
        else {
          backgroundLayer.drawPixel(digit - (z + 1), 23 - i, BLANCO);
        }
        if (z == 0) {
         if (i < 6) backgroundLayer.drawPixel(digit - z, 22 - i, NEGRO);
        } 
        else{
          if (i != 6  && i != 13) backgroundLayer.drawPixel(digit - z, 22 - i, NEGRO); 
        }
      }
      backgroundLayer.swapBuffers();
      if (z != 6) delay (animSpeed);
    }
  }
  else {
    clearDigit (digit);
    for (i = 0; i < 13; i++) {
      if (i != 6) backgroundLayer.drawPixel(digit, 22 - i, BLANCO);
    }
    backgroundLayer.swapBuffers(); 
  }
}
/*------------------------------------------------------------------------------------
Digito 2, paso del 2 al 3
------------------------------------------------------------------------------------*/
void two (uint8_t digit, boolean morph) {
  uint8_t i;
  if (morph){                                                                                                                                
    for (uint8_t i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit - 7, 22-i, BLANCO); 
    }
    backgroundLayer.drawPixel(digit - 7, 16, NEGRO);
    backgroundLayer.swapBuffers();
    delay (animSpeed);
    for (uint8_t z = 0; z < 7; z++) {
      for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit - (6 - z), 22-i, BLANCO); 
      backgroundLayer.drawPixel(digit - (7 - z), 22-i, NEGRO);
      }
      backgroundLayer.swapBuffers();
      if (z != 6) delay (animSpeed);
    }
  }
  else {
    clearDigit (digit);
    for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit, 15 - i, BLANCO);    
    }
    for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit - 7, 22 - i, BLANCO); 
    }
    for (uint8_t z = 9; z < 24; z += 7) {
      for (i = 1; i < 7; i++) {     
      backgroundLayer.drawPixel(digit - i, z, BLANCO); 
      }
    }
    backgroundLayer.swapBuffers();
  }
}
/*------------------------------------------------------------------------------------
Digito 3, paso del 3 al 4
------------------------------------------------------------------------------------*/
void three (uint8_t digit, boolean morph) {    
  if (morph){ 
    for (uint8_t z = 0; z < 6; z++) {
      backgroundLayer.drawPixel(digit - 7, 10 + z, BLANCO); 
      backgroundLayer.drawPixel(digit - (1 + z), 9, NEGRO);
      backgroundLayer.drawPixel(digit - (6 - z), 23, NEGRO);
      if (z == 5) backgroundLayer.drawPixel(digit, 16, NEGRO);
      backgroundLayer.swapBuffers();
      if (z != 5) delay (animSpeed);
    }
  }
  else {
    int8_t i ;
    clearDigit (digit);
    for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit, 15 - i, BLANCO); 
    }
    for (i = 0; i < 7; i++) {
      backgroundLayer.drawPixel(digit, 22 - i, BLANCO); 
    }
    for (uint8_t z = 9; z < 24; z += 7) {
      for (i = 1; i < 7; i++) {
        backgroundLayer.drawPixel(digit - i, z, BLANCO); 
      }
    }
    backgroundLayer.swapBuffers();
  }                  
}
/*------------------------------------------------------------------------------------
Digito 4, paso del 4 al 5
------------------------------------------------------------------------------------*/
void four (uint8_t digit, boolean morph) {
  if (morph) { 
    for (uint8_t z = 0; z < 6; z++) {
      backgroundLayer.drawPixel(digit -(1 + z), 9, BLANCO); 
      backgroundLayer.drawPixel(digit - (1 + z), 23, BLANCO); 
      if (z != 0) backgroundLayer.drawPixel(digit, 9 + z, NEGRO);  
      backgroundLayer.swapBuffers();
      delay (animSpeed);
    }
    backgroundLayer.drawPixel(digit, 15, NEGRO);
    backgroundLayer.swapBuffers();
  }
  else {
    uint8_t i;
    clearDigit (digit);
    for (i = 1; i < 14; i++) {
      if (i != 7) backgroundLayer.drawPixel(digit, 23 - i, BLANCO);
    }
    for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit - 7, 15 - i, BLANCO); 
    }
    for (i = 1; i < 7; i++) {
      backgroundLayer.drawPixel(digit - i, 16, BLANCO); 
    }
    backgroundLayer.swapBuffers();
  }                  
}
/*------------------------------------------------------------------------------------
Digito 5, paso del 5 al 6
------------------------------------------------------------------------------------*/
void five (uint8_t digit, boolean morph) {
  uint8_t i;
  if (morph){ 
    for (uint8_t z = 0; z < 7; z++) {
      for (i = 0; i < 6; i++) {
        backgroundLayer.drawPixel(digit - (1 + z), 22 - i, BLANCO); 
        if (z != 0) backgroundLayer.drawPixel(digit - z, 22 - i, NEGRO);
      }
      backgroundLayer.swapBuffers();
      if (z != 6) delay (animSpeed);
    }
  }
  else {
    clearDigit (digit);
    for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit - 7, 15 - i, BLANCO); // vertical
    }
    for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit, 22 - i, BLANCO); // vertical
    }
    for (uint8_t z = 9; z < 24; z += 7) {
      for (i = 1; i < 7; i++) {
        backgroundLayer.drawPixel(digit - i, z, BLANCO);
      }
    }
    backgroundLayer.swapBuffers();
  }                  
}
/*------------------------------------------------------------------------------------
Digito 6, paso del 6 al 7
------------------------------------------------------------------------------------*/
void six (uint8_t digit, boolean morph) {
  uint8_t i;
  if (morph){ 
    for (uint8_t z = 0; z < 7; z++) {
      for (i = 1; i < 14; i++) {
        if (i != 7) backgroundLayer.drawPixel(digit - (6 - z), 23 - i, BLANCO);
          backgroundLayer.drawPixel(digit - (7 - z), 24 - i, NEGRO); 
          backgroundLayer.drawPixel(digit - (7 - z), 10, NEGRO); 
      }
      backgroundLayer.swapBuffers();
      if (z != 6) delay (animSpeed);
    }
  }
  else {
    clearDigit (digit);
    for (i = 1; i < 14; i++) {
      if (i != 7) backgroundLayer.drawPixel(digit - 7, 23 - i, BLANCO); 
    }
    for (i = 0; i < 6; i++) {
      backgroundLayer.drawPixel(digit, 22 - i, BLANCO); 
    }
    for (uint8_t z = 9; z < 24; z += 7) {
      for (i = 1; i < 7; i++) {
        backgroundLayer.drawPixel(digit - i, z, BLANCO); 
      }
    }
    backgroundLayer.swapBuffers();
  }                  
}
/*------------------------------------------------------------------------------------
Digito 7, paso del 7 al 8
------------------------------------------------------------------------------------*/
void seven (uint8_t digit, boolean morph) {
  uint8_t i;
  if (morph){ 
    for (uint8_t z = 0; z < 7; z++) {
      for (uint8_t i = 0; i < 14; i++) {
        if (z ==6){
          if (i != 0 && i !=7) backgroundLayer.drawPixel(digit- 7, 23 - i, BLANCO);
        }
        else {
          backgroundLayer.drawPixel(digit - (1 + z), 23 - i, BLANCO);
        }
        if (z !=0){
          if (i != 6 && i !=13) backgroundLayer.drawPixel(digit - z, 22 - i, NEGRO); 
        }
    }
    backgroundLayer.swapBuffers();
    if (z != 6) delay (animSpeed);
    }
  }
  else {
    clearDigit (digit);
    for (i = 1; i < 14; i++) {
      if (i != 7) backgroundLayer.drawPixel(digit, 23 - i, BLANCO); 
    }
    for (i = 1; i < 7; i++) {
      backgroundLayer.drawPixel(digit - i, 9, BLANCO); 
    }
    backgroundLayer.swapBuffers();
  }                  
}
/*------------------------------------------------------------------------------------
Digito 8, paso del 8 al 9
------------------------------------------------------------------------------------*/
void eigt (uint8_t digit, boolean morph) {
    uint8_t i;
    if (morph){  
      for (uint8_t z = 0; z < 6; z++) {
        for (i = 0; i < 6; i++) {
          backgroundLayer.drawPixel(digit - (6 - z), 23 - i, BLANCO);
          if (z == 0) {
            backgroundLayer.drawPixel(digit - 7, 22 - i, NEGRO);
          }
          else {
            backgroundLayer.drawPixel(digit - (7 - z), 23 - i, NEGRO);  
          } 
        }
        backgroundLayer.swapBuffers();
        delay (animSpeed);
      }
      for (i = 0; i < 7; i++) {
        backgroundLayer.drawPixel(digit - 1, 23 - i, NEGRO);   
      }
      backgroundLayer.swapBuffers();
    }
    else {
      clearDigit (digit);
      for (i = 1; i < 14; i++) {
        if (i != 7){
          backgroundLayer.drawPixel(digit, 23 - i, BLANCO);
          backgroundLayer.drawPixel(digit - 7, 23 - i, BLANCO);
        }  
      }
      for (i = 1; i < 7; i++) {
        backgroundLayer.drawPixel(digit - i, 9, BLANCO); 
        backgroundLayer.drawPixel(digit - i, 16, BLANCO); 
        backgroundLayer.drawPixel(digit - i, 23, BLANCO); 
      }
      backgroundLayer.swapBuffers();
    }                  
}
/*------------------------------------------------------------------------------------
Digito 9, paso del 9 al 0
------------------------------------------------------------------------------------*/
void nine (uint8_t digit, boolean morph) {
    uint8_t i;
    if (morph){ 
      for (uint8_t z = 0; z < 6; z++) {
        for ( i = 0; i < 6; i++) {
          backgroundLayer.drawPixel((digit - 7), 17 + z, BLANCO); 
          backgroundLayer.drawPixel((digit - 1) - z, 23, BLANCO);
          backgroundLayer.drawPixel((digit - 1) - z, 16, NEGRO);
        }
        backgroundLayer.swapBuffers();
        delay (animSpeed);
      }
    }
    else {
      clearDigit (digit);
      for (uint8_t i = 1; i < 14; i++) {
        if (i != 7) backgroundLayer.drawPixel(digit, 23 - i, BLANCO); 
      }
      for (i = 1; i < 7; i++) {
        backgroundLayer.drawPixel(digit - 7, 16 - i, BLANCO); 
        backgroundLayer.drawPixel(digit - i, 9, BLANCO); 
        backgroundLayer.drawPixel(digit - i, 16, BLANCO); 
      }
      backgroundLayer.swapBuffers();
    }                  
}
/*------------------------------------------------------------------------------------
Digito 0, paso del 0 al 1
------------------------------------------------------------------------------------*/
void cero (uint8_t digit, boolean morph) {
    uint8_t i;
    if (morph){ 
      for (uint8_t z = 0; z < 7; z++) {
        for (i = 0; i < 15; i++) {
          if (i != 7) backgroundLayer.drawPixel(digit - (6 - z), 23 - i, BLANCO);  
           backgroundLayer.drawPixel(digit - (7 - z), 23 - i, NEGRO);
        }
        if (z ==6){
          backgroundLayer.drawPixel(digit, 23, NEGRO);
          backgroundLayer.drawPixel(digit, 9, NEGRO);
        }
        backgroundLayer.swapBuffers();
        if (z !=6) delay (animSpeed);
      }
    }
    else {
      clearDigit (digit);
      for (i = 1; i < 14; i++) {
        if (i != 7) backgroundLayer.drawPixel(digit, 23 - i, BLANCO);
        if (i != 7) backgroundLayer.drawPixel(digit - 7, 23 - i, BLANCO);
      }
      for (i = 1; i < 7; i++) {
        backgroundLayer.drawPixel(digit - i, 9, BLANCO); 
        backgroundLayer.drawPixel(digit - i, 23, BLANCO); 
      }
      backgroundLayer.swapBuffers();
    }                  
}
/*------------------------------------------------------------------------------------
Paso del 2 al 0 para decenas de la hora
------------------------------------------------------------------------------------*/
void twoCero() {
  for (uint8_t z = 0; z < 7; z++) {
    for (uint8_t i = 1; i < 14; i++) {
      if (i != 7) {
        if (i <7) {
          backgroundLayer.drawPixel(4 + z, 23 - i, BLANCO);
          if (z !=0) backgroundLayer.drawPixel(3 + z, 23 - i, NEGRO);
        }
        else {
          backgroundLayer.drawPixel(9 - z, 23 - i, BLANCO);
          if (z !=0) backgroundLayer.drawPixel(10 - z, 23 - i, NEGRO);
        }
      }
      else {
        if (z < 3){
          backgroundLayer.drawPixel(9 - z, 16, NEGRO);
          backgroundLayer.drawPixel(4 + z, 16, NEGRO);
        }
      }
    }
    backgroundLayer.swapBuffers();
    if (z !=6) delay (animSpeed);
  }
}
/*------------------------------------------------------------------------------------
Paso del 3 al cero para decenas de la hora
------------------------------------------------------------------------------------*/
void threeCero() {
  for (uint8_t z = 0; z < 7; z++) {
    for (uint8_t i = 1; i < 14; i++) {
      if (i != 7) {
        backgroundLayer.drawPixel(18 - z, 23 - i, BLANCO);
        if (z !=0) backgroundLayer.drawPixel(19 - z, 23 - i, NEGRO);
      }
      else {
        backgroundLayer.drawPixel(18 - z, 16, NEGRO);
      }
    }
    backgroundLayer.swapBuffers();
    if (z !=6) delay (animSpeed);
  }
}
/*------------------------------------------------------------------------------------
Paso del 5 al cero para unidades de minutos y segundos
------------------------------------------------------------------------------------*/
void fiveCero(uint8_t digit) {
    for (uint8_t z = 0; z < 7; z++) {
      for (uint8_t i = 1; i < 14; i++) {
        if (i != 7) {
          if (i > 7) {
            backgroundLayer.drawPixel((digit - 6) + z, 23 - i, BLANCO);
            if (z !=0) backgroundLayer.drawPixel((digit - 7) + z, 23 - i, NEGRO);
          }
          else {
            backgroundLayer.drawPixel((digit - 1)- z, 23 - i, BLANCO);
            if (z !=0) backgroundLayer.drawPixel(digit - z, 23 - i, NEGRO);
          }
        }
        else {
          if (z < 3){
            backgroundLayer.drawPixel((digit - 6) + z, 16, NEGRO);
          backgroundLayer.drawPixel((digit - 1) - z, 16, NEGRO);
          }
        }
      }
      backgroundLayer.swapBuffers();
      if (z !=6) delay (animSpeed);
  }
}
/*------------------------------------------------------------------------------------
Actualizamos nuevo valor
------------------------------------------------------------------------------------*/
void updateClock(byte newValue, uint8_t p, boolean morph) {
  if (!morph) newValue ++;
  if (p != 23) p = posDigit(p);	                                                      // 1-6 (23->paso de las 23 a las 00)							
  switch (newValue) {
    case 1: cero(p, morph); break;                                                    // 0->1
    case 2: one(p, morph); break;                                                     // 1->2                                        
    case 3: two(p, morph); break;                                                     // 2->3 
    case 4: three(p, morph); break;                                                   // 3->4
    case 5: four(p, morph); break;                                                    // 4->5
    case 6: five(p, morph); break;                                                    // 5->6
    case 7: six(p, morph); break;                                                     // 6->7                                             
    case 8: seven(p, morph); break;                                                   // 7->8
    case 9: eigt(p, morph); break;                                                    // 8->9
    case 0: 
      if (p == 10) {twoCero(); break;}                                                // 2->0                   
      if (p == 23) {threeCero(); break;}                                              // 3->0  
      if (p == 31 || p == 52){fiveCero(p); break;}                                    // 5->0                                                
      nine(p, morph); break;                                                          // 9->0                                                              
    case 10:   
      nine(p, 0); break;                                                              // 9 
  }
}
#endif
    
