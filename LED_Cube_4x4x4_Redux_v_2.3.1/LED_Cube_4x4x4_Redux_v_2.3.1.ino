/*
  Светодиодный куб 4х4х4. Рабработал AnLi.
  Версия  2.3.1 (Redux)
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

#define MaxModes 15 //Количество эффектов
#define numOfRegisters 2 // Число сдвиговых регистров

//---------------Все переменные------------

volatile int DeltaEffectSpeed = 0; //Переменная увеличения скорости анимации
volatile int CurrentMode = 0; //Текущий режим
bool Buttflag1 = false; //Флаг нажатия кнопки 1

byte*registerState;

int EffectSpeed = 100; // Начальное значение скорости анимации
uint32_t myTimer1; // Переменная хранения времени 1

//--------------Макросы----------------
#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr += (x);\
  if (flag)

#define FOR_i(from, to) for(byte i = (from); i < (to); i++) // Макрос цикла For без переменной (i++)

//----------------------//
//--------Setup-------
//---------------------//

void setup() {
  //CurrentMode = 9; // Стартовый режим
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

  //Подключение кнопок с помощью внутренней подтяжки
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);

  //Подключаем прерывание
  attachInterrupt(digitalPinToInterrupt(Button1), Butt1Tick, FALLING);
  attachInterrupt(digitalPinToInterrupt(Button2), Butt2Tick, FALLING);

  Serial.begin(9600);
}

//---------------------------//
//-------Основной цикл--------
//---------------------------//

void loop() {

  Buttflag1 = false; //Сброс флага кнопки 1
  if (CurrentMode >= MaxModes) {
    CurrentMode = 0;
  }

  //Включение текущего режима
  switch (CurrentMode) {
    case 0:
      Serial.println("Random");
      Random(); //Зажигание случайного светодиода
      CurrentMode++;
      break;
    case 1:
      Serial.println("Light");
      Light(); //Светильник
      CurrentMode++;
      break;
    case 2:
      Serial.println("EffectA");
      EffectA(); // Бегающий огонек
      CurrentMode++;
      break;
    case 3:
      Serial.println("EffectD");
      EffectD(); // Бегающий огонек с призрачным следом
      CurrentMode++;
      break;
    case 4:
      Serial.println("EffectB");
      EffectB(); // 2 огонька с заполнением (в начале)
      CurrentMode++;
      break;
    case 5:
      Serial.println("EffectE");
      EffectE(); // 2 огонька без заполнения краёв
      CurrentMode++;
      break;
    case 6:
      Serial.println("Snake2");
      Snake2(); // Вторая змейка
      CurrentMode++;
      break;
    case 7:
      Serial.println("Layers Effect");
      Layerseffect1(); // Анимация слоёв
      CurrentMode++;
      break;
    case 8:
      Serial.println("Layers Effect 2");
      Layerseffect2(); // Анимация слоёв (1 плоскость)
      CurrentMode++;
      break;
    case 9:
      Serial.println("Blink cube");
      BlinkCube(); // Мерцающий куб
      CurrentMode++;
      break;
    case 10:
      Serial.println("Random 2");
      Random2(); // Зажигание случайных столбиков
      CurrentMode++;
      break;
    case 11:
      Serial.println("Spin");
      Spin(); // Спираль
      CurrentMode++;
      break;
    case 12:
      Serial.println("Collomns effect");
      Collomnseffect(); // Анимация колонн
      CurrentMode++;
      break;
    case 13:
      Serial.println("Crazy Fly");
      CrazyFly(); // Блуждающаяя точка
      CurrentMode++;
      break;
    case 14:
      Serial.println("Easy Rain");
      EasyRain(); // Простой дождик
      CurrentMode++;
      break;
  }

  EffectSpeed = random(0, 120) + DeltaEffectSpeed; // изменение скорости анимации
  randomSeed(analogRead(0));
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
}

// --------1.3. Бегающий огонек----------
void EffectA()
{
  for (byte i = 0; i < 3;) {
    Layers(i);
    for (byte j = 0; j < 16;) {
      regWrite(j, HIGH);

      delay(EffectSpeed);
      regWrite(j, LOW);
      j++;
    }
    i++;
  }
}

// --------1.4. 2 огонька с заполнением (в начале)------
void EffectD()
{
  for (byte i = 0; i < 3; i++) {
    Layers(i);
    for (byte i = 0; i < 8; i++) {
      for (byte k = i; k < 8;) {
        delay(EffectSpeed);
        regWrite(k, HIGH);
        regWrite(15 - k, HIGH);

        delay(2 * EffectSpeed);
        regWrite(k, LOW);
        regWrite(15 - k, LOW);
        k++;
      }
    }
    delay(EffectSpeed);
    regWrite(i, HIGH);
    regWrite(15 - i, HIGH);
  }
}

// --------1.5. Бегающий огонек с призрачным следом----------
void EffectB()
{
  FOR_i(0, 2) {
    Layers(i);
    for (byte j = 0; j < 16;) {
      for (byte k = i; k < 16;) {
        regWrite(k, HIGH);
        delay(0.25 * EffectSpeed);
        regWrite(k, LOW);

        delay(0.25 * EffectSpeed);
        regWrite(k, LOW);
        k++;
      }
      regWrite(j, HIGH);
      j++;
    }
  }
}


//  ------1.6. 2 огонька без заполнения краёв----------
void EffectE()
{
  Layers(3);
  for (int j = 15; j >= 0; j--) {
    for (int k = 0; k <= j; k++) {
      regWrite(k, HIGH);
      regWrite(15 - k, HIGH);

      delay(EffectSpeed);
      regWrite(k, LOW);
      regWrite(15 - k, LOW);
    }
  }
}

// --------1.7. Вторая змейка------------------
void Snake2()
{
  byte SnakeWay2[]  = {15, 8, 9, 14, 13, 10, 11, 12, 3, 4, 5, 2, 1, 6, 7, 0}; // Массив пути змейки

  FOR_i(0, 4) {
    Layers(i);
    for (byte j = 0; j < 18;) {
      delay(EffectSpeed);
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

//---------------1.8. Анимация слоёв----------
void Layerseffect1()
{
  for (byte i = 0; i < 16; i++) {
    regWrite(i, HIGH);
  }
  for (int i = 0; i <= 7; i++) {
    for (int j = 3; j >= 0; j--) {
      delay(2 * EffectSpeed);
      Layers(j);
    }
  }
}

//-----------------1.9. Анимация слоёв (1 плоскость)----------
void Layerseffect2()
{
  for (int i = 0; i <= 15; i++) {
    regWrite(i, HIGH);
  }
  for ( int i = 0; i <= 7; i++) {
    for (int j = 3; j >= 0; j--) {
      EVERY_MS(EffectSpeed) {
        Layers2(j);
      }
    }
  }
}

//-----------------1.10. Мерцающий куб ----------
void BlinkCube()
{
  for (int i = 0; i <= 5; i++) {
    Layers(3);
    for (int j = 0; j <= 15; j++) {
      regWrite(j, HIGH);
    }
    delay(5 * EffectSpeed);
    for (int k = 0; k <= 15; k++) {
      regWrite(k, LOW);
    }
    delay(5 * EffectSpeed);
  }
}

// -------------1.11.Зажигание случайных столбиков --------
void Random2()
{
  for (int i = 0; i < 8; i++) {
    byte LED = random(0, 15);
    for (int j = 0; j <= 3; j++) {
      Layers(j);
      regWrite(LED, HIGH);
      delay(EffectSpeed);
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
    for (int j = 0; j <= 15; j++) {
      regWrite(spin[j], HIGH);
      delay(EffectSpeed);
    }
    for (int k = 15; k >= 0; k--) {
      regWrite(spin[k], LOW);
      delay(EffectSpeed);
    }
  }
}

//--------------1.13. Анимация колонн-------------------
void Collomnseffect()
{
  Layers(3);
  FOR_i(0, 8) {

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
}

//--------------1.14. Блуждающаяя точка-------------------
void CrazyFly()
{
  byte flyway [] = {15, 9, 13, 11, 5, 6, 7, 1, 10, 3, 4, 11, 12, 13, 14, 8, 15};

  for (int i = 0; i <= 3; i++) {
    for (byte j = 0; j <= 3; j++) {
      FOR_i(0, 18) {
        regWrite(flyway[i], HIGH);
        delay(2 * EffectSpeed);
        regWrite(flyway[i], LOW);
      }
      Layers2(j);
      //EffectSpeed = random(150, 250);
    }
  }
  for ( int i = 0; i <= 15; i++) {
    regWrite(i, LOW);
  }
}

//-----------1.15. Простой дождь--------
void EasyRain()
{
  EffectSpeed = 150;
  for (byte p = 0; p < 70; p++)
  {
    // Сгенерируем случайные числа,
    // Надо это красиво в массив зарулить, а то как-то криво

    byte i = random(0, 15);
    byte j = random(0, 15);
    byte k = random(0, 15);
    byte n = random(0, 15);
    byte m = random(0, 15);

    // подадим значения на регистры
    regWrite(i, HIGH);
    regWrite(j, HIGH);
    regWrite(k, HIGH);
    regWrite(n, HIGH);
    regWrite(m, HIGH);

    // начнем её движение


    // Используй функцию layers2 просто с конца 3,2,1,0, потом дефолт

    digitalWrite(Layer_3, HIGH);
    delay(EffectSpeed);

    digitalWrite(Layer_3, LOW);
    digitalWrite(Layer_2, HIGH);
    delay(EffectSpeed);

    digitalWrite(Layer_2, LOW);
    digitalWrite(Layer_1, HIGH);
    delay(EffectSpeed);

    digitalWrite(Layer_1, LOW);
    digitalWrite(Layer_0, HIGH);
    delay(EffectSpeed);

    // уберём капли
    digitalWrite(Layer_0, LOW);
    regWrite(i, LOW);
    regWrite(j, LOW);
    regWrite(k, LOW);
    regWrite(n, LOW);
    regWrite(m, LOW);
  }
}


//--------------------------------------------------------------//
//----------------2 Функции отработки прерываний--------------
//--------------------------------------------------------------//

void Butt2Tick() {
  DeltaEffectSpeed += 10;
  if (DeltaEffectSpeed >= 120) {
    DeltaEffectSpeed = 0;
  }
}

void Butt1Tick() {
  if (!Buttflag1) {
    Buttflag1 = true;
    CurrentMode++;
    if (CurrentMode >= MaxModes) {
      CurrentMode = 0;
    }
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
