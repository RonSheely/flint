/*
    Copyright (C) 2017 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, j, result;
    FLINT_TEST_INIT(state);

    flint_printf("add/sub_ui....");
    fflush(stdout);

    /* Check (f + a) - a = f */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
       fmpz_mpoly_ctx_t ctx;
       fmpz_mpoly_t f, g, h;
       ulong c;
       slong len, coeff_bits, exp_bits;

       fmpz_mpoly_ctx_init_rand(ctx, state, 20);

       fmpz_mpoly_init(f, ctx);
       fmpz_mpoly_init(g, ctx);
       fmpz_mpoly_init(h, ctx);

       len = n_randint(state, 100);
       exp_bits = n_randint(state, 200) + 1;
       coeff_bits = n_randint(state, 200);

       for (j = 0; j < 10; j++)
       {
          fmpz_mpoly_randtest_bits(f, state, len, coeff_bits, exp_bits, ctx);
          fmpz_mpoly_randtest_bits(g, state, len, coeff_bits, exp_bits, ctx);
          fmpz_mpoly_randtest_bits(h, state, len, coeff_bits, exp_bits, ctx);

          c = n_randtest(state);

          fmpz_mpoly_add_ui(g, f, c, ctx);
          fmpz_mpoly_assert_canonical(g, ctx);

          fmpz_mpoly_sub_ui(h, g, c, ctx);
          fmpz_mpoly_assert_canonical(h, ctx);

          result = fmpz_mpoly_equal(f, h, ctx);

          if (!result)
          {
             printf("FAIL\n");
             flint_printf("Check (f + a) - a = f\ni = %wd, j = %wd\n", i ,j);
             fflush(stdout);
             flint_abort();
          }
       }

       fmpz_mpoly_clear(f, ctx);
       fmpz_mpoly_clear(g, ctx);
       fmpz_mpoly_clear(h, ctx);
    }

    /* Check aliasing */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
       fmpz_mpoly_ctx_t ctx;
       fmpz_mpoly_t f, g;
       ulong c;
       slong len, coeff_bits, exp_bits;

       fmpz_mpoly_ctx_init_rand(ctx, state, 20);

       fmpz_mpoly_init(f, ctx);
       fmpz_mpoly_init(g, ctx);

       len = n_randint(state, 100);
       exp_bits = n_randint(state, 200) + 1;
       coeff_bits = n_randint(state, 200);

       for (j = 0; j < 10; j++)
       {
          fmpz_mpoly_randtest_bits(f, state, len, coeff_bits, exp_bits, ctx);
          fmpz_mpoly_set(g, f, ctx);

          c = n_randtest(state);

          fmpz_mpoly_add_ui(f, f, c, ctx);
          fmpz_mpoly_assert_canonical(f, ctx);

          fmpz_mpoly_sub_ui(f, f, c, ctx);
          fmpz_mpoly_assert_canonical(f, ctx);

          result = fmpz_mpoly_equal(f, g, ctx);

          if (!result)
          {
             printf("FAIL\n");
             flint_printf("Check aliasing\ni = %wd, j = %wd\n", i ,j);
             fflush(stdout);
             flint_abort();
          }
       }

       fmpz_mpoly_clear(f, ctx);
       fmpz_mpoly_clear(g, ctx);
    }

    FLINT_TEST_CLEANUP(state);

    flint_printf("PASS\n");
    return 0;
}
