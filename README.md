# Vehicle Segmentation with Feedback

This code implements a vehicle segementation system improved by feedback.

[Paper](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=7424020) - Adaptive Method for Segmentation of Vehicles through Local Threshold in the Gaussian Mixture Model

## Citing

If you use this code in an academic context, please cite the following publication:

```bibtex
@INPROCEEDINGS{
7424020, 
author={K. A. B. Lima and K. R. T. Aires and F. W. P. D. Reis},
booktitle={2015 Brazilian Conference on Intelligent Systems (BRACIS)},
title={Adaptive Method for Segmentation of Vehicles through Local Threshold in the Gaussian Mixture Model}, 
year={2015}, 
pages={204-209}, 
keywords={Gaussian processes;image classification;image colour analysis;image segmentation;mixture models;road vehicles;traffic engineering computing;GMM;Gaussian mixture model;adaptive method;background regions;background subtraction;foreground regions;image classification;local threshold;nonlinear problem;probabilistic models;spatial analysis;traffic control;traffic scenes;vehicles color;vehicles segmentation;Adaptation models;Bayes methods;Estimation;Image segmentation;Roads;Vehicle dynamics;Vehicles;Gaussian mixture model;local threshold;vehicle segmentation}, 
doi={10.1109/BRACIS.2015.33}, 
month={Nov},
}
```

## Disclaimer

This is a research code, any fitness for a particular purpose is disclaimed.

## Licence

The source code is released under a GPLv3 licence. A closed-source professional edition is available for commercial purposes. In this case, please contact the authors for further info.

## Instructions

### Requeriments

Python 2.7

```
sudo apt-get install python-dev
```
OpenCV 2.4 with Python support (see [link](http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html))

Boost-Python

```
sudo apt-get install libboost-python-dev
```

Boost-Numpy (see [link](https://github.com/ndarray/Boost.NumPy))

### Compilation

```
sudo python setup.py install
```

## Contributing

You are very welcome to contribute to this project by opening a pull request via Github. 
