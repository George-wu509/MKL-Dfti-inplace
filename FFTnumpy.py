import numpy as np
import cv2 as cv

img = [[-0.49, 2.52,-1.98,-1.48, 1.52, 0.76],
        [-1.48, 0.04, 3.04,-0.96,-3.96,-1.98],
        [-1.98,-3.96, 2.04, 5.04, 2.04, 1.02],
        [-0.99,-1.98, 1.02, 2.52, 1.02, 0.51],
        [0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
        [0.00, 0.00, 0.00, 0.00, 0.00, 0.00]]

# 2D FFT using opencv
output_cv = cv.dft(np.float32(img), flags=cv.DFT_COMPLEX_OUTPUT)

# 2D FFT using numpy
output_numpy = np.fft.fft2(img)


