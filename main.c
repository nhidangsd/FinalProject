/*Name: Nhi Dang
  Email: nhidangsd@gmail.com
 */

/* Mountain Hike path determination program - This program will :
        1) read a set of land elevation data stored as a text file,
        2) store all necessary data in a 2D array
        3) then calculate and determine which is the shortes and most efficient path/route to go
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int min_of_2(int a,int b);
int **malloc2d(int r, int c);
char* flip_coin(void);

void calcSums(int **topog, int *sumList ); //function to find the smallest cost in EACH path/route
char* nextMove(int a, int b,int c); // helper function will be used in calcSums()
void findFastestPath(int *array); //function to find to determine which path/route to go

int rows,cols; // Total colums and rows stored at the top of the provided text file.

int main(int argc, char * argv[])
{
    int ivalRead; //variable to store each value in the text file
    int r,c,line = 1; // current position of: row , colum and line
    
    FILE* inFile = fopen("topo983by450.txt","r"); //open file - Note:"topo983by450.txt" is the name of my text file
    if (inFile==NULL) printf("no such file."); // check if the file is empty or not
    
    //Scan and store the number of cols and rows:
    fscanf(inFile,"%*s %d",&cols);
    fscanf(inFile,"%*s %d",&rows);
    
    while(line++<4) fscanf(inFile,"%*s %*lf");//ignore the unnessary variables
    
    /* Declare and Initialize the topography array and sumList array:
        topography[][] - is used to store all original data that we have scanned from the file
        sumList[]  - is used to save all the smallest cost in each route so that we could compare them later
     
     */
    int **topography = malloc2d(rows, cols);
    int *sumList = (int *)malloc(sizeof(int)*rows);
    
    // Continue to scan the rest of the file and store in the topography  array
    for(r=0; r<rows; r++){
        for(c=0;c<cols;c++){
            fscanf(inFile,"%d\n",&ivalRead);
            topography[r][c]= ivalRead;
        }
    }
    fclose(inFile);
  
    calcSums(topography, sumList);//Calculate the cost in each path and save to sumList array
    findFastestPath(sumList); //find the fastest path
    
    return 0;
}
/*****************/
void calcSums(int **topog, int *sums ) {
    int r,c;
    for( r = 0; r < rows; r++){
        int current_row = r;// reset row to the first colum
        int cost = 0; // elevation path sum
        
        for(c = 0; c < cols-1;c++){
            int fw=0,up=0,down=0; // forward, up, down values
            int min; // smallest step
            
            /* STEP 1:Calculate 3 possible on the next steps:
             -at first row: skip calculating the up_value
             -at last row: skip calculating the down_value
             */
            fw = abs((topog[current_row][c]) - (topog[current_row][c+1]));
            if(current_row > 0) up = abs((topog[current_row][c])- (topog[current_row-1][c+1]));
            if(current_row < (rows-1)) down = abs(topog[current_row][c] - topog[current_row+1][c+1]);
            
            /* STEP 2: Move to the next position
             (note: current_row does not change if move forward)*/
            if(current_row == 0) {
                min = min_of_2(fw,down);
                if(fw>down) current_row++;
            } else if(current_row == rows-1) {
                min = min_of_2(fw,up);
                if(fw >up) current_row--;
            } else {
                min = min_of_2(min_of_2(fw, up), down);
                if( strcmp(nextMove(fw, up, down),"up")==0) current_row--;
                else if(strcmp(nextMove(fw, up, down),"down")==0) current_row++;
            }
            cost+=min; //add up the min_value to the elevation path sum
        }
        sums[r] = cost;//store sums in this array
    }
}

char* flip_coin(){
    time_t t;
    srand((unsigned) time(&t));
    if((rand()%2 )== 1) return "up";
    else return "down";
}
char* nextMove(int fw, int up, int down){
    if(up < fw && up < down) return "up";
    if(down < fw && down < up) return "down";
    if(down == up) return flip_coin();
    else return "fw";
}
int min_of_2(int a,int b){
    if(a>b) return b;
    else return a ;
}
int** malloc2d(int r, int c)
{ int i;
    int** t = malloc(r * sizeof(int*));
    for (i = 0; i < r; i++)
        t[i] = malloc(c * sizeof(int));
    return t;
}
void findFastestPath(int *array){
    int min,index = 0,i=0;
    min=array[0];
    while(i++<rows-1){
        if (min>array[i]){
            min = array[i];
            index = i;
        }
    }
    printf(" - Lowest elevation-change sum: %d\n - Starting row used to calculate sum: %d\n",min,index);
}
