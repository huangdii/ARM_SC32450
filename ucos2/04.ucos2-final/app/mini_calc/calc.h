/* Small Calculator example program
   Copyright (C) 1999, 2000, 2002 Free Software Foundation, Inc.
   Written by Stephane Carrez (stcarrez@nerim.fr)	

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _CALC_H
#define _CALC_H

//#include <sys/param.h>
//#include <imath.h>

#define MAX_VALUES 32
#define TEXT_SIZE	6145

#if !defined(VALUE_16) && !defined(VALUE_32) && !defined(VALUE_64)
# if TEXT_SIZE > 16384
#  define VALUE_64
# elif TEXT_SIZE > 6144
#  define VALUE_32
# else
#  define VALUE_16
# endif
#endif

/* Select the size of integers used by the calculator.  */
#ifdef VALUE_16
typedef short value_t;
typedef int vavalue_t;
#define HEX_CVT_MASK 0x0fff
#define calc_sqrt sqrt
#elif defined(VALUE_32)
typedef long value_t;
typedef value_t vavalue_t;
#define HEX_CVT_MASK 0x0fffffffL
#define calc_sqrt lsqrt
#elif defined(VALUE_64)
typedef long long value_t;
typedef value_t vavalue_t;
#define HEX_CVT_MASK 0x0fffffffffffffffLL
#define calc_sqrt lsqrt64
#endif

/* List of operations/commands supported by the tool.  */
typedef enum op_type 
{
  OP_ADD=40,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_NOT,
  OP_NEG,
  OP_SQRT,
  OP_QUIT,
  OP_LIST,
  OP_RESULT,
  OP_DEC,
  OP_HEX,
  OP_NUMBER= 0x100,
  OP_NUMCODE,
  OP_HELP
} op_type;

typedef struct command
{
  const char* name;
  op_type type;
  const char* help;
} command;

/* Definition of the format that we must use to print the value.  */
typedef enum print_mode
{
  PRINT_DEC,
  PRINT_HEX,
  PRINT_OCT,
  PRINT_BIN
} print_mode;

/* The stack of values.  The operations operate on the stack.  They pop
   one or two values and push the result on the stack (like some HP
   calculators).  */
typedef struct value_stack 
{
  value_t* values;
  int top;
  int max;
  print_mode mode;
} value_stack_t;

int strcmp (const char* s1, const char* s2);
int sprintf (char* buf, const char* pattern, ...);
int calc(value_stack_t* stack);
unsigned long lsqrt (unsigned long x);

#endif
