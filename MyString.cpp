#include "MyString.h"

#pragma warning(disable : 4996)

const unsigned char MASK = 1; // This mask will be used to set and unset the flag
//const unsigned char MASK2 = (UCHAR_MAX << 1); // 11111110, used for removing the flag 

//const uint32_t MASK3 = (1 << 31); // 100000...000

namespace {

	// Source: https://www.techiedelight.com/round-next-highest-power-2/

	uint32_t findNextPowerOf2(uint32_t n) {
		// Handle the case when n is a power of 2. 
		// I chose to increment it so that numbers close to a power of two are rounded up
		// This way, findNextPowerOf2(15) will return 32 for example.
		n++; // n--; 

		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;

		return ++n;
	}
}

bool MyString::isLongStr() const {
	// This will return true if the last bit is set to 1
	return s._length & MASK;
}

MyString::MyString(uint32_t capacity) {
	// The small string can't be used and memory allocation is necessary
	if (capacity > MAX_SIZE_BYTES) {
		l._length = 0;

		l._capacity = findNextPowerOf2(capacity);

		l._data = new char[l._capacity + 1];

		l._capacity <<= 1; // Shift left so that the last bit is used as a flag
		setFlag();
	}

	else {
		s._length = 0;
	}
}

void MyString::copyToSmallStr(const char* data) {
	strcpy(s._data, data);
	s._length = strlen(data);

	// Make place for the flag bit	
	s._length <<= 1;
}

void MyString::copyToLongStr(const char* data) {
	strcpy(l._data, data);
	l._length = strlen(data);
}

MyString operator+(const MyString& lhs, const MyString& rhs) {
	int lhsLen = lhs.length();
	int rhsLen = rhs.length();

	MyString result( lhsLen + rhsLen );

	result[0] = '\0';

	if (result.isLongStr()) {
		strcat(result.l._data, lhs.c_str());
		strcat(result.l._data, rhs.c_str());
		result.l._length = lhsLen + rhsLen;
	}

	else {
		strcat(result.s._data, lhs.c_str());
		strcat(result.s._data, rhs.c_str());
		result.setShortStringLength(lhsLen + rhsLen);
	}

	return result;
}

void MyString::setShortStringLength(const unsigned char len) {
	s._length = len; // add the  two numbers 
	s._length <<= 1; // add the flag back 
}

MyString& MyString::operator+=(const MyString& other) {
	uint32_t otherLen = other.length(); 
	uint32_t thisLen = length();

	if ( otherLen + thisLen >= capacity()) {
		if (!isLongStr()) {
			switchToLongStr(otherLen + thisLen);
		}

		resize();
		strcat(l._data, other.c_str());
		l._length += otherLen;
	}

	else {
		if (isLongStr()) {
			strcat(l._data, other.c_str());
			l._length += otherLen;
		}
		else {
			strcat(s._data, other.c_str());
			setShortStringLength(otherLen + thisLen);
		}
	}

	return *this;
}

MyString::MyString() {
	// No need to allocate memory dynamically 
	s._data[0] = '\0';
	s._length = 0;
}

MyString::MyString(const char* data) : MyString(strlen(data) + 1) {
	// MyString allocated on the heap
	if (isLongStr()) {
		copyToLongStr(data);
		
	}

	else {
		copyToSmallStr(data);
	}
}

MyString::MyString(const MyString& other) {
	copyFrom(other);
}

MyString& MyString::operator=(const MyString& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

void MyString::free() {
	if (isLongStr()) {
		delete[] l._data;
	}

	l._data = nullptr;
	l._length = l._capacity = 0;
}

MyString::~MyString() {
	free();
}

void MyString::resize() {
	// Remove the flag
	s._length ^= MASK;
	l._capacity >>= 1;
	l._capacity *= 2;

	char* newData = new char[l._capacity + 1];

	l._capacity <<= 1;
	setFlag();

	strcpy(newData, l._data);

	delete[] l._data;
	l._data = newData;
}

uint32_t MyString::length() const {
	return isLongStr() ? l._length : s._length >> 1;
}

void MyString::copyFrom(const MyString& other) {
	if (other.isLongStr()) {
		l._capacity = other.l._capacity;
		l._data = new char[l._capacity + 1];
		copyToLongStr(other.l._data);
	
		l._capacity <<= 1;
		setFlag();
	}

	else {
		copyToSmallStr(other.s._data);
		unsetFlag();
	}
}

void MyString::switchToLongStr(int totalLen) {
	if (isLongStr()) {
		return;
	}

	// First, save the data from the smaller string
	// This is necessary, because memory is shared between the two strings
	uint32_t newLen = length();
	uint32_t newCap = findNextPowerOf2(totalLen);
	char* newData = new char[newCap + 1];
	strcpy(newData, s._data);

	l._capacity = (newCap << 1);
	l._length = newLen;
	l._data = newData;

	setFlag();
}

uint32_t MyString::capacity() const {
	// This will set the '1' bit that shows that longStr is being used to 0. 
	return isLongStr() ? (l._capacity ^ MASK) >> 1 : MAX_SIZE_BYTES;
}

char& MyString::operator[](size_t index) {
	return isLongStr() ? l._data[index] : s._data[index];
}

char MyString::operator[](size_t index) const {
	return isLongStr() ? l._data[index] : s._data[index];
}

MyString MyString::substr(size_t begin, size_t howMany) const {
	if (begin + howMany > length()) {
		throw std::length_error("Error, substr out of range");
	}

	MyString res(howMany + 1);
	const char* ptr = c_str();

	if (res.isLongStr()) {
		for (int i = 0; i < howMany; i++) {
			res.l._data[i] = ptr[begin + i];
		}
		res.l._length = howMany;
	}

	else {
		for (int i = 0; i < howMany; i++) {
			res.s._data[i] = ptr[begin + i];
		}
		res.setShortStringLength(howMany);
	}

	res[howMany] = '\0';
	return res;
}

const char* MyString::c_str() const {
	return isLongStr() ? l._data : s._data;
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, MyString& str) {
	char buff[1024];
	is >> buff; 
	size_t len = strlen(buff);

	// I am not using the copyToSmallStr and copyToLongStr functions here
	// because they will recalculate the size of the buffer, which is unnecessary.
	if (len > MAX_SIZE_BYTES) {
		if (str.isLongStr()) {
			delete[] str.l._data;
		}

		str.l._length = len;
		str.l._capacity = findNextPowerOf2(len);

		str.l._data = new char[str.l._capacity + 1];
		strcpy(str.l._data, buff);

		str.l._capacity <<= 1;
		str.setFlag();
	}

	else {
		strcpy(str.s._data, buff);
		str.setShortStringLength(len);
		str.unsetFlag();
	}
	return is;
}

void MyString::setFlag() {
	s._length |= MASK;
}

void MyString::unsetFlag() {
	s._length ^= MASK;
}


