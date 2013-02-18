#pragma once
#include <windows.h>
#include <cstring>
#include <string>
#include <map>

using namespace std;

enum AT_Type{EAT = 0, IAT = 1};

class DLLDumper
{
private:
    HMODULE dllHandler;
    PBYTE dllBase;

    PIMAGE_NT_HEADERS ntHeader;
    PIMAGE_DOS_HEADER dosHeader;

    // EAT Hook //
    PIMAGE_EXPORT_DIRECTORY ied;
    // IAT Hook //
    PIMAGE_IMPORT_DESCRIPTOR iad;
    PIMAGE_THUNK_DATA tdata;
    PIMAGE_IMPORT_BY_NAME imp;

    map<string,PDWORD>dumpedEAT;
    map<string,PDWORD>dumpedIAT;

    PWORD baseOrdinalsAddress;
    PDWORD baseFunctionsAddress;
    char** baseNameAddress;
public:
    DLLDumper(wstring dllName);
    DLLDumper(){}
	~DLLDumper();
    bool setDLL(wstring dllName);
    PDWORD getAddress(string functionName,AT_Type type);
    map<string,PDWORD> getMappedEAT()const{return dumpedEAT;}
    map<string,PDWORD> getMappedIAT()const{return dumpedIAT;}
    PDWORD hookEAT(string functionName, void *newFunction);
};