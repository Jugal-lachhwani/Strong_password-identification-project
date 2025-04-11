#include <iostream>
#include <string>
#include <fstream>

class Password {
private:
    std::string passwordString;
    int passwordStrengthRate;

public:
    Password() : passwordStrengthRate(0) {}

    Password(const std::string& password) : passwordString(password) {
        calculateStrengthRate();
    }

    void initialize(const std::string& password) {
        passwordString = password;
        calculateStrengthRate();
    }

    bool isLong() const {
        return passwordString.size() >= 8;
    }

    bool hasDigits() const {
        for (char c : passwordString) {
            if (isdigit(c)) {
                return true;
            }
        }
        return false;
    }

    bool hasUpperAndLowerCase() const {
        bool hasUpper = false;
        bool hasLower = false;
        for (char c : passwordString) {
            if (isupper(c)) {
                hasUpper = true;
            } else if (islower(c)) {
                hasLower = true;
            }
        }
        return hasUpper && hasLower;
    }

    bool hasSpecialChars() const {
        for (char c : passwordString) {
            if (!isalnum(c) && c != '_') {
                return true;
            }
        }
        return false;
    }

    bool isCommon() const {
        std::ifstream common("topPasswords.txt");
        if (common.is_open()) {
            std::string line;
            while (std::getline(common, line)) {
                if (line == passwordString) {
                    return true;
                }
            }
            common.close();
        }
        return false;
    }

    int getPasswordStrengthRate() const {
        int rate = 0;
        rate += isLong() ? 25 : 0;
        rate += hasDigits() ? 25 : 0;
        rate += hasUpperAndLowerCase() ? 25 : 0;
        rate += hasSpecialChars() ? 25 : 0;
        rate += isCommon() ? 25 : 0;
        return rate;
    }

    void calculateStrengthRate() {
        passwordStrengthRate = getPasswordStrengthRate();
    }

    void displayFeedback() const {
        std::cout << "Contains at least 8 characters: " << (isLong() ? "Yes" : "No") << std::endl;
        std::cout << "Has at least one digit: " << (hasDigits() ? "Yes" : "No") << std::endl;
        std::cout << "Has both upper and lower case letters: " << (hasUpperAndLowerCase() ? "Yes" : "No") << std::endl;
        std::cout << "Has at least one special character: " << (hasSpecialChars() ? "Yes" : "No") << std::endl;
        std::cout << "Is a common password: " << (isCommon() ? "Yes" : "No") << std::endl;
    }
};

class PasswordGenerator {
private:
    int passwordLength;
    std::string generatedPassword;

    int getRandomInRange(int min, int max) const {
        return min + std::rand() % (max - min + 1);
    }

    char getRandomLowerCase() const {
        return 'a' + std::rand() % 26;
    }

    char getRandomUpperCase() const {
        return 'A' + std::rand() % 26;
    }

    char getRandomSpecialChar() const {
        static const std::string specialChars = "!@#$%^&*()_-+=~`";
        return specialChars[getRandomInRange(0, specialChars.size() - 1)];
    }

public:
    PasswordGenerator() : passwordLength(getRandomInRange(8, 16)) {
        generatePassword();
    }

    PasswordGenerator(int length) : passwordLength(length) {
        generatePassword();
    }

    void generatePassword() {
        generatedPassword.clear();

        for (int i = 0; i < passwordLength - 3; ++i) {
            generatedPassword.push_back(getRandomLowerCase());
        }
        generatedPassword.push_back(getRandomUpperCase());
        generatedPassword.push_back(getRandomSpecialChar());
    }

    std::string getPassword() const {
        return generatedPassword;
    }
};

void printMenu() {
    std::cout << "---------- Password Checker CLI ----------" << std::endl;
    std::cout << "1. Generate Strong Password" << std::endl;
    std::cout << "2. Check If A Password Is Strong" << std::endl;
    std::cout << "3. Exit from the Program" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    std::string inputPassword;

    printMenu();
    std::cin >> choice;

    while (choice != 3) {
        switch (choice) {
            case 1: {
                int subChoice;
                std::cout << "1. Create Random Strong Password" << std::endl;
                std::cout << "2. Create a Specific Length Password" << std::endl;
                std::cin >> subChoice;

                if (subChoice == 1) {
                    PasswordGenerator generator;
                    std::cout << "Your New Password is: " << generator.getPassword() << std::endl;
                } else if (subChoice == 2) {
                    int length;
                    std::cout << "Enter the Password Length: ";
                    std::cin >> length;
                    PasswordGenerator generator(length);
                    std::cout << "Your New Password is: " << generator.getPassword() << std::endl;
                } else {
                    std::cout << "Unknown Choice!" << std::endl;
                }
                break;
            }
            case 2: {
                std::cout << "Enter the Password: ";
                std::cin >> inputPassword;

                Password passwordChecker(inputPassword);
                std::cout << "Password Rating: " << passwordChecker.getPasswordStrengthRate() << "%" << std::endl;

                int feedbackChoice;
                std::cout << "Do you want the feedback of your password? (Enter 1 to view it): ";
                std::cin >> feedbackChoice;

                if (feedbackChoice == 1) {
                    passwordChecker.displayFeedback();
                }
                break;
            }
            case 3:
                break;
            default:
                std::cout << "Unknown Choice!" << std::endl;
                break;
        }

        printMenu();
        std::cin >> choice;
    }

    return 0;
}

