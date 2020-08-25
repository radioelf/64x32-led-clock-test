/*
Reloj en matriz de leds 64X32

Basado en: 
  Rui Santos https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  y
  Domótica Fácil con Jota en YouTube https://github.com/domoticafacilconjota/panel-led-mqtt

  Copyright (C) 2020  Radioelf http://radioelf.blogspot.com/

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
25/8/20
*/

#include <WiFi.h>
#include "time.h"
#include <SmartMatrix3.h>                                                             // Librería para controlar el panel LED

const char* ssid     = "wifi_domo";
const char* password = "C607956A25";

const char* ntpServer = "es.pool.ntp.org";                                            // hora.roa.es
const long  gmtOffset_sec = 3600;                                                     // GMT +1h
const int   daylightOffset_sec = 3600;                                                // +1h verano/invierno

#define configTIMER_TASK_STACK_DEPTH 4096         

#define GPIOPINOUT ESP32_FORUM_PINOUT_WITH_LATCH                                      // ESP32 sin controladora
#define COLOR_DEPTH 24                                                                // Profundidad de color

const uint8_t kMatrixWidth = 64;                                                      // Número de LEDs a lo ancho
const uint8_t kMatrixHeight = 32;                                                     // Número de LEDs a lo alto
const uint8_t kRefreshDepth = 36;                                                     // Profundidad para refrescar
const uint8_t kDmaBufferRows = 4;                                                     // Número de filas en el buffer DMA

// Tipo de panel
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;

//SIN opciones 
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE); 
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

// Configuramos los parámetros
SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

int defaultBrightness = (10*255) / 100;
int defaultScrollOffset = 6;


#include <clock.h> 

/*------------------------------------------------------------------------------------
Mostramos información al iniciar
------------------------------------------------------------------------------------*/
void infoIni () {
    Serial.println("Iniciando LCD 64x32 CLOCK..");
    scrollingLayer.setColor(BLANCO);
    scrollingLayer.setMode(wrapForward);
    scrollingLayer.setSpeed(60);
    scrollingLayer.setFont(font6x10);
    scrollingLayer.start("Iniciando LCD 64x32 CLOCK", -1);
    backgroundLayer.swapBuffers();
    delay(5000);
    scrollingLayer.stop();
}
/*------------------------------------------------------------------------------------
Mostramos hora
------------------------------------------------------------------------------------*/
boolean printLocalTime(boolean info){
  static boolean run = false;
  static boolean end_day = false;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("NO se pudo obtener la hora");
    return false;
  }
  if (info) {
    Serial.print("Hora: ");  Serial.print(&timeinfo, "%H");
    Serial.print(":"); Serial.print(&timeinfo, "%M");
    Serial.print(":"); Serial.println(&timeinfo, "%S");
    Serial.print("Horario ");
    timeinfo.tm_isdst ? Serial.println("verano"): Serial.println("invierno");
    if (!run) return true;
  }
  units_hh =  timeinfo.tm_hour % 10;
  dozen_hh =  timeinfo.tm_hour / 10;
  units_mm =  timeinfo.tm_min % 10;
  dozen_mm =  timeinfo.tm_min / 10;
  units_ss =  timeinfo.tm_sec % 10;
  dozen_ss =  timeinfo.tm_sec / 10;

  if (!run) {  
    updateClock(units_ss, 1, 0);
    prevUss = units_ss;
    updateClock(dozen_ss, 2, 0);
    prevDss = dozen_ss;
          
    updateClock(units_mm, 3, 0);
    prevUmm = units_mm;
    updateClock(dozen_mm, 4, 0);
    prevDmm = dozen_mm;
          
    updateClock(units_hh, 5, 0);
    prevUhh = units_hh;
    updateClock(dozen_hh, 6, 0);
    prevDhh = dozen_hh;
    run = true;      
  }

  if (units_ss != prevUss) {
    prevUss = units_ss;
    updateClock(units_ss, 1, 1); 
  }
  if (dozen_ss != prevDss) {
    prevDss = dozen_ss;
    updateClock(dozen_ss, 2, 1);
  }
  if (units_mm != prevUmm) {
    prevUmm = units_mm;
    updateClock(units_mm, 3, 1);
  }
  if (dozen_mm  != prevDmm) {
    prevDmm = dozen_mm;
    updateClock(dozen_mm, 4, 1);
  }
  if (units_hh != prevUhh) {
    prevUhh = units_hh;
    if(units_hh == 0 && end_day){                                                     // Paso de las 23 a las 00?
     updateClock(units_hh, 23, 1);
     end_day = false;
    }
    else {
     updateClock(units_hh, 5, 1);
    }
    if (timeinfo.tm_hour == 23 && end_day == false)
     end_day = true;
  }
  if (dozen_hh != prevDhh) {
    prevDhh = dozen_hh;
    updateClock(dozen_hh, 6, 1);
  }
  return true;
}
/*------------------------------------------------------------------------------------
Configuración
------------------------------------------------------------------------------------*/
void setup(){
  Serial.begin(115200);

  Serial.print("Conectando a: "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (printLocalTime(true)) {
    Serial.println("Obtección correcta de la hora.");
    // Desconectamos  
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi desconectado.");
  }
  // Configuramos las capas que posteriormente que vamos a utilizar
  matrix.addLayer(&backgroundLayer); 
  matrix.addLayer(&scrollingLayer); 
  matrix.addLayer(&indexedLayer);
  
  matrix.begin();
  // Brillo inicial
  matrix.setBrightness(defaultBrightness);
  // Configuremos el margen para los textos que se desplazan
  scrollingLayer.setOffsetFromTop(defaultScrollOffset);
  // Corrección de color
  backgroundLayer.enableColorCorrection(true);

  infoIni ();
  clockIni();
}
/*------------------------------------------------------------------------------------
Principal
------------------------------------------------------------------------------------*/
uint8_t conta =0;
void loop(){
  if (++conta == 240){
    conta =0;
    printLocalTime(true);
  }
  else {
    printLocalTime(false);
  }
  delay(250);
}


