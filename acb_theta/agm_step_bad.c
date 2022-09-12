
#include "acb_theta.h"

void
acb_theta_agm_step_bad(acb_ptr r, acb_srcptr a, acb_srcptr roots, slong g, slong prec)
{
    slong k;

    flint_printf("Current a, roots:\n");
    for (k = 0; k < (1<<g); k++)
    {
        acb_printd(&a[k], 10); flint_printf("\n");
        acb_printd(&roots[k], 10); flint_printf("\n\n");
    }
    
    for (k = 0; k < (1<<g); k++)
    {
	acb_theta_agm_sqrt_lowprec(&r[k], &a[k], &roots[k], prec);
    }  
    acb_theta_agm_step_sqrt(r, r, g, prec);
}
