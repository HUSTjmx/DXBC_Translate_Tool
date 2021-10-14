#pragma once
#include <map>
#include <string>
#include <vector>
#include "Singleton.h"

using namespace std;

//class Channels {
//	map<int, string> 
//};

// The char is or not number.
// If the char is 0-9, return true.
// ...
bool IsNum(char c);

// The char is or not math op.
// Here, we only consider : +, -, *, / ;
// ...
bool IsMathOp(char c);

// The x-th channel of x, y, z, w.
// ...
int GetIndexByChar(char c);

// The symbol of data in the register 
// ...
class Symbol
{
public:

	Symbol() : id(-1) {}

	Symbol(const string& s);

	void Init(const string& s);

	// Compute the channels that we will write.
	// The symbol on the left of "=" need call it.
	// ...
	void ComputeWriteChannels();

	// After some optimized processing, return the great name.
	// ...
	string GetName();

	// ...
	void SetWritedFlag(int flags[4]);

	// The channels that we will write.
	// ...
	int IsWrited[4];

	// The channels that we will read.

private:

	// Judge the symbol is register or not.
	// ...
	bool IsRegister(const string& s);

	// Get the index of register.
	// ...
	int GetIndex(const string& s);

	// The index of register.
	// Is Register ? -1 show it is not register, and 100 indicates it is const or vec4.
	// ...
	int id;

	// Main name.
	// ...
	string m_name;

	// The channels.
	// ...
	string channels;

};

// The item store at SymbolTabel
// ...
class SymbolItem
{
public:

	SymbolItem() {}

	SymbolItem(int x, int y[4]);

	bool operator==(const SymbolItem& other);

	// The index of register.
	// ...
	int id;

	// The channels that are used.
	// ...
	int UsedChannels[4];

	// The var name when it is pushed to table.
	// ...
	string name;
};


// The Symbol Table we use to update register name.
// ...
class SymbolTable : public Singleton<SymbolTable>
{
public:

	friend Singleton<SymbolTable>;


	// Search the item in the table.
	// If has exist, we only return the name has get.
	// If the item is new, we create a new name for it, push to table and return the name.
	// More complex cases, to be done.
	// ...
	string Search(const SymbolItem& item);

private:

	SymbolTable() {}

	// The real symbol table.
	// ...
	vector<SymbolItem> symbols;

};

// Create var name automatically.
// ...
template<int N>
class NameCreator
{
public:
	static const int index = 0;

	string GenName();
};

// Specialization when N = 1.
// ...
template<>
class NameCreator<1>
{
public:
	static int index;

	string GenName();
};

int NameCreator<1>::index = 0;

inline string NameCreator<1>::GenName()
{
	string name_ = "scalar_" + to_string(index);
	index++;
	return string();
}

// Specialization when N = 2.
// ...
template<>
class NameCreator<2>
{
public:
	static int index;

	string GenName();
};

int NameCreator<2>::index = 0;

inline string NameCreator<2>::GenName()
{
	string name_ = "vec2_" + to_string(index);
	index++;
	return string();
}

// Specialization when N = 3.
// ...
template<>
class NameCreator<3>
{
public:
	static int index;

	string GenName();
};

int NameCreator<3>::index = 0;

inline string NameCreator<3>::GenName()
{
	string name_ = "vec3_" + to_string(index);
	index++;
	return string();
}

// Specialization when N = 4.
// ...
template<>
class NameCreator<4>
{
public:
	static int index;

	string GenName();
};

int NameCreator<4>::index = 0;

inline string NameCreator<4>::GenName()
{
	string name_ = "vec4_" + to_string(index);
	index++;
	return string();
}






