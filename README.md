Salient Object Detection - Evaluation Script
============================================

Matlab script to evaluate salient object detection algorithms on Achanta's 
dataset.

If you use any of this work in scientific research or as part of a larger
software system, you are kindly requested to cite the use in any related 
publications or technical documentation. The work is based upon:

    [1] Boris Schauerte, Rainer Stiefelhagen, "How the Distribution of Salient
        Objects in Images Influences Salient Object Detection", In Proceedings
        of the 20th International Conference on Image Processing (ICIP), 2013.

## 1. USAGE

The code should be easy to use. You only need to specify the pathes to the
dataset, which you can do by setting/changing the variables *imagepath* and 
*maskpath*, and the function handle  *saliency_func* to the saliency algorithm 
that you want to evaluate.

The MEX-.cpp file *analyse_recall_precision_mex.cpp* should compile 
automatically, if neeeded. Of course, you need to have Matlab's "mex" 
command properly configured.

You are able to specify some further options such as, e.g., a maximum salient
object size. Just have a look in the code ...

Most of the evaluation measures that you see at the end are described in the
corresponding ICIP paper [1]. You will notice that the script returns multiple
results, which is due to the fact that the results depend on the point where
you calculate the mean/average. This was something I wanted to keep in the
code, because I believe that this may be one of the reasons why the results
reported by some authors differ.

## 2. DEPENDENCIES

The code has two "weak" dependencies. First, the initial example uses my 
[Spectral Visual Saliency Toolbox](http://www.mathworks.com/matlabcentral/fileexchange/32455-spectral-phase-based-visual-saliency "Spectral Visual Saliency Toolbox ")
and, second, you can use [Steve Hoelzer's progress bar](http://www.mathworks.com/matlabcentral/fileexchange/6922-progressbar "Steve Hoelzer's progress bar") to visualize the evaluation progress.

If you want to evaluate quaternion-based saliency detection algorithms, then you have to include the [QTFM](http://qtfm.sourceforge.net/) library as well. However, the [Spectral Visual Saliency Toolbox](http://www.mathworks.com/matlabcentral/fileexchange/32455-spectral-phase-based-visual-saliency "Spectral Visual Saliency Toolbox ") should be able to automatically download and install a patched QTFM version for use with QDCT, PQFT, EigenPQFT, etc.

There are two simple ways to handle the dependencies:

1. Simply add the library locations (i.e. where you downloaded/unpacked/installed them) to Matlab's search path(es) before you execute the script, see [Matlab *addpath*](http://www.mathworks.de/de/help/matlab/ref/addpath.html).
2. By default the script expects to find the [Spectral Visual Saliency Toolbox](http://www.mathworks.com/matlabcentral/fileexchange/32455-spectral-phase-based-visual-saliency "Spectral Visual Saliency Toolbox ") under "../saliency" and the libraries under "../libs". However, you can of course change these default pathes to suit your setup.

## 3. INTERESTED IN SALIENT OBJECT DETECTION?

Then you should also check out these two code fragments that are part of [1]

- [Scripts to Analyse the Salient Object Distribution](https://github.com/bschauerte/salient_object_distribution "Scripts to Analyse the Salient Object Distribution")
- [Code to Calculate the Locally Debiased Region Contrast Saliency for Salient Object Detection](https://github.com/bschauerte/region_contrast_saliency "Code to Calculate the Locally Debiased Region Contrast Saliency for Salient Object Detection")

## 4. AUTHOR INFORMATION

### 4.1 CONTACT

  [Boris Schauerte](http://cvhci.anthropomatik.kit.edu/~bschauer/ "Boris Schauerte, Homepage")

### 4.2 ACKNOWLEDGEMENTS

  Part of this work was/is supported by the German Research Foundation (DFG)
  within the Collaborative Research Program SFB 588 "Humanoid Robots" and the
  Quaero Programme, funded by OSEO, French State agency for innovation.