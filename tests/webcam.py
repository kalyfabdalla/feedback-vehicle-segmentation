import sys
sys.path
sys.path.append('./build/lib.linux-x86_64-2.7')

import ctypes
import BackgroundModel as bm
import Classifier as cf
import cv2
import numpy as np
import numpy.ctypeslib as npct

cap = cv2.VideoCapture(0)

rt,img = cap.read()
output = np.zeros(shape=(480,640), dtype=np.uint8)
thresh = np.zeros(shape=(480,640,3), dtype=np.float32)
cf1 = np.zeros(shape=(480,640), dtype=np.uint8)
cf2 = np.zeros(shape=(480,640), dtype=np.uint8)
cf3 = np.zeros(shape=(480,640), dtype=np.uint8)
cfu = np.zeros(shape=(480,640,3), dtype=np.uint8)

pbm = bm.PixelBackgroundModel(0.01,25.0,9.0,0.97,11.0,0.05,7.0,4,True,0.5,3,640,480,640*480,False)
pcf = cf.Classifier()

while (cv2.waitKey(1)!=27):
    rt,img = cap.read()
    pbm.wrapBackgroundModel(img,output,thresh)

    c1,c2,c3 = (thresh[:,:,0]*255).astype(np.uint8),(thresh[:,:,1]*255).astype(np.uint8),(thresh[:,:,2]*255).astype(np.uint8)

    pcf.wrapThreshold(c1,cf1,4)
    pcf.wrapThreshold(c2,cf2,4)
    pcf.wrapThreshold(c3,cf3,4)

    cfu[:,:,0],cfu[:,:,1],cfu[:,:,2] = cf1,cf2,cf3

    cv2.imshow("thresh",thresh)
    cv2.imshow("cfu",cfu)
