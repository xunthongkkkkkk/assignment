#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

// Function to calculate the prior and posterior probability for NB
void NBProbability();

// determine the feature for each category and increment the counter
void Count(int outcome, int i);

// Function to calculate the mean for the features requiring gaussian Takes in the sum, and count and pointer to address of where to store the mean
void Mean(double *sum, float *count, double *meanstorelocation);

// Function that returns the squared varience
void variancesquared(int start, int stop, double *mean, float *count, int outcome, int feature, double *variencestorelocation);

// Function to calculate and return the gaussian probability 
double Gaussian(float inputx, double *mean, double *varience);

// Function to grab probability of feature
float NBProbabilityGrab(int output,int feature,float input);

// Function to calculate posterior probability and print prediction. Takes in the index to start index to stop and the pointer to the struct to check
void ProbabilityMain(int start, int stop, struct dataset *datacheck);

//Function to calculate Probability of Error
void CalProbError(int start, int split ,int end);

//Function to plot graph of Probability of Error
void plotGraph(int count);

//Function to plot confusion matrix
void plotMatrix(int *matrix, int testing, int training, char *string);

//So that it can read 100 line of text file
struct dataset *volunteers;

//Feature and Output set for training & testing set

/*array to store the prior and conditional probability for both normal (0) and altered(1). Prior Probability is stored in index 0.
The remaining features are indexed as per listed on the AE document 
Syntax: Probability[Outcome][feature]  */
float Probability[2][22]={0};
/* array to store the both [normal(0) and altered(1)] [age(0) or time spent sitting(1)] [mean(0) and varience squared(1)].
 Syntax: GaussianMeanVarience[outcome][feature][mean/varience]
*/
double GaussianMeanVariance[2][2][2]={0};


int ConMatrix[4]={0};//TP(1)TN(2)FP(3)FN(4)

double tr_ProbError[5]; //50, 60, 70, 80, 90
double te_ProbError[5]; //50, 40, 30, 20, 10

int main(void) {
    int datasplit; //stores the line the data starts spliting between testing and training
    int choice;
    //start record time
    clock_t start = clock();

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
    printf("Sperm Fertility Prediction Using Naive Bayes and Gaussian Distrubution\n\nDo you wish to perform predictions(1) or enter diagnostic mode for the program(2)? : ");
    //Obtain choice of operation
    while (1)
    {
        scanf("%d", &choice);
        if(choice == 1 || choice == 2)
        {
            break;
        }
        printf("Please Enter A Valid Input!\n");
    }
    int numRecord;
    struct dataset drInput[1];
    switch (choice)
    {
    case 1://Prediction Mode Takes doctor input and predict using whole dataset as training

        while (1)
        {
            printf("\nPlease Input value for Season of analysis:");
            scanf("%f",&drInput[0].season);
            if(drInput[0].season == -1 || drInput[0].season == (float)(-0.33)|| drInput[0].season == (float)(0.33)||drInput[0].season == 1) //verify if the value is valid
            {
                break;
            }

            printf("\nPlease input a valid value for Season of analysis!\n");
        }
    
        
        while (1)
        {
            printf("\nPlease Input value for Age of Analysis:");
            scanf("%f",&drInput[0].age);
            if(drInput[0].age >= 0 && drInput[0].age <= 1 )//verify if the value is valid
            {
                break;
            }
            printf("\nPlease input a valid value for Age of Analysis! \n");
        }

        while (1)
        {
            printf("\nPlease Input value for Childish Disease:");
            scanf("%d",&drInput[0].disease);
            if(drInput[0].disease == 0 || drInput[0].disease == 1 )
            {
                break;
            }
            printf("\nPlease Input a valid value for Childish Disease! \n");
        }
        
        while (1)
        {
            printf("\nPlease Input value for Accident or serious trauma:");
            scanf("%d",&drInput[0].trauma);
            if(drInput[0].trauma == 0 || drInput[0].trauma == 1 )
            {
                break;
            }
            printf("\nPlease Input a valid value for Accident or serious trauma! \n");
        }
        
        while (1)
        {
            printf("\nPlease Input value for Surgical Intervention:");
            scanf("%d",&drInput[0].surgical);
            if(drInput[0].surgical == 0 || drInput[0].surgical == 1 )
            {
                break;
            }
            printf("\nPlease Input a valid value for Surgical Intervention! \n");
        }
        
        while (1)
        {
            printf("\nPlease Input value for High fevers in last year:");
            scanf("%d",&drInput[0].fever);
            if(drInput[0].fever == 0 || drInput[0].fever == 1 || drInput[0].fever==-1)
            {
                break;
            }
            printf("\nPlease Input a valid value for High fevers in last year! \n");
        }
        
        while (1)
        {
            printf("\nPlease Input value for Frequency of alcohol consumption:");
            scanf("%f",&drInput[0].freq);
            if(drInput[0].freq == 1 || drInput[0].freq == (float)(0.2)|| drInput[0].freq == (float)(0.4)||drInput[0].freq == (float)(0.6)||drInput[0].freq == (float)(0.8))
            {
                break;
            }
            printf("\nPlease Input a valid value for Frequency of alcohol consumption! \n");
        }

        while (1)
        {
            printf("\nPlease Input value for Smoking Habit:");
            scanf("%d",&drInput[0].smoke);
            if(drInput[0].smoke == 0 || drInput[0].smoke == 1 || drInput[0].smoke==-1)
            {
                break;
            }
            printf("\nPlease Input a valid value for Smoking Habit! \n");
        }
        
        while (1)
        {
            printf("\nPlease Input value for Number of hours spent sitting per day:");
            scanf("%f",&drInput[0].sit);
            if(drInput[0].sit >= 0 && drInput[0].sit <= 1 )
            {
                break;
            }
            printf("\nPlease Input a valid value for Number of hours spent sitting per day! \n");
        }
        
        drInput[0].output= 1;
        NBProbability(0,count); //use whole dataset for training
        ProbabilityMain(0,0,drInput); // calculate posterior prob and predict
        if (ConMatrix[0]==1)
        {
            printf("\nPrediction: Altered");
        }
        else if (ConMatrix[3]==1)
        {
            printf("\nPrediction: Normal");
        }

        break;
    
    case 2://diagnostic mode
        datasplit=81;
        NBProbability(0,datasplit-2);
        ProbabilityMain(0,datasplit-2,volunteers);
        ProbabilityMain(datasplit-1,count,volunteers);
        plotGraph(count);

        break;
    }


    //end record time
    clock_t end = clock();
    double timetaken = (double)(end-start) / CLOCKS_PER_SEC;
    printf("\n \nTime Taken to Complete the Program: %.2f sec", timetaken);

}

void NBProbability(int start, int stop)
{    
    int totalcount =0;
    //zerorize Probability and GaussianMeanVarience Array
    memset(Probability, 0, sizeof(Probability[0][0])*2*22);
    memset(GaussianMeanVariance, 0, sizeof(GaussianMeanVariance[0][0][0])*2*2*2);
    //Loops through the total size of the training set. Gets the limit from the size of the output array as output array and features strut are One to One
    for(int i = start; i <= stop; i++)
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
    for (int k = 0; k <=1; k++)
    {
        for (int j = 1; j <=21; j++)
        {
            
            Probability[k][j] = (Probability[k][j]+1)/(Probability[k][0]+21 ); //accounts for the fact that there are features that do not appear in the training data. Setting alpha as 1
        }
        
    }
    

    // Obtaining the mean and varience

    //calculates the mean by taking the sum from the GaussianMeanVarience Array and count from the Probability array and stores it into the back into the GaussianMeanVarience Array
    for (int l = 0; l<=1; l++)
    {
        for (int m = 0; m <=1; m++)
        {
            Mean(&GaussianMeanVariance[l][m][0], &Probability[l][0], &GaussianMeanVariance[l][m][0]);
            variancesquared(start, stop, &GaussianMeanVariance[l][m][0], &Probability[l][0], l, m, &GaussianMeanVariance[l][m][1] ); 
        }
        
    }

    //determining prior probability
    totalcount = Probability[0][0] + Probability[1][0];
    Probability[0][0] = Probability[0][0]/(totalcount);
    Probability[1][0] = Probability[1][0]/(totalcount);
    
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
    GaussianMeanVariance[outcome][1][0]+=volunteers[i].sit;

}

void Mean(double *sum, float *count, double *storelocation)
{

    float mean = 0;
    mean = (1.0/(*count)) * (*sum);
    *storelocation = mean;

}
// Perform calculations of for the varience to be used for gaussian probability. Takes in starting and stopping index, which feature to check, pointer to the mean 
// and count values and pointer to the location to store the varience
void variancesquared(int start, int stop, double *mean, float *count, int outcome, int feature, double *variencestorelocation)
{
    switch (feature) //check if its age (1)or sitting(2)
    {
    case 0:
        for (int i = start; i <= stop ; i++)
        {
            if(volunteers[i].output==outcome)
            {
                *variencestorelocation += pow((volunteers[i].age-*mean),2); //calculate varience and add it into the value of variencestorelocation
            }
        }
        
        break;
    
    case 1:
        for (int i = start; i <= stop ; i++)
        {
            if(volunteers[i].output==outcome)
            {
                *variencestorelocation += pow((volunteers[i].sit-*mean),2);
            }
        }
        break;
    }
    
    *variencestorelocation *= 1.0/(*count-1); //mutiply by 1/n as per the formula for varience

}

double Gaussian(float inputx, double *mean, double *varience)
{
    return (1/(sqrt(2*pi)))*exp(-0.5*(pow((inputx-*mean),2)/ *varience )); // calculate gaussian and return value as a double

}
//returns the probability of based on the output, feature and condition of feature entered
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

// Function calculates and determines posterior probability and confusion matrix
void ProbabilityMain(int start, int stop, struct dataset * datacheck ){
    //zeroize Con Matrix
    memset(ConMatrix, 0, sizeof(ConMatrix)); 
    //Calculate poesterior probability
    for(int t = start; t <= stop; t++){
        int tr_result = 0;
        double tr_normalpostprob = 0;
        double tr_alteredpostprob = 0;
        //printf("x = %d, y = %d", x,y);
        tr_normalpostprob = (log(NBProbabilityGrab(0,1,datacheck[t].season)) + log(Gaussian(datacheck[t].age,&GaussianMeanVariance[0][0][0],&GaussianMeanVariance[0][0][1]))+ log(NBProbabilityGrab(0,3,datacheck[t].disease)) 
        + log(NBProbabilityGrab(0,4,datacheck[t].trauma)) + log(NBProbabilityGrab(0,5,datacheck[t].surgical)) 
        + log(NBProbabilityGrab(0,6,datacheck[t].fever)) + log(NBProbabilityGrab(0,7,datacheck[t].freq)) 
        + log(NBProbabilityGrab(0,8,datacheck[t].smoke)) +log(Gaussian(datacheck[t].sit,&GaussianMeanVariance[0][1][0],&GaussianMeanVariance[0][1][1])) + log(Probability[0][0]));

        tr_alteredpostprob = (log(NBProbabilityGrab(1,1,datacheck[t].season)) +log(Gaussian(datacheck[t].age,&GaussianMeanVariance[1][0][0],&GaussianMeanVariance[1][0][1])) + log(NBProbabilityGrab(1,3,datacheck[t].disease)) 
        + log(NBProbabilityGrab(1,4,datacheck[t].trauma)) + log(NBProbabilityGrab(1,5,datacheck[t].surgical)) 
        + log(NBProbabilityGrab(1,6,datacheck[t].fever)) + log(NBProbabilityGrab(1,7,datacheck[t].freq)) 
        + log(NBProbabilityGrab(1,8,datacheck[t].smoke))+ log(Gaussian(datacheck[t].sit,&GaussianMeanVariance[1][1][0],&GaussianMeanVariance[1][1][1]))+ log(Probability[1][0]));
        
        if (tr_normalpostprob >= tr_alteredpostprob)
        {
           
            tr_result = 0;
        }
        else
        {
    
            tr_result = 1;
        }
        //check if the result is true or false
        if (datacheck[t].output == tr_result)
        {
            switch (tr_result)//check if its true positive of true negetive
            {
            case 1:
                ConMatrix[0]++; //True Positive
                break;
            
            case 0:
                ConMatrix[1]++;//True Negetive
                break;
            }
        }
        else
        {
            switch (tr_result)
            {
            case 1:
                ConMatrix[2]++; //False Positive
                break;
            
            case 0:
                ConMatrix[3]++; //False Negetive
                break;
            }
        }
    }

}

//Perform the calculations for the training split and call function to print confusion matrix
void CalProbError(int start , int split ,int end)
{
    char TrainTest[2][15]={"Training","Testing"};//string to print title of confusion matrix
    NBProbability(start,split);//calculate prior and conditional probability of the training set
    ProbabilityMain(start,split-1,volunteers);//Calculate posterior probability and populate confusion matrix
    tr_ProbError[(split/10)-5]=((float)(ConMatrix[2]+ConMatrix[3])/(float)(ConMatrix[0]+ConMatrix[1]+ConMatrix[2]+ConMatrix[3]));
    plotMatrix(ConMatrix,(1+end-split),split,TrainTest[0]);//plot confusion matrix of the training split
    ProbabilityMain(split,end,volunteers);//Calculate posterior probability and populate confusion matrix
    te_ProbError[(split/10)-5]=((float)(ConMatrix[2]+ConMatrix[3])/(float)(ConMatrix[0]+ConMatrix[1]+ConMatrix[2]+ConMatrix[3]));
    plotMatrix(ConMatrix,(1+end-split),split,TrainTest[1]);//Plot confusion matrix of the testing split
}

//Plot confusion matrix 
void plotMatrix(int *matrix, int testing, int training, char *string)
{
    
    FILE *gnuplot = popen("gnuplot -persistent", "w"); //file pointer to GNUPlot program
    fprintf(gnuplot, "reset \n" // Styling commands
        "$matrix <<EOD \n"
        "%d %d \n"//Pass values from comatrix in to datablock here
        "%d %d \n"
        "EOD \n"
        "set autoscale fix\n"
        "set tics scale 0\n"
        "set xrange[-0.5:1.5] \n"
        "set yrange [-0.5:1.5] \n"
        "unset colorbox \n"
        "set palette defined (0 'red', 1 'green') \n"
        "set title \"%s dataset %d:%d split\" \n"
        "set label 1 \"True Negetive\" at -0.15, 0.8 front nopoint\n"
        "set label 2 \"False Negetive\" at 0.85, 0.8 front nopoint\n"
        "set label 3 \"False Positive\" at -0.15, -0.25 front nopoint\n"
        "set label 4 \"True Positive\" at 0.85, -0.25 front nopoint\n"
        "unset cbtics \n"
        "unset key \n"
        "plot '$matrix' matrix with image,\'' matrix using 1:2:(sprintf('%%d',$3)) with labels font ',16' \n", matrix[2],matrix[0],matrix[1],matrix[3],string,training,testing
    
    );
}

void plotGraph(int count){
    //calculate probability of error based on different split set of data
    //e.g 50:50, 60:40, 70:30, ...
    for(int split = 50; split <= 90; split+= 10)
    {
        CalProbError(0, split, count);
    }

    //send styling command to gnu plot
    //training data plot
    char *trte_commandsGnuPlot[] = {
        "set title \"Probability of Error\"",
        "set xrange [1:5]",
        "set yrange [0:0.5]",
        "set xtics (\"50:50\" 1, \"60:40\" 2, \"70:30\" 3, \"80:20\" 4, \"90:10\" 5)",
        "plot 'trainingset.temp' with lines, 'testingset.temp' with lines"
    };

    //x label for plotting training set and testing set
    char tr_x[5][10] = {"50:50", "60:40", "70:30", "80:20", "90:10"};

    FILE *temp_tr = fopen("trainingset.temp", "w");
    FILE *temp_te = fopen("testingset.temp", "w");

    FILE *trte_gnuplot = popen("gnuplot -persistent", "w");

    int i;

    //pass probability of error data value to temp file
    for(int i = 0; i < 5; i++){
        fprintf(temp_tr, "%d %f\n", i+1, tr_ProbError[i]);
        fprintf(temp_te, "%d %f\n", i+1, te_ProbError[i]);
    }

    //pass command to gnu plot
    for(int i = 0; i < 5; i++){
        fprintf(trte_gnuplot, "%s \n", trte_commandsGnuPlot[i]);
    }


}