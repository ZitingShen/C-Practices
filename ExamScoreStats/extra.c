/* 
 *  Author: Ziting Shen
 *  Data: 10/02/2015
 *
 *  This program takes a file containing exam scores, sorting them and 
 *  printing out the min, max, mean, median and standard deviation values.
 *  It also draws a histogram of the scores.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readfile.h"

/* 
 * N: strart size for first dynamically allocated array of values 
 * MAXFILENAME: max length of input file name string
 */
#define N   20   
#define MAXFILENAME   32
#define HISTWIDTH   50

float *get_values(int *size, int *capacity, char *filename);
float *double_capacity(int *capacity, float *arr);

void sort(float *values, int size);
void swap(float *values, int i, int j); 
int check(float *values, int size);

float mean_calculation(float *values, int size);
float median_calculation(float *values, int size);
float sd_calculation(float *values, int size, float mean);
void stats(float *values, int size);

void histogram(float *values, int size, int width);

/**********************************************************************/
int main(int argc, char *argv[]) {

  char filename[MAXFILENAME]; // the file name
  float *vals = NULL; // the array of the scores
  int size, capacity; // the size and capacity of the array
  int width = 0; // the width of the buckets

  // this code checks command line args to make sure program run
  // with filename argument.  If not, calls exit to quit the program
  if(argc != 2) {
    printf ("usage: ./stats filename\n");
    exit(1);
  }
  // this copies the filename command line argument into the 
  // string variable filename
  strncpy(filename, argv[1], MAXFILENAME-1);
  scanf("%d", &width);

  // call get_values function:
  vals = get_values(&size, &capacity, filename);

  // sort the scores with selection sort
  sort(vals, size);

  // calculate and print out the min, max, mean, median and standard deviation
  // of the scores
  stats(vals, size);

  // draw the histogram of the scores
  histogram(vals, size, width);

  // print out the amount of array space not used to store scores
  printf("wasted array capacity: %d \n", capacity - size);

  free(vals);
  return 0;
}
/**********************************************************************/
/* This function reads in values from the input file into a dynamically
 * allocated array of floats that it returns.  It uses a doubling 
 * algorithm to increase the size of the array as it needs more space
 * for the values read in from the file.
 * 
 *   size: set to the number of data values read in from the file
 *   capacity: set to the total number of buckets in the returned array
 *   filename: the name of the file to open
 *
 *   returns: the base address of the array of values read in 
 *            or NULL on error
 */
float *get_values(int *size, int *capacity, char *filename){

  float *values = NULL; // array of values
  int returned = 0; // returned value when reading in a float

  *size = 0;
  *capacity = 20;

  if(open_file(filename)) {  // step 1: open the file
    return NULL;
  }
 
  values = malloc(sizeof(float)*(*capacity));
  
  while(!returned) {
    returned = read_float(values + *size);
    if(returned)
      break;
    *size += 1;
    if(*size == *capacity)
      values = (float *)double_capacity(capacity, values);
  }
  
  close_file();
  return values;   // return the values array
}
/**********************************************************************/
/* This function doubles the size of the dynamically allocated array.
 * 
 *   capacity: the total number of buckets of arr
 *   arr: array of the values
 *
 *   returns: the base address of the new array with doubled capacity
 */
float *double_capacity(int *capacity, float *arr){
  float *temp = NULL; // new array's base address
  int old_capacity; // old capacity
  int i; // loop counter

  old_capacity = *capacity;
  *capacity *= 2;
  temp  = (float *)malloc(sizeof(float)*(*capacity));

  for(i = 0; i < old_capacity; i++){
    temp[i] = arr[i];
  }
  free(arr);

  return temp;
}

/**********************************************************************/
/* This function sorts the array of scores with selection sort.
 * 
 *   values: the array of data that will be sorted
 *   size: the number of data values
 */
void sort(float *values, int size) {
  int i, j; // the loop counters
  float min; // the temporary minimum value
  int index; // index of the temporary minimum value

  for (i = 0; i < size; i++) {
    min = values[i];
    index = i;
    for (j = i + 1; j < size; j++)
      if (values[j] < min) {
        min = values[j];
        index = j;
      }
    swap(values, index, i);
  }

  if (!check(values, size)) printf("Unsorted! \n");
}

/**********************************************************************/
/* This function swaps the value of ith and jth entry in values
 * 
 *   values: the array of scores
 *   i, j: indices of the two entries that will be swapped
 */
void swap(float *values, int i, int j) {
  float temp; // temporarily store the value
  
  temp = values[i];
  values[i] = values[j];
  values[j] = temp;
}

/**********************************************************************/
/* This function checks if the array is sorted.
 * 
 *   values: the array of data
 *   size: the number of data values
 * 
 *   returns: 1 if sorted, 0 if not
 */
int check(float *values, int size) {
  int i; // loop counter

  for(i = 0; i < size - 1; i++) 
    if (values[i] > values[i+1]) 
      return 0;
  return 1;
}

/**********************************************************************/
/* This function calculates the mean value of the array.
 * 
 *   values: the array of data
 *   size: the number of data values
 * 
 *   returns: the mean value
 */
float mean_calculation(float *values, int size) {
  float sum = 0; // the sum of all the entries in the array
  int i; // loop counter

  for(i = 0; i < size; i++) {
    sum += values[i];
  }

  return sum/size;
}

/**********************************************************************/
/* This function calculates the median value of the array.
 * 
 *   values: the array of data
 *   size: the number of data values
 * 
 *   returns: the median value
 */
float median_calculation(float *values, int size) {
  if(size % 2 == 1)
    return values[size/2];
  return (values[size/2-1] + values[size/2])/2;
}

/**********************************************************************/
/* This function calculates the standard deviation of the array.
 * 
 *   values: the array of data
 *   size: the number of data values
 *   mean: the mean value of data
 * 
 *   returns: the standard deviation value
 */
float sd_calculation(float *values, int size, float mean) {
  float diff = 0; // the difference between each entry and the mean
  float sum = 0; // the sum of squares of differences
  int i; // loop counter

  for(i = 0; i < size; i++) {
    diff = values[i] - mean;
    sum = sum + diff*diff;
  }

  return sqrt(sum/(size - 1));
}

/**********************************************************************/
/* This function prints out the five related values of the array.
 * 
 *   values: the array of data
 *   size: the number of data values
 */
void stats(float *values, int size) {
  float min, max, mean, median, sd; // the related stats

  min = values[0];
  max = values[size-1];
  mean = mean_calculation(values, size);
  median = median_calculation(values, size);
  sd = sd_calculation(values, size, mean);

  printf("\n");
  printf("Grade Stats: \n ------------ \n");
  printf("   total: %d \n", size);
  printf("     min: %.2f \n", min);
  printf("     max: %.2f \n", max);
  printf("    mean: %.2f \n", mean);
  printf("  median: %.2f \n", median);
  printf(" std dev: %.2f \n", sd);
  printf("\n");
}

/**********************************************************************/
/* This function draws the histogram of the array.
 * 
 *   values: the array of data
 *   size: the number of data values
 *   width: the width of each bucket
 */
void histogram(float *values, int size, int width) {
  int i, j; // loop counter
  int b; // number of buckets
  int *buckets; // the array of buckets
  int bstart, bend; // the both ends of bucket limits
  float bmax = 0; // maximum of all buckets
  int scale = 1; // scale of stars
  int starnum; // number of stars

  bstart = (int) values[0] / width * width;
  bend = ((int) values[size - 1] / width + 1) * width;
  b = (bend - bstart) / width;
  buckets = (int *)malloc(sizeof(int)*b);
  if(bend > 100) bend = 100;

  for(i = 0; i < b; i++)
    buckets[i] = 0;

  for(i = 0; i < size; i++)
    buckets[(int) (values[i] - bstart) / width]++;

  for(i = 0; i < b; i++)
    if(buckets[i] >  bmax) 
      bmax = buckets[i];
  
  while(bmax > HISTWIDTH) {
    scale *= 10;
    bmax /= 10;
  }

  printf("Grade Histogram: \n");
  for(i = 0; i < b - 1; i++){
    printf("%d - %d: ", bstart + width * i, bstart + width * (i + 1) - 1);
    starnum = buckets[i] / scale + 1;
    for(j = 0; j < starnum; j++)
      printf("*");
    printf("\n");
  }
  printf("%d - %d: ", bstart + width * (b - 1), bend);
  starnum = buckets[b - 1] / scale + 1;
  for(j = 0; j < starnum; j++)
    printf("*");
  printf("\n");

  printf("scale: * = %d grades \n\n", scale);

  free(buckets);
}
