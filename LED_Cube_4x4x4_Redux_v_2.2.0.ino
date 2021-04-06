/*
  Светодиодный куб 4х4х4. Рабработал AnLi.
  Страница проекта на GitHub:
  Схема и плата на EasyEDA: https://easyeda.com/editor#id=|5cb62fc882c94fa4b59210dbcad63457|d4bf3f2b6d364da8a2fccc2df8eb6aac
  Версия  2.2.1(Redux)
*/

#define latchPin  10 //Синхронизирующий пин
#define clockPin  13 //Пин-защелка
#define dataPin  11 // Пин данных
#define Layer_0  9 // Пин слоя 0
#define Layer_1  8 // Пин слоя 1
#define Layer_2  7 // Пин слоя 2
#define Layer_3  6 // Пин слоя 3

volatile int DeltaEffectSpeed = 950; //Переменная увеличения скорости анимации
byte ModeNumber = 0; //Текущий режим
volatile boolean Butt1Flag = 0; //Флаг нажатия на кнопку

byte numOfRegisters = 2; // Число сдвиговых регистров
byte*registerState;

int EffectSpeed = 150; // Начальное значение скорости анимации
byte CurrentMode = 0; // Текущий режим

// Макросы
#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr += (x);\
  if (flag)

#define FOR_i(from, to) for(byte i = (from); i < (to); i++) // Макрос цикла For без переменной (i++)

void setup() {
  registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < numOfRegisters; i++) {
    registerState[i] = 0;
  }
  // Настройка пинов управления сдвиговыми регистрами
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Настройка пинов слоёв
  pinMode(Layer_0, OUTPUT);
  pinMode(Layer_1, OUTPUT);
  pinMode(Layer_2, OUTPUT);
  pinMode(Layer_3, OUTPUT);
  //Подключим прерывания
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, Butt1Press, FALLING);
  attachInterrupt(1, Butt2Press, FALLING);

  Serial.begin(9600);
}

void loop() {

  if (CurrentMode >= 14) {
      CurrentMode = 0;
    }
  //Управление текущим режимом
  switch (CurrentMode) {
    case 0:
      Random(); //-------- 1. Зажигание случайного светодиода----------
    case 1:
      Light(); // ----------2. Светильник-------------
    case 2:
      EffectA(); // --------3. Бегающий огонек----------
    case 3:
      EffectB(); // --------4. Бегающий огонек с призрачным следом----------
    case 4:
      EffectD(); // --------5. 2 огонька с заполнением (в начале)------
    case 5:
      EffectE(); //  ------6. 2 огонька без заполнения краёв------
    case 6:
      Snake2(); // --------7. Вторая змейка------------------
    case 7:
      Layerseffect1(); //---------------8. Анимация слоёв----------
    case 8:
      Layerseffect2(); //---------------8. Анимация слоёв (1 плоскость)----------
    case 9:
      BlinkCube(); //-----------------9. Мерцающий куб ----------
    case 10:
      Random2(); // -------------10.Зажигание случайных столбиков --------
    case 11:
      Spin(); // ----------------11.Спираль-----------------
    case 12:
      Collomnseffect(); //--------------12. Анимация колонн-------------------
    case 13:
      CrazyFly(); //--------------13. Блуждающаяя точка-------------------
  }
  EffectSpeed = EffectSpeed + DeltaEffectSpeed; // изменение скорости анимации
  randomSeed(analogRead(0));
}

void Random() //-------- 1. Зажигание случайного светодиода----------
{
  for (byte i = 0; i < 71;) {
    byte LED = random(0, 15);
    byte Layer = random(0, 3);
    EVERY_MS(EffectSpeed) {
      Layers2(Layer);
      regWrite(LED, HIGH);
      i++;
    }
  }
  CurrentMode++;
}

void Light() // ----------2. Светильник-------------
{
  Layers(3);
  FOR_i(0, 16) {
    regWrite(i, HIGH);
  }
  delay(50 * EffectSpeed);
  CurrentMode++;
}

void EffectA() // --------3. Бегающий огонек----------
{
  for (byte i = 0; i < 4;) {
    Layers(i);
    for (byte j = 0; j < 16;) {
      regWrite(j, HIGH);

      EVERY_MS(3 * EffectSpeed) {
        regWrite(j, LOW);
        j++;
      }
    }
    i++;
  }
  CurrentMode++;
}

void EffectB() // --------4. Бегающий огонек с призрачным следом----------
{
  FOR_i(0, 4) {
    Layers(i);
    for (byte j = 0; j < 16;) {
      for (byte k = i; k < 16;) {
        regWrite(k, HIGH);
        delay(0.15 * EffectSpeed);
        regWrite(k, LOW);

        EVERY_MS(EffectSpeed) {
          regWrite(k, LOW);
          k++;
        }
      }
      regWrite(j, HIGH);
      j++;
    }
  }
  CurrentMode++;
}

void EffectD() // --------5. 2 огонька с заполнением (в начале)------
{
  for (byte i = 0; i < 3; i++) {
    Layers(i);
    for (byte i = 0; i < 8; i++) {
      for (byte k = i; k < 8;) {
        EVERY_MS(EffectSpeed) {
          regWrite(k, HIGH);
          regWrite(15 - k, HIGH);

          EVERY_MS(2 * EffectSpeed) {
            regWrite(k, LOW);
            regWrite(15 - k, LOW);
            k++;
          }
        }
      }
      EVERY_MS(EffectSpeed) {
        regWrite(i, HIGH);
        regWrite(15 - i, HIGH);
      }
    }
  }
  CurrentMode++;
}

void EffectE() //  ------6. 2 огонька без заполнения краёв----------
{
    Layers(3);
    for (byte j = 15; j >= 0; ) {
      for (byte k = 0; k <= j;) {
        regWrite(k, HIGH);
        regWrite(15 - k, HIGH);

        EVERY_MS(EffectSpeed) {
          regWrite(k, LOW);
          regWrite(15 - k, LOW);
          k++;
        }
      }
    }
  CurrentMode++;
}

void Snake2() // --------7. Вторая змейка------------------
{
  byte SnakeWay2[]  = {15, 8, 9, 14, 13, 10, 11, 12, 3, 4, 5, 2, 1, 6, 7, 0}; // Массив пути змейки
  FOR_i(0, 4) {
    Layers(i);
    for (byte j = 0; j < 18;) {
      EVERY_MS(EffectSpeed) {
        regWrite(SnakeWay2[j], HIGH);
        j++;
        if (j == 17) {
          for (byte k = 0; k < 16; k++) {
            regWrite(k, LOW);
          }
        }
      }
    }
  }
  CurrentMode++;
}

void Layerseffect1() //---------------8. Анимация слоёв----------
{
  for (byte i = 0; i < 16; i++) {
    regWrite(i, HIGH);
  }
  FOR_i(0, 7) {
    for (int j = 4; j >= 0;) {
      EVERY_MS(EffectSpeed) {
        Layers(j);
        j--;
      }
    }
  }
  CurrentMode++;
}

void Layerseffect2() //-----------------9. Анимация слоёв (1 плоскость)----------
{
  FOR_i(0, 16) {
    regWrite(i, HIGH);
  }
  FOR_i(0, 7) {
    for (int j = 3; j >= 0;) {
      EVERY_MS(EffectSpeed) {
        Layers2(j);
        j--;
      }
    }
  }
  CurrentMode++;
}

void BlinkCube() //-----------------10. Мерцающий куб ----------
{
  FOR_i(0, 5) {
    Layers(3);
    FOR_i(0, 16) {
      regWrite(i, HIGH);
    }
    delay(5 * EffectSpeed);
    FOR_i(0, 16) {
      regWrite(i, LOW);
    }
    delay(5 * EffectSpeed);
  }
  CurrentMode++;
}

void Random2() // -------------11.Зажигание случайных столбиков --------
{
  FOR_i(0, 12) {
    byte LED = random(0, 15);
    FOR_i(0, 4) {
      EVERY_MS(120) {
        Layers(i);
        regWrite(LED, HIGH);
        i++;
      }
    }
  }
  CurrentMode++;
}

void Spin() // ----------------12.Спираль-----------------
{
  byte spin [] = {4, 5, 6, 7, 0, 15, 8, 9, 10, 11, 12, 3, 2, 1, 14, 13};
  for (byte i = 0; i < 4; i++)
  {
    Layers(i);
    for (byte j = 0; i < 17; j++) {
      regWrite(spin[j], HIGH);
      delay(EffectSpeed);
    }
    for (int k = 15; k >= 0; k--) {
      regWrite(spin[k], LOW);
      delay(EffectSpeed);
    }
  }
  CurrentMode++;
}

void Collomnseffect() //--------------13. Анимация колонн-------------------
{
  FOR_i(0, 8) {
    Layers(3);

    // Сначала вперёд
    FOR_i(8, 12) {
      regWrite(i, HIGH);
    }
    delay(EffectSpeed);
    FOR_i(12, 16) {
      regWrite(i, HIGH);
    }
    delay(EffectSpeed);
    FOR_i(0, 5) {
      regWrite(i, HIGH);
    }
    delay(EffectSpeed);
    FOR_i(4, 8) {
      regWrite(i, HIGH);
    }
    delay(EffectSpeed);

    // Потом назад
    FOR_i(4, 8) {
      regWrite(i, LOW);
    }
    delay(EffectSpeed);
    FOR_i(0, 5) {
      regWrite(i, LOW);
    }
    delay(EffectSpeed);
    FOR_i(12, 16) {
      regWrite(i, LOW);
    }
    delay(EffectSpeed);
    FOR_i(8, 12) {
      regWrite(i, LOW);
    }
    delay(EffectSpeed);
  }
  CurrentMode++;
}

void CrazyFly() //--------------14. Блуждающаяя точка-------------------
{
  byte flyway [] = {15, 9, 13, 11, 5, 6, 7, 1, 10, 3, 4, 11, 12, 13, 14, 8, 15};

  FOR_i(0, 4) {
    for (byte j = 0; j < 4;) {
      FOR_i(0, 18) {
        regWrite(flyway[i], HIGH);
        delay(2 * EffectSpeed);
        regWrite(flyway[i], LOW);
      }
      Layers2(j);
      j++;
    }
  }
  CurrentMode++;
}

void Layers(byte currentLayer)
{
  switch (currentLayer) {
    case 0:
      digitalWrite(Layer_0, HIGH);
      digitalWrite(Layer_1, LOW);
      digitalWrite(Layer_2, LOW);
      digitalWrite(Layer_3, LOW);
      break;
    case 1:
      digitalWrite(Layer_0, HIGH);
      digitalWrite(Layer_1, HIGH);
      digitalWrite(Layer_2, LOW);
      digitalWrite(Layer_3, LOW);
      break;
    case 2:
      digitalWrite(Layer_0, HIGH);
      digitalWrite(Layer_1, HIGH);
      digitalWrite(Layer_2, HIGH);
      digitalWrite(Layer_3, LOW);
      break;
    case 3:
      digitalWrite(Layer_0, HIGH);
      digitalWrite(Layer_1, HIGH);
      digitalWrite(Layer_2, HIGH);
      digitalWrite(Layer_3, HIGH);
      break;
    default:
      digitalWrite(Layer_0, LOW);
      digitalWrite(Layer_1, LOW);
      digitalWrite(Layer_2, LOW);
      digitalWrite(Layer_3, LOW);
      break;
  }
}

void Layers2(byte currentLayer2)
{
  switch (currentLayer2) {
    case 0:
      digitalWrite(Layer_0, HIGH);
      digitalWrite(Layer_1, LOW);
      digitalWrite(Layer_2, LOW);
      digitalWrite(Layer_3, LOW);
      break;
    case 1:
      digitalWrite(Layer_0, LOW);
      digitalWrite(Layer_1, HIGH);
      digitalWrite(Layer_2, LOW);
      digitalWrite(Layer_3, LOW);
      break;
    case 2:
      digitalWrite(Layer_0, LOW);
      digitalWrite(Layer_1, LOW);
      digitalWrite(Layer_2, HIGH);
      digitalWrite(Layer_3, LOW);
      break;
    case 3:
      digitalWrite(Layer_0, LOW);
      digitalWrite(Layer_1, LOW);
      digitalWrite(Layer_2, LOW);
      digitalWrite(Layer_3, HIGH);
      break;
    default:
      digitalWrite(Layer_0, LOW);
      digitalWrite(Layer_1, LOW);
      digitalWrite(Layer_2, LOW);
      digitalWrite(Layer_3, LOW);
      break;
  }
}

void Butt1Press()
{
  Butt1Flag = true;
  CurrentMode++;
}

void Butt2Press(){
  DeltaEffectSpeed--;
  if (DeltaEffectSpeed >= 0) {
    DeltaEffectSpeed = 950;
  }
}

void regWrite(int pin, bool state){
  byte reg = pin / 8;
  byte actualPin = pin - (8 * reg);
  digitalWrite(latchPin, LOW);

  for (byte i = 0; i < numOfRegisters; i++) {
    byte* states = &registerState[i];
    if (i == reg) {
      bitWrite(*states, actualPin, state);
    }
    shiftOut(dataPin, clockPin, MSBFIRST, *states);
  }
  digitalWrite(latchPin, HIGH);
}
