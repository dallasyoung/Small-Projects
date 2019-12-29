#include <iostream>
#include <windows.h>

#define print(x) std::cout << x << std::endl

#define line() print("")

int
main()
{
  print("Starting program...");

  line();

  /*
    When a handle to a process is needed, it can be retrieved in a few
    different ways. If the handle is for the currently running
    process, you can use the GetCurrentProcess() function. Reviewing
    documentation for this function is important, however, as this
    doesn't retrieve the processes' handle, rather, it retrieves a
    pseudo-handle that only it can use for itself. If the real handle
    is needed for use in another process, it can be retrieved using
    the OpenProcess() function, or even the DuplicateHandle()
    function.
   */
  HANDLE pseudoProc = GetCurrentProcess();
  HANDLE duplicateHandle = NULL;
  DuplicateHandle(pseudoProc, pseudoProc, pseudoProc, &duplicateHandle, PROCESS_ALL_ACCESS, FALSE, 0);
  HANDLE realProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

  print("pseudoProc:\t" << pseudoProc);
  print("duplicateProc:\t" << realProc);
  print("realProc:\t" << realProc);

  line();

  print("done");
}
