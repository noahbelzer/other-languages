/*
g++ test1.cpp -lOpenCL
./a.out
*/

#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <iostream>
using namespace std;

int main () {
    cl_uint platform_count;
    
    // “Don’t give me platforms yet — just tell me how many exist.”
    cl_int result = clGetPlatformIDs(
        0,               // num_entries = 0 → “I am not giving you space to store platforms”
        nullptr,         // platforms = nullptr → “Do not try to write platform IDs”
        &platform_count  // &platformCount → “Write the number of platforms into this variable”
    );

    if (result != CL_SUCCESS) {
        cerr << "Error retrieving platform ID: " << result << endl;
    }

    // CL_SUCCESS == 0
    cout << "Result: " << result << endl;

    cout << "platform_count: " << platform_count << endl;


    // platform_count = 1, so we only need space for one platform
    cl_platform_id array_of_cl_platform_id[1];

    // “Here is memory. Put the platform ID(s) here.”
    cl_int second_result = clGetPlatformIDs(
        platform_count,               // num_entries = 1 → “I can store one platform”
        array_of_cl_platform_id,      // pointer to valid storage
        nullptr                       // num_platforms = nullptr → “I don’t need the count again”
    );

    if (second_result != CL_SUCCESS) {
        cerr << "Error retrieving platform ID: " << second_result << endl;
    }

    

    size_t name_size;
    clGetPlatformInfo(
        array_of_cl_platform_id[0],
        CL_PLATFORM_NAME,
        0,                           // param_value_size = 0 → “I’m not giving you memory yet”
        nullptr,                     // param_value = nullptr → “don’t write yet”
        &name_size                   // param_value_size_ret = &name_size → OpenCL tells us how many bytes we need
    );

    char* platform_name = new char[name_size];
    cl_int info_result = clGetPlatformInfo(
        array_of_cl_platform_id[0],  // the platform handle
        CL_PLATFORM_NAME,            // property we want
        name_size,                   // size of buffer
        platform_name,               // pointer to buffer
        nullptr                      // we already know the size
    );

    cout << "Platform name: " << platform_name << endl;
    delete[] platform_name;  // avoid memory leak
    return 0;
}