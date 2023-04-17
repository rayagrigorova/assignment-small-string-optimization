#include "MyString.h"

const uint32_t MASK = 1;

namespace {
	// Source: https://www.techiedelight.com/round-next-highest-power-2/
	uint32_t findNextPowerOf2(uint32_t n)
	{
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
	// This will return true if the last bit of _capacity is set to 1
	return l._capacity & MASK;
}

MyString::MyString(uint32_t capacity)
{
	// The small string can't be used and memory allocation is necessary
	if (capacity > MAX_SIZE_BYTES) {
		l._length = capacity - 1;
		l._capacity = findNextPowerOf2(capacity);
		l._data = new char[l._capacity];
		setToLongStrType();
	}

	else {
		s._length = 0;
	}
}

void MyString::copyToSmallStr(const char* data) {
	strcpy(s._data, data);
	s._length = strlen(data);
}

void MyString::copyToLongStr(const char* data) {
	strcpy(l._data, data);
	l._length = strlen(data);
}

MyString operator+(const MyString& lhs, const MyString& rhs)
{
	MyString result(lhs.length() + rhs.length());

	result[0] = '\0';

	if (result.isLongStr()) {
		strcat(result.l._data, lhs.c_str());
		strcat(result.l._data, rhs.c_str());
	}

	else {
		strcat(result.s._data, lhs.c_str());
		strcat(result.s._data, rhs.c_str());
	}

	return result;
}

MyString& MyString::operator+=(const MyString& other)
{
	if (other.length() + length() >= capacity()) {
		setToLongStrType();
		strcat(l._data, other.l._data);
		resize();
	}

	else {
		if (isLongStr()) {
			strcpy(l._data, other.l._data);
		}
		else {
			copyToSmallStr(other.s._data);
		}
	}

	return *this;
}

MyString::MyString() 
{
	// No need to allocate memory dynamically 
	s._data[0] = '\0';
	s._length = 0;
}

MyString::MyString(const char* data) : MyString(strlen(data) + 1)
{
	// MyString allocated on the heap
	if (isLongStr()) {
		copyToLongStr(data);
	}

	else {
		copyToSmallStr(data);
	}
}

MyString::MyString(const MyString& other)
{
	copyFrom(other);
}

MyString& MyString::operator=(const MyString& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}


void MyString::free()
{
	if (isLongStr()) {
		delete[] l._data;
	}

	l._data = nullptr;
	l._length = l._capacity = 0;
}

MyString::~MyString()
{
	free();
}

void MyString::resize() {
	l._capacity = (l._capacity >> 1);
	l._capacity *= 2;
	setToLongStrType();

	char* newData = new char[l._capacity];
	strcpy(newData, l._data);

	delete[] l._data;
	l._data = newData;
}

uint32_t MyString::length() const
{
	return isLongStr() ? l._length : s._length;
}

void MyString::copyFrom(const MyString& other)
{
	if (other.isLongStr()) {
		l._capacity = other.l._capacity;
		l._data = new char[l._capacity + 1];
		copyToLongStr(other.l._data);
	}

	else {
		copyToSmallStr(other.s._data);
	}
}

void MyString::setToLongStrType() {
	if (isLongStr()) {
		return;
	}

	// First, save the data from the smaller string
	// This is necessary, because memory is shared between the two strings
	uint32_t newLen = s._length;
	uint32_t newCap = findNextPowerOf2(newLen);
	char* newData = new char[newCap];
	strcpy(newData, s._data);

	l._capacity = newCap;
	l._length = newLen;
	l._data = newData;

	setFlag();
}

uint32_t MyString::capacity() const {
	return isLongStr() ? l._capacity : MAX_SIZE_BYTES;
}

char& MyString::operator[](size_t index) 
{
	return isLongStr() ? l._data[index] : s._data[index];
}

char MyString::operator[](size_t index) const 
{
	return isLongStr() ? l._data[index] : s._data[index];
}

MyString MyString::substr(size_t begin, size_t howMany) const
{
	if (begin + howMany > length())
		throw std::length_error("Error, Substr out of range");


	MyString res(howMany + 1);

	if (isLongStr()) {
		for (int i = 0; i < howMany; i++)
			res.l._data[i] = l._data[begin + i];
	}

	else {
		for (int i = 0; i < howMany; i++)
			res.s._data[i] = s._data[begin + i];
	}

	res[howMany] = '\0';
	return res;
}

const char* MyString::c_str() const
{
	return isLongStr() ? l._data : s._data;
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, MyString& str)
{
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
	}

	else {
		strcpy(str.s._data, buff);
		str.s._length = len;
	}
	return is;
}

void MyString::setFlag() {
	l._capacity |= MASK;
}

