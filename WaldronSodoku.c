/*
Author: Christian Waldron
Date: 18 September 2019
Class: CS3600 (Operating Systems)
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CHILD_THREADS 27

typedef int boolean;
#define TRUE 1
#define FALSE 0

boolean columns[9];

boolean rows[9];

boolean boxes[9];

int sodokuPuzzle[9][9];

int boxCounter = 0;

typedef struct 
{
   int firstRow;
   int lastRow;
   int firstColumn;
   int lastColumn;
} runParam;

void *columnCheck(void *param);

void *rowCheck(void *param);

void *boxCheck(void *param);

void getSodokuArray()
{
   int sodokuIntInput[200];
   FILE *inputFile;
   inputFile = fopen("SodokuPuzzle.txt", "r");
   int i = 0;
   int j = 0;
   int k = 0;
   int l = 0;
   
   while((!feof(inputFile)) && (i < 200))
   {
      fscanf(inputFile, "%d", &sodokuIntInput[i]);
      i++;
   }

   while(j < i && k < 9)
   {
      while(l < 9)
      {
         sodokuPuzzle[l][k] = sodokuIntInput[j];
         l++;
         j++;
      }
      l = 0;
      k++;
   }
   fclose(inputFile);
}

/*manipulates column threads*/
void *columnCheck(void *param)
{
   runParam *inP;
   int sum = 0;
   int product = 1;
   int fRow, lRow, fColumn, lColumn;
   pthread_t self;
   
   inP = (runParam*)param;
   fRow = inP->firstRow;
   lRow = inP->lastRow;
   fColumn = inP->firstColumn;
   lColumn = inP->lastColumn;
   self = pthread_self();
   int i = fRow;
   while(i <= lRow)
   {
      sum = sum + sodokuPuzzle[fColumn][i];
      product = product * sodokuPuzzle[fColumn][i];
      i++;
   }
   
   printf("Running a column thread.\n");
   printf("The sum is %d \n", sum);
   printf("The Product is %d \n", product);
   
   if(sum == 45)
   {
      if(product == 362880)
      {
         printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d valid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
         printf("\n");
         columns[fColumn] = TRUE;
      }
      else
      {
         printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
         printf("\n");
         columns[fColumn] = FALSE;
      }
   }
   
   else
   {         
      printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
      printf("\n");
      columns[fColumn] = FALSE;
   }
}


/*manipulates row threads*/
void *rowCheck(void *param)
{
   runParam *inP;
   int sum = 0;
   int product = 1;
   int fRow, lRow, fColumn, lColumn;
   pthread_t self;
   
   inP = (runParam*)param;
   fRow = inP->firstRow;
   lRow = inP->lastRow;
   fColumn = inP->firstColumn;
   lColumn = inP->lastColumn;
   self = pthread_self();
   int i = fColumn;
   
   while(i <= lColumn)
   {
      sum = sum + sodokuPuzzle[i][fRow];
      product = product * sodokuPuzzle[i][fRow];
      i++;
   }
   
   printf("Running a row thread.\n");
   printf("The sum is %d \n", sum);
   printf("The product is %d \n", product);
   
   if(sum == 45)
   {
      if(product == 362880)
      {
         printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d valid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
         printf("\n");
         rows[fRow] = TRUE;
      }
      else
      {
         printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
         printf("\n");
         rows[fRow] = FALSE;
      }
   }
   
   else
   {         
      printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
      printf("\n");
      rows[fRow] = FALSE;
   }
}

/*manipulates box threads*/
void *boxCheck(void *param)
{
   runParam *inP;
   int sum = 0;
   int product = 1;
   int fRow, lRow, fColumn, lColumn;
   pthread_t self;
   
   inP = (runParam*)param;
   fRow = inP->firstRow;
   lRow = inP->lastRow;
   fColumn = inP->firstColumn;
   lColumn = inP->lastColumn;
   self = pthread_self();
   int i = fRow;
   int j = fColumn;
   while(i <= lRow)
   {
      j = fColumn;
      while(j <= lColumn)
      {
         sum = sum + sodokuPuzzle[j][i];
         product = product * sodokuPuzzle[j][i];
         j++;
      }
      i++;
   }
   
   printf("Running a box thread.\n");
   printf("The sum is %d \n", sum);
   printf("The poduct is %d \n", product);
   
   if(sum == 45)
   {
      if(product == 362880)
      {
         printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d valid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
         printf("\n");
         boxes[boxCounter] = TRUE;
      }
      else
      {
         printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
         printf("\n");
         boxes[boxCounter] = FALSE;
      }
   }
   
   else
   {         
      printf("%lx TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid \n", (unsigned long)self, fRow, lRow, fColumn, lColumn);
      printf("\n");
      boxes[boxCounter] = FALSE;
   }
   boxCounter++;
}

void sodokuValidator()
{
   boolean vKey = TRUE;
   int i = 0;
   int j = 0;
   
   while(i < 9)
   {
      if(columns[i] == FALSE)
      {
         vKey = FALSE;
      }
      i++;
   }
   i = 0;
   
   while(i < 9)
   {
      if(rows[i] == FALSE)
      {
         vKey = FALSE;
      }
      i++;
   }
   i = 0;
   
   while(i < 9)
   {
      if(boxes[i] == FALSE)
      {
         vKey = FALSE;
      }
      i++;
   }
   if(vKey == TRUE)
   {
      printf("The sodoku answer is valid. \n");
   }
   else
   {
      printf("The sodoku answer is invalid. \n");
    }
}
   
int main()
{
   int i = 0;
   getSodokuArray();
   
   pthread_t tid[NUM_CHILD_THREADS];
   pthread_attr_t attr[NUM_CHILD_THREADS];
   runParam thrParam[NUM_CHILD_THREADS];
   
   /*get default attributes*/
   while (i < NUM_CHILD_THREADS)
   {
      pthread_attr_init(&(attr[i]));
      i++;
   }
   i = 0;
   
   
   /*sets up column threads*/
   while(i < 9)
   {
      thrParam[i].firstRow = 0;
      thrParam[i].lastRow = 8;
      thrParam[i].firstColumn = i;
      thrParam[i].lastColumn = i;
      i++;
   }
   int j = 0;
   /*sets up row threads*/
   while(i < 18)
   {
      thrParam[i].firstRow = j;
      thrParam[i].lastRow = j;
      thrParam[i].firstColumn = 0;
      thrParam[i].lastColumn =8;
      i++;
      j++;
   }

   
   /*sets up box threads (couldn't figure out algorithm)*/
   thrParam[18].firstRow = 0;
   thrParam[18].lastRow = 2;
   thrParam[18].firstColumn = 0;
   thrParam[18].lastColumn = 2;
  
   thrParam[19].firstRow = 0;
   thrParam[19].lastRow = 2;
   thrParam[19].firstColumn = 3;
   thrParam[19].lastColumn = 5;
  
   thrParam[20].firstRow = 0;
   thrParam[20].lastRow = 2;
   thrParam[20].firstColumn = 6;
   thrParam[20].lastColumn = 8;
  
   thrParam[21].firstRow = 3;
   thrParam[21].lastRow = 5;
   thrParam[21].firstColumn = 0;
   thrParam[21].lastColumn = 2;
  
   thrParam[22].firstRow = 3;
   thrParam[22].lastRow = 5;
   thrParam[22].firstColumn = 3;
   thrParam[22].lastColumn = 5;
  
   thrParam[23].firstRow = 3;
   thrParam[23].lastRow = 5;
   thrParam[23].firstColumn = 6;
   thrParam[23].lastColumn = 8;
  
   thrParam[24].firstRow = 6;
   thrParam[24].lastRow = 8;
   thrParam[24].firstColumn = 0;
   thrParam[24].lastColumn = 2;
  
   thrParam[25].firstRow = 6;
   thrParam[25].lastRow = 8;
   thrParam[25].firstColumn = 3;
   thrParam[25].lastColumn = 5;
  
   thrParam[26].firstRow = 6;
   thrParam[26].lastRow = 8;
   thrParam[26].firstColumn = 6;
   thrParam[26].lastColumn = 8; 
  
   i = 0;
  
   /*create column threads*/
   while(i < 9)
   {
      pthread_create(&(tid[i]), &(attr[i]), columnCheck, &(thrParam[i]));
      i++;
   }
  
   /*create row threads*/
   while(i < 18)
   {
      pthread_create(&(tid[i]), &(attr[i]), rowCheck, &(thrParam[i]));
      i++;
   }
  
   /*create box threads*/
   while(i < 27)
   {
      pthread_create(&(tid[i]), &(attr[i]), boxCheck, &(thrParam[i]));
      i++;
   }
   i = 0;
  
   /*wait for threads*/
   while(i < NUM_CHILD_THREADS)
   {
      pthread_join(tid[i], NULL);
      i++;
   }
   i = 0;
   sodokuValidator();
   
   return 0;
}