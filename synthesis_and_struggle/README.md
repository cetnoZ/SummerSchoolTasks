
# Синтез и борьба

## Сложность = ***

Поиск и синтез лекарства крайне затруднительно. Для разрушения РНК вируса используется таргетное модулирование генома и данная процедура является многоэтапной. Для полного синтеза используется сеть медицинских центров. Транспорт медиков вынужден перемещаться между центрами. От каждого центра до любого другого можно добраться ровно одним способом, и длины дорог везде одинаковы. Однако для ускорения поиска лекарства было принято решения провести реформу. Было решено выбрать некоторое количество центров и назначить их ГУЦПЛ(главное управление центра поиска лекарства). На обслуживание ГУЦПЛ приходится тратить много средств - K даляров в квартал. Считается, что ГУЦПЛ всегда имеет возможность произвести синтез.
Каждому центру, не являющемуся ГУЦПЛ, было принято решение назначить ГУЦПЛ, который будет отвечать за синтез в данном центре. В таком случае затраты на обслуживание равняются Dl даляров в квартал, где l — расстояние от мед. центра до соответствующего ГУЦПЛ, измеряемое в количестве дорог, которые надо проехать.

Ваша задача минимизировать расходы на проведение центролизации.

## Входные данные
В первой строке заданы два целых числа N, K (1 ≤ N ≤ 100, 1 ≤ K ≤ 10^5).

Во второй строке записано N - 1 целых чисел Vi, нумерующихся с единицы ( Vi ≤ Vi+1, 0 ≤ Di ≤ 10^5).

В следующих n - 1 строках заданы пары номеров городов соединенных дорогой.

## Выходные данные
В строке выведите минимальное количество даляров, затрачиваемых на обслуживание за квартал.

## Пример
### Входные данные
 6 10
 <br>1 6 11 16 20
 <br>4 6
 <br>2 3
 <br>1 2
 <br>2 4
 <br>2 5

### Выходные данные
 20




