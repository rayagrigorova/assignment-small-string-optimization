#pragma once
#include <iostream>

// Sources used:
//https://github.com/Angeld55/Object-oriented_programming_FMI/tree/master/Week%2008
//https://github.com/elliotgoodrich/SSO-23

const int MAX_SIZE_BYTES = 15;

class MyString
{
	// This type of string will be used if the length of the string is more than 14 bytes
	struct longStr {
		char* _data; // 8 bytes (for a 64-bit computer)
		uint32_t _length; // 4 bytes 
		// The last bit of capacity will be used to indicate whether we are using the small or the big string. 
		// Otherwise, it would be impossible to tell. 
		// The most significant bit of _capacity will be '1' if longStr is used, and '0' if smallStr is used
		// The maximal length of the string is reduced from 4 294 967 295 to 2 147 483 647 characters.
		uint32_t _capacity; // 4 bytes
	};

	// This type of string will be used if the length of the string is less than or equal to 14 bytes
	struct shortStr {
		char _data[MAX_SIZE_BYTES]; // 15 bytes (but 14 will be used because of '\0')

		// A number between 0 and 255 is big enough to represent the size of the smaller string 
		// In this case, shortStr can be between 0 and 15 bytes 
	   // The last bit will be used to indicate whether we are using the small or the big string.
		unsigned char _length; // 1 byte
	};

	// Memory will be shared between the two types of string
	union {
		longStr l;
		shortStr s;
	};

	void copyFrom(const MyString& data);
	void free();
	void resize();

	void copyToSmallStr(const char* data);
	void copyToLongStr(const char* data);

	bool isLongStr() const; // This will return true if the long string is in use
	void switchToLongStr(); // Once the small string becomes too small, switch to the bigger one
	void setFlag(); // Set the bit indicating that longStr is being used
	void unsetFlag(); // Unset the bit indicating that longStr is being used

	uint32_t capacity() const;

	explicit MyString(uint32_t capacity); 

public:

	MyString();
	MyString(const char* data);

	MyString(const MyString& other);
	MyString& operator=(const MyString& other);

	uint32_t length() const;
	MyString& operator+=(const MyString& other);

	MyString substr(size_t begin, size_t howMany) const;

	char& operator[](size_t index);
	char operator[](size_t index) const;
	~MyString();

	const char* c_str() const;

	friend MyString operator+(const MyString& lhs, const MyString& rhs);
	friend std::istream& operator>>(std::istream&, MyString& str);
	friend std::ostream& operator<<(std::ostream& os, const MyString& str);
};

MyString operator+(const MyString& lhs, const MyString& rhs);

std::ostream& operator<<(std::ostream& os, const MyString& str);

std::istream& operator>>(std::istream& os, const MyString& str);