#include <iostream>
#include <vector>
#include "mkl_dfti.h"

int main()
{
    size_t inplace_opt = 1; //inplace_opt = 0: DFTI_NOT_INPLACE, inplace_opt = 1: DFTI_INPLACE
    

    DFTI_DESCRIPTOR_HANDLE _fftDescriptor;
    size_t nDim = 2;
    uint32_t _nCols = 6;
    uint32_t _nRows = 4;
    long lengths[2]; lengths[0] = (long)_nCols; lengths[1] = (long)_nRows;
    long status;

    std::vector<float> tileData = {-0.49f, 2.52f,-1.98f,-1.48f, 1.52f, 0.76f,-1.48f, 0.04f, 3.04f,-0.96f,-3.96f,-1.98f,
                                   -1.98f,-3.96f, 2.04f, 5.04f, 2.04f, 1.02f,-0.99f,-1.98f, 1.02f, 2.52f, 1.02f, 0.51f,
                                    0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f};

    // Create the descriptor
    status = DftiCreateDescriptor(&_fftDescriptor, DFTI_SINGLE, DFTI_REAL, nDim, lengths);

    // SetValue DFTI_PLACEMENT using DFTI_NOT_INPLACE or DFTI_INPLACE
    if (inplace_opt == 0)
        status = DftiSetValue(_fftDescriptor, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
    else if (inplace_opt == 1)
        status = DftiSetValue(_fftDescriptor, DFTI_PLACEMENT, DFTI_INPLACE);
    else
        return false;

    // DftiSetValue and DftiCommitDescriptor
    int strides[3]; strides[0] = 0; strides[1] = 1; strides[2] = (int)_nCols;
    status = DftiSetValue(_fftDescriptor, DFTI_INPUT_STRIDES, strides);
    status = DftiSetValue(_fftDescriptor, DFTI_OUTPUT_STRIDES, strides);
    status = DftiCommitDescriptor(_fftDescriptor);
    if (!DftiErrorClass(status, DFTI_NO_ERROR))
        return false;


    // DftiComputeForward using DFTI_NOT_INPLACE or DFTI_INPLACE
    if (inplace_opt == 0) // DFTI_NOT_INPLACE
    {
        float* tileDataptr = &(tileData[0]);
        std::vector<float> tileData_output((_nRows + 2) * _nCols);
        status = DftiComputeForward(_fftDescriptor, (void*)(tileDataptr), (void*)(&tileData_output[0]));

        // Forward FFT result(DFTI_NOT_INPLACE) using MKL 2023.1.0:
        // tileData_output = 
        //            {  1.85f, 0.00f,-13.97f, 0.16f,-0.65f, 6.23f,-2.25f, 0.00f,-0.65f,-6.23f,-13.97f, -0.16f,
        //              -3.35f, 7.40f,  3.42f,-7.04f,-3.71f,-6.61f,-2.75f,-0.50f,-8.03f,11.12f, 23.36f, -1.43f,
        //               8.25f, 0.00f, -1.38f,11.47f,-1.12f,-6.71f,-3.25f, 0.00f,-1.13f, 6.71f, -1.38f,-11.47f};

        // Forward FFT result(DFTI_NOT_INPLACE) using MKL 2021.4.0:
        // tileData_output = 
        //            {  1.85f, 0.00f,-13.97f, 0.16f,-0.65f, 6.23f,-2.25f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
        //              -3.35f, 3.42f, -3.71f,-2.75f,-8.03f,23.36f, 7.40f,-7.04f,-6.61f,-0.50f,11.12f,-1.43f,
        //               8.25f, 0.00f, -1.38f,11.47f,-1.12f,-6.71f,-3.25f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f};
    }
    else if (inplace_opt == 1) // DFTI_INPLACE
    {
        float* tileDataptr = &(tileData[0]);
        status = DftiComputeForward(_fftDescriptor, (void*)(tileDataptr));

        // Forward FFT result(DFTI_INPLACE) using MKL 2023.1.0::
        // tileData = 
        //            {-10.00f, 0.00f, -7.12f,-5.14f,-3.10f,  1.61f, 0.80f, 0.00f,-3.10f,-1.61f,-7.12f, 5.14f,
        //               7.16f, 7.40f,-10.80f, 1.44f, 3.84f,  0.93f, 0.06f,-0.50f,-0.47f, 3.58f, 9.14f,-9.91f,
        //              -3.60f, 0.00f,  5.47f, 6.17f,-3.57f,-11.33f,-0.20f, 0.00f,-3.57f,11.33f, 5.47f,-6.17f};

        // Forward FFT result(DFTI_INPLACE) using MKL 2021.4.0:
        // tileData = 
        //            {  1.85f, 0.00f,-13.97f, 0.16f,-0.65f, 6.23f,-2.25f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
        //              -3.35f, 3.42f, -3.71f,-2.75f,-8.03f,23.36f, 7.40f,-7.04f,-6.61f,-0.50f,11.12f,-1.43f,
        //               8.25f, 0.00f, -1.38f,11.47f,-1.12f,-6.71f,-3.25f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f};
    }

}
