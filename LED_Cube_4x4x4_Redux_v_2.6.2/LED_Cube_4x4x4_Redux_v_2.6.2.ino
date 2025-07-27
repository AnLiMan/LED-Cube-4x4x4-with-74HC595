/*
  Светодиодный куб 4х4х4. Версия 3.0.0
*/

//-------------------Все постоянные-----------------

#define Button1 3 //Пин первой кнопки
#define Button2 2 //Пин второй кнопки

#define latchPin  10 //Синхронизирующий пин
#define clockPin  13 //Пин-защелка
#define dataPin  11 // Пин данных
#define Layer_0  9 // Пин слоя 0
#define Layer_1  8 // Пин слоя 1
#define Layer_2  7 // Пин слоя 2
#define Layer_3  6 // Пин слоя 3
#define GreenLED  A5 // Зелёный светодиод индикации
#define RedLED  A4 // Красный светодиод индикации

#define MaxModes 15 //Количество эффектов
#define numOfRegisters 2 // Число сдвиговых регистров

bool PrintingSerial = true; // Переменная вывода названия режима в консоль

//---------------Глобальные переменные------------
int CurrentMode = 0; // Текущий режим
volatile bool WorkFlag = true; // Флаг разрешения работы
volatile bool EverlastingCycleFlag = false; //Флаг для включения бесконечного режима работы
volatile bool ModeChanged = false; // Флаг смены режима

int EffectSpeed = 150; // Начальное значение скорости анимации
uint32_t myTimer1; // Переменная хранения времени
byte* registerState;

// Переменные для двойного нажатия
volatile uint32_t lastButton1Time = 0; //
volatile uint32_t lastButton2Time = 0; //
const uint32_t doubleClickTimeout = 300; //

#define FOR_i(from, to) for(byte i = (from); i < (to); i++) // Макрос цикла For

//------------------Настройка-------------------
void setup() {
  registerState = new byte[numOfRegisters];
  memset(registerState, 0, numOfRegisters);

  // Настройка пинов управления сдвиговыми регистрами
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Настройка пинов слоёв и индикации
  pinMode(Layer_0, OUTPUT);
  pinMode(Layer_1, OUTPUT);
  pinMode(Layer_2, OUTPUT);
  pinMode(Layer_3, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);

  //Подключение кнопок с помощью внутренней подтяжки
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);

  //Подключаем прерывание
  attachInterrupt(digitalPinToInterrupt(Button1), Butt1Tick, FALLING);
  attachInterrupt(digitalPinToInterrupt(Button2), Butt2Tick, FALLING);

  //Отобразим текущие режимы индикации
  digitalWrite(GreenLED, EverlastingCycleFlag);
  digitalWrite(RedLED, WorkFlag);


  clearCube(); //Очистим куб

  //Если выводим в консоль название режима
  if (PrintingSerial) {
    Serial.begin(9600);
  }

}

void loop() {
  randomSeed(analogRead(0)); //Сделаем рандом более рандомным

  if (WorkFlag) {
    if (ModeChanged) {
      ModeChanged = false;
      clearCube();
      myTimer1 = millis();
    }
    
//Включение текущего режима
    switch (CurrentMode) {
      case 0: Random(); break;
      case 1: Light(); break;
      case 2: EffectA(); break;
      case 3: MatrixRain(); break;
      case 4: Layerseffect2(); break;
      case 5: EffectB(); break;
      case 6: Collomnseffect(); break;
      case 7: Snake2(); break;
      case 8: Layerseffect1(); break;
      case 9:  EffectD(); break;
      case 10: BlinkCube(); break;
      case 11: Random2(); break;
      case 12: Spin(); break;
      case 13: EffectE(); break;
      case 14: CrazyFly(); break;
      case 15: EasyRain(); break;
    }
  } else {
    clearCube();
  }

}

//--------------------------------------------------------------//
//-----------------1. Основные режимы работы-------------
//--------------------------------------------------------------//

//-------- 1.1. Зажигание случайного светодиода----------
void Random() {
  if (PrintingSerial) {
    Serial.println("Random");
  }

  static byte counter = 0;
  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();
    clearCube();

    byte LED = random(0, 16);
    byte Layer = random(0, 4);

    Layers2(Layer);
    regWrite(LED, HIGH);

    if (!EverlastingCycleFlag && ++counter >= 70) {
      counter = 0;
      CurrentMode = (CurrentMode + 1) % MaxModes;
      ModeChanged = true;
      EffectSpeed = random(150, 500);
    }
  }
}

// ----------1.2. Светильник-------------
void Light() {
  if (PrintingSerial) {
    Serial.println("Light");
  }

  static bool state = HIGH;
  static byte counter = 0;

  if (millis() - myTimer1 >= EffectSpeed * 5) {
    myTimer1 = millis();

    Layers(3);
    for (byte i = 0; i < 16; i++) {
      regWrite(i, state);
    }

    state = !state;
    if (!EverlastingCycleFlag && ++counter >= 10) {
      counter = 0;
      CurrentMode = (CurrentMode + 1) % MaxModes;
      ModeChanged = true;
    }
  }
}

// --------1.3. Бегающий огонек----------
void EffectA() {
  if (PrintingSerial) {
    Serial.println("EffectA");
  }

  static byte i = 0, j = 0;

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    if (j == 0) Layers(i);
    regWrite(j, HIGH);

    if (j > 0) regWrite(j - 1, LOW);
    else if (i > 0) regWrite(15, LOW);

    j++;
    if (j >= 16) {
      j = 0;
      i++;
      if (i >= 3) {
        i = 0;
        EffectSpeed = random(150, 500);
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

// --------1.4. 2 огонька------
void EffectD() {
  if (PrintingSerial) {
    Serial.println("EffectD");
  }

  static byte i = 0, j = 0;

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    if (j == 0) Layers(i);
    regWrite(j, HIGH);
    regWrite(15 - j, HIGH);

    if (j > 0) {
      regWrite(j - 1, LOW);
      regWrite(16 - j, LOW);
    }

    j++;
    if (j >= 8) {
      j = 0;
      i++;
      if (i >= 4) {
        i = 0;
        EffectSpeed = random(150, 500);
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

// --------1.5. 2 огонька друг за другом----------
void EffectB() {
  if (PrintingSerial) {
    Serial.println("EffectB");
  }

  static byte i = 0, j = 0, k = 0;

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    if (j == 0 && k == 0) Layers(i);

    if (k < 16) {
      regWrite(k, HIGH);
      if (k > 0) regWrite(k - 1, LOW);
      k++;
    } else {
      j++;
      k = j;
      if (j >= 15) {
        j = 0;
        k = 0;
        i++;
        if (i >= 4) {
          i = 0;
          EffectSpeed = random(150, 500);
          if (!EverlastingCycleFlag) ModeChanged = true;
        }
      }
    }
  }
}

//  ------1.6. 2 огонька без заполнения краёв----------
void EffectE() {
  if (PrintingSerial) {
    Serial.println("EffectE");
  }

  static byte i = 0, j = 0;

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    if (j == 0) Layers(3);

    if (i < 16) {
      if (i > 0) {
        regWrite(i - 1, LOW);
        regWrite(16 - i, LOW);
      }
      regWrite(i, HIGH);
      regWrite(15 - i, HIGH);
      i++;
    } else {
      i = 0;
      j++;
      if (j >= 1) {
        j = 0;
        EffectSpeed = random(150, 500);
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

// --------1.7. Вторая змейка------------------
void Snake2() {
  if (PrintingSerial) {
    Serial.println("Snake2");
  }

  static byte i = 0, j = 0;
  byte SnakeWay2[] = {15, 8, 9, 14, 13, 10, 11, 12, 3, 4, 5, 2, 1, 6, 7, 0};

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    if (j == 0) {
      Layers(i);
      clearLayer();
    }

    if (j < 16) {
      regWrite(SnakeWay2[j], HIGH);
      j++;
    } else {
      j = 0;
      i++;
      if (i >= 4) {
        i = 0;
        EffectSpeed = random(150, 500);
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

//---------------1.8. Анимация слоёв----------
void Layerseffect1() {
  if (PrintingSerial) {
    Serial.println("Layerseffect1");
  }

  static byte i = 0, j = 3;

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    FOR_i(0, 16) regWrite(i, HIGH);
    Layers(j);

    j--;
    if (j == 255) { // Переполнение byte
      j = 3;
      i++;
      if (i >= 8) {
        i = 0;
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

//-----------------1.9. Анимация слоёв (1 плоскость)----------
void Layerseffect2() {
  if (PrintingSerial) {
    Serial.println("Layerseffect2");
  }

  static byte i = 0, j = 3;
  static bool direction = false; // false - вниз, true - вверх

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    FOR_i(0, 16) regWrite(i, HIGH);
    Layers2(j);

    if (!direction) {
      j--;
      if (j == 255) { // Переполнение byte
        j = 0;
        direction = true;
      }
    } else {
      j++;
      if (j >= 4) {
        j = 3;
        direction = false;
        i++;
        if (i >= 8) {
          i = 0;
          if (!EverlastingCycleFlag) ModeChanged = true;
        }
      }
    }
  }
}

//-----------------1.10. Мерцающий куб ----------
void BlinkCube() {
  if (PrintingSerial) {
    Serial.println("BlinkCube");
  }

  static bool state = false;
  static byte counter = 0;

  if (millis() - myTimer1 >= EffectSpeed * 5) {
    myTimer1 = millis();

    Layers(3);
    FOR_i(0, 16) regWrite(i, state);

    state = !state;
    counter++;
    if (counter >= 10) {
      counter = 0;
      if (!EverlastingCycleFlag) ModeChanged = true;
    }
  }
}

// -------------1.11.Зажигание случайных столбиков --------
void Random2() {
  if (PrintingSerial) {
    Serial.println("Random2");
  }

  static byte i = 0, j = 0;
  static byte LED = 0;

  if (millis() - myTimer1 >= EffectSpeed * 5) {
    myTimer1 = millis();

    if (j == 0) LED = random(0, 15);
    Layers2(j);
    regWrite(LED, HIGH);

    j++;
    if (j >= 4) {
      j = 0;
      i++;
      if (i >= 8) {
        i = 0;
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

// ----------------1.12.Спираль-----------------
void Spin() {
  if (PrintingSerial) {
    Serial.println("Spin");
  }

  static byte i = 0, j = 0, k = 0;
  byte spin[] = {4, 5, 6, 7, 0, 15, 8, 9, 10, 11, 12, 3, 2, 1, 14, 13};

  if (millis() - myTimer1 >= EffectSpeed * 5) {
    myTimer1 = millis();

    if (k == 0) Layers(i);

    if (j < 16) {
      if (k == 0) regWrite(spin[j], HIGH);
      else regWrite(spin[15 - j], LOW);
      j++;
    } else {
      j = 0;
      k = !k;
      if (k == 0) {
        i++;
        if (i >= 4) {
          i = 0;
          if (!EverlastingCycleFlag) ModeChanged = true;
        }
      }
    }
  }
}

//--------------1.13. Анимация колонн-------------------
void Collomnseffect() {
  if (PrintingSerial) {
    Serial.println("Collomnseffect");
  }

  static byte i = 0, step = 0;

  if (millis() - myTimer1 >= EffectSpeed) {
    myTimer1 = millis();

    Layers(3);
    clearLayer();

    switch (step) {
      case 0: FOR_i(8, 12) regWrite(i, HIGH); break;
      case 1: FOR_i(12, 16) regWrite(i, HIGH); break;
      case 2: FOR_i(0, 4) regWrite(i, HIGH); break;
      case 3: FOR_i(4, 8) regWrite(i, HIGH); break;
      case 4: FOR_i(4, 8) regWrite(i, LOW); break;
      case 5: FOR_i(0, 4) regWrite(i, LOW); break;
      case 6: FOR_i(12, 16) regWrite(i, LOW); break;
      case 7: FOR_i(8, 12) regWrite(i, LOW); break;
    }

    step++;
    if (step >= 8) {
      step = 0;
      i++;
      if (i >= 8) {
        i = 0;
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

//--------------1.14. Блуждающаяя точка-------------------
void CrazyFly() {
  if (PrintingSerial) {
    Serial.println("CrazyFly");
  }

  static byte i = 0, j = 0, k = 0;
  byte flyway[] = {15, 9, 13, 11, 5, 6, 7, 1, 10, 3, 4, 11, 12, 13, 14, 8, 15};

  if (millis() - myTimer1 >= EffectSpeed * 5) {
    myTimer1 = millis();

    if (k == 0) {
      clearCube();
      Layers2(j);
    }

    if (k < 17) {
      regWrite(flyway[k], HIGH);
      k++;
    } else {
      k = 0;
      j++;
      if (j >= 4) {
        j = 0;
        i++;
        if (i >= 4) {
          i = 0;
          EffectSpeed = random(150, 250);
          if (!EverlastingCycleFlag) ModeChanged = true;
        }
      }
    }
  }
}

//-----------1.15. Простой дождь--------
void EasyRain() {
  if (PrintingSerial) {
    Serial.println("EasyRain");
  }

  static byte i = 0, j = 0, k = 0, m = 3;
  static byte DropArray[6] = {0};

  if (millis() - myTimer1 >= EffectSpeed * 5) {
    myTimer1 = millis();

    if (j == 0 && k == 0) {
      // Сгенерируем случайные числа
      for (byte l = 0; l < 6; l++) {
        DropArray[l] = random(0, 15);
      }
    }

    if (k < 6) {
      regWrite(DropArray[k], HIGH);
      k++;
    } else if (m > 0) {
      Layers2(m);
      m--;
    } else {
      clearCube();
      i++;
      j = 0;
      k = 0;
      m = 3;

      if (i >= 11) {
        i = 0;
        if (!EverlastingCycleFlag) ModeChanged = true;
      }
    }
  }
}

//-----------1.16. Матричный дождь (как в фильме "Матрица")--------
void MatrixRain() {
  static uint32_t lastUpdate = 0;
  static byte dropPos[4] = {0};  // Позиция головы капли для каждого столбца (0-7)
  static byte dropSpeed[4] = {0}; // Скорость капли (1-3)
  static byte dropLength[4] = {0}; // Длина капли (2-4)
  static byte dropActive[4] = {0}; // Активна ли капля

  if (PrintingSerial) {
    Serial.println("MatrixRain");
  }

  // Управление скоростью анимации
  if (millis() - myTimer1 < EffectSpeed / 3) return;
  myTimer1 = millis();

  // Очистка только текущего кадра
  clearCube();

  for (byte x = 0; x < 4; x++) {
    // Создание новых капель
    if (!dropActive[x] && random(20) == 0) {
      dropActive[x] = true;
      dropPos[x] = 0;
      dropLength[x] = random(2, 5);
      dropSpeed[x] = random(1, 4);
    }

    // Движение активных капель
    if (dropActive[x] && (millis() % dropSpeed[x] == 0)) {
      dropPos[x]++;

      // Удаление капель, ушедших за пределы
      if (dropPos[x] > 4 + dropLength[x]) {
        dropActive[x] = false;
        continue;
      }
    }

    // Отрисовка капель с правильным направлением (сверху вниз)
    if (dropActive[x]) {
      for (byte y = 0; y < 4; y++) {
        // Вычисляем позицию в столбце
        int posInDrop = dropPos[x] - y;

        // Если текущий светодиод входит в каплю
        if (posInDrop >= 0 && posInDrop < dropLength[x]) {
          // Яркость уменьшается к хвосту
          byte brightness = 255 * (dropLength[x] - posInDrop) / dropLength[x];

          // Реализуем яркость через ШИМ (быстрое мигание)
          if (random(255) < brightness) {
            // Определяем этаж (Z-координата)
            byte z = 3 - y; // Разворачиваем направление (0 - низ, 3 - верх)
            Layers2(z);
            regWrite(x + (3 - y) * 4, HIGH); // Инверсия Y-координаты
          }
        }
      }
    }
  }

  // Добавляем случайные "вспышки" для эффекта
  if (random(50) == 0) {
    byte flashX = random(4);
    byte flashZ = random(4);
    Layers2(flashZ);
    for (byte y = 0; y < 4; y++) {
      regWrite(flashX + y * 4, HIGH);
    }
    delayMicroseconds(500); // Короткая вспышка
  }
}

//--------------------------------------------------------------//
//-----------------2. Обновленные прерывания---------------------
//--------------------------------------------------------------//
void Butt1Tick() {  // Кнопка 1 - вкл/выкл куба
  static uint32_t lastInterruptTime = 0;
  uint32_t now = millis();

  // Защита от дребезга
  if (now - lastInterruptTime < 50) return;
  lastInterruptTime = now;

  // Обработка нажатия
  WorkFlag = !WorkFlag;
  digitalWrite(RedLED, WorkFlag);

  if (WorkFlag) {
    ModeChanged = true; // При включении сбрасываем режим
  }
}

void Butt2Tick() {  // Кнопка 2 - управление режимами
  static uint32_t lastInterruptTime = 0;
  uint32_t now = millis();

  // Защита от дребезга
  if (now - lastInterruptTime < 50) return;
  lastInterruptTime = now;

  // Проверка на двойное нажатие
  if (now - lastButton2Time < doubleClickTimeout) {
    // Двойное нажатие - переключение зацикливания
    EverlastingCycleFlag = !EverlastingCycleFlag;
    digitalWrite(GreenLED, EverlastingCycleFlag);
    lastButton2Time = 0;
  } else {
    // Одинарное нажатие - смена режима
    CurrentMode = (CurrentMode + 1) % MaxModes;
    ModeChanged = true;
    lastButton2Time = now;

    // Автоматически включаем куб при смене режима
    if (!WorkFlag) {
      WorkFlag = true;
      digitalWrite(RedLED, WorkFlag);
    }
  }
}

//--------------------------------------------------------------//
//-----------------3. Иные вспомогательные функциии--------------
//--------------------------------------------------------------//

// Очистка всего куба
void clearCube() {
  digitalWriteFast(Layer_0, LOW);
  digitalWriteFast(Layer_1, LOW);
  digitalWriteFast(Layer_2, LOW);
  digitalWriteFast(Layer_3, LOW);
  for (byte i = 0; i < 16; i++) {
    regWrite(i, LOW);
  }
}

// Очистка текущего слоя
void clearLayer() {
  FOR_i(0, 16) {
    regWrite(i, LOW);
  }
}

//--------------3.1 Отрисовка слоёв 1 -----------
void Layers(byte layer) {
 switch (layer) {
    case 0:
      digitalWriteFast(Layer_0, HIGH);
      digitalWriteFast(Layer_1, LOW);
      digitalWriteFast(Layer_2, LOW);
      digitalWriteFast(Layer_3, LOW);
      break;
    case 1:
      digitalWriteFast(Layer_0, HIGH);
      digitalWriteFast(Layer_1, HIGH);
      digitalWriteFast(Layer_2, LOW);
      digitalWriteFast(Layer_3, LOW);
      break;
    case 2:
      digitalWriteFast(Layer_0, HIGH);
      digitalWriteFast(Layer_1, HIGH);
      digitalWriteFast(Layer_2, HIGH);
      digitalWriteFast(Layer_3, LOW);
      break;
    case 3:
      digitalWriteFast(Layer_0, HIGH);
      digitalWriteFast(Layer_1, HIGH);
      digitalWriteFast(Layer_2, HIGH);
      digitalWriteFast(Layer_3, HIGH);
      break;
    default:
      digitalWriteFast(Layer_0, LOW);
      digitalWriteFast(Layer_1, LOW);
      digitalWriteFast(Layer_2, LOW);
      digitalWriteFast(Layer_3, LOW);
      break;
  }
}

//------------3.2 Отрисовка слоёв 2------------
void Layers2(byte layer) {
   switch (layer) {
    case 0:
      digitalWriteFast(Layer_0, HIGH);
      digitalWriteFast(Layer_1, LOW);
      digitalWriteFast(Layer_2, LOW);
      digitalWriteFast(Layer_3, LOW);
      break;
    case 1:
      digitalWriteFast(Layer_0, LOW);
      digitalWriteFast(Layer_1, HIGH);
      digitalWriteFast(Layer_2, LOW);
      digitalWriteFast(Layer_3, LOW);
      break;
    case 2:
      digitalWriteFast(Layer_0, LOW);
      digitalWriteFast(Layer_1, LOW);
      digitalWriteFast(Layer_2, HIGH);
      digitalWriteFast(Layer_3, LOW);
      break;
    case 3:
      digitalWriteFast(Layer_0, LOW);
      digitalWriteFast(Layer_1, LOW);
      digitalWriteFast(Layer_2, LOW);
      digitalWriteFast(Layer_3, HIGH);
      break;
    default:
      digitalWriteFast(Layer_0, LOW);
      digitalWriteFast(Layer_1, LOW);
      digitalWriteFast(Layer_2, LOW);
      digitalWriteFast(Layer_3, LOW);
      break;
  }
}

//-----------------3.3. Отправка данных на сдвиговые регистры-----------
void regWrite(int pin, bool state) {
  byte reg = pin / 8;
  byte actualPin = pin - (8 * reg);
  digitalWriteFast(latchPin, LOW);

  for (byte i = 0; i < numOfRegisters; i++) {
    byte* states = &registerState[i];
    if (i == reg) {
      bitWrite(*states, actualPin, state);
    }
    shiftOut(dataPin, clockPin, MSBFIRST, *states);
  }
  digitalWriteFast(latchPin, HIGH);
}

//-----------------3.4.Быстрый digitalWrite-----------
void digitalWriteFast(uint8_t pin, bool x) {
  //Отключаем ШИМ на пине из списка
  switch (pin) {
    case 3: bitClear(TCCR2A, COM2B1); break;
    case 5: bitClear(TCCR0A, COM0B1); break;
    case 6: bitClear(TCCR0A, COM0A1); break;
    case 9: bitClear(TCCR1A, COM1A1); break;
    case 10: bitClear(TCCR1A, COM1B1); break;
    case 11: bitClear(TCCR2A, COM2A1); break;
  }

  // Установка пина на HIGH / LOW
  if (pin < 8) {
    bitWrite(PORTD, pin, x);
  } else if (pin < 14) {
    bitWrite(PORTB, (pin - 8), x);
  } else if (pin < 20) {
    bitWrite(PORTC, (pin - 14), x);
  }
}
