#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <string.h>
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100

int    pic[PICSIZE][PICSIZE];
double outpicx[PICSIZE][PICSIZE];
double outpicy[PICSIZE][PICSIZE];
double mag[PICSIZE][PICSIZE];
int    edgeflag[PICSIZE][PICSIZE];
double xmask[MAXMASK][MAXMASK];
double ymask[MAXMASK][MAXMASK];
double xconv[PICSIZE][PICSIZE];
double yconv[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
    int     i,j,y,x,s,t,mr,centx,centy;
    double  maskval,xsum,ysum,sig,maxival,minival,maxval,percentage;
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
    for (y=-mr; y<=mr; y++)
    {
        for (x=-mr; x<=mr; x++)
        {
            // compute xmask value
            maskval = (x)*(exp(-1*(((y*y)+(x*x))/(2*(sig*sig)))));
            (xmask[y+centy][x+centx]) = maskval;

            // compute ymask value
            maskval = (y)*(exp(-1*(((y*y)+(x*x))/(2*(sig*sig)))));
            (ymask[y+centy][x+centx]) = maskval;

            //printf("\n %.2lf %d %d %d %d %d %d\n ",sig,mr,p,q,centx,centy,maskval);
        }
    }

    // Convolution
    for (i=mr; i<=255-mr; i++)
    {
        for (j=mr; j<=255-mr; j++)
        {
            xsum = 0, ysum = 0;
            for (y=-mr; y<=mr; y++)
            {
                for (x=-mr; x<=mr; x++)
                {
                    xsum += pic[i+y][j+x] * xmask[y+centy][x+centx];
                    ysum += pic[i+y][j+x] * ymask[y+centy][x+centx];
                }
            }
            outpicx[i][j] = xsum;
            xconv[i][j] = xsum;
            outpicy[i][j] = ysum; // I don't think outpic is what is intended head. Need to take a closer look.
            yconv[i][j] = ysum;
        }
    }

    /* Applying the Magnitude formula in the code*/
    maxival = 0;
    for (i=mr; i<256-mr; i++)
    {
        for (j=mr; j<256-mr; j++)
        {
            mag[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                     (outpicy[i][j]*outpicy[i][j])));
            if (mag[i][j] > maxival)
                maxival = mag[i][j];

        }
    }

    // Output magnitude image
    //strcpy(foobar, outputfname);
    //fo1=fopen(foobar,"wb");//strcat(foobar,"mag.pgm"),"wb");

    fprintf(fo1,"P5\n256 256\n255\n"); // Output PGM Header

    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            mag[i][j] = (mag[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(mag[i][j])));
        }
    }

    fclose(fo1);
}

