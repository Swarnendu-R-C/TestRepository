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
            std::cout << "original mruid value read is " << mruid << std::endl;
        }
        /*
        uint32_t test= 0x1;
        for(int i = 0; i < 20; i++)
        {
            test += 1;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &test)) 
            {
                std::cout << "MRU id value set is " << std::hex << test << std::endl;
            }
            sleep(1);
        }
        break;
        */
        if(strcmp(targetPath, "/proc0") == 0)
        {
            mruid = 0x10000;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }
        if(strcmp(targetPath, "/proc1") == 0)
        {
            mruid = 0x10001;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }
        if(strcmp(targetPath, "/proc2") == 0)
        {
            mruid = 0x10000;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }
        if(strcmp(targetPath, "/proc3") == 0)
        {
            mruid = 0x10001;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }

        if(strcmp(targetPath, "/proc4") == 0)
        {
            mruid = 0x10000;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }
        if(strcmp(targetPath, "/proc5") == 0)
        {
            mruid = 0x10001;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }
        if(strcmp(targetPath, "/proc6") == 0)
        {
            mruid = 0x10000;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }
        if(strcmp(targetPath, "/proc7") == 0)
        {
            mruid = 0x10001;
            if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                       &mruid)) 
            {
                std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
            }
        }

        /*
        if (pdbg_target_set_attribute(proc, "ATTR_MRU_ID", 4, 1,
                   &mruid)) 
        {
            std::cout << "Writing back original mru id value" << std::hex << mruid << std::endl;
        }*/
    }
    return 0;
}
