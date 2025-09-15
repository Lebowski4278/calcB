#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <windows.h>

using namespace std;

enum ConsoleColor {
    BLACK = 0,
    GREEN = 2,
    RED = 4,
    WHITE = 7
};

void setColor(ConsoleColor text, ConsoleColor background = BLACK) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((background << 4) | text));
}

void resetColor() {
    setColor(WHITE, BLACK);
}

double calc(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    default:
        throw runtime_error("Неизвестная операция!");
    }
}

double runCalc(string expression) {
    vector<double> numbers;
    vector<char> operators;
    stringstream ss(expression);
    string token;
    while (ss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            operators.push_back(token[0]);
        }
        else {
            try {
                numbers.push_back(stod(token));
            }
            catch (...) {
                throw runtime_error("Неверный формат числа!");
            }
        }
    }
    if (numbers.size() != operators.size() + 1) {
        throw runtime_error("Неверный формат выражения!");
    }
    for (size_t i = 0; i < operators.size();) {
        if (operators[i] == '*' || operators[i] == '/') {
            double result = calc(numbers[i], numbers[i + 1], operators[i]);
            numbers[i] = result;
            numbers.erase(numbers.begin() + i + 1);
            operators.erase(operators.begin() + i);
        }
        else {
            i++;
        }
    }
    double result = numbers[0];
    for (size_t i = 0; i < operators.size(); i++) {
        result = calc(result, numbers[i + 1], operators[i]);
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "rus");
    while (true) {
        resetColor();
        cout << "> ";
        string input;
        getline(cin, input);
        if (input == "esc") {
            break;
        }
        if (input.empty()) {
            continue;
        }
        try {
            double result = runCalc(input);
            setColor(GREEN);
            cout << ">>> " << result << endl;
        }
        catch (exception e) {
            setColor(RED);
            cout << e.what() << endl;
        }
    }
    return 0;
}
