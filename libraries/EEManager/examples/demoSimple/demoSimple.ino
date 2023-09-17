// структура для хранения данных
struct Data {
  byte valB;
  int valI;
};
Data data;  // переменная, с которой мы работаем в программе

#include <EEManager.h>  // подключаем либу
EEManager memory(data); // передаём нашу переменную (фактически её адрес)

// можно задать таймаут обновления, по умолч. 5 секунд (5000мс)
//EEManager memory(data, 1000);

void setup() {
  Serial.begin(9600);
  // для esp8266 не забудь вызвать EEPROM.begin(размер)!
  // EEPROM.begin(memory.blockSize());
  
  /*
    Запускаем менеджер, передаём:
    - Стартовый адрес в памяти для записи даты
    - Ключ хранения (0.. 255) или символ
  */
  memory.begin(0, 'b');
  // здесь данные уже прочитаны из памяти
  Serial.println(data.valB);
  Serial.println(data.valI);

  // меняем данные
  data.valB = random(100);
  data.valI = random(10000);
  memory.update();  // отложить обновление

  // вызывать update несколько раз - безопасно
  // данные запишутся только после выхода таймаута
  memory.update();
  memory.update();
  memory.update();
}

void loop() {
  // данные автоматически запишутся здесь по таймауту
  memory.tick();

 
}
