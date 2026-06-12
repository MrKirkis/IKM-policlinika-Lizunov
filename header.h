#include <exception>
#include <iostream>
#include <string>

class AppException : public std::exception {
private:
    std::string message;

public:
    explicit AppException(const std::string& text);
    const char* what() const noexcept override;
};

class Patient {
private:
    std::string name;
    int priority;

public:
    Patient();
    Patient(const std::string& patientName, int patientPriority);

    std::string getName() const;
    int getPriority() const;
};

class QueueNode {
private:
    Patient patient;
    QueueNode* next;

public:
    explicit QueueNode(const Patient& patientValue);

    const Patient& getPatient() const;
    QueueNode* getNext() const;
    void setNext(QueueNode* nextNode);
};

class PatientQueue {
private:
    QueueNode* head;
    int count;

    void copyFrom(const PatientQueue& other);

public:
    PatientQueue();
    PatientQueue(const PatientQueue& other);
    PatientQueue& operator=(const PatientQueue& other);
    ~PatientQueue();

    bool isEmpty() const;
    int getCount() const;

    void addPatient(const std::string& name, int priority);
    Patient servePatient();
    void clear();
    void print(std::ostream& out) const;
};

class CommandProcessor {
private:
    static void executeCommand(const std::string& command,
                               std::istream& input,
                               std::ostream& output,
                               PatientQueue& queue,
                               int commandNumber);

public:
    static void processCommands(std::istream& input,
                                std::ostream& output,
                                PatientQueue& queue,
                                int lastCommandNumber,
                                const std::string& firstCommand);

    static void processFile(const std::string& inputFileName,
                            const std::string& outputFileName);
};

int inputInt(const std::string& message);
int inputIntMin(const std::string& message, int minValue);
int inputIntInRange(const std::string& message, int minValue, int maxValue);
std::string inputString(const std::string& message);

void createQueueFromKeyboard(PatientQueue& queue);
void createRandomQueue(PatientQueue& queue);
void createQueueFromFile(PatientQueue& queue);
void processFile(const std::string& inputFileName, const std::string& outputFileName);

void printMenu();
void printCreateQueueMenu();
void createQueueMenu(PatientQueue& queue);
