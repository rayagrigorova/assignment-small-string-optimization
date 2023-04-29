#include <iostream>

#include "MyString.h"

int main() {
    MyString s1("Hello");
    std::cout << s1 << std::endl; // "Hello"
    std::cout << "Length: " << s1.length() << std::endl; // 5
    std::cout << s1.substr(0, 2) << std::endl; // "He"
    std::cout << "Is long string: " << s1.isLongStr() << std::endl; // false
    std::cout << std::endl;

    MyString s2(", world!");
    s1 += s2; 
    std::cout << s1 << std::endl; // Hello, world!
    std::cout << "Length: " << s1.length() << std::endl; // 13
    std::cout << "Is long string: " << s1.isLongStr() << std::endl; // false, "Hello, world!" is 13 characters 
    std::cout << std::endl;

    MyString s3("Long string hhndjhnsjjsdjskdjjsds");
    std::cout << s3 << std::endl;
    std::cout << "Length: " << s3.length() << std::endl; // 33
    std::cout << "Is long string: " << s3.isLongStr() << std::endl; // true
    std::cout << std::endl;

    s1 += s3; // Hello, world!Long string hhndjhnsjjsdjskdjjsds
    MyString s4(s1); // "Hello, world!Long string hhndjhnsjjsdjskdjjsds"
    std::cout << s4 << std::endl;
    std::cout << "Length: " << s4.length() << std::endl; // 46
    std::cout << "Is long string: " << s4.isLongStr() << std::endl; // true
    std::cout << std::endl;

    MyString s5(s3.substr(5, 6)); // "string"
    std::cout << s5 << std::endl;
    std::cout << "Length: " << s5.length() << std::endl; // 6
    std::cout << "Is long string: " << s5.isLongStr() << std::endl; // false
    std::cout << std::endl;

    MyString s6;
    std::cin >> s6;
    std::cout << s6 << std::endl;
    std::cout << "Length: " << s6.length() << std::endl;
    std::cout << "Is long string: " << s6.isLongStr() << std::endl; // true or false, depends on user input 
    std::cout << std::endl;

    s1 = "12312341526124"; // 14
    s3 = s1; // 12312341526124
    std::cout << s3 << std::endl;
    std::cout << "Length: " << s3.length() << std::endl; // 14
    std::cout << "Is long string: " << s3.isLongStr() << std::endl; // false
    std::cout << std::endl;

    MyString s7("hsjhsfjsdfddjd"); 
    std::cout << s7 << std::endl;
    std::cout << "Length: " << s7.length() << std::endl; // 14 
    std::cout << "Is long string: " << s7.isLongStr() << std::endl; // false
    s7 += "1";
    std::cout << s7 << std::endl;
    std::cout << "Length: " << s7.length() << std::endl; // 15
    std::cout << "Is long string: " << s7.isLongStr() << std::endl; // true
}

