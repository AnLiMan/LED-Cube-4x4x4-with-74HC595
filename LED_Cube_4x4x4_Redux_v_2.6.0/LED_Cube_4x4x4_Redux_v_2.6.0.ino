/*
  Светодиодный куб 4х4х4. Разработал AnLi.
  Версия  2.6.0 (Redux)
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

//---------------Все переменные------------

//volatile int DeltaEffectSpeed = 0; //Переменная увеличения скорости анимации
int CurrentMode = 0; //Текущий режим
volatile bool WorkFlag = true; //Флаг разрешения работы
volatile bool EverlastingCycleFlag = false; //Флаг для включения бесконечного режима работы
volatile bool Butt1Flag = false; //Флаг нажатия кнопки 1
volatile bool Butt2Flag = false; //Флаг нажатия кнопки 2


byte*registerState;

int EffectSpeed = 150; // Начальное значение скорости анимации
uint32_t myTimer1; // Переменная хранения времени 1

#define FOR_i(from, to) for(byte i = (from); i < (to); i++) // Макрос цикла For

//----------------------//
//--------Setup-------
//---------------------//

void setup() {
  CurrentMode = 0; // Стартовый режим
  registerState = new byte[numOfRegisters];
  //Сбросим значения с регистров
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

  //Serial.begin(9600);
}

//---------------------------//
//-------Основной цикл--------
//---------------------------//

void loop() {
  randomSeed(analogRead(0)); //Сделаем рандом более рандомным

  //Если вышли за предел количества режимов, сбросим в ноль
  if (CurrentMode >= MaxModes) {
    CurrentMode = 0;
  }

  //Если работа разрешена
  if (WorkFlag) {

    //Включение текущего режима
    switch (CurrentMode) {
      case 0:
        Random(); //Зажигание случайного светодиода
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500); // изменение скорости анимации
        break;
      case 1:
        Light(); //Светильник
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 2:
        EffectA(); // Бегающий огонек
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 3:
        EffectD(); //  2 огонька
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 4:
        EffectB(); // 2 огонька друг за другом
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 5:
        EffectE(); // 2 огонька без заполнения краёв
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 6:
        Snake2(); // Вторая змейка
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 7:
        Layerseffect1(); // Анимация слоёв
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 8:
        Layerseffect2(); // Анимация слоёв (1 плоскость)
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 9:
        BlinkCube(); // Мерцающий куб
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 10:
        Random2(); // Зажигание случайных столбиков
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 11:
        Spin(); // Спираль
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 12:
        Collomnseffect(); // Анимация колонн
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 13:
        CrazyFly(); // Блуждающаяя точка
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
      case 14:
        EasyRain(); // Простой дождик
        if (EverlastingCycleFlag == false) {
          CurrentMode++;
        }
        EffectSpeed = random(150, 500);
        break;
    }
  }

  //Если работа не разрешена, гасим куб
  else {
    Layers(4);
    FOR_i(0, 16) {
      regWrite(i, LOW);
    }
  }
}

//--------------------------------------------------------------//
//-----------------1. Основные режимы работы-------------
//--------------------------------------------------------------//

//-------- 1.1. Зажигание случайного светодиода----------
void Random()
{
  myTimer1 = millis();
  for (byte i = 0; i < 71;) {
    byte LED = random(0, 15);
    byte Layer = random(0, 3);

    if (millis() - myTimer1 >= EffectSpeed) {
      myTimer1 += EffectSpeed;
      Layers2(Layer);
      regWrite(LED, HIGH);
      i++;
    }
  }
  FOR_i(0, 16) {
    regWrite(i, LOW);
  }
  myTimer1 = millis();
}

// ----------1.2. Светильник-------------
void Light()
{
  Layers(3);
  for (byte i = 0; i < 1; ) {
    while (millis() - myTimer1 <= 50 * EffectSpeed) {
      FOR_i(0, 16) {
        regWrite(i, HIGH);
      }
    }
    if (millis() - myTimer1 >= 50 * EffectSpeed) {
      FOR_i(0, 16) {
        regWrite(i, LOW);
        myTimer1 = millis();
      }
    }
    i++;
  }
}

// --------1.3. Бегающий огонек----------
void EffectA()
{
  for (byte i = 0; i < 3;) {
    Layers(i);
    for (byte j = 0; j < 16;) {
      regWrite(j, HIGH);
      if (millis() - myTimer1 >= EffectSpeed) {
        myTimer1 += EffectSpeed;
        regWrite(j, LOW);
        j++;
      }
    }
    i++;
  }
}
// --------1.4. 2 огонька------
void EffectD()
{
  for (byte i = 0; i < 4; ) {
    Layers(i);
    for (byte j = 0; j < 8; j++) {
      for (byte k = j; k < 8;) {
        regWrite(k, HIGH);
        regWrite(15 - k, HIGH);
        if (millis() - myTimer1 >= EffectSpeed) {
          myTimer1 += EffectSpeed;
          regWrite(k, LOW);
          regWrite(15 - k, LOW);
          k++;
        }
      }
    }
    i++;
  }
}

// --------1.5. 2 огонька друг за другом----------
void EffectB()
{
  for (byte i = 0; i < 4; ) {
    Layers(i);
    for (byte j = 0; j < 16;) {
      for (byte k = j; k < 16;) {
        regWrite(k, HIGH);
        regWrite(k + 1, HIGH);
        if (millis() - myTimer1 >= EffectSpeed) {
          myTimer1 += EffectSpeed;
          regWrite(k, LOW);
          k++;
        }
        if (k >= 15) {
          j++;
        }
      }
      regWrite(j, HIGH);
    }
    //Проверка на бесконечность режима
    if (EverlastingCycleFlag == false) {
      i++;
    }
  }
}

//  ------1.6. 2 огонька без заполнения краёв----------
void EffectE()
{
  for (int m = 0; m < 1;) {
    Layers(3);
    for (int i = 15; i >= 0; i--) {
      for (int j = 0; j <= i;) {
        regWrite(j, HIGH);
        regWrite(15 - j, HIGH);

        if (millis() - myTimer1 >= EffectSpeed) {
          myTimer1 += EffectSpeed;
          regWrite(j, LOW);
          regWrite(15 - j, LOW);
          j++;
        }
      }
    }
    m++;
  }
}

// --------1.7. Вторая змейка------------------
void Snake2()
{
  byte SnakeWay2[]  = {15, 8, 9, 14, 13, 10, 11, 12, 3, 4, 5, 2, 1, 6, 7, 0}; // Массив пути змейки

  FOR_i(0, 4) {
    Layers(i);
    for (byte j = 0; j < 18;) {
      if (millis() - myTimer1 >= EffectSpeed) {
        myTimer1 += EffectSpeed;
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
}


//---------------1.8. Анимация слоёв----------
void Layerseffect1()
{
  FOR_i(0, 16) {
    regWrite(i, HIGH);
  }
  FOR_i(0, 8) {
    for (int j = 3; j >= 0;) {
      if (millis() - myTimer1 >= 2 * EffectSpeed) {
        myTimer1 += EffectSpeed;
        Layers(j);
        j--;
      }
    }
  }
}

//-----------------1.9. Анимация слоёв (1 плоскость)----------
void Layerseffect2()
{
  FOR_i(0, 16) {
    regWrite(i, HIGH);
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 3; j >= 0;) {
      if (millis() - myTimer1 >= EffectSpeed) {
        Layers2(j);
        j--;
        myTimer1 += EffectSpeed;
      }
    }
    for (int k = 0; k <= 3;) {
      if (millis() - myTimer1 >= EffectSpeed) {
        Layers2(k);
        k++;
        myTimer1 += EffectSpeed;
      }
    }
  }
}

//-----------------1.10. Мерцающий куб ----------
void BlinkCube()
{
  bool flag = 0;
  for (int i = 0; i <= 10;) {
    Layers(3);
    if (millis() - myTimer1 >= 5 * EffectSpeed) {
      myTimer1 += 5 * EffectSpeed;
      flag = !flag;
      i++;
      FOR_i(0, 16) {
        regWrite(i, flag);
      }
    }
  }
}

// -------------1.11.Зажигание случайных столбиков --------
void Random2()
{
  FOR_i(0, 8) {
    byte LED = random(0, 15);
    for (int j = 0; j <= 3;) {
      if (millis() - myTimer1 >= 5 * EffectSpeed) {
        myTimer1 += EffectSpeed;
        Layers(j);
        regWrite(LED, HIGH);
        j++;
      }
    }
  }
}

// ----------------1.12.Спираль-----------------
void Spin()
{
  byte spin [] = {4, 5, 6, 7, 0, 15, 8, 9, 10, 11, 12, 3, 2, 1, 14, 13};
  for (int i = 0; i <= 3; i++)
  {
    Layers(i);
    for (int j = 0; j <= 15;) {
      if (millis() - myTimer1 >= 5 * EffectSpeed) {
        myTimer1 += EffectSpeed;
        regWrite(spin[j], HIGH);
        j++;
      }
    }
    for (int k = 15; k >= 0;) {
      if (millis() - myTimer1 >= 5 * EffectSpeed) {
        myTimer1 += EffectSpeed;
        regWrite(spin[k], LOW);
        k--;
      }
    }
  }
}

//--------------1.13. Анимация колонн-------------------
void Collomnseffect()
{
  Layers(3);
  FOR_i(0, 8) {

    // Сначала вперёд
    FOR_i(7, 12) {
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
    FOR_i(7, 12) {
      regWrite(i, LOW);
    }
    delay(EffectSpeed);
  }
}

//--------------1.14. Блуждающаяя точка-------------------
void CrazyFly()
{
  byte flyway [] = {15, 9, 13, 11, 5, 6, 7, 1, 10, 3, 4, 11, 12, 13, 14, 8, 15};

  FOR_i(0, 4) {
    for (int j = 0; j < 4;) {
      for (int k = 0; k < 18;) {
        regWrite(flyway[k], HIGH);
        if (millis() - myTimer1 >= 5 * EffectSpeed) {
          myTimer1 += EffectSpeed;
          regWrite(flyway[k], LOW);
          k++;
          if (k == 17)
          {
            j++;
          }
        }
        Layers2(j);
        EffectSpeed = random(150, 250);
      }
    }
    FOR_i(0, 16) {
      regWrite(i, LOW);
    }
  }
}

//-----------1.15. Простой дождь--------
void EasyRain()
{
  byte DropArray[5]; //Массив для случайных чисел

  FOR_i(0, 11) {
    // Сгенерируем случайные числа,
    for (int j = 0; j < 6; j++) {
      byte value = random(0, 15);
      DropArray[j] = value;
    }

    // Подадим значения на регистры
    for (int k = 0; k < 6; k++) {
      regWrite(DropArray[k], HIGH);
    }

    // Начнем движение капель
    for (int m = 3; m <= 0; ) {
      if (millis() - myTimer1 >= 5 * EffectSpeed) {
        Layers2(m);
        m++;
        myTimer1 += 5 * EffectSpeed;
      }
    }

    // уберём капли
    Layers2(4);
    for (int l = 0; l < 6; l++) {
      regWrite(DropArray[l], LOW);
    }
  }
}


//--------------------------------------------------------------//
//----------------2 Функции отработки прерываний--------------
//--------------------------------------------------------------//

void Butt2Tick() {
  Butt2Flag = false;
  if (!Butt2Flag) {
    Butt2Flag = true;
    EverlastingCycleFlag = !EverlastingCycleFlag;
    digitalWrite(GreenLED, EverlastingCycleFlag);
  }
}

void Butt1Tick() {
  Butt1Flag = false;
  if (!Butt1Flag) {
    Butt1Flag = true;
    WorkFlag = !WorkFlag;
    digitalWrite(RedLED, WorkFlag);
  }
}


//--------------------------------------------------------------//
//-----------------3. Иные вспомогательные функциии--------------
//--------------------------------------------------------------//

//--------------3.1 Отрисовка слоёв 1 -----------
void Layers(byte currentLayer)
{
  switch (currentLayer) {
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
void Layers2(byte currentLayer2)
{
  switch (currentLayer2) {
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
    case 3: bitClear(TCCR2A, COM2B1);
      break;
    case 5: bitClear(TCCR0A, COM0B1);
      break;
    case 6: bitClear(TCCR0A, COM0A1);
      break;
    case 9: bitClear(TCCR1A, COM1A1);
      break;
    case 10: bitClear(TCCR1A, COM1B1);
      break;
    case 11: bitClear(TCCR2A, COM2A1);
      break;
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
