#!/usr/bin/python

__author__ = 'Lima, K. A. B.'

import sys, argparse
sys.path
sys.path.append('./build/lib.linux-x86_64-2.7')

from os import listdir
from os.path import isfile, join
import ctypes
import BackgroundModel as bm
import Classifier as cf
import Filtering as filt
import cv2
import numpy as np
import numpy.ctypeslib as npct
import string

param_l1 = 4.0
param_l2 = 1.2
param_tau_cd = 4.0
param_tau_lo = 0.5
param_input = 0
param_output = None
wrt = None
fps = 30
param_shadow = 1
delay = 1

parser = argparse.ArgumentParser(description='Press \'esc\' (escape) to exit .')
parser.add_argument('-i','--input', help='Input file name ou device id (e.g. 0 or video.mpg)',required=True)
parser.add_argument('-o','--output',help='Output file name', required=False)
parser.add_argument('-d','--descriptors',action='store_true',help='The output is the descriptors', required=False)
parser.add_argument('-l1','--dthr_l1',help='(Default 4.0) Upper value of Mahalanobis threshold', required=False)
parser.add_argument('-l2','--dthr_l2',help='(Default 1.2) Lower value of Mahalanobis threshold', required=False)
parser.add_argument('-cd','--tau_cd',help='(Default 4.0) Value of Chrominance distortion threshold', required=False)
parser.add_argument('-lo','--tau_lo',help='(Default 0.5) Value of Luminance distortion threshold', required=False)
parser.add_argument('-f','--folders',action='store_true',help='The input and output are folders', required=False)
parser.add_argument('-uh','--use_convexhull',action='store_true',help='Use convex hulls to select threshold', required=False)
parser.add_argument('-uc','--use_contour',action='store_true',help='Use contours to select threshold', required=False)
parser.add_argument('-v','--verbose',action='store_true',help='Display images', required=False)
parser.add_argument('-s','--shadow',help='1-Estimate shadow; 2-Incorporate shadow; 3-Remove shadow', required=True)
parser.add_argument('-delay',action='store_true',help='Increase delay between frames', required=False)

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
param_shadow = int(args.shadow)

cap = None

if args.folders:
    input_files = [f for f in listdir(param_input) if isfile(join(param_input, f))]
    img = cv2.imread(join(param_input, input_files[0]),1)
    wid = int(img.shape[1]) #3
    hei = int(img.shape[0]) #4
    total_size = len(input_files)
else:
    try:
        param_input = int(param_input)
        cap = cv2.VideoCapture(param_input)
    except ValueError:
        cap = cv2.VideoCapture(param_input)
        int(cap.get(cv2.cv.CV_CAP_PROP_FPS))
        pass
    wid = int(cap.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH)) #3
    hei = int(cap.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT)) #4

if args.output and not args.folders:
    fourcc = cv2.cv.CV_FOURCC(*'MPEG')
    wrt = cv2.VideoWriter(param_output,fourcc,fps,(wid,hei))

if args.delay:
    delay=100

#rt,img = cap.read()
output = np.zeros(shape=(hei,wid), dtype=np.uint8)
thresh = np.zeros(shape=(hei,wid,3), dtype=np.float32)
#thresh = np.zeros(shape=(hei,wid,3), dtype=np.uint8)

cf1 = np.zeros(shape=(hei,wid), dtype=np.uint8)
cf2 = np.zeros(shape=(hei,wid), dtype=np.uint8)
cf3 = np.zeros(shape=(hei,wid), dtype=np.uint8)

f1 = np.zeros(shape=(hei,wid), dtype=np.uint8)
f2 = np.zeros(shape=(hei,wid), dtype=np.uint8)
f3 = np.zeros(shape=(hei,wid), dtype=np.uint8)

cfu = np.zeros(shape=(hei,wid,3), dtype=np.uint8)
ctu = np.zeros(shape=(hei,wid,3), dtype=np.uint8)

minus_mask_shadow = np.zeros(shape=(hei,wid), dtype=np.uint8)+205

pbm = bm.PixelBackgroundModel(0.01,16.0,16.0,0.97,9.0,3.0,25.0,0.05,7.0,4,True,0.5,3,wid,hei,wid*hei,False)
pcf = cf.Classifier()
ft = filt.Filtering()

thresh_mahala = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_l1/7.0)*255))
thresh_chroma = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_tau_cd/7.0)*255))
thresh_lumina = np.zeros(shape=(hei,wid), dtype=np.uint8)+int((param_tau_lo*255))

while (True):
    img = None
    img_filename = None
    if args.folders:
        if len(input_files)==0:
            sys.stdout.write('\r100.00%\r')
            sys.stdout.flush()
            break
        else:
            img_filename = input_files.pop(0)
            img = cv2.imread(join(param_input, img_filename),1)
            pct = (total_size-float(len(input_files)))/total_size
            sys.stdout.write('\r%.2f%%\r' % (pct*100))
            sys.stdout.flush()
    else:
        rt,img = cap.read()
        if not rt:
            break

    pbm.wrapBackgroundModel(img,output,thresh)

    c1 = (thresh[:,:,0]*255).astype(np.uint8)
    c2 = (thresh[:,:,1]*255).astype(np.uint8)
    c3 = (thresh[:,:,2]*255).astype(np.uint8)

    pcf.wrapThresholdMatrix(c1,thresh_mahala,cf1,cf.THRESHOLD_TYPE_BINARY)
    pcf.wrapThresholdMatrix(c2,thresh_chroma,cf2,cf.THRESHOLD_TYPE_BINARY)
    pcf.wrapThresholdMatrix(c3,thresh_lumina,cf3,cf.THRESHOLD_TYPE_BINARY_INV)

    if args.verbose:
        cv2.imshow("mahala",cf1)
        cv2.imshow("chroma",cf2)
        cv2.imshow("lumina",cf3)


    if args.descriptors:
        cfu[:,:,0],cfu[:,:,1],cfu[:,:,2] = cf1,cf2,cf3
    else:
        img_shadow = np.bitwise_and(np.bitwise_and(np.bitwise_and(np.bitwise_not(cf2),cf3),cf1),minus_mask_shadow)
        ax = cf1-img_shadow
        if param_shadow==2:
            rt,ax = cv2.threshold(ax,100,255,cv2.THRESH_BINARY)
        elif param_shadow==3:
            rt,ax = cv2.threshold(ax,140,255,cv2.THRESH_BINARY)
        cfu[:,:,0],cfu[:,:,1],cfu[:,:,2] = ax,ax,ax


    if args.output:
        if not args.folders:
            wrt.write(cfu)
        else:
            replc = string.replace(img_filename,'in','out')
            cocnt = 'out'+img_filename
            if img_filename==replc:
                cv2.imwrite(join(param_output, '.'.join(cocnt.split('.')[0:-1])+'.png'),cfu)
            else:
                cv2.imwrite(join(param_output, '.'.join(replc.split('.')[0:-1])+'.png'),cfu)

    if args.verbose:
        cv2.imshow("classification",cfu)

    ft.wrapFilter(cf1,f1,1),ft.wrapFilter(cf2,f2,1),ft.wrapFilter(cf3,f3,1)

    thresh_mahala = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_l1/7.0)*255))
    thresh_chroma = np.zeros(shape=(hei,wid), dtype=np.uint8)+int(((param_tau_cd/7.0)*255))
    thresh_lumina = np.zeros(shape=(hei,wid), dtype=np.uint8)+int((param_tau_lo*255))

    if args.use_convexhull:
        ft.wrapFillConvexHull(f2,thresh_mahala,int(((param_l2/7.0)*255)))
        ft.wrapFillConvexHull(f2,thresh_chroma,int(((param_l2/7.0)*255)))
        #ft.wrapFillConvexHull(f2,thresh_lumina,int(0.3*255))
    elif args.use_contour:
        ft.wrapFillContours(f2,thresh_mahala,int(((param_l2/7.0)*255)))
        ft.wrapFillContours(f2,thresh_chroma,int(((param_l2/7.0)*255)))
        #ft.wrapFillContours(f2,thresh_lumina,int(0.3*255))

    ctu[:,:,0],ctu[:,:,1],ctu[:,:,2] = thresh_mahala,thresh_chroma,thresh_lumina
    cv2.imshow("filtering",ctu)


    wKey = cv2.waitKey(delay)
    if wKey==27:
        break

#print "Releasing..."
if wrt:
    wrt.release()
if not args.folders:
    cap.release()
