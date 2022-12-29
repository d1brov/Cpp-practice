#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

///////////////////////////////////////////////////////////////////////////////
// Implement your own guard that manages the handle obtained                 //
// as a result of calling the CreateFile Win32 API function.                 //
///////////////////////////////////////////////////////////////////////////////
class FileHandleGuard
{
private:
    HANDLE hFile_;

public:
    FileHandleGuard() = delete;

    explicit FileHandleGuard(HANDLE hFile) : hFile_(hFile) {
        printf("[%d] Handle acquired\n", hFile_);
    }

    ~FileHandleGuard() {
        CloseHandle(hFile_);
        printf("[%d] Handle closed\n", hFile_);
    }

    explicit operator HANDLE() const {
        return hFile_;
    }
};

///////////////////////////////////////////////////////////////////////////////
// Example of usage CreateFile(...)                                          //
// from :                                                                    //
// https://learn.microsoft.com/en-us/windows/win32/fileio/opening-a-file-for-reading-or-writing
///////////////////////////////////////////////////////////////////////////////
#define BUFFERSIZE 256
DWORD g_BytesTransferred = 0;

void DisplayError(LPTSTR lpszFunction)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL);

    lpDisplayBuf =
        (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf)
                + lstrlen((LPCTSTR)lpszFunction)
                + 40) // account for format string
            * sizeof(TCHAR));

    if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error code %d as follows:\n%s"),
        lpszFunction,
        dw,
        lpMsgBuf)))
    {
        printf("FATAL ERROR: Unable to output error code.\n");
    }

    _tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped)
{
    _tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
    _tprintf(TEXT("Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
    g_BytesTransferred = dwNumberOfBytesTransfered;
}

int __cdecl _tmain(int argc, TCHAR* argv[])
{
    HANDLE hFile;
    DWORD  dwBytesRead = 0;
    char   ReadBuffer[BUFFERSIZE] = { 0 };
    OVERLAPPED ol = { 0 };

    printf("\n");
    if (argc != 2)
    {
        printf("Usage Error: Incorrect number of arguments\n\n");
        _tprintf(TEXT("Usage:\n\t%s <text_file_name>\n"), argv[0]);
        return 1;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // FileHandleGuard usage                                                     //
    ///////////////////////////////////////////////////////////////////////////////
    auto handle = FileHandleGuard(CreateFile(argv[1],
                               GENERIC_READ,
                               FILE_SHARE_READ,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                               NULL));

    if (handle == INVALID_HANDLE_VALUE)
    {
        DisplayError(TEXT("CreateFile"));
        _tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), argv[1]);
        return 1;
    }

    if (FALSE == ReadFileEx(handle, ReadBuffer, BUFFERSIZE - 1, &ol, FileIOCompletionRoutine))
    {
        DisplayError(TEXT("ReadFile"));
        printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
        return 1;
    }
    SleepEx(5000, TRUE);
    dwBytesRead = g_BytesTransferred;

    if (dwBytesRead > 0 && dwBytesRead <= BUFFERSIZE - 1)
    {
        ReadBuffer[dwBytesRead] = '\0'; // NULL character

        _tprintf(TEXT("Data read from %s (%d bytes): \n"), argv[1], dwBytesRead);
        printf("%s\n", ReadBuffer);
    }
    else if (dwBytesRead == 0)
    {
        _tprintf(TEXT("No data read from file %s\n"), argv[1]);
    }
    else
    {
        printf("\n ** Unexpected value for dwBytesRead ** \n");
    }
}