#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension

setup(name="VehicleSegmentation",
    ext_modules=[
        Extension("BackgroundModel", ["src/include/background_estimator.cpp"],
        include_dirs = ['/usr/local/include','/usr/local/include/python2.7','src/include'],
        library_dirs=['/usr/local/lib'],
        libraries = ["boost_python","opencv_calib3d" ,"opencv_contrib" ,"opencv_core" ,"opencv_features2d" ,
        "opencv_flann" ,"opencv_gpu" ,"opencv_highgui" ,"opencv_imgproc" ,
        "opencv_legacy" ,"opencv_ml" ,"opencv_nonfree" ,"opencv_objdetect" ,
        "opencv_ocl" ,"opencv_photo" ,"opencv_stitching" ,"opencv_superres" ,
        "opencv_ts" ,"opencv_video" ,"opencv_videostab"]),
        Extension("Classifier", ["src/include/classifier.cpp"],
        include_dirs = ['/usr/local/include','/usr/local/include/python2.7','src/include'],
        library_dirs=['/usr/local/lib'],
        libraries = ["boost_python","opencv_calib3d" ,"opencv_contrib" ,"opencv_core" ,"opencv_features2d" ,
        "opencv_flann" ,"opencv_gpu" ,"opencv_highgui" ,"opencv_imgproc" ,
        "opencv_legacy" ,"opencv_ml" ,"opencv_nonfree" ,"opencv_objdetect" ,
        "opencv_ocl" ,"opencv_photo" ,"opencv_stitching" ,"opencv_superres" ,
        "opencv_ts" ,"opencv_video" ,"opencv_videostab"]),
        Extension("Filtering", ["src/include/filtering.cpp"],
        include_dirs = ['/usr/local/include','/usr/local/include/python2.7','src/include'],
        library_dirs=['/usr/local/lib'],
        libraries = ["boost_python","opencv_calib3d" ,"opencv_contrib" ,"opencv_core" ,"opencv_features2d" ,
        "opencv_flann" ,"opencv_gpu" ,"opencv_highgui" ,"opencv_imgproc" ,
        "opencv_legacy" ,"opencv_ml" ,"opencv_nonfree" ,"opencv_objdetect" ,
        "opencv_ocl" ,"opencv_photo" ,"opencv_stitching" ,"opencv_superres" ,
        "opencv_ts" ,"opencv_video" ,"opencv_videostab"]),
    ])


#from distutils.core import setup
#from Cython.Build import cythonize

#setup(ext_modules = cythonize(
#           "../background_estimator.pyx",                 # our Cython source
#           sources=["background_estimator.cpp"],  # additional source file(s)
#           language="c++",             # generate C++ code
#           library_dirs=['/usr/local/lib'],
#           libraries = ["opencv_calib3d" ,"opencv_contrib" ,"opencv_core" ,"opencv_features2d" ,
#           "opencv_flann" ,"opencv_gpu" ,"opencv_highgui" ,"opencv_imgproc" ,
#           "opencv_legacy" ,"opencv_ml" ,"opencv_nonfree" ,"opencv_objdetect" ,
#           "opencv_ocl" ,"opencv_photo" ,"opencv_stitching" ,"opencv_superres" ,
#           "opencv_ts" ,"opencv_video" ,"opencv_videostab"],
#      ))
