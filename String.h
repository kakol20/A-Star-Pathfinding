#pragma once

#include <iostream>

class String
{
public:

	//String();
	String(const char* string = "");
	String(const String& copyString);

	String& operator=(const String& copyString);
	String& operator=(const char* string);

	const char* GetChar() const;

	const char* GetContext(const char* delimiter) const;

	String operator+(const String& copyString);
	String operator+(const char* string);

	String& operator+=(const String& copyString);
	String& operator+=(const char* string);

	bool operator==(const String& copyString);
	bool operator==(const char* string);

	bool operator!=(const String& copyString);
	bool operator!=(const char* string);

	size_t Length() const;

	char operator[](int index) const;

	void Clear();

	bool IsEmpty();

	int Find(char string);
	int Find(const char* string);

	int Count(char string);

	~String();

private:

	char* m_string = nullptr;
};

std::ostream& operator<<(std::ostream& os, const String& string);
std::istream& operator>>(std::istream& is, String& string);
