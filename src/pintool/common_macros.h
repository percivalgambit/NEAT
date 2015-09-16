#ifndef PINTOOL_COMMON_MACROS_H_
#define PINTOOL_COMMON_MACROS_H_

#include <pin.H>

#include <cstdlib>
#include <iostream>

#define ERROR(message)       \
  do {                       \
    cerr << message << endl; \
    exit(1);                 \
  } while (0)

/*!
 * Convert a FLT32 variable into the string representation of its value in hex.
 * @param[in]   fp  variable to convert to hex
 */
#define FLT32_TO_HEX(fp) \
  StringHex(*reinterpret_cast<const UINT32 *>(&fp), 8, FALSE)

#endif  // PINTOOL_COMMON_MACROS_H_
