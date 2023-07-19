# MKL-Dfti-inplace
Forward FFT calculation using Intel oneAPI Math Kernel Library with inplace and not-inplace setting 

Add MKL library into project in VS:
1. Add MKL library "include" directory into project property > C/C++ > General > Additional Include Directories
2. Add MKL library "lib" directory into project property > Linker > General > Additional Library Directories
3. Add mkl_core_dll.lib, mkl_intel_lp64_dll.lib, and mkl_intel_thread_dll.lib into project property > Linker > Input > Additional Dependencies

