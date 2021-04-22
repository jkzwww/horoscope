#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
//linked list can insert anywhere and remove anywhere

struct zodiacData
{
    char name[30];
    float startDate;
};

struct zodiac
{
    zodiacData myData;
    zodiac* next;
};

struct luckData
{
    int stars;
    char* luck;
    char* luckyColour;
};

//operations
zodiac* makeNode(zodiacData newData);
void insertNode(zodiac** hPtr,int mode,zodiacData* myDataPtr);
void deleteNode(zodiac** hPtr);
void searchNode(zodiac** hPtr);
void printList(zodiac** hPtr);
void getHoroscope(zodiac** hPtr);
void initializeLuckData(luckData(*luckDataList)[10],char (*colorList)[16][10]);
void printHoroscope(luckData myData);

int main()
{
    //seed random
    srand(time(NULL));

    //header pointer
    zodiac* hPtr = NULL;

    //menu choice
    int choice;

    //file pointer
    FILE* fileptr;
    char filename[50];
    printf(" Enter the input file name: ");
	scanf("%s",filename);
	fileptr= fopen(filename, "r");

    //read data for zodiac signs
	if (fileptr == NULL)
		printf("Error, can't open the input file %s \n", filename);
	else
    {
        while (!feof(fileptr))
		{
            zodiacData myData;
			fscanf(fileptr," %s %f",myData.name,&myData.startDate);
            printf("%s %.2f\n",myData.name,myData.startDate);
            insertNode(&hPtr,0,&myData);
		}
		printf("Created the Linked List\n");
    }

    fclose(fileptr);

    //initialize color
    char colours[16][10];
      
    FILE* fptr = fopen("colours.dat","r");

    for(int i = 0 ; i <16 ; i++)
    {
        fscanf(fptr,"%s",colours[i]);
    }

    fclose(fptr);

    //horoscope data
    luckData luckDataList[10];
    initializeLuckData(&luckDataList,&colours);

    //menu
    do
    {
        printf("\n\n MENU \n\n");
        printf("1. Insert new zodiac sign\n");
        printf("2. Delete zodiac sign\n");
        printf("3. Search for zodiac sign\n");
        printf("4. Print zodiac sign list\n");
        printf("5. Get your horoscope of the day\n");
        printf("6. Quit\n");
        printf("\n");

        printf("Enter your choice: "); /*Prompt user*/
        scanf("%d", &choice);

        switch(choice)
			{
			case 1:
				insertNode(&hPtr,1,nullptr);
				break;
			case 2:
				deleteNode(&hPtr);
				break;
			case 3:
				searchNode(&hPtr);
				break;
			case 4:
				printList(&hPtr);
				break;
            case 5:
                 
                //print horoscope
                getHoroscope(&hPtr);

                int luckyNo = rand()%10; 

                printHoroscope(luckDataList[luckyNo]);

                break;
			}
    } while (choice != 6);
    

    //deallocate memory
    /*for(int i = 0; i < 10; i++)
    {
        free(luckDataList[i].luck);
        free(luckDataList[i].luckyColour);
    }*/

    return 0;
      
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


zodiac* makeNode(zodiacData newData)
{
    zodiac* ptr;

    ptr = (zodiac*) malloc (sizeof(zodiac));

    if ( ptr != NULL)
    {
        ptr->next = NULL;
        ptr->myData = newData;

        return ptr;
    }
    else
	{
		printf("Memory not allocated\n");
		return(ptr);
	}
}


//insert as ascending value
void insertNode(zodiac** hPtr,int mode,zodiacData* myDataPtr)
{
    zodiac* newNode;
    zodiacData newData;

    if(mode == 1)
    {
        printf("\nEnter zodiac sign: ");
        scanf("%s",&newData.name);
    
        printf("\nEnter start date of zodiac sign (month.date):");
        scanf( "%f",&newData.startDate);
    }
    else
    {
        newData = *myDataPtr;
    }
   

    newNode = makeNode(newData);

    zodiac* previousNode = NULL;
    zodiac* currentNode;

    //start from head node
    currentNode = *hPtr;
    //loop till correct location (ascending date)
    while(currentNode != NULL && newNode->myData.startDate > currentNode->myData.startDate)
    {
        //if node alr present in list
        //and currentnode value is smaller 
        //move to next node for comparison
        //so the current one will become the previous node
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    

    //no node in list yet
    if(previousNode == NULL) //hptr = null
    {
        newNode->next = *hPtr; //still null
        *hPtr = newNode; //make newnode as head
    }
    else
    {
        //alr something in list
        //inset new node in between previous and current
        previousNode->next = newNode;
        newNode->next = currentNode;
    }
    


}


void deleteNode(zodiac** hPtr)
{
    char oldName[30];
    printf("\nEnter the zodiac sign to be deleted: ");
    scanf("%s", oldName);

    zodiac* prevNode;
    zodiac* currentNode;
    zodiac* tempNode;

    //headpointer is the one
    if(!strcmp((*hPtr)->myData.name,oldName))
    {
        tempNode = *hPtr;
        *hPtr = (*hPtr)->next; //next node become first
        free(tempNode);
    }
    else
    {
        prevNode = *hPtr;
        currentNode = (*hPtr)->next;

        //not last ptr , value doesnt match
        while(currentNode != NULL && strcmp(currentNode->myData.name,oldName))
        {
            ///move to next node for compare
            prevNode = currentNode;
            currentNode = currentNode->next;
        } 

        //value exists (so doesnt go out of list range)
        if(currentNode != NULL)
        {
            tempNode = currentNode;
            prevNode->next = currentNode->next;
            free(tempNode);
        }
    }
}


void searchNode(zodiac** hPtr)
{
    char myName[30];
    float myDate = 0;
    int choice;

    zodiac* prevNode;
    zodiac* currentNode;

    printf("\nSearch by [0]name [1]date : ");
    scanf("%d",&choice);

    if(choice == 0)
    {
        printf("\nEnter the zodiac sign: ");
        scanf("%s", myName);

        //if first node matches
        if(!strcmp((*hPtr)->myData.name,myName))
        {
            currentNode = *hPtr;
        }
        else
        {
            prevNode = *hPtr;
            currentNode = (*hPtr)->next;

            while(currentNode != NULL && strcmp(currentNode->myData.name,myName))
            {
                prevNode = currentNode;
                currentNode = currentNode->next;
            }
        }
    }
    else
    {
        printf("\nEnter the start date of zodiac sign(month.date): ");
        scanf("%f",&myDate);

        //if first node matches
        if(myDate == (*hPtr)->myData.startDate)
        {
            currentNode = *hPtr;
        }
        else
        {
            prevNode = *hPtr;
            currentNode = (*hPtr)->next;

            while(currentNode != NULL && currentNode->myData.startDate != myDate)
            {
                prevNode = currentNode;
                currentNode = currentNode->next;
            }
        }
    }

    


    // || myDate == (*hPtr)->myData.startDate  && currentNode->myData.startDate != myDate

    if(currentNode == NULL)
    {
        printf("The zodiac sign does not exist :( \n");
    }
    else
    {
        printf("The zodiac sign is %s \n",currentNode->myData.name);
        printf("It starts at %.2f \n",currentNode->myData.startDate);
    }
}


void printList(zodiac** hPtr)
{
    zodiac* tempNode;
    tempNode = *hPtr;

    if(*hPtr == NULL)
    {
        printf("No zodiac sign entry exists yet :( \n");
    }
    else
    {
        printf("Here's the list : \n");

        while(tempNode != NULL)
        {
            printf("%s   %.2f\n",tempNode->myData.name,tempNode->myData.startDate);
            tempNode = tempNode->next;
        }

        printf("----------------------------------------------------\n");
    }
}


void initializeLuckData(luckData(*luckDataList)[10],char (*colorList)[16][10])
{
   
    //allocate space for 5 pointers to string
    char** luckString = (char**)malloc(5*sizeof(char*));
    //allocate memory for each string
    for(int i = 0; i < 5; i++)
    {
        luckString[i] = (char*)malloc(30*sizeof(char));
    }

    //enter data
    sprintf(luckString[0], "Really bad TvT");
    sprintf(luckString[1], "Bad >.<");
    sprintf(luckString[2], "Okay~ 0v0");
    sprintf(luckString[3], "Good >w<");
    sprintf(luckString[4], "Lucky!! ^ v ^");


    for(int i = 0; i < 10 ; i++)
    {
        (*luckDataList)[i].stars = rand()%6 + 1;
        int star = (*luckDataList)[i].stars - 1;
        (*luckDataList)[i].luck = (char*)malloc(30*sizeof(char));
        (*luckDataList)[i].luck = luckString[star];


        int randCol = rand()%16;
        (*luckDataList)[i].luckyColour = (char*)malloc(10*sizeof(char));
        (*luckDataList)[i].luckyColour = (*colorList)[randCol];
    }

    free(luckString);
}

void getHoroscope(zodiac** hPtr)
{
    float myDate;
    char myName[30];
    char zodiacSign[30];

    printf("\nEnter your name: ");
    scanf("%s", myName);

    printf("\nWhen is your birthday? \nformat : 05.07 (month.date) \nEnter date: ");
    scanf("%f",&myDate);

    zodiac* tempNode;
    tempNode = *hPtr;

    while(myDate > tempNode->myData.startDate && myDate >= tempNode->next->myData.startDate )
    {
        tempNode = tempNode->next;
    }

    //special case (capricorn from 1/1)
    if(myDate >= 1.1 && myDate < 1.20)
    {
        sprintf(zodiacSign,"capricorn");
    }
    else
    {
        sprintf(zodiacSign,tempNode->myData.name);
    }

    printf("\nWelcome, %s ~(> v <)/",myName);
    printf("\nYour zodiac sign is %s",zodiacSign);
    
}


void printHoroscope(luckData myData)
{
    int numStar = myData.stars;
    char starString[10];

    for(int i = 0; i < numStar; i++)
    {
        starString[i] = '*';
    }

    printf("\nYour luckiness today is [%s] out of [*****] !! \n",starString);
    printf("which is %s\n",myData.luck);
    printf("Your lucky colour is %s!\n",myData.luckyColour);
    printf("Have a nice day, the stars will look after you! 0 v 0 )/ <3\n");
}