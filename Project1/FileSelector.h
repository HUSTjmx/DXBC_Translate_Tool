// Todo
// ...
#pragma once
#include <string>
#include "Singleton.h"

using namespace std;

// File path info structure.
// Members:
//     path_ : path of the folder.
//     name_ : the pure name of the file.
//     exten_ : the class of the file.
// ...
struct FilePathInfo
{
	string path_;
	string name_;
	string exten_;

	// Return full path of the file.
	// ...
	string Path();

	// Return a new file path that has the same folder and exten.
	// ...
	string Create(const char* name);

};

// Awake the window that select a file, and record the path
// info of the file selected.
// ...
class FileSelector : public Singleton<FileSelector>
{

public:

	// We must declare this parent class as a friend, 
	// because we need call it`s construct function that is private in the parent class.
	// ...
	friend class Singleton<FileSelector>;

	// Awake the window, store the info to input that transport by ref.
	// ...
	void Select(FilePathInfo& info);

private:

	FileSelector() {}
};

