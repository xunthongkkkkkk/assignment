#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define pi 3.14159

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
struct calculator
{
    float normal;
    float altered;
    float predictedoutcome;
};

// Function to calculate the probability for both NB and Gaussian.
//void ProbabilityCalculator();

// Function to calculate the prior and posterior probability for NB
void NBProbability();
void ProbabilityCalculator();

// determine the feature for each category and increment the counter
void Count(int outcome, int i);

// Function to calculate the mean for the features requiring gaussian Takes in the sum, and count and pointer to address of where to store the mean
void Mean(float *sum, float *count, float *meanstorelocation);

// Function that returns the squared varience of 
void variancesquared(float *mean, float *count, int outcome, int feature, float *variencestorelocation);

// Function to calculate and return the gaussian probability 
float Gaussian(float inputx, float *mean, float *varience);


//So that it can read 100 line of text file
struct dataset *volunteers;

//Feature and Output set for training & testing set
struct feature *trainingF;
int *trainingO;
struct feature *testingF;
int *testingO;
int arrlength;
int datasplit;

struct calculator *calculatedprob;


/*array to store the prior and posterior probability for both normal (0) and altered(1). Prior Probability is stored in index 0.
The remaining features are indexed as per listed on the AE document 
Syntax: Probability[Outcome][feature]  */
float Probability[2][22];
/* array to store the both [normal(0) and altered(1)] [age(0) or time spent sitting(1)] [mean(0) and varience squared(1)].
 Syntax: GaussianMeanVarience[outcome][feature][mean/varience]
*/
float GaussianMeanVariance[2][2][2];

int main(void) {
    //Read text file
    FILE *file = fopen("fertility_Diagnosis_Data_Group5_8.txt", "r");
    
    //Check if file has been read successfully
    if (file == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }
    else {
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
        volunteers=malloc(sizeof(struct dataset)*(count+1));

        printf("\n There are %d lines of data in the file provided. \nPlease enter which line the testing data starts:",count+1);
        scanf("%d",&datasplit);
        trainingF=malloc(sizeof(struct feature)*(datasplit-1));
        trainingO=malloc(sizeof(int)*(datasplit-1));
        testingF=malloc(sizeof(struct feature)*(1+count-datasplit));
        testingO=malloc(sizeof(int)*(1+count-datasplit));
        calculatedprob=malloc(10000);
        arrlength = datasplit-1;
        
        //Set file position to the beginning of the file of the given stream
        rewind(file);

        int i;
        for (i = 0; i <= count; i++) {
            //Through loop, read formatted input from file and store in respective places
            fscanf(file, "%f,%f,%d,%d,%d,%d,%f,%d,%f,%d\n", &volunteers[i].season, 
                &volunteers[i].age, &volunteers[i].disease, &volunteers[i].trauma, 
                &volunteers[i].surgical, &volunteers[i].fever, &volunteers[i].freq, 
                &volunteers[i].smoke, &volunteers[i].sit, &volunteers[i].output);
        }

        for(i = 0; i < datasplit-1; i++){
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

        



        for(i = datasplit-1; i < count+1; i++){
            testingF[i].season = volunteers[i].season;
            testingF[i].age = volunteers[i].age;
            testingF[i].disease = volunteers[i].disease;
            testingF[i].trauma = volunteers[i].trauma;
            testingF[i].surgical = volunteers[i].surgical;
            testingF[i].fever = volunteers[i].fever;
            testingF[i].freq = volunteers[i].freq;
            testingF[i].smoke = volunteers[i].smoke;
            testingF[i].sit = volunteers[i].sit;
            testingO[i] = volunteers[i].output;


            /*
            printf("%.2f %.2f %d %d %d %d %.2f %d %.2f %d\n", testingF[i-80].season, 
                testingF[i-80].age, testingF[i-80].disease, testingF[i-80].trauma, 
                testingF[i-80].surgical, testingF[i-80].fever, testingF[i-80].freq, 
                testingF[i-80].smoke, testingF[i-80].sit, testingO[i-80]);
            */
        }
        
    }
    NBProbability();
    float input;
    printf("\n test normal age gaussian: ");
    scanf("%f", &input);
    printf("\n gaussian test: %f",Gaussian(input,&GaussianMeanVariance[0][0][0],&GaussianMeanVariance[0][0][1]));
    ProbabilityCalculator();
    
}

void ProbabilityCalculator()
{
    for(int i = arrlength; i<100; i++)
    {
        calculatedprob[i].normal = 1;
        calculatedprob[i].altered = 1;
        if (testingF[i].season == (float) -1)
        {
            calculatedprob[i].normal *= Probability[0][1];
            calculatedprob[i].altered *= Probability[1][1];
            
        }
        else if (testingF[i].season == (float) -0.33)
        {
            calculatedprob[i].normal *= Probability[0][2];
            calculatedprob[i].altered *= Probability[1][2];
        }
        else if (testingF[i].season == (float) 0.33)
        {
            calculatedprob[i].normal *= Probability[0][3];
            calculatedprob[i].altered *= Probability[1][3];
        }
        else if (testingF[i].season == (float) 1)
        {
            calculatedprob[i].normal *= Probability[0][4];
            calculatedprob[i].altered *= Probability[1][4];
        }

        switch (testingF[i].disease)
        {
        case 0:
            calculatedprob[i].normal *= Probability[0][5];
            calculatedprob[i].altered *= Probability[1][5];
            break;
        case 1:
            calculatedprob[i].normal *= Probability[0][6];
            calculatedprob[i].altered *= Probability[1][6];
            break;
        }

        switch (testingF[i].trauma)
        {
        case 0:
            calculatedprob[i].normal *= Probability[0][7];
            calculatedprob[i].altered *= Probability[1][7];
            break; 
        case 1:
            calculatedprob[i].normal *= Probability[0][8];
            calculatedprob[i].altered *= Probability[1][8];
            break;
        }

        switch (testingF[i].surgical)
        {
        case 0:
            calculatedprob[i].normal *= Probability[0][9];
            calculatedprob[i].altered *= Probability[1][9];
            break; 
        case 1:
            calculatedprob[i].normal *= Probability[0][10];
            calculatedprob[i].altered *= Probability[1][10];
            break;
        }
        
        switch (testingF[i].fever)
        {
        case -1:
            calculatedprob[i].normal *= Probability[0][11];
            calculatedprob[i].altered *= Probability[1][11];
            break; 
        case 0:
            calculatedprob[i].normal *= Probability[0][12];
            calculatedprob[i].altered *= Probability[1][12];
            break; 
        case 1:
            calculatedprob[i].normal *= Probability[0][13];
            calculatedprob[i].altered *= Probability[1][13];
            break;
        }
        
        if (testingF[i].freq == (float) 0.2)
        {
            calculatedprob[i].normal *= Probability[0][14];
            calculatedprob[i].altered *= Probability[1][14];
        }
        else if (testingF[i].freq == (float) 0.4)
        {
            calculatedprob[i].normal *= Probability[0][15];
            calculatedprob[i].altered *= Probability[1][15];
            
        }
        
        else if (testingF[i].freq == (float) 0.6)
        {
    
            calculatedprob[i].normal *= Probability[0][16];
            calculatedprob[i].altered *= Probability[1][16];
            
        }
        else if (testingF[i].freq == (float) 0.8)
        {
            calculatedprob[i].normal *= Probability[0][17];
            calculatedprob[i].altered *= Probability[1][17];
        }
        else if (testingF[i].freq == (float) 1)
        {
            calculatedprob[i].normal *= Probability[0][18];
            calculatedprob[i].altered *= Probability[1][18];
        }
        
        switch (testingF[i].smoke)
        {
        case -1:
            calculatedprob[i].normal *= Probability[0][19];
            calculatedprob[i].altered *= Probability[1][19];
            break; 
        case 0:
            calculatedprob[i].normal *= Probability[0][20];
            calculatedprob[i].altered *= Probability[1][20];
            break; 
        case 1:
            calculatedprob[i].normal *= Probability[0][21];
            calculatedprob[i].altered *= Probability[1][21];
        break;
        }
        
        calculatedprob[i].normal *= Probability[0][0];
        calculatedprob[i].altered *= Probability[1][0];
        
        
        if(calculatedprob[i].normal >= calculatedprob[i].altered)
        {
            calculatedprob[i].predictedoutcome = 0;
        }
        else
        {
            calculatedprob[i].predictedoutcome = 1;
        }
        printf("\nindex %i, normal prob: %f, altered prob: %f, prediction: %f",i, calculatedprob[i].normal, calculatedprob[i].altered ,calculatedprob[i].predictedoutcome);
        

        
    

        
    }
    


}


void NBProbability()
{    
    printf("Size of array: %d", arrlength);
    //Loops through the total size of the training set. Gets the limit from the size of the output array as output array and features strut are One to One
    for(int i = 0; i < arrlength; i++)
    {
        //Determines if the sample is normal(0) or altered(1)
        switch (trainingO[i])
        {
        case 0 : //normal
            Count(0,i);
            break;
        
        case 1: //altered
            Count(1,i);
            break;
        }
    }
    //Determining posterior probability
    for (int k = 0; k < 2; k++)
    {
        for (int j = 1; j < 22; j++)
        {
            Probability[k][j] = Probability[k][j]/Probability[k][0];
            printf("\n outcome: %d Variable: %d Probability: %f", k, j, Probability[k][j]);
        }
        
    }
    

    // Obtaining the mean and varience

    //calculates the mean by taking the sum from the GaussianMeanVarience Array and count from the Probability array and stores it into the back into the GaussianMeanVarience Array
    printf("\n normal Count: %f", Probability[0][0]);
    for (int l = 0; l<=1; l++)
    {
        for (int m = 0; m <=1; m++)
        {
            Mean(&GaussianMeanVariance[l][m][0], &Probability[l][0], &GaussianMeanVariance[l][m][0]);
            variancesquared(&GaussianMeanVariance[l][m][0], &Probability[l][0], l, m, &GaussianMeanVariance[l][m][1] ); 
        }
        
    }

    //determining prior probability
    Probability[0][0] = Probability[0][0]/arrlength;
    Probability[1][0] = Probability[1][0]/arrlength;
    printf("\nPrior Prob:\n Normal: %f \n Altered: %f", Probability[0][0],Probability[1][0]);
    
}

// Takes in if the sample is normal or altered and the row to check.
void Count(int outcome, int i)
{
    // Count for the different seasons
    Probability[outcome][0]++;
    if (trainingF[i].season == -1) //Winter
    {
        Probability[outcome][1]++;
    }
    else if (trainingF[i].season == -0.33) //Spring
    {
        Probability[outcome][2]++;
    }
    else if (trainingF[i].season == 0.33) //Summer
    {
        Probability[outcome][3]++;
    }
    else if (trainingF[i].season == 1) //Fall
    {
        Probability[outcome][4]++;
    }
    
    //Determining the count for the disease
    switch (trainingF[i].disease)
    {
    case 0: //Yes
        Probability[outcome][5]++;
        break;
    
    case 1: //No
        Probability[outcome][6]++;
        break;
    }

    switch (trainingF[i].trauma)
    {
    case 0: //Yes
        Probability[outcome][7]++;
        break;
    
    case 1: //No
        Probability[outcome][8]++;
        break;
    }

    switch (trainingF[i].surgical)
    {
    case 0: //Yes
        Probability[outcome][9]++;
        break;
    
    case 1: //No
        Probability[outcome][10]++;
        break;
    }

    switch (trainingF[i].fever)
    {
    case -1: //< 3 months ago
        Probability[outcome][11]++;
        break;
    
    case 0: // > 3 months ago
        Probability[outcome][12]++;
        break;

    case 1: // no fever
        Probability[outcome][13]++;
        break;     
    }

    if (trainingF[i].freq == 0.2) // Several times a day
    {
        Probability[outcome][14]++;
    }
    else if (trainingF[i].freq == 0.4) // Every Day
    {
        Probability[outcome][15]++;
    }
    else if (trainingF[i].freq == 0.6) // Several Times a week
    {
        Probability[outcome][16]++;
    }
    else if (trainingF[i].freq == 0.8) // Once a week
    {
        Probability[outcome][17]++;
    }
    else if (trainingF[i].freq == 1) // Hardly
    {
        Probability[outcome][18]++;
    }
    
    switch (trainingF[i].smoke)
    {
    case -1:  //never
        Probability[outcome][19]++;
        break;
    
    case 0: // occasional
        Probability[outcome][20]++;
        break;

    case 1: // daily
        Probability[outcome][21]++;
        break;     
    }

    // Used to obtain the summation of the normal and altered values for features requiring gaussian
    GaussianMeanVariance[outcome][0][0]+=trainingF[i].age;
    printf("\n %d normal age sum %f",i ,GaussianMeanVariance[0][0][0]);
    GaussianMeanVariance[outcome][1][0]+=trainingF[i].sit;

}

void Mean(float *sum, float *count, float *storelocation)
{
    printf("\n sum: %f", *sum);
    printf("\n count: %f", *count);
    float mean = 0;
    mean = (1.0/(*count)) * (*sum);
    *storelocation = mean;
    printf("\n Mean: %f", mean);

}

void variancesquared(float *mean, float *count, int outcome, int feature, float *variencestorelocation)
{
    switch (feature)
    {
    case 0:
        for (int i = 0; i < arrlength; i++)
        {
            if(trainingO[i]==outcome)
            {
                *variencestorelocation += pow((trainingF[i].age-*mean),2);
            }
        }
        
        break;
    
    case 1:
        for (int i = 0; i < arrlength; i++)
        {
            if(trainingO[i]==outcome)
            {
                *variencestorelocation += pow((trainingF[i].sit-*mean),2);
            }
        }
        break;
    }
    
    *variencestorelocation *= 1.0/(*count-1);
    printf("\n varience: %f", *variencestorelocation);
}

float Gaussian(float inputx, float *mean, float *varience)
{
    return (1/(sqrt(2*pi)))*exp(-0.5*(pow((inputx-*mean),2)/ *varience ));
}

