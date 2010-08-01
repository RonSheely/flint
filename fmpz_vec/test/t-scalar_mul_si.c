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

    Copyright (C) 2009, 2010 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    printf("scalar_mul_si....");
    fflush(stdout);

    _fmpz_vec_randinit();

    // Check aliasing of a and b
    for (i = 0; i < 10000; i++)
    {
        fmpz *a, *b;
        long len = n_randint(100);
        long n = (long) n_randtest();
        if (n_randint(2))
            n = -n;

        a = _fmpz_vec_init(len);
        b = _fmpz_vec_init(len);
        _fmpz_vec_randtest(a, len, n_randint(200));

        _fmpz_vec_scalar_mul_si(b, a, len, n);
        _fmpz_vec_scalar_mul_si(a, a, len, n);

        result = (_fmpz_vec_equal(a, b, len));
        if (!result)
        {
            printf("FAIL:\n");
            _fmpz_vec_print(a, len), printf("\n\n");
            _fmpz_vec_print(b, len), printf("\n\n");
            abort();
        }

        _fmpz_vec_clear(a, len);
        _fmpz_vec_clear(b, len);
    }

    // Check agreement with _fmpz
    for (i = 0; i < 10000; i++)
    {
        fmpz *a, *b;
        long len, n;
        fmpz_t x;
        
        len = n_randint(100);
        a = _fmpz_vec_init(len);
        b = _fmpz_vec_init(len);
        _fmpz_vec_randtest(a, len, n_randint(200));
        n = (long) n_randtest();
        if (n_randint(2))
            n = -n;
        fmpz_set_si(x, n);

        _fmpz_vec_scalar_mul_fmpz(b, a, len, x);
        _fmpz_vec_scalar_mul_si(a, a, len, n);

        result = (_fmpz_vec_equal(a, b, len));
        if (!result)
        {
            printf("FAIL:\n");
            _fmpz_vec_print(a, len), printf("\n\n");
            _fmpz_vec_print(b, len), printf("\n\n");
            printf("%li\n\n", n);
            abort();
        }

        _fmpz_vec_clear(a, len);
        _fmpz_vec_clear(b, len);
        fmpz_clear(x);
    }

    _fmpz_vec_randclear();
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
