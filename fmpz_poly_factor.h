/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2006, 2007, 2008, 2009, 2010 William Hart
    Copyright (C) 2009 Andy Novocin
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#ifndef FMPZ_POLY_FACTOR_H
#define FMPZ_POLY_FACTOR_H

#undef ulong /* interferes with system includes */
#include <stdio.h>
#define ulong unsigned long

#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "nmod_poly.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct {
    fmpz c;
    fmpz_poly_struct *p;
    long *exp;
    long num;
    long alloc;
} fmpz_poly_factor_struct;

typedef fmpz_poly_factor_struct fmpz_poly_factor_t[1];

void fmpz_poly_factor_init(fmpz_poly_factor_t fac);

void fmpz_poly_factor_init2(fmpz_poly_factor_t fac, long alloc);

void fmpz_poly_factor_realloc(fmpz_poly_factor_t fac, long alloc);

void fmpz_poly_factor_fit_length(fmpz_poly_factor_t fac, long len);

void fmpz_poly_factor_clear(fmpz_poly_factor_t fac);

void fmpz_poly_factor_insert(fmpz_poly_factor_t fac, 
                             const fmpz_poly_t p, long exp);

void fmpz_poly_factor_concat(fmpz_poly_factor_t res, 
                             const fmpz_poly_factor_t fac);

void fmpz_poly_factor_print(const fmpz_poly_factor_t fac);

#ifdef __cplusplus
}
#endif

#endif

