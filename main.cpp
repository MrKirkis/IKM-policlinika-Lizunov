#include "header.h"

#include <clocale>
#include <cstdlib>
#include <ctime>
#include <iostream>

void printMenu() {
    std::cout << std::endl;
    std::cout << " ПОЛИКЛИНИКА " << std::endl;
    std::cout << "1. Создать новую очередь" << std::endl;
    std::cout << "2. Добавить пациента в текущую очередь" << std::endl;
    std::cout << "3. Обслужить пациента из текущей очереди" << std::endl;
    std::cout << "4. Выполнить команды из файла" << std::endl;
    std::cout << "5. Показать текущую очередь" << std::endl;
    std::cout << "6. Очистить текущую очередь" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << std::endl;
}

void printCreateQueueMenu() {
    std::cout << std::endl;
    std::cout << " СОЗДАНИЕ НОВОЙ ОЧЕРЕДИ " << std::endl;
    std::cout << "1. Ввод с клавиатуры" << std::endl;
    std::cout << "2. Случайный ввод" << std::endl;
    std::cout << "3. Считать из файла" << std::endl;
    std::cout << "0. Назад" << std::endl;
    std::cout << std::endl;
}

void createQueueMenu(PatientQueue& queue) {
    printCreateQueueMenu();
    int choice = inputIntInRange("Выберите способ создания очереди: ", 0, 3);

    switch (choice) {
    case 1:
        createQueueFromKeyboard(queue);
        break;

    case 2:
        createRandomQueue(queue);
        break;

    case 3:
        createQueueFromFile(queue);
        break;

    case 0:
        std::cout << "Возврат в главное меню." << std::endl;
        break;
    }
}

int main() {
    setlocale(LC_ALL, "");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    PatientQueue queue;
    bool isRunning = true;

    while (isRunning) {
        printMenu();

        try {
            int choice = inputIntInRange("Выберите пункт меню: ", 0, 6);

            switch (choice) {
            case 1:
                createQueueMenu(queue);
                break;

            case 2: {
                std::string name = inputString("Введите имя пациента: ");
                int priority = inputInt("Введите приоритет пациента: ");

                queue.addPatient(name, priority);
                std::cout << "Пациент добавлен в очередь." << std::endl;
                break;
            }

            case 3: {
                Patient patient = queue.servePatient();
                std::cout << "Обслужен пациент: " << patient.getName() << std::endl;
                break;
            }

            case 4: {
                std::string inputFileName = inputString("Введите имя входного файла: ");
                std::string outputFileName = inputString("Введите имя выходного файла: ");

                processFile(inputFileName, outputFileName);
                std::cout << "Команды выполнены. Результат записан в файл "
                          << outputFileName << "." << std::endl;
                break;
            }

            case 5:
                queue.print(std::cout);
                break;

            case 6:
                queue.clear();
                std::cout << "Очередь очищена." << std::endl;
                break;

            case 0:
                isRunning = false;
                std::cout << "Работа программы завершена." << std::endl;
                break;
            }
        } catch (const AppException& error) {
            std::cout << "Ошибка: " << error.what() << std::endl;
        } catch (const std::exception& error) {
            std::cout << "Непредвиденная ошибка: " << error.what() << std::endl;
        }
    }

    return 0;
}
