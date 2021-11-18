#pragma once
#include <string>

using std::string;

// TODO come up with a better name come on man

/*
	A very nice string wrapper, yes.
	String does const char* constructor, I do const char* constructor.
	He has string constructor, I have string constructor.
	He compares in operators(<, >, ==, !=) by numerical char value,
	I use windows codepage 1250 value comparison according to Slovak alphabet sequence.
	Great success!!!
*/
class SvkStr : public string
{

public:
	
	SvkStr();
	SvkStr(const char* ptr);
	SvkStr(const string& str);

	~SvkStr();

	/*
		Compares strings in codepage 1250 (only slovak alphabet)
		<0 - string a is sequentially before string b
		=0 - string a is sequentially in the same place as string b
		>0 - string a is sequentially after string b
	*/
	static int compare(const string& a, const string& b);

	/*
		Compares strings in codepage 1250 (only slovak alphabet)
		<0 - string a is sequentially before other string
		=0 - string a is sequentially in the same place as other string
		>0 - string a is sequentially after other string
	*/
	int inline compare(const string& other);

	friend inline bool operator<(const SvkStr& left, const SvkStr& right);
	friend inline bool operator>(const SvkStr& left, const SvkStr& right);
	friend inline bool operator<=(const SvkStr& left, const SvkStr& right);
	friend inline bool operator>=(const SvkStr& left, const SvkStr& right);

private:

	/* Very nice char evauluator array according to slovak alphabet in codepage 1250 */
	static constexpr int chVal[]
	{
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,32, 0, 0,34,43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,32, 0, 0,34,43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,24, 0,24, 0,30, 2, 0, 0, 3,19, 0, 0, 6,10, 0, 0, 0,15, 0, 8, 0, 0,23,25,26, 0, 0, 0, 0, 0,36, 0, 0,41, 0, 0,30, 2, 0, 0, 3,19, 0, 0, 6,10, 0, 0, 0,15, 0, 8, 0, 0,23,25,26, 0, 0, 0, 0, 0,36, 0, 0,41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 5, 7, 9,11,12,13,14,16,17,18,21,22,24,27,28,29,31,33,35,37,38,39,40,42, 0, 0, 0, 0, 0, 0, 1, 4, 5, 7, 9,11,12,13,14,16,17,18,21,22,24,27,28,29,31,33,35,37,38,39,40,42, 0, 0, 0, 0, 0
	};
	
	/*
		Compares chars in codepage 1250 (only slovak alphabet)
		<0 - char a is sequentially before char b
		=0 - char a is sequentially in the same place as char b
		>0 - char a is sequentially after char b
	*/
	static inline int charCompare(const char a, const char b);

};

inline SvkStr::SvkStr() :
	string()
{}

inline SvkStr::SvkStr(const char* ptr) :
	string(ptr)
{}

inline SvkStr::SvkStr(const string& str) :
	string(str)
{}

inline SvkStr::~SvkStr()
{}

inline int SvkStr::compare(const string& a, const string& b)
{
	const size_t sizeA = a.size();
	const size_t sizeB = b.size();
	const size_t count = sizeA < sizeB ? sizeA : sizeB;
	int cval;

	for (size_t i = 0; i < count; i++)
	{
		// Compare all characters
		cval = charCompare(a[i], b[i]);

		if (cval != 0)
		{ 
			return cval;
		}
	}

	// Chars are equal, compare string sizes

	return sizeA - sizeB;
}

inline int SvkStr::compare(const string& other)
{
	return SvkStr::compare(*this, other);
}

inline bool operator<(const SvkStr& left, const SvkStr& right)
{
	return SvkStr::compare(left, right) < 0;
}

inline bool operator>(const SvkStr& left, const SvkStr& right)
{
	return SvkStr::compare(left, right) > 0;
}

inline bool operator<=(const SvkStr& left, const SvkStr& right)
{
	return !(left > right);
}

inline bool operator>=(const SvkStr& left, const SvkStr& right)
{
	return !(left < right);
}

inline int SvkStr::charCompare(const char a, const char b)
{
	return chVal[a + 128] - chVal[b + 128];
}
