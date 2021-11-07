#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PI 3.141592654
//A datatype that can store multiple items with different data types
struct dataset
{   
    float season;
    float age;
    int disease;
    int trauma;
    int surgical;
    int fever;
    float freq;
    int smoke;
    float sit;
    int output;
};
struct feature
{
    float season;
    float age;
    int disease;
    int trauma;
    int surgical;
    int fever;
    float freq;
    int smoke;
    float sit;
};

// Function to calculate the prior and posterior probability for NB
void NBProbability();
float nmean2();
float amean2();
float nmean9();
float amean9();
float nvariance2(float normalmean2);
float avariance2(float alteredmean2);
float nvariance9(float normalmean9);
float avariance9(float alteredmean9);
float ngaussian2(float normalmean2, float normalsd2, float input);
float agaussian2(float alteredmean2, float alteredsd2, float input);
float ngaussian9(float normalmean9, float normalsd9, float input);
float agaussian9(float alteredmean9, float alteredsd9, float input);

// 
void NBcount(int outcome, int i);

//So that it can read 100 line of text file
struct dataset volunteers[100];

//Feature and Output set for training & testing set
struct feature trainingF[80];
int trainingO[80];
struct feature testingF[20];
int testingO[20];
int main(void) 
{
    //Read text file
    FILE *file = fopen("fertility_Diagnosis_Data_Group5_8.txt", "r");
    
    //Check if file has been read successfully
    if (file == NULL) 
    {
        printf("Cannot open file.\n");
        return 1;
    }
    else 
    {
        //Check for number of line
        char ch;
        int count = 0;
        while (ch != EOF)
        {
            ch = fgetc(file);
            if (ch == '\n')
            {
                count++;
            }
        }
        
        //Set file position to the beginning of the file of the given stream
        rewind(file);
        int i;
        for (i = 0; i <= count; i++) 
        {
            //Through loop, read formatted input from file and store in respective places
            fscanf(file, "%f,%f,%d,%d,%d,%d,%f,%d,%f,%d\n", &volunteers[i].season, 
                &volunteers[i].age, &volunteers[i].disease, &volunteers[i].trauma, 
                &volunteers[i].surgical, &volunteers[i].fever, &volunteers[i].freq, 
                &volunteers[i].smoke, &volunteers[i].sit, &volunteers[i].output);
        }
        for(i = 0; i < 80; i++)
        {
            trainingF[i].season = volunteers[i].season;
            trainingF[i].age = volunteers[i].age;
            trainingF[i].disease = volunteers[i].disease;
            trainingF[i].trauma = volunteers[i].trauma;
            trainingF[i].surgical = volunteers[i].surgical;
            trainingF[i].fever = volunteers[i].fever;
            trainingF[i].freq = volunteers[i].freq;
            trainingF[i].smoke = volunteers[i].smoke;
            trainingF[i].sit = volunteers[i].sit;
            trainingO[i] = volunteers[i].output;
            /*
            printf("%.2f %.2f %d %d %d %d %.2f %d %.2f %d\n", trainingF[i].season, 
                trainingF[i].age, trainingF[i].disease, trainingF[i].trauma, 
                trainingF[i].surgical, trainingF[i].fever, trainingF[i].freq, 
                trainingF[i].smoke, trainingF[i].sit, trainingO[i]);
            */
        }
        for(i = 80; i < 100; i++)
        {
            testingF[i-80].season = volunteers[i].season;
            testingF[i-80].age = volunteers[i].age;
            testingF[i-80].disease = volunteers[i].disease;
            testingF[i-80].trauma = volunteers[i].trauma;
            testingF[i-80].surgical = volunteers[i].surgical;
            testingF[i-80].fever = volunteers[i].fever;
            testingF[i-80].freq = volunteers[i].freq;
            testingF[i-80].smoke = volunteers[i].smoke;
            testingF[i-80].sit = volunteers[i].sit;
            testingO[i-80] = volunteers[i].output;
            /*
            printf("%.2f %.2f %d %d %d %d %.2f %d %.2f %d\n", testingF[i-80].season, 
                testingF[i-80].age, testingF[i-80].disease, testingF[i-80].trauma, 
                testingF[i-80].surgical, testingF[i-80].fever, testingF[i-80].freq, 
                testingF[i-80].smoke, testingF[i-80].sit, testingO[i-80]);
            */
        }
    }

float normalmean2 = nmean2();
float alteredmean2 = amean2();
float normalmean9 = nmean9();
float alteredmean9 = amean9();
float normalsd2 = nvariance2(normalmean2);
float alteredsd2 = avariance2(alteredmean2);
float normalsd9 = nvariance9(normalmean9);
float alteredsd9 = avariance9(alteredmean9);

float input;
printf("\nPlease input the value of x :");
scanf("%f",&input);
ngaussian2(normalmean2, normalsd2, input);
agaussian2(alteredmean2, alteredsd2, input);
ngaussian9(normalmean9,normalsd9, input);
agaussian9(alteredmean9, alteredsd9, input);

}


float nmean2()
{
    float normalsum2 = 0;
    float n = 80;
    float normalmean2;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 0)
        {
            normalsum2 += trainingF[i].age;
        }

    }
    normalmean2 = 1.0/n * normalsum2;
    return normalmean2;
}
float amean2()
{
    float alteredsum2 = 0;
    float n = 80;
    float alteredmean2;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 1)
        {
            alteredsum2 += trainingF[i].age;
        }
    }  
    alteredmean2 = 1.0/n * alteredsum2;
    return alteredmean2;
}

float nmean9()
{
    float normalsum9 = 0;
    float n = 80;
    float normalmean9;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 0)
        {
            normalsum9 += trainingF[i].sit;
        }
    }
    normalmean9 = 1.0/n * normalsum9;
    return normalmean9;
}

float amean9()
{
    float alteredsum9 = 0;
    float n = 80;
    float alteredmean9;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 1)
        {
            alteredsum9 += trainingF[i].sit;
        }
    }
    alteredmean9 = 1.0/n * alteredsum9;
    return alteredmean9;
}
float nvariance2(float normalmean2)
{
    float sub;
    float sum2 = 0;
    float n = 80;
    float normalvariance2;
    float normalsd2;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 0)
        {
            sub = pow(trainingF[i].age - normalmean2,2);
            sum2 += sub;
        }
    }
    normalvariance2=1.0/(n-1) * sum2;
    normalsd2 = sqrt(normalvariance2);
    return normalsd2;
}

float avariance2(float alteredmean2)
{
    float sub;
    float sum2 = 0;
    float n = 80;
    float alteredvariance2;
    float alteredsd2;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 1)
        {
            sub = pow(trainingF[i].age - alteredmean2,2);
            sum2 += sub;
        }
    }
    alteredvariance2=1.0/(n-1) * sum2;
    alteredsd2 = sqrt(alteredvariance2);
    return alteredsd2;
}
float nvariance9(float normalmean9)
{
    float sub;
    float sum2 = 0;
    float n = 80;
    float normalvariance9;
    float normalsd9;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 0)
        {
            sub = pow(trainingF[i].sit - normalmean9,2);
            sum2 += sub;
        }
    }
    normalvariance9=1.0/(n-1) * sum2;
    normalsd9 = sqrt(normalvariance9);
    return normalsd9;
}

float avariance9(float alteredmean9)
{
    float sub;
    float sum2 = 0;
    float n = 80;
    float alteredvariance9;
    float alteredsd9;
    for(int i=0;i<n;i++)
    {
        if(trainingO[i] == 1)
        {
            sub = pow(trainingF[i].sit - alteredmean9,2);
            sum2 += sub;
        }
    }
    alteredvariance9=1.0/(n-1) * sum2;
    alteredsd9 = sqrt(alteredvariance9);
    return alteredsd9;
}
float ngaussian2(float normalmean2, float normalsd2, float input)
{
    float normalgaussian2;
    normalgaussian2 = 1.0 /(sqrt(2.0*PI))*exp(-0.5*pow((input-normalmean2),2)/pow(normalsd2,2));
    return normalgaussian2;
}

float agaussian2(float alteredmean2, float alteredsd2, float input)
{
    float alteredgaussian2;
    alteredgaussian2 = 1.0 /(sqrt(2.0*PI))*exp(-0.5*pow((input-alteredmean2),2)/pow(alteredsd2,2));
    return alteredgaussian2;
}

float ngaussian9(float normalmean9, float normalsd9, float input)
{
    float normalgaussian9;
    normalgaussian9 = 1.0 /(sqrt(2.0*PI))*exp(-0.5*pow((input-normalmean9),2)/pow(normalsd9,2));
    return normalgaussian9;
}

float agaussian9(float alteredmean9, float alteredsd9, float input)
{
    float alteredgaussian9;
    alteredgaussian9 = 1.0 /(sqrt(2.0*PI))*exp(-0.5*pow((input-alteredmean9),2)/pow(alteredsd9,2));
    return alteredgaussian9;
}
