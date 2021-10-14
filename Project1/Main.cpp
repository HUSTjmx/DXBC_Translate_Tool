#include <iostream>
#include "Translator.h"
#include "FileSelector.h"


int main() {
	FilePathInfo info_;
	FileSelector::Instance().Select(info_);
	Translator::Instance().Execute(info_.Path(), info_.Create((info_.name_ + "_new").c_str()));
	int i;
	cin >> i;
	return 1;
}