// амплитудный анализ звука

#include "VolAnalyzer.h"
VolAnalyzer analyzer(A0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (analyzer.tick()) {
    Serial.print(analyzer.pulse() * 20); // скачок громкости
    Serial.print(',');
    Serial.print(analyzer.getVol());    // громкость 0-100
    Serial.print(',');
    Serial.print(analyzer.getRaw());    // сырая величина
    Serial.print(',');
    Serial.println(analyzer.getMax());  // амплитудная огибающая
  }
}