#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension

setup(name="BackgroundEstimator",
    ext_modules=[
        Extension("bgest", ["background_estimator.cpp"],
        library_dirs=['/usr/local/lib'],
        libraries = ["boost_python","opencv_calib3d" ,"opencv_contrib" ,"opencv_core" ,"opencv_features2d" ,
        "opencv_flann" ,"opencv_gpu" ,"opencv_highgui" ,"opencv_imgproc" ,
        "opencv_legacy" ,"opencv_ml" ,"opencv_nonfree" ,"opencv_objdetect" ,
        "opencv_ocl" ,"opencv_photo" ,"opencv_stitching" ,"opencv_superres" ,
        "opencv_ts" ,"opencv_video" ,"opencv_videostab"])
    ])
