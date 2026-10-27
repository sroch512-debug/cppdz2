
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <cmath>

class MathParser {
public:
    static double evaluate(const std::string& str) {
        size_t i = 0;
        double res = parseExpression(str, i);

        skipSpaces(str, i);

        if (i < str.length()) {
            throw std::runtime_error("Neozhidanniy simvol: '" + std::string(1, str[i]) + "'");
        }

        return res;
    }

private:
    static void skipSpaces(const std::string& str, size_t& i) {
        while (i < str.length() && std::isspace(static_cast<unsigned char>(str[i]))) {
            i++;
        }
    }

    static double parseExpression(const std::string& str, size_t& i) {
        double res = parseTerm(str, i);

        while (true) {
            skipSpaces(str, i);

            if (i >= str.length()) break;

            char op = str[i];

            if (op == '+' || op == '-') {
                i++;
                double num = parseTerm(str, i);

                if (op == '+') res += num;
                else res -= num;
            }
            else {
                break;
            }
        }

        return res;
    }

    static double parseTerm(const std::string& str, size_t& i) {
        double res = parseFactor(str, i);

        while (true) {
            skipSpaces(str, i);

            if (i >= str.length()) break;

            char op = str[i];

            if (op == '*' || op == '/') {
                i++;
                double num = parseFactor(str, i);

                if (op == '*') {
                    res *= num;
                }
                else {
                    if (num == 0.0) {
                        throw std::runtime_error("Delenie na nol");
                    }
                    res /= num;
                }
            }
            else {
                break;
            }
        }

        return res;
    }

    static double parseFactor(const std::string& str, size_t& i) {
        skipSpaces(str, i);

        if (i >= str.length()) {
            throw std::runtime_error("Konec virazheniya");
        }

        if (str[i] == '+' || str[i] == '-') {
            char op = str[i++];
            double num = parseFactor(str, i);
            return op == '-' ? -num : num;
        }

        if (str[i] == '(') {
            i++;
            double res = parseExpression(str, i);

            skipSpaces(str, i);

            if (i >= str.length() || str[i] != ')') {
                throw std::runtime_error("Net zakryvayushchey skobki");
            }

            i++;
            return res;
        }

        if (std::isdigit(static_cast<unsigned char>(str[i])) || str[i] == '.') {
            size_t len;
            double num = std::stod(str.substr(i), &len);
            i += len;
            return num;
        }

        throw std::runtime_error("Neizvestniy simvol: '" + std::string(1, str[i]) + "'");
    }
};

int main() {
    std::string virazheniya[] = {
        "2 + 2",
        "10 + 2 * 6",
        "(10 + 2) * 6",
        "-5.5 + (3 * 2.5) / 2",
        "100 / (2 + 3) * 4",
        "-(3 + 5) * -2"
    };

    for (const auto& str : virazheniya) {
        try {
            double res = MathParser::evaluate(str);
            std::cout << str << " = " << res << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Oshibka v virazhenii \"" << str << "\": "
                << e.what() << std::endl;
        }
    }

    return 0;
}
