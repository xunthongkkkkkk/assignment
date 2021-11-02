#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// 
void NBcount(int outcome, int i);

//So that it can read 100 line of text file
struct dataset volunteers[100];

//Feature and Output set for training & testing set
struct feature trainingF[80];
int trainingO[80];
struct feature testingF[20];
int testingO[20];

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

        for(i = 0; i < 80; i++){
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

        for(i = 80; i < 100; i++){
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
}

void NBProbability()
{
    //Loops through the total size of the training set. Gets the limit from the size of the output array as output array and features strut are One to One
    for(int i = 0; i < sizeof(trainingO); i++)
    {
        //Determines if the sample is normal(0) or altered(1)
        switch (trainingO[i])
        {
        case 0 : //normal
            NBcount(0,i);
            break;
        
        case 1: //altered
            NBcount(1,i);
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

    //determining prior probability
    Probability[0][0] = Probability[0][0]/sizeof(trainingO);
    Probability[1][0] = Probability[1][0]/sizeof(trainingO);
    printf("\nPrior Prob:\n Normal: %f \n Altered: %f", Probability[0][0],Probability[1][0]);
    
}

void NBcount(int outcome, int i)
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
}
