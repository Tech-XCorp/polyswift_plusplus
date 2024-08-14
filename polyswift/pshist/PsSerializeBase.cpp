/**
 * @file    PsSerializeBase.cpp
 *
 * @brief   Base class for serializing data for sync/dump data methods
 *
 * @version $Id: PsSerializeBase.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// vphist
#include <PsSerializeBase.h>

// Instantiate history classes
template class PsSerializeBase<int>;
template class PsSerializeBase<float>;
template class PsSerializeBase<double>;

