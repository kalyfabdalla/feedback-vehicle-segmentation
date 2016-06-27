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
param_output = None
wrt = None
fps = -1

parser = argparse.ArgumentParser(description='Press \'esc\' (escape) to exit .')
parser.add_argument('-i','--input', help='Input file name ou device id (e.g. 0 or video.mpg)',required=True)
parser.add_argument('-o','--output',help='Output file name', required=False)
parser.add_argument('-d','--descriptors',action='store_true',help='The output is the descriptors', required=False)
parser.add_argument('-l1','--dthr_l1',help='Upper value of Mahalanobis threshold', required=False)
parser.add_argument('-l2','--dthr_l2',help='Lower value of Mahalanobis threshold', required=False)
parser.add_argument('-cd','--tau_cd',help='Value of Chrominance distortion threshold', required=False)
parser.add_argument('-lo','--tau_lo',help='Value of Luminance distortion threshold', required=False)

args = parser.parse_args()
if args.dthr_l1:
    param_l1 = float(args.dthr_l1)
if args.dthr_l1:
    param_l2 = float(args.dthr_l2)
if args.tau_cd:
    param_tau_cd = float(args.tau_cd)
if args.tau_lo:
    param_tau_lo = float(args.tau_lo)
if args.output:
    param_output = args.output

param_input = args.input
param_output = args.output

cap = None

try:
    param_input = int(param_input)
    cap = cv2.VideoCapture(param_input)
    fps = 30
except ValueError:
    print param_input
    cap = cv2.VideoCapture(param_input)
    int(cap.get(cv2.cv.CV_CAP_PROP_FPS))
    pass

wid = int(cap.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH)) #3
hei = int(cap.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT)) #4

if args.output:
    fourcc = cv2.cv.CV_FOURCC(*'MPEG')
    wrt = cv2.VideoWriter(param_output,fourcc,fps,(wid,hei))

rt,img = cap.read()
output = np.zeros(shape=(hei,wid), dtype=np.uint8)
thresh = np.zeros(shape=(hei,wid,3), dtype=np.float32)

cf1 = np.zeros(shape=(hei,wid), dtype=np.uint8)
cf2 = np.zeros(shape=(hei,wid), dtype=np.uint8)
cf3 = np.zeros(shape=(hei,wid), dtype=np.uint8)

f1 = np.zeros(shape=(hei,wid), dtype=np.uint8)
f2 = np.zeros(shape=(hei,wid), dtype=np.uint8)
f3 = np.zeros(shape=(hei,wid), dtype=np.uint8)

cfu = np.zeros(shape=(hei,wid,3), dtype=np.uint8)
ctu = np.zeros(shape=(hei,wid,3), dtype=np.uint8)

pbm = bm.PixelBackgroundModel(0.01,9.0,16.0,0.97,9.0,4.0,16.0,0.05,7.0,4,True,0.5,3,wid,hei,wid*hei,False)
pcf = cf.Classifier()
ft = filt.Filtering()

thresh_mahala = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_l1/7.0)*255))
thresh_chroma = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_tau_cd/7.0)*255))
thresh_lumina = np.zeros(shape=(hei,wid), dtype=np.uint8)+int((param_tau_lo*255))

while (cv2.waitKey(1)!=27):
    rt,img = cap.read()
    pbm.wrapBackgroundModel(img,output,thresh)

    c1 = (thresh[:,:,0]*255).astype(np.uint8)
    c2 = (thresh[:,:,1]*255).astype(np.uint8)
    c3 = (thresh[:,:,2]*255).astype(np.uint8)

    pcf.wrapThresholdMatrix(c1,thresh_mahala,cf1,cf.THRESHOLD_TYPE_BINARY)
    pcf.wrapThresholdMatrix(c2,thresh_chroma,cf2,cf.THRESHOLD_TYPE_BINARY)
    pcf.wrapThresholdMatrix(c3,thresh_lumina,cf3,cf.THRESHOLD_TYPE_BINARY)

    if args.descriptors:
        cfu[:,:,0],cfu[:,:,1],cfu[:,:,2] = cf1,cf2,cf3
    else:
        ax = np.bitwise_and(cf1,np.bitwise_or(cf2,cf3))
        cfu[:,:,0],cfu[:,:,1],cfu[:,:,2] = ax,ax,ax

    #print wrt
    if wrt:
        wrt.write(cfu)

    cv2.imshow("classification",cfu)

    ft.wrapFilter(cf1,f1,1),ft.wrapFilter(cf2,f2,1),ft.wrapFilter(cf3,f3,1)

    thresh_mahala = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_l1/7.0)*255))
    thresh_chroma = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_tau_cd/7.0)*255))
    thresh_lumina = np.zeros(shape=(hei,wid), dtype=np.uint8)+int((param_tau_lo*255))


    #ft.wrapFillConvexHull(f1,thresh_mahala,int(((param_l2/7.0)*255)))
    #ft.wrapFillConvexHull(f2,thresh_chroma,int(((param_tau_cd/7.0)*255)))
    #ft.wrapFillConvexHull(f3,thresh_lumina,int(param_tau_lo*255))

    ft.wrapFillContours(f1,thresh_mahala,int(((param_l2/7.0)*255)))
    ft.wrapFillContours(f1,thresh_chroma,int(((param_l2/7.0)*255)))
    ft.wrapFillContours(f3,thresh_lumina,int(0.3*255))

    ctu[:,:,0],ctu[:,:,1],ctu[:,:,2] = thresh_mahala,thresh_chroma,thresh_lumina
    #cv2.imshow("filtering",ctu)

    #cv2.imshow("mahala",thresh_mahala)
    #cv2.imshow("chroma",thresh_chroma)
    #cv2.imshow("lumina",thresh_lumina)



print "Releasing..."
if wrt:
    wrt.release()
cap.release()
