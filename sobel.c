#include <stdio.h>                          /* Sobel.c */
#include <stdlib.h>
#include <string.h>
#include <math.h>

int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];
int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
double ival[256][256],maxival;

main(argc,argv)
int argc;
char **argv;
{
    int i,j,p,q,mr,sum1,sum2;
    double threshold;
    FILE *fo1, *fo2, *fp1, *fopen();
    char *foobar;
    char inputfname[300];
    char outputfname[300];

    // Get input fname
    argc--;
    argv++;
    foobar = *argv;
    strcpy(inputfname, foobar);
    fp1=fopen(foobar,"rb");

    // Get output fname
    argc--;
    argv++;
    foobar = *argv;
    strcpy(outputfname, foobar);

    // Get threshold
    argc--;
    argv++;
    foobar = *argv;
    threshold = atof(foobar);

    // Read in input image
    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            pic[i][j]  =  getc (fp1);
            pic[i][j]  &= 0377;
        }
    }

    fclose(fp1);

    // Scanning convolution
    mr = 1;
    for (i=mr; i<256-mr; i++)
    {
        for (j=mr; j<256-mr; j++)
        {
            sum1 = 0;
            sum2 = 0;
            for (p=-mr; p<=mr; p++)
            {
                for (q=-mr; q<=mr; q++)
                {
                    sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                    sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
            }
            outpicx[i][j] = sum1;
            outpicy[i][j] = sum2;
        }
    }

    /* Applying the Magnitude formula in the code*/
    maxival = 0;
    for (i=mr; i<256-mr; i++)
    {
        for (j=mr; j<256-mr; j++)
        {
            ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                     (outpicy[i][j]*outpicy[i][j])));
            if (ival[i][j] > maxival)
                maxival = ival[i][j];

        }
    }


    // Output magnitude image

    strcpy(foobar, outputfname);
    fo1=fopen(strcat(foobar,"mag.pgm"),"wb");

    fprintf(fo1,"P5\n256 256\n255\n"); // Output PGM Header

    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(ival[i][j])));
        }
    }

    fclose(fo1);

    // Output x image and y image
    strcpy(foobar, outputfname);
    fo1=fopen(strcat(foobar,"x.pgm"),"wb");
    strcpy(foobar, outputfname);
    fo2=fopen(strcat(foobar,"y.pgm"),"wb");

    fprintf(fo1,"P5\n256 256\n255\n"); // Output PGM Header
    fprintf(fo2,"P5\n256 256\n255\n"); // Output PGM Header

    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            fprintf(fo1,"%c",(char)((int)(outpicx[i][j])));
            fprintf(fo2,"%c",(char)((int)(outpicy[i][j])));
        }
    }

    fclose(fo1);
    fclose(fo2);

}
