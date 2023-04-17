#include <iostream>

#include "MyString.h"

int main() {
    MyString s1("Hello");
    std::cout << s1 << std::endl;

    MyString s2(", world!");
    s1 += s2;
    std::cout << s1 << std::endl;

    MyString s3("Long string hhndjhnsjjsdjskdjjsds");
    std::cout << s3 << std::endl;

    MyString s4(s1 + s3);
    std::cout << s4 << std::endl;

    MyString s5(s3.substr(5, 6));
    std::cout << s5 << std::endl;

    MyString s6;
    std::cin >> s6;
    std::cout << s6 << std::endl;

    s6 = s1;
    std::cout << s6;

}

