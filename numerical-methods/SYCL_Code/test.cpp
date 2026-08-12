/*
Set-up: 
Download the toolkit
- Go to Intel oneAPI Base Toolkit: https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html
- Choose Windows → offline installer.

- Install Miscrosoft Visual Studio Community. During the installation, download "Desktop development with C++""
- Set-up the oneAPI exe

- To connect to GitHub, Download Git for Windows at https://git-scm.com/install/windows

- Go the start menu and open: Intel oneAPI command prompt for Intel 64 for Visual Studio 2026
*/

#include <CL/sycl.hpp>
#include <iostream>

int main() {
    // CPU backend
    sycl::queue q(sycl::cpu_selector{});
    std::vector<int> data(5);

    {
        sycl::buffer<int> buf(data.data(), data.size());
        q.submit([&](sycl::handler &h) {
            auto acc = buf.get_access<sycl::access::mode::write>(h);
            h.parallel_for(data.size(), [=](int i) {
                acc[i] = i * i; // CUDA-style kernel
            });
        });
    }

    for(auto x : data) std::cout << x << " ";
    std::cout << std::endl;
}