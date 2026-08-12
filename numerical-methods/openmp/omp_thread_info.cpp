// Test to ensure the CPU cores are active
// Compile: g++ -fopenmp -O2 omp_thread_info.cpp -o omp_thread_info
// Run using 16 physical cores: OMP_NUM_THREADS=16 ./omp_thread_info
#include <iostream>
#include <omp.h>
#include <vector>
#include <iomanip>

int main() {
    std::cout << "\n========== OpenMP Thread Information ==========\n" << std::endl;
    
    // Get system information
    int max_threads = omp_get_max_threads();
    int num_procs = omp_get_num_procs();
    
    std::cout << "System Information:" << std::endl;
    std::cout << "  Available CPU cores/threads: " << num_procs << std::endl;
    std::cout << "  Max OpenMP threads available: " << max_threads << std::endl;
    std::cout << std::endl;
    
    // Test with different thread counts
    std::vector<int> thread_counts = {1, 2, 4, max_threads};
    
    for (int num_threads : thread_counts) {
        if (num_threads > max_threads) continue;
        
        std::cout << "--- Running with " << num_threads << " thread(s) ---" << std::endl;
        
        omp_set_num_threads(num_threads);
        
        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();
            int total_threads = omp_get_num_threads();
            
            // Each thread prints its info (only first thread prints header)
            #pragma omp single
            {
                std::cout << "Active threads: " << total_threads << std::endl;
            }
            
            #pragma omp critical
            {
                std::cout << "  Thread " << std::setw(2) << thread_id 
                          << " / " << (total_threads - 1) 
                          << " is running on CPU" << std::endl;
            }
        }
        std::cout << std::endl;
    }
    
    // Intensive parallel region to see all threads working
    std::cout << "--- All cores at work (using max threads) ---" << std::endl;
    omp_set_num_threads(max_threads);
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        
        // Simulate some work
        double sum = 0.0;
        #pragma omp for
        for (int i = 0; i < 100000000; i++) {
            sum += i * 0.1;
        }
        
        #pragma omp critical
        {
            std::cout << "  Thread " << std::setw(2) << thread_id 
                      << " completed work (sum=" << sum << ")" << std::endl;
        }
    }
    
    std::cout << "\n=========== End of Information ===========\n" << std::endl;
    
    return 0;
}
