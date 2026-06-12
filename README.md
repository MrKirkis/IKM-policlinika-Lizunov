# IKM-policlinika-Lizunov
ИКМ по C++: тема 12, поликлиника.

Очередь реализована собственным классом PatientQueue на основе односвязного списка.
Классы проекта: Patient, QueueNode, PatientQueue, CommandProcessor, AppException.
Готовые контейнеры STL для хранения очереди не используются.
Узлы очереди оформлены отдельным классом QueueNode.

Сборка:
g++ -std=c++11 main.cpp poliklinika.cpp input.cpp -o poliklinika
