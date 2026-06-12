# IKM-policlinika-Lizunov
ИКМ по C++: тема 12, поликлиника.

Очередь реализована собственным классом PatientQueue на основе односвязного списка.
Классы проекта: Patient, QueueNode, PatientQueue, CommandProcessor, AppException.
Готовые контейнеры STL для хранения очереди не используются.
Узлы очереди оформлены отдельным классом QueueNode.

Проект состоит из четырёх файлов исходного кода: header.h, main.cpp, poliklinika.cpp, input.cpp. Компиляция выполняется командой:
g++ -std=c++11 main.cpp poliklinika.cpp input.cpp -o poliklinika
