#include <iostream>
#include <windows.h>

#define print(x) std::cout << x << std::endl

#define line() print("")

int count_cpus(DWORD_PTR);

/*
  Every Windows process has a property in the form of a bitmask called
  its Processor Affinity. This property is the size of a CPU register
  (4 bytes on x86, 8 bytes on x64), and each bit in the mask
  represents a CPU detected by the OS. If the bit is 1, it means the
  process is allowed to execute on that core, and vice versa. This
  affinaty can also be set at runtime to restrict the execution of the
  program.

  This used to mean that on x64 systems, Windows could only support a
  maximum of 64 processors, so starting with Windows 7/Server 2008,
  Windows now uses the concept of Processor Affinity Groups. Basically,
  at startup, Windows will discover all installed processors and
  assign them to a group from 1-20. Each of these groups represents a
  different Processor Affinity Mask. The underlying mechanism by
  which processes query for this mask doesn't change, but the mask for
  group 1 (processors 1-64) will likely be different from the mask for
  group 2 (processors 65-128).
*/
int
main()
{
  print("Starting program...");

  line();

  HANDLE process = GetCurrentProcess();
  DWORD_PTR processMask = 0, systemMask = 0;
  BOOL result = GetProcessAffinityMask(process, &processMask, &systemMask);
  /*
    In addition to the Process Afinity Mask, GetProcessAffinityMask()
    also returns another System Affinity Mask that represents all of
    the available CPUs in the system. You can use this to determine
    how many logical processors are available in a system, though in a
    real program, you're probably better off using something like
    GetLogicalProcessorInformation()
   */
  int num_cpus = 1;
  if(!result) print("Error while querying for the affinity masks !!");
  else num_cpus = count_cpus(systemMask);

  print("Process Mask:\t0x" << std::hex << processMask << std::dec << " (" << processMask << ")");
  print("System Mask:\t0x" << std::hex << systemMask << std::dec << " (" << systemMask << ")");
  print("CPUs found:\t" << num_cpus);

  line();

  print("done");
}

int
count_cpus(DWORD_PTR mask)
{
  int core_count = 0;
  for(int i = 8 * sizeof(DWORD_PTR) - 1; i >= 0; i--) {
    if(mask >> i & 0x1) core_count++;
  }
  return(core_count);
}
