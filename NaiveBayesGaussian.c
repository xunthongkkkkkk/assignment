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


// Function to calculate the probability for both NB and Gaussian.
//void ProbabilityCalculator();

// Function to calculate the prior and posterior probability for NB
void NBProbability();

// determine the feature for each category and increment the counter
void Count(int outcome, int i);

// Function to calculate the mean for the features requiring gaussian Takes in the sum, and count and pointer to address of where to store the mean
void Mean(float *sum, float *count, float *meanstorelocation);

// Function that returns the squared varience of 
void variancesquared(float *mean, float *count, int outcome, int feature, float *variencestorelocation);

// Function to calculate and return the gaussian probability 
double Gaussian(float inputx, float *mean, float *varience);

// Function to grab probability of feature
float NBProbabilityGrab(int output,int feature,float input);

// Function to calculate posterior probability and print prediction
void ProbabilityMain(int count);


//So that it can read 100 line of text file
struct dataset *volunteers;

//Feature and Output set for training & testing set

/*array to store the prior and conditional probability for both normal (0) and altered(1). Prior Probability is stored in index 0.
The remaining features are indexed as per listed on the AE document 
Syntax: Probability[Outcome][feature]  */
float Probability[2][22];
/* array to store the both [normal(0) and altered(1)] [age(0) or time spent sitting(1)] [mean(0) and varience squared(1)].
 Syntax: GaussianMeanVarience[outcome][feature][mean/varience]
*/
float GaussianMeanVariance[2][2][2];
int datasplit; //stores the line the data starts spliting between testing and training

int main(void) {

    int count = 0;
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

    }
    
    NBProbability();
    ProbabilityMain(count);
    
}

/*
void ProbabilityCalculator()
{
    
}

*/

void NBProbability()
{    
    printf("Size of array: %d", datasplit-1);
    //Loops through the total size of the training set. Gets the limit from the size of the output array as output array and features strut are One to One
    for(int i = 0; i < datasplit; i++)
    {
        //Determines if the sample is normal(0) or altered(1)
        switch (volunteers[i].output)
        {
        case 0 : //normal
            Count(0,i);
            break;
        
        case 1: //altered
            Count(1,i);
            break;
        }
    }
    //Determining conditional probability
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
    Probability[0][0] = Probability[0][0]/(datasplit-1);
    Probability[1][0] = Probability[1][0]/(datasplit-1);
    printf("\nPrior Prob:\n Normal: %f \n Altered: %f", Probability[0][0],Probability[1][0]);
    
}

// Takes in if the sample is normal or altered and the row to check.
void Count(int outcome, int i)
{
    // Count for the different seasons
    Probability[outcome][0]++;
    if (volunteers[i].season == -1) //Winter
    {
        Probability[outcome][1]++;
    }
    else if (volunteers[i].season == (float)(-0.33)) //Spring
    {
        Probability[outcome][2]++;
    }
    else if (volunteers[i].season == (float)(0.33)) //Summer
    {
        Probability[outcome][3]++;
    }
    else if (volunteers[i].season == 1) //Fall
    {
        Probability[outcome][4]++;
    }
    
    //Determining the count for the disease
    switch (volunteers[i].disease)
    {
    case 0: //Yes
        Probability[outcome][5]++;
        break;
    
    case 1: //No
        Probability[outcome][6]++;
        break;
    }

    switch (volunteers[i].trauma)
    {
    case 0: //Yes
        Probability[outcome][7]++;
        break;
    
    case 1: //No
        Probability[outcome][8]++;
        break;
    }

    switch (volunteers[i].surgical)
    {
    case 0: //Yes
        Probability[outcome][9]++;
        break;
    
    case 1: //No
        Probability[outcome][10]++;
        break;
    }

    switch (volunteers[i].fever)
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

    if (volunteers[i].freq == (float)(0.2)) // Several times a day
    {
        Probability[outcome][14]++;
    }
    else if (volunteers[i].freq == (float)(0.4)) // Every Day
    {
        Probability[outcome][15]++;
    }
    else if (volunteers[i].freq == (float)(0.6)) // Several Times a week
    {
        Probability[outcome][16]++;
    }
    else if (volunteers[i].freq == (float)(0.8)) // Once a week
    {
        Probability[outcome][17]++;
    }
    else if (volunteers[i].freq == 1) // Hardly
    {
        Probability[outcome][18]++;
    }
    
    switch (volunteers[i].smoke)
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
    GaussianMeanVariance[outcome][0][0]+=volunteers[i].age;
    printf("\n %d normal age (%f) sum %f",i ,volunteers[i].age,GaussianMeanVariance[0][0][0]);
    GaussianMeanVariance[outcome][1][0]+=volunteers[i].sit;

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
        for (int i = 0; i < datasplit; i++)
        {
            if(volunteers[i].output==outcome)
            {
                *variencestorelocation += pow((volunteers[i].age-*mean),2);
            }
        }
        
        break;
    
    case 1:
        for (int i = 0; i < datasplit; i++)
        {
            if(volunteers[i].output==outcome)
            {
                *variencestorelocation += pow((volunteers[i].sit-*mean),2);
            }
        }
        break;
    }
    
    *variencestorelocation *= 1.0/(*count-1);
    printf("\n varience: %f", *variencestorelocation);
}

double Gaussian(float inputx, float *mean, float *varience)
{
    return (1/(sqrt(2*pi)))*exp(-0.5*(pow((inputx-*mean),2)/ *varience ));

}

float NBProbabilityGrab(int output,int feature,float input){

    if(feature == 1){
        if (input == -1){
            return Probability[output][1];      //Winter
        }
        else if (input == (float)(-0.33)){
            return Probability[output][2];       //Spring
        }
        else if (input == (float)(0.33)){
            return Probability[output][3];      //Summer
        }
        else if (input == 1){
            return Probability[output][4];       //Fall
        }
    }
    if(feature == 3){
        if (input == 0){
            return Probability[output][5];      //Yes
        }
        else if (input == 1){
            return Probability[output][6];       //No
        }
    }
    if(feature == 4){
        if (input == 0){
            return Probability[output][7];       //Yes
        }
        else if (input == 1){
            return Probability[output][8];       //No
        }
    }
    if(feature == 5){
        if (input == 0){
            return Probability[output][9];       //Yes
        }
        else if (input == 1){
            return Probability[output][10];       //No
        }
    }
    if(feature == 6){
        if (input == -1){
            return Probability[output][11];       //Less than 3 months ago
        }
        else if (input == 0){
            return Probability[output][12];       //More than 3 months ago
        }
        else if (input == 1){
            return Probability[output][13];      //No
        }
    }
    if(feature == 7){
        if (input == (float)(0.2)){
            return Probability[output][14];       //Saveral times a day
        }
        else if (input == (float)(0.4)){
            return Probability[output][15];       //Every day
        }
        else if (input == (float)(0.6)){
            return Probability[output][16];      //Several times a week
        }
        else if (input == (float)(0.8)){
            return Probability[output][17];      //Once a week
        }
        else if (input == 1){
            return Probability[output][18];      //Hardly ever or never
        }
    }
    if(feature == 8){
        if (input == -1){
            return Probability[output][19];       //Never
        }
        else if (input == 0){
            return Probability[output][20];       //Occasionally
        }
        else if (input == 1){
            return Probability[output][21];      //Daily
        }
    }
    
}

void ProbabilityMain(int count){
    for (int i=(datasplit-1); i <=count; i++){
        double normalpostprob = 0;
        double alteredpostprob = 0;
        //printf("x = %d, y = %d", x,y);
        normalpostprob = (NBProbabilityGrab(0,1,volunteers[i].season) *Gaussian(volunteers[i].age,&GaussianMeanVariance[0][0][0],&GaussianMeanVariance[0][0][1])* NBProbabilityGrab(0,3,volunteers[i].disease) 
        * NBProbabilityGrab(0,4,volunteers[i].trauma) * NBProbabilityGrab(0,5,volunteers[i].surgical) 
        * NBProbabilityGrab(0,6,volunteers[i].fever) * NBProbabilityGrab(0,7,volunteers[i].freq) 
        * NBProbabilityGrab(0,8,volunteers[i].smoke) *Gaussian(volunteers[i].sit,&GaussianMeanVariance[0][1][0],&GaussianMeanVariance[0][1][1]) * Probability[0][0]);

        alteredpostprob = (NBProbabilityGrab(1,1,volunteers[i].season) *Gaussian(volunteers[i].age,&GaussianMeanVariance[1][0][0],&GaussianMeanVariance[1][0][1]) * NBProbabilityGrab(1,3,volunteers[i].disease) 
        * NBProbabilityGrab(1,4,volunteers[i].trauma) * NBProbabilityGrab(1,5,volunteers[i].surgical) 
        * NBProbabilityGrab(1,6,volunteers[i].fever) * NBProbabilityGrab(1,7,volunteers[i].freq) 
        * NBProbabilityGrab(1,8,volunteers[i].smoke)*Gaussian(volunteers[i].sit,&GaussianMeanVariance[1][1][0],&GaussianMeanVariance[1][1][1])* Probability[1][0]);
        
        if (normalpostprob >= alteredpostprob){
            printf("\nLine %d: Normal Probability: %e  Altered Probability: %e  Prediction: Normal", i+1, normalpostprob, alteredpostprob);
        }
        else{
            printf("\nLine %d: Normal Probability: %e  Altered Probability: %e  Prediction: Altered", i+1, normalpostprob, alteredpostprob);
        }
    }
}
