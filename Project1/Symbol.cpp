#include "Symbol.h"
#include <iostream>

bool IsNum(char c)
{
	return c >= '0' && c <= '9';
}

bool IsMathOp(char c) 
{
	return c == '+' || c == '-' || c == '/' || c == '*';
}

Symbol::Symbol(const string& s)
{
	Init(s);
}

void Symbol::Init(const string& s)
{
	auto index = s.find('.');

	//the symbol Indicates constant or vec
	if (s.find(',') != s.npos || IsNum(s[s.length() - 1]))
	{
		cout << s << endl;
		this->m_name = s;
		this->id = 100;
		return;
	}

	auto type_ = s.substr(0, index);
	auto channels = s.substr(index + 1);
	
	// Init
	this->m_name = type_;
	this->channels = channels;
	this->id = GetIndex(type_);
	for (int i = 0;i < 4;++i)
		this->IsWrited[i] = 0;
}

void Symbol::ComputeWriteChannels()
{
	for (auto c : channels)
	{
		IsWrited[GetIndexByChar(c)] = 1;
	}
}

string Symbol::GetName()
{
	if (id == 100) return m_name;

	string writedChannels;
	if (channels.size() == 4)
		for (int i = 0;i < 4; ++i)
		{
			if (IsWrited[i])writedChannels += channels[i];
		}
	else writedChannels = channels;

	return m_name + "." + writedChannels;
}

void Symbol::SetWritedFlag(int flags[4])
{
	for (int i = 0;i < 4; ++i)
		IsWrited[i] = flags[i];
}

bool Symbol::IsRegister(const string& s)
{
    return s[0] == 'r';
}

int Symbol::GetIndex(const string& s)
{
    if (!IsRegister(s))return -1;
    auto id = s.substr(1);
    return stoi(id);
}

int GetIndexByChar(char c)
{
	switch (c)
	{
	case 'x':
		return 0;
	case 'y':
		return 1;
	case 'z':
		return 2;
	case 'w':
		return 3;
	default:
		break;
	}
	return -1;
}

SymbolItem::SymbolItem(int x, int y[4])
{
	id = x;
	for (int i = 0;i < 4;++i)
		UsedChannels[i] = y[i];
}

bool SymbolItem::operator==(const SymbolItem& other)
{
	if (id != other.id) return false;
	for (int i = 0; i < 4; ++i)
	{
		if (UsedChannels[i] != other.UsedChannels[i])return false;
	}
	return true;
}

string SymbolTable::Search(const SymbolItem& item)
{
	for (auto i = symbols.rbegin(); i != symbols.rend(); i++)
	{
		//todo
	}
	return string();
}
