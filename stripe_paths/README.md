# Циклические нуклеотиды

Время = 2сек. Память = 512мб.

## Сложность = ?

Запуск всем известного космического зонда "Коммивояджер-3" дал ученым ограмное количество образцов с разных планет. Во время их исследования один из ученых заметил, что в образцах содержится вирус инопланетного происхождения, который обладает весьма необычной ДНК. 

После исследования инопланетной ДНК выяснилось, что она не является линейной, а напоминает связную структуру в которой хорошо различимы n узлов который соединены двумя видами оснований. В ходе изучения ученый решили подвесить ДНК за узел 1 для упрощения проведения опытов, однако это дало больше ранее не обнаруженных свойств о ДНК. Оказалось что:
* ДНК не содержит циклов;
* любой узел ДНК содержит максимум два соединенных с ним узла, которые находятся ниже после подвешивания.

Чтобы связать свои наработки с уже существующими знаниями о молекулярной биологии ученые решили называть путь между двумя узлами, состоящий из последовательно соединенных основаниями узлов, нуклеотидом. Также они стали называть нулеотид "прямым" если каждый последующий узел в нуклеотиде находится ниже чем предыдущий в порядке обхода от начала к концу нуклеотида.

В ходе анализа ДНК обнаружились "циклические" нуклеотиды. Нуклеотид называют "циклическим", если типы оснований соединений нуклеотида в порядке от его начального узла к конечному образуют "циклический" паттерн. "Циклическим" называют такую последовательность оснований ДНК, которая может быть разбита на положительное количество одинаковых сегментов, каждый из которых может быть представлен в виде k подряд идущих оснований типа 1, после которых идет k оснований типа 0 подряд, при некотором k > 0.

Ваша задача помочь ученым в классификации ДНК, для этого они просят найти количество "прямых" "циклических" нуклеотидов в ДНК.

## Входные данные

Первая строка содержит одно целое число n (1 <= n <= 10^4) -- количество вершин в инопланетной ДНК.

Далее следует n строк описывающие узлы ДНК. Строка описывающая i-й узел состоит из трех целых чисел l<sub>i</sub>, r<sub>i</sub> и c<sub>i</sub> (0 <= l<sub>i</sub>, r<sub>i</sub> <= n, 0 <= c<sub>i</sub> <= 1) -- правый и левый соседние узлы ДНК, которые находятся ниже i-го узла, а также тип основания связывает узел с узлом на один уровень выше его.

Гарантируется, что данные описывают корректное дерево.

## Выходные данные

Выведите единственное целое число — количество "прямых" "циклических" нуклеотидов.

### Входные данные

5<br>
1 2 1<br>
2 3 0<br>
3 4 1<br>
4 5 0

### Выходные данные

3<br>

### Примечания

Тип основания для узла 1 можно считать отсутствующим вне зависимости от c<sub>1</sub>, потому что выше узла 1 нет ни одного узла.