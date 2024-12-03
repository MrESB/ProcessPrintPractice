#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

void PrintProcesses(FILE* myCsvFile, DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
        }
        CloseHandle(hProcess);
    }

    _ftprintf(myCsvFile, TEXT("%s,%u\n"), szProcessName, processID);
}

int main(void)
{
    FILE* myCsvFile;
    errno_t err = _wfopen_s(&myCsvFile, TEXT("processes.csv"), TEXT("w"));
    if (err != 0)
    {
        return 1;
    }

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        fclose(myCsvFile);
        return 1;
    }

    cProcesses = cbNeeded / sizeof(DWORD);
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcesses(myCsvFile, aProcesses[i]);
        }
    }

    fclose(myCsvFile);
    return 0;
}
