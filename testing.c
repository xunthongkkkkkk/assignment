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