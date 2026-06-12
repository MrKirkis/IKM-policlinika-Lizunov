#include "header.h"

#include <limits>

int inputInt(const std::string& message) {
    std::cout << message;

    int value;
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw AppException("нужно ввести целое число.");
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

int inputIntMin(const std::string& message, int minValue) {
    int value = inputInt(message);

    if (value < minValue) {
        throw AppException("число должно быть не меньше " +
                           std::to_string(minValue) + ".");
    }

    return value;
}

int inputIntInRange(const std::string& message, int minValue, int maxValue) {
    int value = inputInt(message);

    if (value < minValue || value > maxValue) {
        throw AppException("число должно быть в диапазоне от " +
                           std::to_string(minValue) + " до " +
                           std::to_string(maxValue) + ".");
    }

    return value;
}

std::string inputString(const std::string& message) {
    std::cout << message;

    std::string value;
    std::getline(std::cin, value);

    if (value.empty()) {
        throw AppException("строка не должна быть пустой.");
    }

    return value;
}
