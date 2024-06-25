#pragma once

#include "windows.h"
#include "psapi.h"
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;


#include "nonogram.h" 

void init(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double getCurrentValue(){
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
        (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}

void runCoveragePercentageSize(uintmax_t iterationsPerSize, uint16_t size){
double_t coveragePercentage = 0;
    std::chrono::microseconds duration{};

    SIZE_T virtualMemory{};
    SIZE_T physicalMemory{};
    SIZE_T pageFaults{};
    SIZE_T pagedPoolUsage{};
    SIZE_T nonPagedPoolUsage{};
    SIZE_T pageFile{};

    double_t cpuUsage{};

    init(); // initialize the 
    for(uintmax_t i = 0; i < iterationsPerSize; i++){
        nonogram* nonoCoverage = new nonogram(size, size);

        //
        ///TIME , DURATION
        //

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        nonoCoverage->solveLogicMethod(&nonoCoverage->nonoWorking);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        duration = duration + std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        //
        ///COVERAGE
        //
        coveragePercentage += nonoCoverage->currentProgress();

        //
        ///CPU Utilization
        //

        cpuUsage += getCurrentValue();

        //
        ///MEMORY
        //
        PROCESS_MEMORY_COUNTERS_EX pmc;
        HANDLE hProcess = GetCurrentProcess();

        GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

        virtualMemory = virtualMemory + pmc.PrivateUsage;
        physicalMemory = physicalMemory + pmc.PeakWorkingSetSize; // measuring max up to here instead of current BigO() type of calculation
        pageFaults = pageFaults + pmc.PageFaultCount;
        pagedPoolUsage = pagedPoolUsage + pmc.QuotaPeakPagedPoolUsage;
        nonPagedPoolUsage = nonPagedPoolUsage + pmc.QuotaPeakNonPagedPoolUsage;
        pageFile = pageFile + pmc.PeakPagefileUsage;

        CloseHandle(hProcess);

        

        delete nonoCoverage;
    }
    duration /= iterationsPerSize;
    
    coveragePercentage =  coveragePercentage/( (double_t)iterationsPerSize );

    virtualMemory /= (double_t)iterationsPerSize;
    physicalMemory /= (double_t)iterationsPerSize;
    pageFaults /= (double_t)iterationsPerSize;
    pagedPoolUsage /= (double_t)iterationsPerSize;
    nonPagedPoolUsage /= (double_t)iterationsPerSize;
    pageFile /= (double_t)iterationsPerSize;

    cpuUsage /= iterationsPerSize;

    std::cout << "Size," << size << ",Coverage percentage," << coveragePercentage << ",Duration," << duration.count() <<",PhysicalMemory," << physicalMemory << ",VirtualMemory," << virtualMemory << ",PageFaults," << pageFaults << ",PagedPoolUsage," << pagedPoolUsage << ",nonPagedPoolUsage," << nonPagedPoolUsage << ",PageFile," << pageFile << ",CPU," << cpuUsage << std::endl;
}

void runCoveragePercentage(uintmax_t iterationsPerSize){

    for(uint16_t size = 4; size < 100; size++){
        runCoveragePercentageSize(iterationsPerSize, size);
    }
    for(uint16_t size = 100; size < 1001; size = size + 50){
        runCoveragePercentageSize(iterationsPerSize, size);
    }
}

void runDFSBenchmark(uintmax_t iterationsPerSize){
    
    for(uint16_t size = 4; size < 101; size++){
        std::chrono::microseconds duration{};

        SIZE_T virtualMemory{};
        SIZE_T physicalMemory{};
        SIZE_T pageFaults{};
        SIZE_T pagedPoolUsage{};
        SIZE_T nonPagedPoolUsage{};
        SIZE_T pageFile{};

        double_t cpuUsage{};
        init(); // initialize the 
        for(uintmax_t i = 0; i < iterationsPerSize; i++){
            nonogram* nonoDFS = new nonogram(size, size);

            //
            ///TIME , DURATION
            //

            std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            nonoDFS->solveDFS();
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            duration = duration + std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            //
            ///CPU Utilization
            //

            cpuUsage += getCurrentValue();

            //
            ///MEMORY
            //
            PROCESS_MEMORY_COUNTERS_EX pmc;
            HANDLE hProcess = GetCurrentProcess();

            GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

            virtualMemory = virtualMemory + pmc.PrivateUsage;
            physicalMemory = physicalMemory + pmc.PeakWorkingSetSize; // measuring max up to here instead of current BigO() type of calculation
            pageFaults = pageFaults + pmc.PageFaultCount;
            pagedPoolUsage = pagedPoolUsage + pmc.QuotaPeakPagedPoolUsage;
            nonPagedPoolUsage = nonPagedPoolUsage + pmc.QuotaPeakNonPagedPoolUsage;
            pageFile = pageFile + pmc.PeakPagefileUsage;

            CloseHandle(hProcess);

            

            delete nonoDFS;
        }
        duration /= iterationsPerSize;

        virtualMemory /= (double_t)iterationsPerSize;
        physicalMemory /= (double_t)iterationsPerSize;
        pageFaults /= (double_t)iterationsPerSize;
        pagedPoolUsage /= (double_t)iterationsPerSize;
        nonPagedPoolUsage /= (double_t)iterationsPerSize;
        pageFile /= (double_t)iterationsPerSize;

        cpuUsage /= iterationsPerSize;
        std::cout << "Size," << size << ",Duration," << duration.count() <<",PhysicalMemory," << physicalMemory << ",VirtualMemory," << virtualMemory << ",PageFaults," << pageFaults << ",PagedPoolUsage," << pagedPoolUsage << ",nonPagedPoolUsage," << nonPagedPoolUsage << ",PageFile," << pageFile << ",CPU," << cpuUsage <<  std::endl;
    }
}

void getStandardDeviationDuration(uintmax_t iterationsPerSize, uint16_t size = 1000){ 
    
        std::chrono::microseconds duration{};

        uintmax_t n = 0;
        double_t mean = 0.0;
        double_t M2 = 0.0;

        init(); // initialize the 
        for(uintmax_t i = 0; i < iterationsPerSize; i++){
            nonogram* nonoCoverage = new nonogram(size, size);

            //
            ///TIME , DURATION
            //

            std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            nonoCoverage->solveLogicMethod(&nonoCoverage->nonoWorking);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            duration = duration + std::chrono::duration_cast<std::chrono::microseconds>(end - start);


            //
            /// Welford's online algorithm for updating mean and M2 (thanks wikipedia)
            //
            n++;
            double delta = duration.count() - mean;
            mean += delta / n;
            double delta2 = duration.count() - mean;
            M2 += delta * delta2;

            

            delete nonoCoverage;
        }
        duration /= iterationsPerSize;
        

        double variance = (n > 1) ? M2 / (n - 1) : 0.0;
        double standardDeviation = std::sqrt(variance);

        std::cout << "Size," << size << "Duration," << duration.count() << "standardDeviation:" << standardDeviation << std::endl;
    
}

//Comment or delete all below if it produces any problems, as I can't distribute the dataSetNonograms file
#include "dataSetNonograms.h"
void runWebpnDataset(){
    nonogram* webpnHere;
    std::chrono::microseconds duration{};

    webpnHere = new nonogram(webpbn1);
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#1," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn6);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#6," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn21);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#21," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn27);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#27," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn23);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#23," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn2413);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#2413," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn16);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#16," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn529);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#529," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn65);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#65," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn7604);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#7604," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn1694);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#1694," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn1611);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#1611," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn436);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#436," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn4645);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#4645," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn3541);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#3541," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn803);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#803," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn6574);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#6574," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn10810);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#10810," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn2040);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#2040," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn6739);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#6739," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn8098);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#8098," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn2556);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#2556," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn2712);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#2712," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn10088);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#10088," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn18297);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#18297," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn9892);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#9892," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn12548);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#12548," << duration.count();
    delete webpnHere;

    webpnHere = new nonogram(webpbn22336);
    start = std::chrono::steady_clock::now();
    webpnHere->solveDFS();
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "#22336," << duration.count();
    delete webpnHere;
}