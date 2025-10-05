/*-------------------------------------------------------------------------------------------------
Project 2: Food webs and dynamic memory
           Creates a web(dynamic array) of organisms that the user enters, allows user to enter
           new organisms, delete organsisms, print the organisms, allows debug/basic/quit modes of operation
           and allows user to add relations between organisms (which eats which)
Course: CS 211, Spring 2025, UIC
Author: Besian Tahiri           
--------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Org_struct {
    char name[20];
    int* prey; //dynamic array of indices  
    int numPrey;
} Org;

//this function adds a organism to our web, returns void, and takes a Org** (pointer to pointer of
//our web(heap)),int* (number of organisms), and a char*(name of our new organism) parameter
void addOrgToWeb(Org** pWeb, int* pNumOrgs, char* newOrgName) {
    Org* newWeb = (Org*)malloc(sizeof(Org) * (*pNumOrgs + 1));
    for(int i =0; i < *pNumOrgs;++i){//we rewrite from old array into new array
        strcpy(newWeb[i].name,pWeb[0][i].name);
        newWeb[i].prey = pWeb[0][i].prey;
        newWeb[i].numPrey = pWeb[0][i].numPrey; 
    }
    free(*pWeb); //free old array
    strcpy(newWeb[*pNumOrgs].name,newOrgName); //we add new organism
    newWeb[*pNumOrgs].prey = NULL;
    newWeb[*pNumOrgs].numPrey = 0;
    *pNumOrgs +=1; //update number of organisms now
    *pWeb = newWeb; // set our old pointer to point to the new, bigger array
    
    return; // this void return statement is optional

}

//this function returns bool (indicating success of function or fail), takes a Org* (our web),
// int(number of organisms),int(the predators index whose relation we wish to change) and another
//int (index of prey we wish to add) parameter. Its purpose is to add a relation to one of the organisms in our web, it adds a prey
//to a particular organism
bool addRelationToWeb(Org* web, int numOrgs, int predInd, int preyInd) {
    if(predInd < 0 || preyInd < 0){ //we are checking first if the indices are valid
        return false;
    }
    if(predInd >= numOrgs || preyInd >= numOrgs){ //here too
        return false;
    }
    if(predInd == preyInd){//here too
        return false;
    }
    int* newPrey = (int*)malloc(sizeof(int) * (web[predInd].numPrey + 1));//we allocate a bigger prey
    for(int i = 0; i < web[predInd].numPrey;++i){                               //array
        if(web[predInd].prey[i] == preyInd){//if we have two identical indices we return false
            return false;
        }
        newPrey[i] = web[predInd].prey[i]; //copy from old array into new array
    }
    newPrey[web[predInd].numPrey] = preyInd; //append new index
    free(web[predInd].prey); //free old array
    web[predInd].prey = newPrey; //set old array to point to our new, bigger, array
    web[predInd].numPrey++; //update number of preys variable
    return true; //everything went good, return true
    
}

//this function removes one organism from or dynamic web array, it takes a Org**( pointer to pointer in
//main), int* (our number of organisms in web), and another int (which is the index we wish to remove)
//it returns bool indicating success of function
bool removeOrgFromWeb(Org** pWeb, int* pNumOrgs, int index) {
    Org* newWeb;
    int* newPrey;
    
    if(index < 0 || index >= (*pNumOrgs)){ //if out of bounds index return false
        printf("Invalid extinction index. No organism removed from the food web.\n");
        return false;
    }
    if(*pNumOrgs == 1){
        if(pWeb[0][0].prey != NULL){
            free(pWeb[0][0].prey); //if organism eats itself we free the prey array
        }
        free(*pWeb); //we free the web array
        *pWeb = NULL; // and we set our web array to point to NULL
        *pNumOrgs = 0;
        return true;//return true
    }
    
    else{
        free(pWeb[0][index].prey);//we free exinct organisms prey array
        newWeb = (Org*)malloc(sizeof(Org) * (*pNumOrgs - 1)); //new array is 1 smaller than previous 
        for(int i = index; i < *pNumOrgs - 1;++i){                //number of organsims
            
                newWeb[i] = pWeb[0][i+1]; //if are at index or greater we copy the i + 1 organism
            
        }
        for(int i = 0; i < index;++i){ //from 0 to index we copy as normal
            newWeb[i] = pWeb[0][i];
        }
         
        free(*pWeb);//free old array
        *pWeb = newWeb; //set it to point to new one
        *pNumOrgs -= 1; //reduce number of organisms by one
        
        for(int i = 0; i < *pNumOrgs;++i){//we go through each organsims prey array now
            if(pWeb[0][i].numPrey == 1 && pWeb[0][i].prey[0] == index ){
                free(pWeb[0][i].prey);//in case we have only 1 prey and that happens to be our extinct
                pWeb[0][i].prey = NULL;//organism we free the prey array, set it to null and set
                pWeb[0][i].numPrey = 0;// the number of preys to 0
            }
            else{    
                for(int j = 0; j < pWeb[0][i].numPrey;++j){ //we look through each prey in each organsim
                    
                    if(pWeb[0][i].prey[j] == index){//we found an index that is our extinct
                        newPrey =(int*)malloc(sizeof(int) * (pWeb[0][i].numPrey - 1));//allocate new arr
                        for(int z = 0; z < j;++z){//for all preys that are below the extinct index j
                            newPrey[z] = pWeb[0][i].prey[z];//we copy as normal
                        }
                        for(int z = j; z < pWeb[0][i].numPrey - 1;++z){
                            newPrey[z] = pWeb[0][i].prey[z + 1];//now at or above index we shift one
                        }                                       //forward
                        free(pWeb[0][i].prey);//we free our old array
                        pWeb[0][i].prey = newPrey;//set it to point to new one
                        pWeb[0][i].numPrey -= 1;//decrement number of preys by one
                        
                        

                    }  
                    
                    
                }
               
                        
                
            }
            for(int z = 0; z < pWeb[0][i].numPrey;++z){//after we have gotten rid of any extinct
                if(pWeb[0][i].prey[z] > index){//preys from the prey array of the ith organism
                    pWeb[0][i].prey[z] -= 1;    //we reduce each preys index value by one if it was >
                }                               //index (since we shifted everything above index forward)
            }
        }
        return true; //everything went good so return true
    }    
       
}

//function with purpose of freeing our web(heap allocated memory).returns void and takes Org* (our web)
//and int (number of organisms in web)
//as parameters
void freeWeb(Org* web, int numOrgs) {

    for(int i = 0; i < numOrgs;++i){
        free(web[i].prey);
    }
    free(web);
}
//function which returns void, takes Org*(our web) and int(number of organisms) as parameters, with
//purpose of printing our web in a simple manner
void printWeb(Org* web, int numOrgs) {
   
    for(int i = 0; i < numOrgs;++i){
        printf("  (%d) %s",i,web[i].name);// we print each organsims name
        if(web[i].numPrey != 0){//if they have preys we print out " eats "
            printf(" eats ");
            for(int j = 0; j < web[i].numPrey - 1;++j){ //followed by "name, " for each prey prior to last
                printf("%s, ",web[web[i].prey[j]].name);
            }
            printf("%s ",web[web[i].prey[web[i].numPrey - 1]].name); //last is printed without ,

        }
        printf("\n");
    }
}

//function that returns void, prints our web in more detail, takes Org*(pointer to our web),int(number 
//of organisms), and bool (indicating mode of operation) as parameters
void displayAll(Org* web, int numOrgs, bool modified) {

    if (modified) printf("UPDATED ");
    printf("Food Web Predators & Prey:\n");
    printWeb(web,numOrgs); 
    printf("\n");
    bool marker[numOrgs]; //each organism that gets eaten will be marked here
    for(int i = 0; i < numOrgs;++i){       //ex: if organism with index 3 gets eaten index 3 of
        marker[i] = false;                                  //our marker will be marked true
    }
    if (modified) printf("UPDATED ");
    printf("Apex Predators:\n");
    for(int i = 0; i < numOrgs;++i){
        for(int j = 0; j < web[i].numPrey;++j){ //we loop through each prey of each organism
            marker[web[i].prey[j]] = true;    //if we find an index that is prey, that same index of
        }                                       //our marker becomes true
    }
    for(int i = 0; i < numOrgs;++i){
        if(marker[i] != true){          //if our marker is still false that means that organism is
            printf("  %s\n",web[i].name);//not eaten by anyone, therefore apex
        }
    }
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Producers:\n");
    for(int i = 0; i < numOrgs;++i){
        if(web[i].numPrey == 0){    //if we have 0 preys then that organism is a producer
            printf("  %s\n",web[i].name);
        }
    }
    printf("\n");
    int maxPrey = 0;
    if (modified) printf("UPDATED ");
    printf("Most Flexible Eaters:\n");//we declare a variable that will hold the maximum
    for(int i = 0; i < numOrgs;++i){
        if(web[i].numPrey > maxPrey){
            maxPrey = web[i].numPrey; //we find what the maximum amount of preys is in our web
        }
    }
    for(int i = 0; i < numOrgs;++i){
        if(web[i].numPrey == maxPrey){ //if a particular organism has the maximum preys we print it
            printf("  %s\n",web[i].name);
        }
    }
    printf("\n");
    int tastiestMarker[numOrgs]; //each value in array will correspond to how many times the organism
    for(int i = 0 ; i < numOrgs;++i){                               //with same index has been eaten
        tastiestMarker[i] = 0; //initialize to 0
    }
    if (modified) printf("UPDATED ");
    printf("Tastiest Food:\n");
    for(int i = 0 ; i < numOrgs;++i){
        for(int j = 0 ; j < web[i].numPrey;++j){
            tastiestMarker[web[i].prey[j]]++; //if we find a particular organism thats prey
        }                                       //we increment the corresponding index on our marker
    }
    int mostEaten = tastiestMarker[0];
    for(int i = 0; i < numOrgs;++i){        //we find index that has been eaten most times
        if(tastiestMarker[i] > mostEaten){
            mostEaten = tastiestMarker[i];
        }
    }
    for(int i = 0 ; i < numOrgs;++i){// if a particular index is the max, we print that one
        if(tastiestMarker[i] == mostEaten){
            printf("  %s\n",web[i].name);
        }
    }
    printf("\n");
    int heights[numOrgs]; //heights array
    for(int i = 0 ; i < numOrgs;++i){
        heights[i] = 0; //set all heights to 0
    }
    bool changedHeight = true;;//will be marked true if a change happend in our loop, false otherwise
    int maxPreyHeight = 0; //the max prey height will be stored here
    if (modified) printf("UPDATED ");
    printf("Food Web Heights:\n");
    while(changedHeight){
        changedHeight = false;
        for(int i = 0 ; i < numOrgs;++i){ //will loop through all organisms
            maxPreyHeight = heights[i] - 1; //maxpreyheight for each organism is their current height-1
            for(int j = 0 ;j < web[i].numPrey;++j){//we loop through each prey 
                if(heights[web[i].prey[j]] > maxPreyHeight){//if any heights our greater than our 
                    maxPreyHeight = heights[web[i].prey[j]];//current maxheight we set the new max
                    changedHeight = true;//we set changed height to true so the loop reiterates
                }
            }
            heights[i] = maxPreyHeight + 1;//our new height is set as one more the max height
        }// if we didnt change our height it would set heights[i] to same value as before
    }
    for(int i = 0 ; i < numOrgs;++i){
        printf("  %s: %d\n",web[i].name,heights[i]);//we print
    }
    

    if (modified) printf("UPDATED ");
    printf("Vore Types:\n");
    printf("  Producers:\n");
    for(int i = 0;i < numOrgs;++i){
        if(heights[i] == 0){ //if our height is 0 we have a producer so we print their name
            printf("    %s\n",web[i].name);
        }
    }
    printf("Herbivores:\n");
    for(int i = 0; i < numOrgs;++i){
        if(heights[i] == 1){ //if our height is 1 we have a herbivore so we print their name
            printf("    %s\n",web[i].name);
        }
    }
    printf("Omnivores:\n");
    bool omnivore;
    for(int i = 0; i < numOrgs;++i){
        omnivore = false; //assume we dont have an omnivore
        if(heights[i] > 1){ //if our height is > 1 we have a omnivore or carnivore 
            for(int j = 0; j < web[i].numPrey;++j){
                if(heights[web[i].prey[j]] == 0){ //we go through all preys, if we have one producer
                    omnivore = true; //we have a omnivore so we set the bool to true
                }
            }
            if(omnivore){
                 printf("    %s\n",web[i].name); //if we have an omnivore print it

            }
        }
    }
    bool carnivore;
    printf("Carnivores:\n");
     for(int i = 0; i < numOrgs;++i){
        if(heights[i] > 1){ //if height is > 1 we could have omnivore or carnivore
            carnivore = true;//assume we have a carnivore
            for(int j = 0; j < web[i].numPrey;++j){
                if(heights[web[i].prey[j]] == 0){ //if any of our preys is a producers we don have a 
                    carnivore = false; //carnivore so we set carnivore to false
                }
            }
            if(carnivore){
                printf("    %s\n",web[i].name);//if we had a carnivore we print it
            }
        }
    }
    printf("\n");

}
//function which returns bool (indicating success or not), purpose to set the mode of operation of our
//program by reading the command line inputs. Takes int (number of args from input), char* arr[] (our 
//arguments),bool* (will hold basicmode T-F value),bool*(will hole debugmode),bool* (will holdquitmode)
bool setModes(int argc, char* argv[], bool* pBasicMode, bool* pDebugMode, bool* pQuietMode) {

    if(argc >= 5 ){ //if extra arguments return false
        return false;
    }
    if(argc == 1){//if no arguments we return true since modes are already set to false
        return true;
    }
    for(int i = 1; i < argc;++i){
        if(argv[i][0] == '-' && argv[i][1] == 'b'){
            if(*pBasicMode){
                return false; //if duplicate return false
            }
            else{
                *pBasicMode = true;
            }
        }
        if(argv[i][0] == '-' && argv[i][1] == 'q'){
            if(*pQuietMode){
                return false; //if duplicate return false
            }
            else{
                *pQuietMode = true;
            }
        }
        if(argv[i][0] == '-' && argv[i][1] == 'd'){
            if(*pDebugMode){
                return false; //if duplicate return false
            }
            else{
                *pDebugMode = true;
            }
        }
        if(argv[i][0] != '-' || (argv[i][1] != 'b' &&  argv[i][1] != 'q' && argv[i][1] != 'd')){
            return false; //if any are invalid arguments we return false
        }
    }
    return true;
    
}

//this function returns void, takes one bool argument (which is our mode), and prints whether that
//mode is on or off
void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}


int main(int argc, char* argv[]) {  

    // -----------------------------------------------------------------------------------------------------
    // TODO (Task 1): add comments to the main application to show understanding
    //  DO NOT MODIFY THE CODE BENEATH THIS COMMENT (except for debugging purposes and adding comments).
    //  The code in main() represents the primary application, which has been provided for you.
    //  There are many calls to functions that you must write above. Read through the code below, understand  
    //  how it works, check how functions are called, and add relevant comments to show your understanding.
    //  However, no changes should be made to the code in main(). You can put in debugging print statements, 
    //  but those should be removed or commented out prior to submitting to the autograder.
    // -----------------------------------------------------------------------------------------------------
    
    //bool variables which will be set to true/false depending on what mode we are on
    bool basicMode = false;
    bool debugMode = false;
    bool quietMode = false; 

    if (!setModes(argc, argv, &basicMode, &debugMode, &quietMode)) {
        printf("Invalid command-line argument. Terminating program...\n");
        return 1;
    } //if incorrect arguments/number of arguments terminate program

    printf("Program Settings:\n");
    printf("  basic mode = ");
    printONorOFF(basicMode);
    printf("  debug mode = ");
    printONorOFF(debugMode);
    printf("  quiet mode = ");
    printONorOFF(quietMode);
    printf("\n");   //this block of code will print on screen which mode is on

    
    int numOrgs = 0;    //initial number of organisms is 0
    printf("Welcome to the Food Web Application\n\n");
    printf("--------------------------------\n\n");

    Org* web = NULL;    // initially our organism pointer points to nowhere, so we declare it to NULL

    printf("Building the initial food web...\n");
    //if quitMode is true then no prompts will be displayed, this is true for all if statements below
    if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
    char tempName[20] = "";
    scanf("%s",tempName);   //we declare a string which will hold the name of our organism and input
    if (!quietMode) printf("\n"); // from user
    while (strcmp(tempName,"DONE") != 0) { //if the user enters DONE we stop reading in organisms
        addOrgToWeb(&web,&numOrgs,tempName); //call function which adds an organism to our web array
        if (debugMode) {
            printf("DEBUG MODE - added an organism:\n");//if debugMode is true we will print the web
            printWeb(web,numOrgs);                      //after each newly added organism
            printf("\n");
        }
        if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
        scanf("%s",tempName); //read a new organsim name or DONE to finish
        if (!quietMode) printf("\n");
    }
    if (!quietMode) printf("\n");

    if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
    if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
    if (!quietMode) printf("The format is <predator index> <prey index>: ");//prompts if !quitMode
        
    int predInd, preyInd;   //we declare the variables which will hold the predator and prey indices
    scanf("%d %d",&predInd, &preyInd);//we read them from input
    if (!quietMode) printf("\n");
    //if indices are valid we enter the loop and add the relation to the web
    while (predInd >= 0 && preyInd >= 0 && predInd < numOrgs &&  preyInd < numOrgs && predInd != preyInd) {
        addRelationToWeb(web,numOrgs,predInd,preyInd);//we call the function to add a relation
        if (debugMode) {
            printf("DEBUG MODE - added a relation:\n");
            printWeb(web,numOrgs);//prints web if in debug mode
            printf("\n");
        }
        if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
        if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
        if (!quietMode) printf("The format is <predator index> <prey index>: ");//prompts if !quitMode
        
        scanf("%d %d",&predInd, &preyInd);  //reads new relation/pair of indices
        if (!quietMode) printf("\n");
    }
    printf("\n");

    printf("--------------------------------\n\n");
    printf("Initial food web complete.\n");
    printf("Displaying characteristics for the initial food web...\n");//prompts
    
    displayAll(web,numOrgs,false);//displays all info regarding our web

    if (!basicMode) {//if we are in basic mode program ends, if we arent then we could modify our web
        printf("--------------------------------\n\n");//and if statement executes
        printf("Modifying the food web...\n\n");
        char opt = '?'; //this variable will hold the option the user chooses to modify the web

        while (opt != 'q') { //while the user doesnt quit(choose 'q') we keep modifying
            if (!quietMode) {
                printf("Web modification options:\n");
                printf("   o = add a new organism (expansion)\n");
                printf("   r = add a new predator/prey relation (supplementation)\n");
                printf("   x = remove an organism (extinction)\n");
                printf("   p = print the updated food web\n");
                printf("   d = display ALL characteristics for the updated food web\n");
                printf("   q = quit\n");
                printf("Enter a character (o, r, x, p, d, or q): ");//prompts options
            }
            scanf(" %c", &opt);//reads in the option chosen by user
            if (!quietMode) printf("\n\n");

            if (opt == 'o') {//if user chose 'o' we add a new organsim to our web
                char newName[20];//declare new string which will hold new name
                if (!quietMode) printf("EXPANSION - enter the name for the new organism: ");
                scanf("%s",newName);//read in new name
                if (!quietMode) printf("\n");
                printf("Species Expansion: %s\n", newName);
                addOrgToWeb(&web,&numOrgs,newName);//adds new organism
                printf("\n");

                if (debugMode) {
                    printf("DEBUG MODE - added an organism:\n");
                    printWeb(web,numOrgs);
                    printf("\n");//if debug mode prints web
                }

            } else if (opt == 'x') {//if user chose 'x' we extinct/get rid of organism from our web
                int extInd; //index of organism to extinct
                if (!quietMode) printf("EXTINCTION - enter the index for the extinct organism: ");
                scanf("%d",&extInd); //read in index of organsim to extinct
                if (!quietMode) printf("\n");
                if (extInd >= 0 && extInd < numOrgs) {//if valid index we extinct
                    printf("Species Extinction: %s\n", web[extInd].name); //else print invalid index
                    removeOrgFromWeb(&web,&numOrgs,extInd);
                } else {
                    printf("Invalid index for species extinction\n");
                }
                printf("\n");
                
                if (debugMode) {//if debug mode print web
                    printf("DEBUG MODE - removed an organism:\n");
                    printWeb(web,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'r') {//if user chose 'r' we add a new relation
                if (!quietMode) printf("SUPPLEMENTATION - enter the pair of indices for the new predator/prey relation.\n");
                if (!quietMode) printf("The format is <predator index> <prey index>: ");
                scanf("%d %d",&predInd, &preyInd);//read in new indices for relation
                if (!quietMode) printf("\n");

                if (addRelationToWeb(web,numOrgs,predInd,preyInd)) {//if successfully added relation we 
                    printf("New Food Source: %s eats %s\n", web[predInd].name, web[preyInd].name);//print
                };                                                                               //prompt
                printf("\n");
                if (debugMode) {//if debug mode print web
                    printf("DEBUG MODE - added a relation:\n");
                    printWeb(web,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'p') {//if user chose 'p' we print web
                printf("UPDATED Food Web Predators & Prey:\n");
                printWeb(web,numOrgs);
                printf("\n");
                
            } else if (opt == 'd') {//if user chose 'd' we display all characteristics of web
                printf("Displaying characteristics for the UPDATED food web...\n\n");
                displayAll(web,numOrgs,true);

            }
            printf("--------------------------------\n\n");
        
        }
        
    }

    freeWeb(web,numOrgs);// we free our allocated memory

    //printf("\n\n");

    return 0;
}

