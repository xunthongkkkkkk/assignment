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

//So that it can read 100 line of text file
struct dataset volunteers[100];

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

            printf("%.2f %.2f %d %d %d %d %.2f %d %.2f %d \n", volunteers[i].season, 
                volunteers[i].age, volunteers[i].disease, volunteers[i].trauma, 
                volunteers[i].surgical, volunteers[i].fever, volunteers[i].freq, 
                volunteers[i].smoke, volunteers[i].sit, volunteers[i].output);
        }
    }
}