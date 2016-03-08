/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2016 Arb authors

******************************************************************************/

#include "arb_mat.h"

void
_fmpq_mat_randtest_positive_semidefinite(fmpq_mat_t mat, flint_rand_t state, mp_bitcnt_t bits)
{
    slong n;
    fmpq_mat_t R, RT;
    if (!fmpq_mat_is_square(mat)) abort(); /* assert */
    n = fmpq_mat_nrows(mat);
    fmpq_mat_init(R, n, n);
    fmpq_mat_init(RT, n, n);
    fmpq_mat_randtest(R, state, bits);
    fmpq_mat_transpose(RT, R);
    fmpq_mat_mul(mat, R, RT);
    fmpq_mat_clear(R);
    fmpq_mat_clear(RT);
}


int fmpq_mat_is_invertible(const fmpq_mat_t A)
{
    int r;
    fmpq_t t;
    fmpq_init(t);
    fmpq_mat_det(t, A);
    r = !fmpq_is_zero(t);
    fmpq_clear(t);
    return r;
}

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("ldl....");
    fflush(stdout);

    flint_randinit(state);

    /* check special matrices */
    {
        slong n;
        for (n = 1; n < 10; n++)
        {
            slong lprec;
            arb_mat_t L, A;
            arb_mat_init(L, n, n);
            arb_mat_init(A, n, n);
            for (lprec = 2; lprec < 10; lprec++)
            {
                int result;
                slong prec;
                prec = 1 << lprec;

                /* zero */
                arb_mat_zero(A);
                result = arb_mat_ldl(L, A, prec);
                if (result)
                {
                    flint_printf("FAIL (zero):\n");
                    flint_printf("n = %wd, prec = %wd\n", n, prec);
                    flint_printf("L = \n"); arb_mat_printd(L, 15);
                    flint_printf("\n\n");
                }

                /* negative identity */
                arb_mat_one(A);
                arb_mat_neg(A, A);
                result = arb_mat_ldl(L, A, prec);
                if (result)
                {
                    flint_printf("FAIL (negative identity):\n");
                    flint_printf("n = %wd, prec = %wd\n", n, prec);
                    flint_printf("L = \n"); arb_mat_printd(L, 15);
                    flint_printf("\n\n");
                }

                /* identity */
                arb_mat_one(A);
                result = arb_mat_ldl(L, A, prec);
                if (!result || !arb_mat_equal(L, A))
                {
                    flint_printf("FAIL (identity):\n");
                    flint_printf("n = %wd, prec = %wd\n", n, prec);
                    flint_printf("L = \n"); arb_mat_printd(L, 15);
                    flint_printf("\n\n");
                }
            }
            arb_mat_clear(L);
            arb_mat_clear(A);
        }
    }

    for (iter = 0; iter < 10000; iter++)
    {
        fmpq_mat_t Q;
        arb_mat_t A, L, D, U, T;
        slong n, qbits, prec;
        int q_invertible, r_invertible;

        n = n_randint(state, 8);
        qbits = 1 + n_randint(state, 100);
        prec = 2 + n_randint(state, 202);

        fmpq_mat_init(Q, n, n);
        arb_mat_init(A, n, n);
        arb_mat_init(L, n, n);
        arb_mat_init(D, n, n);
        arb_mat_init(U, n, n);
        arb_mat_init(T, n, n);

        _fmpq_mat_randtest_positive_semidefinite(Q, state, qbits);
        q_invertible = fmpq_mat_is_invertible(Q);

        if (!q_invertible)
        {
            arb_mat_set_fmpq_mat(A, Q, prec);
            r_invertible = arb_mat_ldl(L, A, prec);
            if (r_invertible)
            {
                flint_printf("FAIL: matrix is singular over Q but not over R\n");
                flint_printf("n = %wd, prec = %wd\n", n, prec);
                flint_printf("\n");

                flint_printf("Q = \n"); fmpq_mat_print(Q); flint_printf("\n\n");
                flint_printf("A = \n"); arb_mat_printd(A, 15); flint_printf("\n\n");
                flint_printf("L = \n"); arb_mat_printd(L, 15); flint_printf("\n\n");
            }
        }
        else
        {
            /* now this must converge */
            while (1)
            {
                arb_mat_set_fmpq_mat(A, Q, prec);
                r_invertible = arb_mat_ldl(L, A, prec);
                if (r_invertible)
                {
                    break;
                }
                else
                {
                    if (prec > 10000)
                    {
                        flint_printf("FAIL: failed to converge at 10000 bits\n");
                        flint_printf("n = %wd, prec = %wd\n", n, prec);
                        flint_printf("Q = \n"); fmpq_mat_print(Q); flint_printf("\n\n");
                        flint_printf("A = \n"); arb_mat_printd(A, 15); flint_printf("\n\n");
                        abort();
                    }
                    prec *= 2;
                }
            }

            /* multiply out the decomposition */
            {
                slong i;
                arb_mat_zero(D);
                arb_mat_transpose(U, L);
                for (i = 0; i < n; i++)
                {
                    arb_set(arb_mat_entry(D, i, i), arb_mat_entry(L, i, i));
                    arb_one(arb_mat_entry(L, i, i));
                    arb_one(arb_mat_entry(U, i, i));
                }
                arb_mat_mul(T, L, D, prec);
                arb_mat_mul(T, T, U, prec);
            }

            if (!arb_mat_contains_fmpq_mat(T, Q))
            {
                flint_printf("FAIL (containment, iter = %wd)\n", iter);
                flint_printf("n = %wd, prec = %wd\n", n, prec);
                flint_printf("\n");

                flint_printf("Q = \n"); fmpq_mat_print(Q); flint_printf("\n\n");
                flint_printf("A = \n"); arb_mat_printd(A, 15); flint_printf("\n\n");
                flint_printf("L = \n"); arb_mat_printd(L, 15); flint_printf("\n\n");
                flint_printf("U = \n"); arb_mat_printd(U, 15); flint_printf("\n\n");
                flint_printf("L*U = \n"); arb_mat_printd(T, 15); flint_printf("\n\n");

                abort();
            }
        }

        fmpq_mat_clear(Q);
        arb_mat_clear(A);
        arb_mat_clear(L);
        arb_mat_clear(D);
        arb_mat_clear(U);
        arb_mat_clear(T);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
