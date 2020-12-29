#pragma once
#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

std::string calculation;
std::vector<std::string> numbers_symbols;
std::vector<std::string> numbers;
std::vector<std::string> symbols;
typedef std::map<std::string, int> mapmassive;
typedef std::map<std::string, int>::iterator iter2;
extern bool wrong;
mapmassive operations;
mapmassive operindex;

void check_numbers(std::vector<std::string> num) {
    for (int i = 0; i < num.size(); i++) {
        if (num[i] == "nan" or num[i] == "inf") {
            wrong = true;
            break;
        }
    }
}

void check_equation(std::vector<std::string> equation) {
    int open_s = 0;
    int close_s = 0;
    for (int i = 0; i < equation.size(); i++) {
        if (i >= 1) {
            if (operations.find(equation[i])->second < 5 and operations.find(equation[i])->second > 1 and
                operations.find(equation[i - 1])->second < 5 and operations.find(equation[i - 1])->second > 1) {
                wrong = true;
                break;
            }
        }
        if (operations.find(equation[i])->second == 5 and equation[i + 1] != "(") {
            wrong = true;
            break;
        }
        if (equation[i] == "(") {
            open_s += 1;
        } else if (equation[i] == ")") {
            close_s += 1;
        }

    }
    if (close_s != open_s) {
        wrong = true;
    }
}

std::vector<std::string> separate(std::string new_equation) {
    std::vector<std::string> new_equation_vector;
    std::string container = "";
    bool f = false;
    bool negative = false;

    for (int i = 0; i < new_equation.size(); i++) {

        while ((48 <= new_equation[i] and new_equation[i] <= 57) or new_equation[i] == 46) {
            if (new_equation[i] == 46) {
                f = true;
                //cout<<"\nfloat";
            }
            container += new_equation[i];
            i += 1;
        }
        if (container != "") {
            if (negative) {
                container = "-" + container;
                negative = false;
            }
            if (new_equation[i] != 120) {
                if (f) {
                    new_equation_vector.push_back(container);
                    f = false;
                } else {
                    new_equation_vector.push_back(container + ".0");
                }
                container = "";

            } else {
                if (negative) {
                    container = "-" + container;
                    negative = false;
                }
                if (f) {
                    new_equation_vector.push_back(container);
                    f = false;
                } else {
                    new_equation_vector.push_back(container + ".0");
                }
                new_equation_vector.push_back("*");
                new_equation_vector.push_back("x");
                container = "";

                i += 1;
            }
        } else if (new_equation[i] == 120) {
            if (negative) {
                new_equation_vector.push_back("-1.0");
                new_equation_vector.push_back("*");
                new_equation_vector.push_back("x");
                negative = false;
            } else {
                new_equation_vector.push_back("x");
            }
            i += 1;
        }
        if (new_equation[i] != 120) {
            while (97 <= new_equation[i] and new_equation[i] <= 122) {
                container += new_equation[i];
                i += 1;
            }
            if (container != "") {
                if (negative) {
                    new_equation_vector.push_back("-1.0");
                    new_equation_vector.push_back("*");
                    negative = false;
                }
                if (container == "e") {
                    new_equation_vector.push_back(std::to_string(M_E));
                } else if (container == "pi") {
                    new_equation_vector.push_back(std::to_string(M_PI));
                } else {
                    if (operations.find(container) == operations.end()) {
                        wrong = true;
                        break;
                    }
                    new_equation_vector.push_back(container);
                }
                container = "";
            }
        }
        switch (new_equation[i]) {
            case 40: {
                if (!new_equation_vector.empty()) {
                    if (operations.find(new_equation_vector.back()) == operations.end()) {
                        new_equation_vector.push_back("*");
                    }
                }
                new_equation_vector.push_back("(");
                break;
            }                 //)
            case 41: {
                new_equation_vector.push_back(")");
                break;
            }                 //(
            case 43: {
                new_equation_vector.push_back("+");
                break;
            }                 //+
            case 45: {
                if (new_equation_vector.empty()) {
                    negative = true;

                } else if (!new_equation_vector.empty() and new_equation_vector.back() == "(") {
                    negative = true;
                } else {
                    new_equation_vector.push_back("-");
                }
                break;
            }                 //-
            case 47: {
                new_equation_vector.push_back("/");
                break;
            }                 //:
            case 42: {
                new_equation_vector.push_back("*");
                break;
            }                 //*
        }
    }
    return new_equation_vector;
}

void init() {
    operations.insert(std::make_pair(")", 0));
    operations.insert(std::make_pair("(", 6));
    operations.insert(std::make_pair("+", 2));
    operations.insert(std::make_pair("-", 2));
    operations.insert(std::make_pair("*", 3));
    operations.insert(std::make_pair("/", 3));
    operations.insert(std::make_pair("sin", 5));
    operations.insert(std::make_pair("cos", 5));
    operations.insert(std::make_pair("tg", 5));
    operations.insert(std::make_pair("ctg", 5));
    operations.insert(std::make_pair("exp", 5));
    for (iter2 newit = operations.begin(); newit != operations.end(); newit++) {
        operindex.insert(std::make_pair(newit->first, newit->first.size() * newit->first[newit->first.size() - 1]));
    }
}

void counting() {
    switch (operindex.find(symbols[symbols.size() - 1])->second) {
        case 45: {
            if (numbers.size() < 2) {
                wrong = true;
                break;
            }
            numbers[numbers.size() - 2] = std::to_string(
                    std::stof(numbers[numbers.size() - 2]) - std::stof(numbers.back()));
            numbers.pop_back();
            symbols.pop_back();
            break;
        }
        case 43: {
            if (numbers.size() < 2) {
                wrong = true;
                break;
            }
            numbers[numbers.size() - 2] = std::to_string(
                    std::stof(numbers[numbers.size() - 2]) + std::stof(numbers.back()));
            numbers.pop_back();
            symbols.pop_back();
            break;
        }
        case 42: {
            if (numbers.size() < 2) {
                wrong = true;
                break;
            }
            numbers[numbers.size() - 2] = std::to_string(
                    std::stof(numbers[numbers.size() - 2]) * std::stof(numbers.back()));
            numbers.pop_back();
            symbols.pop_back();
            break;
        }
        case 47: {
            if (numbers.size() < 2) {
                wrong = true;
                break;
            }
            numbers[numbers.size() - 2] = std::to_string(
                    std::stof(numbers[numbers.size() - 2]) / std::stof(numbers.back()));
            numbers.pop_back();
            symbols.pop_back();
            break;
        }
        case 94: {
            if (numbers.size() < 2) {
                wrong = true;
                break;
            }
            numbers[numbers.size() - 2] = std::to_string(
                    pow(std::stof(numbers[numbers.size() - 2]), std::stof(numbers.back())));
            numbers.pop_back();
            symbols.pop_back();
            break;
        }
        case 345: {
            if (numbers.size() < 1) {
                wrong = true;
                break;
            }
            numbers.back() = std::to_string(std::cos(std::stof(numbers.back())));
            symbols.pop_back();
            break;
        }
        case 330: {
            if (numbers.size() < 1) {
                wrong = true;
                break;
            }
            numbers.back() = std::to_string(std::sin(std::stof(numbers.back())));
            symbols.pop_back();
            break;
        }
        case 206: {
            if (numbers.size() < 1) {
                wrong = true;
                break;
            }
            numbers.back() = std::to_string(std::tan(std::stof(numbers.back())));
            symbols.pop_back();
            break;
        }
        case 336: {
            if (numbers.size() < 1) {
                wrong = true;
                break;
            }
            numbers.back() = std::to_string(std::exp(std::stof(numbers.back())));
            symbols.pop_back();
            break;
        }
        case 309: {
            if (numbers.size() < 1) {
                wrong = true;
                break;
            }
            numbers.back() = std::to_string(1 / std::tan(std::stof(numbers.back())));
            symbols.pop_back();
            break;
        }
        case 464: {
            if (numbers.size() < 1) {
                wrong = true;
                break;
            }
            numbers.back() = std::to_string(std::sqrt(std::stof(numbers.back())));
            symbols.pop_back();
            break;
        }
    }
}
