#!/usr/bin/python

__author__ = 'Lima, K. A. B.'

import sys, argparse
sys.path
sys.path.append('./build/lib.linux-x86_64-2.7')

import ctypes
import BackgroundModel as bm
import Classifier as cf
import Filtering as filt
import cv2
import numpy as np
import numpy.ctypeslib as npct

param_l1 = 4.0
param_l2 = 1.2
param_tau_cd = 4.0
param_tau_lo = 0.5
param_input = 0
param_output = 0

parser = argparse.ArgumentParser(description='This is a demo script by nixCraft.')
parser.add_argument('-i','--input', help='Input file name ou device id (e.g. 0 or video.mpg)',required=True)
parser.add_argument('-o','--output',help='Output file name', required=True)
parser.add_argument('-l1','--dthr_l1',help='Upper value of Mahalanobis threshold', required=False)
parser.add_argument('-l2','--dthr_l2',help='Lower value of Mahalanobis threshold', required=False)
parser.add_argument('-cd','--tau_cd',help='Value of Chrominance distortion threshold', required=False)
parser.add_argument('-lo','--tau_lo',help='Value of Luminance distortion threshold', required=False)

args = parser.parse_args()
if args.dthr_l1:
    param_l1 = args.dthr_l1
if args.dthr_l1:
    param_l2 = args.dthr_l2
if args.tau_cd:
    param_tau_cd = args.tau_cd
if args.tau_lo:
    param_tau_lo = args.tau_lo

param_input = args.input
param_output = args.output

try:
    param_input = int(param_input)
except ValueError:
    pass

cap = cv2.VideoCapture(param_input)

rt,img = cap.read()
output = np.zeros(shape=(480,640), dtype=np.uint8)
thresh = np.zeros(shape=(480,640,3), dtype=np.float32)

cf1 = np.zeros(shape=(480,640), dtype=np.uint8)
cf2 = np.zeros(shape=(480,640), dtype=np.uint8)
cf3 = np.zeros(shape=(480,640), dtype=np.uint8)

f1 = np.zeros(shape=(480,640), dtype=np.uint8)
f2 = np.zeros(shape=(480,640), dtype=np.uint8)
f3 = np.zeros(shape=(480,640), dtype=np.uint8)

#cfu = np.zeros(shape=(480,640,3), dtype=np.uint8)
ctu = np.zeros(shape=(480,640,3), dtype=np.uint8)

pbm = bm.PixelBackgroundModel(0.01,16.0,16.0,0.97,16.0,0.05,7.0,4,True,0.5,3,640,480,640*480,False)
pcf = cf.Classifier()
ft = filt.Filtering()

thresh_mahala = np.zeros(shape=(480,640), dtype=np.uint8)+int(((param_l1/7.0)*255))
thresh_chroma = np.zeros(shape=(480,640), dtype=np.uint8)+int(((param_tau_cd/7.0)*255))
thresh_lumina = np.zeros(shape=(480,640), dtype=np.uint8)+int((param_tau_lo*255))

while (cv2.waitKey(1)!=27):
    rt,img = cap.read()
    pbm.wrapBackgroundModel(img,output,thresh)

    c1 = (thresh[:,:,0]*255).astype(np.uint8)
    c2 = (thresh[:,:,1]*255).astype(np.uint8)
    c3 = (thresh[:,:,2]*255).astype(np.uint8)

    pcf.wrapThresholdMatrix(c1,thresh_mahala,cf1,cf.THRESHOLD_TYPE_BINARY)
    pcf.wrapThresholdMatrix(c2,thresh_chroma,cf2,cf.THRESHOLD_TYPE_BINARY)
    pcf.wrapThresholdMatrix(c3,thresh_lumina,cf3,cf.THRESHOLD_TYPE_BINARY)

    cfu = np.bitwise_and(cf1,np.bitwise_or(cf2,cf3))

    cv2.imshow("classification",cfu)

    ft.wrapFilter(cf1,f1,1),ft.wrapFilter(cf2,f2,1),ft.wrapFilter(cf3,f3,1)

    thresh_mahala = np.zeros(shape=(480,640), dtype=np.uint8)+int(((param_l1/7.0)*255))
    thresh_chroma = np.zeros(shape=(480,640), dtype=np.uint8)+int(((param_tau_cd/7.0)*255))
    thresh_lumina = np.zeros(shape=(480,640), dtype=np.uint8)+int((param_tau_lo*255))

    ft.wrapFillConvexHull(f1,thresh_mahala,int(((param_l2/7.0)*255)))
    ft.wrapFillConvexHull(f2,thresh_chroma,int(((param_tau_cd/7.0)*255)))
    ft.wrapFillConvexHull(f3,thresh_lumina,int(param_tau_lo*255))
