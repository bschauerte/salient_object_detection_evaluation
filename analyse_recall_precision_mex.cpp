/**
 * Copyright 2011,2013 B. Schauerte. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are 
 * met:
 * 
 *    1. Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided with the 
 *       distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY B. SCHAUERTE ''AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL B. SCHAUERTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing 
 * official policies, either expressed or implied, of B. Schauerte.
 */

/**
 * If you use any of this work in scientific research or as part of a larger
 * software system, you are kindly requested to cite the use in any related 
 * publications or technical documentation. The work is based upon:
 *
 * [1] B. Schauerte, R. Stiefelhagen, "How the Distribution of Salient
 *     Objects in Images Influences Salient Object Detection". In Proceedings
 *     of the 20th International Conference on Image Processing (ICIP), 2013.
 */

#include <cmath>
#include <vector>

#ifdef __MEX
#define __CONST__ const
#include "mex.h"
#include "matrix.h"
#endif

#ifndef SQR
#define SQR(x) ((x)*(x))
#endif

// this code is inspired by LingPipe's ScoredPrecisionRecallEvaluation.java and PrecisionRecallEvaluation.java (Version 4.0.1)
template <typename T>
class AnalysePrecisionRecall
{
public:
  AnalysePrecisionRecall(const T* precisions, const T* recalls, int N);
  ~AnalysePrecisionRecall(void);
  
  T getAreaUnder(bool do_interpolation = false);
  T getAveragePrecision(void);
  T getBreakEvenPoint(void);
  T getMaximumFScore(const T beta = T(1));
  
  static T getFScore(const T precision, const T recall, const T beta)
  {
    T fscore = 0;
    
    if ((precision * recall) == 0)
      fscore = 0;
    else
      fscore = (1 + SQR(beta)) * (precision * recall) / ((SQR(beta) * precision) + recall); // f-score
    
    return fscore;
  }
  
  static T getAreaUnder(const T* x, const T* y, const int N)
  {
    // x has to be increasing monotonic
    T area(0);
    
    for (int i = 1; i < N; i++)
    {
      area += (x[i]-x[i-1])*(y[i]+y[i-1]) / T(2);
    }
    
    return area;
  }
protected:
private:
  const T* precisions;
  const T* recalls;
  const int N;
};

template <typename T>
AnalysePrecisionRecall<T>::AnalysePrecisionRecall(const T* _precisions, const T* _recalls, int _N)
: precisions(_precisions), recalls(_recalls), N(_N)
{
}

template <typename T>
AnalysePrecisionRecall<T>::~AnalysePrecisionRecall(void)
{
}

template <typename T>
T 
AnalysePrecisionRecall<T>::getAreaUnder(bool do_interpolation)
{
  if (do_interpolation)
  {
    T min_precision = 0;
    std::vector<T> tmp_precisions(precisions,precisions + N);
    for (int i = N-1; i >= 0; i--)
    {
      if (precisions[i] > min_precision)
        min_precision = precisions[i];
      
      tmp_precisions[i] = min_precision;
    }
    
    std::vector<T> vprecisions(tmp_precisions.begin(),tmp_precisions.end());
    std::vector<T> vrecalls(recalls,recalls + N);
    
    // insert the (artificial) (1,0) element
    vprecisions.insert(vprecisions.begin(),1);
    vrecalls.insert(vrecalls.begin(),0);
    
    // add the (artificial) (0,1) element
    vprecisions.push_back(0);
    vrecalls.push_back(1);
    
    return getAreaUnder(&vrecalls[0],&vprecisions[0],vrecalls.size());
  }
  else
  {
    std::vector<T> vprecisions(precisions,precisions + N);
    std::vector<T> vrecalls(recalls,recalls + N);

    // insert the (artificial) (1,0) element
    vprecisions.insert(vprecisions.begin(),1);
    vrecalls.insert(vrecalls.begin(),0);
    
    // add the (artificial) (0,1) element
    vprecisions.push_back(0);
    vrecalls.push_back(1);
    
    return getAreaUnder(&vrecalls[0],&vprecisions[0],vrecalls.size());
  }
}

template <typename T>
T 
AnalysePrecisionRecall<T>::getAveragePrecision(void)
{
  T val = 0;
  for (int i = 0; i < N; i++)
    val += precisions[i];
  val /= T(N);
  return val;
}

template <typename T>
T 
AnalysePrecisionRecall<T>::getBreakEvenPoint(void)
{
  for (int i = 0; i < N; i++)  
    if (recalls[i] > precisions[i])  
      return precisions[i];  
  return T(0);
}

template <typename T>
T 
AnalysePrecisionRecall<T>::getMaximumFScore(const T beta)
{
  T max_fscore = 0;
  for (int i = 0; i < N; i++)
  {
    const T fscore = getFScore(precisions[i],recalls[i],beta);
    if (fscore > max_fscore)
      max_fscore = fscore;
  }
  return max_fscore;
}

#ifdef __MEX
template <typename T>
void
_mexFunction(int nlhs, mxArray* plhs[],
             int nrhs, const mxArray* prhs[])
{
  // get input data
  __CONST__ mxArray *mprecisions = prhs[0];
  __CONST__ mxArray *mrecalls    = prhs[1];
  
  if (mxIsComplex(mrecalls) || mxIsComplex(mprecisions))
    mexErrMsgTxt("only real data allowed");

  // get the number of elements
  const size_t numel = mxGetNumberOfElements(mrecalls);
  if (numel != mxGetNumberOfElements(mprecisions))
    mexErrMsgTxt("the recall and precision input arrays need to have the same number of elements");

  // get the real data pointers
  __CONST__ T* recalls    = (T*)mxGetData(mrecalls);
  __CONST__ T* precisions = (T*)mxGetData(mprecisions);
  
  // check whether or not the recall values are monotonic increasing
  bool is_monotonic = true;
  for (int i = 1; i < numel && is_monotonic; i++)
  {
    if (recalls[i-1] > recalls[i])
      is_monotonic = false;
  }
  if (is_monotonic == false)
    mexErrMsgTxt("the recall values are not monotonic increasing");
  
  AnalysePrecisionRecall<T> apr(precisions,recalls,numel);
  
  if (nlhs >= 1)
  {
    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    mxGetPr(plhs[0])[0] = (double)apr.getAreaUnder(false);
  }
  if (nlhs >= 2)
  {
    plhs[1] = mxCreateDoubleMatrix(1,1,mxREAL);
    mxGetPr(plhs[1])[0] = (double)apr.getAreaUnder(true);
  }
  if (nlhs >= 3)
  {
    plhs[2] = mxCreateDoubleMatrix(1,1,mxREAL);
    mxGetPr(plhs[2])[0] = (double)apr.getAveragePrecision();
  }
  if (nlhs >= 4)
  {
    plhs[3] = mxCreateDoubleMatrix(1,1,mxREAL);
    mxGetPr(plhs[3])[0] = (double)apr.getBreakEvenPoint();
  }
  if (nlhs >= 4)
  {
    plhs[4] = mxCreateDoubleMatrix(1,1,mxREAL);
    mxGetPr(plhs[4])[0] = (double)apr.getMaximumFScore();
  }
}

void
mexFunction(int nlhs, mxArray* plhs[],
            int nrhs, const mxArray* prhs[])
{
  // check number of input parameters
  if (nrhs < 2 || nrhs > 3)
    mexErrMsgTxt("input arguments: precisions recalls [betas]");
  
  // output order is: ...

  // only float and double are currently supported
  if (!mxIsDouble(prhs[0]) && !mxIsSingle(prhs[0])) 
  	mexErrMsgTxt("Only float and double are supported.");
  
  // for code simplicity: the input array and mask need to be of the same type
  if (mxGetClassID(prhs[0]) != mxGetClassID(prhs[1]))
    mexErrMsgTxt("The arrays containing the precision and recall values need to have the same data type.");
  
  switch (mxGetClassID(prhs[0]))
  {
    case mxSINGLE_CLASS:
      _mexFunction<float>(nlhs,plhs,nrhs,prhs);
      break;
    case mxDOUBLE_CLASS:
      _mexFunction<double>(nlhs,plhs,nrhs,prhs);
      break;
    default:
      // this should never happen
      break;
  }
}
#endif