#include "header.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>

AppException::AppException(const std::string& text) : message(text) {}

const char* AppException::what() const noexcept {
    return message.c_str();
}

Patient::Patient() : name(""), priority(0) {}

Patient::Patient(const std::string& patientName, int patientPriority)
    : name(patientName), priority(patientPriority) {
    if (patientName.empty()) {
        throw AppException("имя пациента не может быть пустым.");
    }
}

std::string Patient::getName() const {
    return name;
}

int Patient::getPriority() const {
    return priority;
}

QueueNode::QueueNode(const Patient& patientValue)
    : patient(patientValue), next(nullptr) {}

const Patient& QueueNode::getPatient() const {
    return patient;
}

QueueNode* QueueNode::getNext() const {
    return next;
}

void QueueNode::setNext(QueueNode* nextNode) {
    next = nextNode;
}

PatientQueue::PatientQueue() : head(nullptr), count(0) {}

PatientQueue::PatientQueue(const PatientQueue& other) : head(nullptr), count(0) {
    copyFrom(other);
}

PatientQueue& PatientQueue::operator=(const PatientQueue& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }

    return *this;
}

PatientQueue::~PatientQueue() {
    clear();
}

void PatientQueue::copyFrom(const PatientQueue& other) {
    QueueNode* current = other.head;

    while (current != nullptr) {
        addPatient(current->getPatient().getName(),
                   current->getPatient().getPriority());
        current = current->getNext();
    }
}

bool PatientQueue::isEmpty() const {
    return head == nullptr;
}

int PatientQueue::getCount() const {
    return count;
}

void PatientQueue::addPatient(const std::string& name, int priority) {
    Patient patient(name, priority);
    QueueNode* newNode = new QueueNode(patient);

    if (head == nullptr || priority < head->getPatient().getPriority()) {
        newNode->setNext(head);
        head = newNode;
        ++count;
        return;
    }

    QueueNode* current = head;

    // При равных приоритетах новый пациент вставляется после пациентов,
    // которые уже стоят в очереди с таким же приоритетом.
    while (current->getNext() != nullptr &&
           current->getNext()->getPatient().getPriority() <= priority) {
        current = current->getNext();
    }

    newNode->setNext(current->getNext());
    current->setNext(newNode);
    ++count;
}

Patient PatientQueue::servePatient() {
    if (isEmpty()) {
        throw AppException("невозможно обслужить пациента: очередь пуста.");
    }

    QueueNode* oldHead = head;
    Patient patient = oldHead->getPatient();
    head = head->getNext();
    delete oldHead;
    --count;

    return patient;
}

void PatientQueue::clear() {
    while (head != nullptr) {
        QueueNode* temp = head;
        head = head->getNext();
        delete temp;
    }

    count = 0;
}

void PatientQueue::print(std::ostream& out) const {
    if (isEmpty()) {
        out << "Очередь пуста." << std::endl;
        return;
    }

    out << std::left << std::setw(5) << "№"
        << std::setw(20) << "Имя"
        << std::setw(10) << "Приоритет" << std::endl;
    out << "-----------------------------------" << std::endl;

    QueueNode* current = head;
    int number = 1;

    while (current != nullptr) {
        out << std::left << std::setw(5) << number
            << std::setw(20) << current->getPatient().getName()
            << std::setw(10) << current->getPatient().getPriority()
            << std::endl;
        current = current->getNext();
        ++number;
    }
}

void CommandProcessor::executeCommand(const std::string& command,
                                      std::istream& input,
                                      std::ostream& output,
                                      PatientQueue& queue,
                                      int commandNumber) {
    if (command == "+") {
        std::string name;
        int priority;

        if (!(input >> name >> priority)) {
            throw AppException("команда добавления №" +
                               std::to_string(commandNumber) +
                               " должна иметь вид: + name priority.");
        }

        queue.addPatient(name, priority);
    } else if (command == "-") {
        Patient patient = queue.servePatient();
        output << patient.getName() << std::endl;
    } else {
        throw AppException("неизвестная команда №" +
                           std::to_string(commandNumber) + ".");
    }
}

void CommandProcessor::processCommands(std::istream& input,
                                       std::ostream& output,
                                       PatientQueue& queue,
                                       int lastCommandNumber,
                                       const std::string& firstCommand) {
    if (lastCommandNumber < 0) {
        throw AppException("номер последней команды не может быть отрицательным.");
    }

    std::string command = firstCommand;

    for (int i = 0; i <= lastCommandNumber; ++i) {
        if (i == 0) {
            if (command.empty() && !(input >> command)) {
                throw AppException("не удалось прочитать команду №0.");
            }
        } else {
            if (!(input >> command)) {
                throw AppException("не удалось прочитать команду №" +
                                   std::to_string(i) + ".");
            }
        }

        executeCommand(command, input, output, queue, i);
    }
}

void CommandProcessor::processFile(const std::string& inputFileName,
                                   const std::string& outputFileName) {
    std::ifstream input(inputFileName.c_str());
    if (!input.is_open()) {
        throw AppException("не удалось открыть входной файл.");
    }

    std::ofstream output(outputFileName.c_str());
    if (!output.is_open()) {
        throw AppException("не удалось открыть выходной файл.");
    }

    int lastCommandNumber;
    if (!(input >> lastCommandNumber) || lastCommandNumber < 0) {
        throw AppException("первая строка файла должна содержать неотрицательный номер последней команды.");
    }

    PatientQueue queue;
    processCommands(input, output, queue, lastCommandNumber, "");
}

void createQueueFromKeyboard(PatientQueue& queue) {
    int patientCount = inputIntMin("Введите количество пациентов: ", 0);
    PatientQueue newQueue;

    for (int i = 1; i <= patientCount; ++i) {
        std::cout << std::endl << "Пациент " << i << std::endl;
        std::string name = inputString("Введите имя пациента: ");
        int priority = inputInt("Введите приоритет пациента: ");
        newQueue.addPatient(name, priority);
    }

    queue = newQueue;
    std::cout << "Новая очередь создана." << std::endl;
    queue.print(std::cout);
}

void createRandomQueue(PatientQueue& queue) {
    int patientCount = inputIntMin("Введите количество пациентов: ", 0);
    int minPriority = inputInt("Введите минимальный приоритет: ");
    int maxPriority = inputInt("Введите максимальный приоритет: ");

    if (minPriority > maxPriority) {
        throw AppException("минимальный приоритет не может быть больше максимального.");
    }

    PatientQueue newQueue;
    int range = maxPriority - minPriority + 1;

    std::cout << std::endl << "Сгенерированные пациенты:" << std::endl;
    for (int i = 1; i <= patientCount; ++i) {
        int priority = minPriority + std::rand() % range;
        std::string name = "Patient" + std::to_string(i);

        std::cout << name << " " << priority << std::endl;
        newQueue.addPatient(name, priority);
    }

    queue = newQueue;
    std::cout << std::endl << "Новая очередь создана." << std::endl;
    queue.print(std::cout);
}

void createQueueFromFile(PatientQueue& queue) {
    std::string inputFileName = inputString("Введите имя входного файла: ");

    std::ifstream input(inputFileName.c_str());
    if (!input.is_open()) {
        throw AppException("не удалось открыть входной файл.");
    }

    int firstNumber;
    if (!(input >> firstNumber) || firstNumber < 0) {
        throw AppException("первая строка файла должна содержать неотрицательное целое число.");
    }

    PatientQueue newQueue;
    std::string firstToken;

    if (!(input >> firstToken)) {
        if (firstNumber == 0) {
            queue = newQueue;
            std::cout << "Файл содержит пустой список пациентов." << std::endl;
            return;
        }
        throw AppException("после первого числа в файле нет данных.");
    }

    if (firstToken == "+" || firstToken == "-") {
        std::cout << "Файл распознан как файл команд." << std::endl;
        std::cout << "Результат обслуживания:" << std::endl;
        CommandProcessor::processCommands(input, std::cout, newQueue,
                                          firstNumber, firstToken);
        queue = newQueue;
        std::cout << "Текущая очередь после выполнения команд:" << std::endl;
        queue.print(std::cout);
    } else {
        if (firstNumber == 0) {
            throw AppException("в файле указано 0 пациентов, но после первого числа есть данные.");
        }

        std::string name = firstToken;
        int priority;

        if (!(input >> priority)) {
            throw AppException("данные пациента №1 должны иметь вид: name priority.");
        }

        newQueue.addPatient(name, priority);

        for (int i = 2; i <= firstNumber; ++i) {
            if (!(input >> name >> priority)) {
                throw AppException("не удалось прочитать данные пациента №" +
                                   std::to_string(i) + ".");
            }
            newQueue.addPatient(name, priority);
        }

        queue = newQueue;
        std::cout << "Файл распознан как список пациентов." << std::endl;
        std::cout << "Новая очередь создана." << std::endl;
        queue.print(std::cout);
    }
}

void processFile(const std::string& inputFileName, const std::string& outputFileName) {
    CommandProcessor::processFile(inputFileName, outputFileName);
}
