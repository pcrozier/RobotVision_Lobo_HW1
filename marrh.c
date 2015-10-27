#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <string.h>
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100
#define ON 255
#define OFF 0

int    pic[PICSIZE][PICSIZE];
double outpicx[PICSIZE][PICSIZE];
double outpicy[PICSIZE][PICSIZE];
double mag[PICSIZE][PICSIZE];
int cand[PICSIZE][PICSIZE];
int fin[PICSIZE][PICSIZE];
int    edgeflag[PICSIZE][PICSIZE];
double xmask[MAXMASK][MAXMASK];
double ymask[MAXMASK][MAXMASK];
double xconv[PICSIZE][PICSIZE];
double yconv[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
    int     i,j,y,x,s,t,mr,centx,centy, moretodo, HI, LO, p,q;
    double  maskval,xsum,ysum,sig,maxival,minival,maxval,percentage,slope;
    FILE    *fo1, *fo2,*fo3,*fp1, *fopen();
    char    *foobar;
    char outputfname[300];

    // Get input fname
    argc--;
    argv++;
    foobar = *argv;
    fp1=fopen(foobar,"rb");

    // Get output fname
    argc--;
    argv++;
    foobar = *argv;
    strcpy(outputfname, foobar);
    //fo1=fopen(foobar,"wb");

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
    strcpy(foobar, outputfname);
    fo1=fopen(strcat(foobar,"_mag.pgm"),"wb");

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

    // Peaks code
    for(i=mr;i<256-mr;i++){
        for(j=mr; j<256-mr; j++)
        {

            if((xconv[i][j]) == 0.0)
            {
                xconv[i][j] = .00001;
            }
            slope = yconv[i][j]/xconv[i][j];
            if( (slope <= .4142)&&(slope > -.4142))
            {
                if((mag[i][j] > mag[i][j-1])&&(mag[i][j] > mag[i][j+1]))
                {
                    cand[i][j] = 255;
                }
            }
            else if( (slope <= 2.4142)&&(slope > .4142))
            {
                if((mag[i][j] > mag[i-1][j-1])&&(mag[i][j] > mag[i+1][j+1]))
                {
                    cand[i][j] = 255;
                }
            }
            else if( (slope <= -.4142)&&(slope > -2.4142))
            {
                if((mag[i][j] > mag[i+1][j-1])&&(mag[i][j] > mag[i-1][j+1]))
                {
                    cand[i][j] = 255;
                }
            }
            else
            {
                if((mag[i][j] > mag[i-1][j])&&(mag[i][j] > mag[i+1][j]))
                {
                    cand[i][j] = 255;
                }
            }
        }
    }

    // Output peaks image
    strcpy(foobar, outputfname);
    fo2=fopen(strcat(foobar,"_peaks.pgm"),"wb");

    fprintf(fo2,"P5\n256 256\n255\n"); // Output PGM Header

    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            fprintf(fo2,"%c",(char)((int)(cand[i][j])));
        }
    }

    fclose(fo2);

    // Double threshold
    HI = 200;
    LO = 50;

    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            if (cand[i][j] == ON)
            {
                if (mag[i][j] > HI){
                    cand[i][j] = OFF;
                    fin[i][j] = ON;
                } else if (mag[i][j] < LO){
                    cand[i][j] = fin[i][j] = OFF;
                }
            }
        }
    }

    moretodo = ON;
    while (moretodo == ON)
    {
        moretodo = OFF;
        for (i=0; i<256; i++)
        {
            for (j=0; j<256; j++)
            {
                if (cand[i][j] == ON)
                {
                    for (p=-1; p<=1; p++)
                    {
                        for (q=-1; q<=1; q++)
                        {
                            if (fin[i+p][j+q] == ON)
                            {
                                cand[i][j] = OFF;
                                fin[i][j] = ON;
                                moretodo = ON;
                            }
                        }
                    }
                }
            }
        }
    }

    // Output final image
    strcpy(foobar, outputfname);
    fo3=fopen(strcat(foobar,"_final.pgm"),"wb");

    fprintf(fo1,"P5\n256 256\n255\n"); // Output PGM Header

    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            fprintf(fo3,"%c",(char)((int)(fin[i][j])));
        }
    }

    fclose(fo3);
}

