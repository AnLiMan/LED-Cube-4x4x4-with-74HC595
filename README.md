# LED-Cube-4x4x4-with-74HC595
Схема светодиодного куба 4х4х4, под управлением Arduino Nano, со сдвиговыми регистрами 74HC595.  
64 светодиода, 16 режимов анимации (можно дописать свои). Куб имеет две кнопки, обеспечивающие 
Куб использует динамическую индикацию (послойная отрисовка) и потребляет всего около 0.12 А, 
когда светятся все 64 светодиода. 
Куб питается постоянным током напряжения 5В. Аноды светодиодов (колонны) подключаются 
к Р1 - Р16. Катоды (слои) подключаются к Р100 - Р103. 

Репозиторий содержит Gerber-файл немного усовершенстваной и исправленной платы, отличающийся от фото, но на основе прежней схемы
 
## Перечень элементов:
1. Arduino NANO 328p – 1 шт
2. Светодиоды - 64 шт (3,5 мм ), 1 красный, 1 зелёный 
3. Сдвиговые регистры - 2 шт (74HC595)
4. Транзисторы bd139 - 4 шт
5. Резисторы - 18 шт, 220 Ом
6. Кнопки - 2 шт

# Управление
Управление кубом полуатоматическое, режимы переключается последовательно
друг за другом. Кнопки отвечают за включение и выключение куба (индикация
красный светодиод), а зацикливание текущего режима отображается зелёным светодиодом
