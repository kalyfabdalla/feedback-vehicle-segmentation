#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension

includes_path = ['/usr/local/include','/usr/local/include/python2.7','/vsf/src/include','/boost/python/include']
libraries_path = ['/usr/lib','/usr/local/lib']
libraries = ["python2.7","boost_python","opencv_calib3d" ,"opencv_contrib" ,"opencv_core" ,"opencv_features2d" ,
        "opencv_flann" ,"opencv_gpu" ,"opencv_highgui" ,"opencv_imgproc" ,
        "opencv_legacy" ,"opencv_ml" ,"opencv_objdetect" ,
        "opencv_ocl" ,"opencv_photo" ,"opencv_stitching" ,"opencv_superres" ,
        "opencv_ts" ,"opencv_video" ,"opencv_videostab"]

setup(name="VehicleSegmentation",
    ext_modules=[
        Extension("BackgroundModel", ["/vsf/src/include/background_estimator.cpp"],
        include_dirs = includes_path,
        library_dirs = libraries_path,
        libraries = libraries),
        Extension("Classifier", ["/vsf/src/include/classifier.cpp"],
        include_dirs = includes_path,
        library_dirs = libraries_path,
        libraries = libraries),
        Extension("Filtering", ["/vsf/src/include/filtering.cpp"],
        include_dirs = includes_path,
        library_dirs = libraries_path,
        libraries = libraries),
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
