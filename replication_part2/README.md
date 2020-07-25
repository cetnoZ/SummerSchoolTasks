# ============= Репликация ч2 =============

## Сложность = |**   |

Для увеличения продолжительности существования, вирусы используют коревалюацию. Процесс при котором вирус добавляет в свой генотип произвольный ген. 

Ваша задача - эмуляция поведения генотипа вируса в условиях отсутствия возможности предсказать добавляемый ген.

## Входные данные

Первая строка ввода содержит целое число N, P - количество запрсов (2 ≤ N ≤ 10^6, 1 ≤ P ≤ 10^9).
Далее следует N запросов, где операция добавления: "+ Ai", где Ai - длина i-го добавляемого гена (1 ≤ Ai ≤ 10^9). "-" - операция удаления.
Гарантируется что в любой момент времени генетическое дерево будет не пустым.

## Выходные данные

После каждой опреции выводится строка вида "C T", где C - количество операций замещения в ходе операции, T - новый топовый элемент в иерархии
