# distutils: language = c++
# distutils: sources = background_estimator.cpp


cdef extern from "background_estimator.hpp":
  cdef cppclass PixelBackgroundModel:
    PixelBackgroundModel() except +
    PixelBackgroundModel(float, float, float, float, float, float, float, int, bool, float, int, int, int, int, bool ) except +
    void setRoi(int, int, int, int)
    void backgroundModel(unsigned char*, unsigned char*, float*)
