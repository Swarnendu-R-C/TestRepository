#include <iostream>
#include <vector>
#include <string.h>
#include <attributes_info.H>
#include <libphal.H> 
#include <cstring>
#include <unistd.h>

extern "C"
{
#include "libpdbg.h"
}
void print_process_memory() {
    FILE *file = fopen("/proc/self/statm", "r");
    if (file == NULL) {
        perror("Could not open /proc/self/statm");
        return;
    }

    long size, resident, share, text, lib, data, dt;
    if (fscanf(file, "%ld %ld %ld %ld %ld %ld %ld", &size, &resident, &share, &text, &lib, &data, &dt) == 7) {
        printf("Memory Usage (in pages):\n");
        printf("  Total program size: %ld pages\n", size);
        printf("  Resident set size: %ld pages\n", resident);
        printf("  Shared pages: %ld pages\n", share);
        printf("  Text (code): %ld pages\n", text);
        printf("  Data + Stack: %ld pages\n", data);
    } else {
        perror("Error reading /proc/self/statm");
    }

    fclose(file);

    // Convert pages to bytes
    long page_size = sysconf(_SC_PAGESIZE);
    printf("\nPage size: %ld bytes\n", page_size);
    printf("Total memory in bytes: %ld bytes\n", size * page_size);
}

void pdbgLogCallback(int, const char* fmt, va_list ap)
{
    va_list vap;
    va_copy(vap, ap);
    std::vector<char> logData(1 + std::vsnprintf(nullptr, 0, fmt, ap));
    std::vsnprintf(logData.data(), logData.size(), fmt, vap);
    va_end(vap);
    std::string logstr(logData.begin(), logData.end());
    std::cout << "PDBG:" << logstr << std::endl;
}

int main()
{
    constexpr auto devtree = "/var/lib/phosphor-software-manager/pnor/rw/DEVTREE";
    print_process_memory();
    // PDBG_DTB environment variable set to CEC device tree path
    if (setenv("PDBG_DTB", devtree, 1))
    {
	std::cerr << "Failed to set PDBG_DTB: " << strerror(errno) << std::endl;
        return 0;
    }

    constexpr auto PDATA_INFODB_PATH = "/usr/share/pdata/attributes_info.db";
    // PDATA_INFODB environment variable set to attributes tool  infodb path
    if (setenv("PDATA_INFODB", PDATA_INFODB_PATH, 1))
    {
        std::cerr << "Failed to set PDATA_INFODB: ({})" << strerror(errno) << std::endl;
        return 0;
    }

	std::cerr << "waiting to collect top data" << std::endl;
    sleep(30);
	std::cerr << "waiting to collect top data" << std::endl;
    sleep(30);

    //initialize the targeting system 
    if (!pdbg_targets_init(NULL))
    {   
        std::cerr << "pdbg_targets_init failed" << std::endl;
        return 0;
    }

    // set log level and callback function
    pdbg_set_loglevel(PDBG_DEBUG);
    pdbg_set_logfunc(pdbgLogCallback);

    struct pdbg_target *proc;
    pdbg_for_each_target("proc", NULL, proc)
    {
		uint32_t proc_no = pdbg_target_index(pdbg_target_parent("proc",	proc));
        const char* targetPath = pdbg_target_path(proc);
        if( targetPath)
        {
            std::cout << "test proc " << proc_no << " pdbg target path is " << targetPath << std::endl;
        }
        uint32_t mruid=0;
        if (pdbg_target_get_attribute(proc, "ATTR_MRU_ID", 4, 1,
  				       &mruid)) 
        {
            std::cout << "mruid value read is " << mruid << std::endl;
        }
    }
	std::cerr << "waiting to collect top data" << std::endl;
    sleep(30);
	std::cerr << "waiting to collect top data" << std::endl;
    sleep(30);
    print_process_memory();
    return 0;
}
