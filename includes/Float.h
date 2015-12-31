/*
 * utils.h
 *
 *  Created on: Feb 8, 2014
 *      Author: denia
 */

#ifndef FLOAT_H
#define FLOAT_H

#include <stdint.h>

typedef uint16_t float16 ;

extern float16 Float32To16 (float) ;
extern float Float16To32 (float16) ;

#endif // FLOAT_H