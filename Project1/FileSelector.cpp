#undef UNICODE
#include "FileSelector.h"
#include "windows.h"
#include <stdio.h>
#include <stdlib.h>

void FileSelector::Select(FilePathInfo& info)
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME file = { 0 };
	file.hwndOwner = NULL;
	file.lStructSize = sizeof(file);
	file.lpstrFilter = "�����ļ�(*.*)\0*.*\0Exe�ļ�(*.exe)\0*.exe\0";  //Ҫѡ����ļ���׺
	file.lpstrInitialDir = "";//Ĭ�ϵ��ļ�·�� 
	file.lpstrFile = szBuffer;//����ļ��Ļ����� 
	file.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	file.nFilterIndex = 0;
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&file);
	string filePath = file.lpstrFile;

	int start = filePath.find_last_of('\\'); //��ȡ���һ��\\������
	int end = filePath.find_last_of('.'); //��ȡ���һ��.������

	if (start == end) Select(info);
	if (end == -1)return;

	info.path_ = filePath.substr(0, start + 1);
	info.name_  = filePath.substr(start + 1, end - start - 1);
	info.exten_ = filePath.substr(end, filePath.length() - end);
}

string FilePathInfo::Path()
{
	return path_ + name_ + exten_;
}

string FilePathInfo::Create(const char* name)
{
	return path_ + name + exten_;
}
