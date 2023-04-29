#include "MyString.h"

#pragma warning(disable : 4996)

const unsigned char MASK = 1; // This mask will be used to set and unset the flag

namespace {

	// Source: https://www.techiedelight.com/round-next-highest-power-2/

	uint32_t findNextPowerOf2(uint32_t n) {
		n--; 

		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;

		return ++n;
	}
}

bool MyString::isLongStr() const {
	// This will return true if the last bit of s._length is set to 1
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

	bool isLong = result.isLongStr(); 

	char* dataPtr = isLong ? result.l._data : result.s._data;
	strcat(dataPtr, lhs.c_str());
	strcat(dataPtr, rhs.c_str());

	if (isLong) {
		result.l._length = lhsLen + rhsLen;
	}

	else {
		result.setShortStringLength(lhsLen + rhsLen);
	}

	return result;
}

void MyString::setShortStringLength(const unsigned char len) {
	s._length = len;
	s._length <<= 1; 
}

MyString& MyString::operator+=(const MyString& other) {
	uint32_t otherLen = other.length(); 
	uint32_t thisLen = length();
	bool isLong = isLongStr();

	// The small string can't be resized 
	// Switch to long string (if not already in use)
	if ( otherLen + thisLen >= capacity()) { 
		// Move the data from the small string to the big one
		// The capacity will be set to a value big enough to contain both strings.
		if (!isLong) {
			switchToLongStr(otherLen + thisLen);
		}

		else {
			resize();
		}

		strcat(l._data, other.c_str());
		l._length += otherLen;
	}

	else {
		char* dataPtr = isLong ? l._data : s._data;
		strcat(dataPtr, other.c_str());

		if(isLong){
			l._length += otherLen;
		}

		else {
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
	}
}

void MyString::switchToLongStr(int totalLen) {
	if (isLongStr()) {
		return;
	}

	// First, save the data from the smaller string
	// This is necessary, because memory is shared between the two strings
	uint32_t newLen = (s._length >> 1);
	uint32_t newCap = findNextPowerOf2(totalLen);
	char* newData = new char[newCap + 1];
	strcpy(newData, s._data);

	l._capacity = (newCap << 1);
	l._length = newLen;
	l._data = newData;

	setFlag();
}

uint32_t MyString::capacity() const {
	return isLongStr() ? l._capacity >> 1 : MAX_SIZE_BYTES - 1;
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
	bool isLong = res.isLongStr();

	const char* thisPtr = c_str();
	char* resPtr = isLong ? res.l._data : res.s._data;

	for (int i = 0; i < howMany; i++) {
		resPtr[i] = thisPtr[begin + i];
	}

	if(isLong){
		res.l._length = howMany;
	}

	else {
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

	if (len >= MAX_SIZE_BYTES) {
		if (str.isLongStr()) {
			delete[] str.l._data;
		}

		str.initLongStr(buff, len);
	}

	else {
		str.initSmallStr(buff, len);
	}

	return is;
}

void MyString::initSmallStr(const char* data, uint32_t len) {
	strcpy(s._data, data);
	setShortStringLength(len);
}

void MyString::initLongStr(const char* data, uint32_t len) {
	l._length = len;
	l._capacity = findNextPowerOf2(len);

	l._data = new char[l._capacity + 1];
	strcpy(l._data, data);

	l._capacity <<= 1;
	setFlag();
}


void MyString::setFlag() {
	s._length |= MASK;
}

void MyString::unsetFlag() {
	s._length ^= MASK;
}


