#include "stdafx.h"
#include "DLLDumper.h"


DLLDumper::DLLDumper(wstring dllName)
{
    if(!(dllHandler = GetModuleHandleW(dllName.c_str())))
		return;

    if(dllHandler == NULL)
        return;

    dllBase = reinterpret_cast<PBYTE>(dllHandler);
    dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dllHandler);

    ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(dllBase + dosHeader->e_lfanew);

    if(ntHeader->Signature != IMAGE_NT_SIGNATURE)
        return;

    // EAT Data

    ied = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(dllBase + ntHeader->OptionalHeader.DataDirectory[EAT].VirtualAddress);

    baseOrdinalsAddress = reinterpret_cast<PWORD>(dllBase + ied->AddressOfNameOrdinals);
    baseFunctionsAddress = reinterpret_cast<PDWORD>(dllBase + ied->AddressOfFunctions);
    baseNameAddress = reinterpret_cast<char**>(dllBase + ied->AddressOfNames);

    char **tmp = baseNameAddress;
    char *name;
    PDWORD address;
    for(WORD i = 0 ; i < ied->NumberOfFunctions ; i++)
    {
        name = reinterpret_cast<char*>(*tmp + (DWORD)dllBase);
        address = reinterpret_cast<PDWORD>(dllBase + baseFunctionsAddress[baseOrdinalsAddress[i]]);
        dumpedEAT.insert(make_pair(name,address));
        tmp++;
    }

    // IAT Data

    iad = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(dllBase + ntHeader->OptionalHeader.DataDirectory[IAT].VirtualAddress);
    if(!(iad->Name))
        return;

    while(iad->FirstThunk)
    {
        //char *name = (char*)(dllBase + iad->Name);

        tdata = reinterpret_cast<PIMAGE_THUNK_DATA>(dllBase + iad->OriginalFirstThunk);
        int n = 0;
        while(tdata->u1.Function)
        {
            imp = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(tdata->u1.AddressOfData);
            char *funcName = reinterpret_cast<char*>(dllBase + (DWORD)imp->Name);
            PDWORD address = reinterpret_cast<PDWORD>(dllBase + iad->FirstThunk) + n;
            dumpedIAT.insert(make_pair(funcName,address));
            n++;
            tdata++;
        }
        iad++;
    }
}

bool DLLDumper::setDLL(wstring dllName)
{
    if(!(dllHandler = GetModuleHandleW(dllName.c_str())))
        return 0;

    if(dllHandler == NULL)
        return 0;

    dllBase = reinterpret_cast<PBYTE>(dllHandler);

    dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dllHandler);

    ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(dllBase + dosHeader->e_lfanew);

    if(ntHeader->Signature != IMAGE_NT_SIGNATURE)
        return 0;

    // EAT Data

    ied = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(dllBase + ntHeader->OptionalHeader.DataDirectory[EAT].VirtualAddress);

    baseOrdinalsAddress = reinterpret_cast<PWORD>(dllBase + ied->AddressOfNameOrdinals);
    baseFunctionsAddress = reinterpret_cast<PDWORD>(dllBase + ied->AddressOfFunctions);
    baseNameAddress = reinterpret_cast<char**>(dllBase + ied->AddressOfNames);

    char **tmp = baseNameAddress;
    char *name;
    PDWORD address;

    for(WORD i = 0 ; i < ied->NumberOfFunctions ; i++)
    {
        name = reinterpret_cast<char*>(*tmp + (DWORD)dllBase);
		if(strcmp(name,"socket")==0)
			break;
        address = reinterpret_cast<PDWORD>(dllBase + baseFunctionsAddress[baseOrdinalsAddress[i]]);
        dumpedEAT.insert(make_pair(name,address));
        tmp++;
    };
    // IAT Data
    iad = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(dllBase + ntHeader->OptionalHeader.DataDirectory[IAT].VirtualAddress);
    if(!(iad->Name))
        return 0;

    while(iad->FirstThunk)
    {
        //char *name = (char*)(dllBase + iad->Name);

        tdata = reinterpret_cast<PIMAGE_THUNK_DATA>(dllBase + iad->OriginalFirstThunk);
        int n = 0;
        while(tdata->u1.Function)
        {
            imp = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(tdata->u1.AddressOfData);
            char *funcName = reinterpret_cast<char*>(dllBase + (DWORD)imp->Name);
            PDWORD address = reinterpret_cast<PDWORD>(dllBase + iad->FirstThunk) + n;
            dumpedIAT.insert(make_pair(funcName,reinterpret_cast<PDWORD>(*address)));
            n++;
            tdata++;
        }
        iad++;
    }
    return 1;
}

PDWORD DLLDumper::getAddress(string functionName, AT_Type type = EAT)
{
    if(type == EAT)
        if(dumpedEAT.find(functionName)!=dumpedEAT.end())
            return dumpedEAT.find(functionName)->second;
        else return NULL;
    else if(type == IAT)
    {
        if(dumpedIAT.find(functionName)!=dumpedIAT.end())
            return dumpedIAT.find(functionName)->second;
        else return NULL;
    }
    return NULL;
}

PDWORD DLLDumper::hookEAT(string functionName, void* newFunction)
{
    ied = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(dllBase + ntHeader->OptionalHeader.DataDirectory[EAT].VirtualAddress);

    baseOrdinalsAddress = reinterpret_cast<PWORD>(dllBase + ied->AddressOfNameOrdinals);
    baseFunctionsAddress = reinterpret_cast<PDWORD>(dllBase + ied->AddressOfFunctions);
    baseNameAddress = reinterpret_cast<char**>(dllBase + ied->AddressOfNames);

    char **tmp = baseNameAddress;
    char *name;
    PDWORD address;
    PDWORD oldAddress;
    for(WORD i = 0 ; i < ied->NumberOfFunctions ; i++)
    {
        name = reinterpret_cast<char*>(*tmp + (DWORD)dllBase);
        PWORD pOrdinal = (WORD *)(ied->AddressOfNameOrdinals +
                                  (sizeof(WORD) * i) +
                                  dllBase);
        address = reinterpret_cast<PDWORD>(dllBase + ied->AddressOfFunctions + 4 * (*pOrdinal));
        if(strcmp(functionName.c_str(),name)==0)
        {
            DWORD oldprot, oldprot2;
            oldAddress = reinterpret_cast<PDWORD>(dllBase + *address);

            VirtualProtect(address, sizeof(DWORD), PAGE_READWRITE, (DWORD *)&oldprot);
            *address = (DWORD)(newFunction) - (DWORD)dllHandler;
            VirtualProtect(address, sizeof(DWORD), oldprot, (DWORD *)&oldprot2);

            return oldAddress;
        }
        tmp++;
    }
    return NULL;
}
DLLDumper::~DLLDumper(void)
{
}
