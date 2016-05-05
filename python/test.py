import sys
sys.path
sys.path.append('./build/lib.linux-x86_64-2.7')

import ctypes
import PixelBackgroundModel as bm
import cv2
import numpy as np
import numpy.ctypeslib as npct

cap = cv2.VideoCapture(0)

rt,img = cap.read()
output = np.zeros(shape=(480,640), dtype=np.uint8)+255
thresh = np.ones(shape=(480,640), dtype=np.float)*5.0
pbm = bm.PixelBackgroundModel(0.001,40,9.0,0.97,11.0,0.05,7.0,4,True,0.5,3,640,480,640*480,False)

print img.shape
ptr_img = ctypes.addressof(img.ctypes.data_as(ctypes.c_void_p))
ptr_output = ctypes.addressof(output.ctypes.data_as(ctypes.c_void_p))
ptr_thresh = ctypes.addressof(thresh.ctypes.data_as(ctypes.c_void_p))

while (cv2.waitKey(1)!=27):
    rt,img = cap.read()
    pbm.backgroundModel(ptr_img,ptr_output,ptr_thresh)
    cv2.imshow("img",img)
    cv2.imshow("output",output)
