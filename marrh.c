#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <string.h>
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100

int    pic[PICSIZE][PICSIZE];
double outpic1[PICSIZE][PICSIZE];
double outpic2[PICSIZE][PICSIZE];
int    edgeflag[PICSIZE][PICSIZE];
double mask[MAXMASK][MAXMASK];
double conv[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
    int     i,j,p,q,s,t,mr,centx,centy;
    double  maskval,sum,sig,maxival,minival,maxval,percentage;
    FILE    *fo1, *fo2,*fp1, *fopen();
    char    *foobar;

    // Get input fname
    argc--;
    argv++;
    foobar = *argv;
    fp1=fopen(foobar,"rb");

    // Get output fname
    argc--;
    argv++;
    foobar = *argv;
    fo1=fopen(foobar,"wb");

    //argc--; argv++;
    //foobar = *argv;
    //fo2=fopen(foobar,"wb");

    // Get sigma
    argc--;
    argv++;
    foobar = *argv;
    //sig = strtod(foobar, NULL);//atof(foobar);
    sig = (double)atoi(foobar);
    printf("\n%s %.2lf\n",foobar,sig);
    // Get percentage
    argc--;
    argv++;
    foobar = *argv;
    percentage = atof(foobar);
    //ZEROTOL = atof(foobar);

    mr = (int)(sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);

    // Read in input image
    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            pic[i][j]  =  getc (fp1);
        }
    }

    // Build a mask
    for (p=-mr; p<=mr; p++)
    {
        for (q=-mr; q<=mr; q++)
        {
            maskval = ((2-(((p*p)+(q*q))/(sig*sig)))*
                       (exp(-1*(((p*p)+(q*q))/(2*(sig*sig))))));
            //printf("\n %.2lf %d %d %d %d %d %d\n ",sig,mr,p,q,centx,centy,maskval);
            (mask[p+centy][q+centx]) = maskval;
        }
    }

    // Convolution
    for (i=mr; i<=255-mr; i++)
    {
        for (j=mr; j<=255-mr; j++)
        {
            sum = 0;
            for (p=-mr; p<=mr; p++)
            {
                for (q=-mr; q<=mr; q++)
                {
                    sum += pic[i+p][j+q] * mask[p+centy][q+centx];
                }
            }
            outpic1[i][j] = sum;
            conv[i][j] = sum;
        }
    }
}

