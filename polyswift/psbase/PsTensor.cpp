/**
 *
 * @file    PsTensor.cpp
 *
 * @brief   Base class Tensors that expands on PsTensor by adding
 *          resize methods.
 *
 * @version $Id: PsTensor.cpp 6319 2006-11-14 22:39:46Z sizemore $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbase includes
#include <PsTensor.h>

//for the fields (remember to change map, PsField, PrTensor)
template class PsTensor<float, 1>;
template class PsTensor<float, 2>;
template class PsTensor<float, 3>;
template class PsTensor<float, 4>;
template class PsTensor<float, 5>;
template class PsTensor<float, 6>;
template class PsTensor<float, 7>;

template class PsTensor<double, 1>;
template class PsTensor<double, 2>;
template class PsTensor<double, 3>;
template class PsTensor<double, 4>;
template class PsTensor<double, 5>;
template class PsTensor<double, 6>;
template class PsTensor<double, 7>;

