/* Name: Ziting Shen & Zheyuan Shi
 * Date: 09/10/2015
 *
 * This program will take an array of float numbers (no more than 100 numbers)
 * and sort them. The users will know the amount of numbers, the minimum and
 * maximum of numbers, and the array before and after being sorted.
 */
#include <stdio.h>      // the C standard I/O library
#include <stdlib.h>     // the C standard library
#include <string.h>     // the C string library
#include "readfile.h"   // my file reading routines
 
// #define any constant values here:
#define MAXFILENAME  128
#define ARRAYSIZE    100

// function prototypes:
void get_filename_from_commandline(char filename[], int argc, char *argv[]);
void open_file_and_check(char *filename);
void get_filename_from_commandline(char filename[], int argc, char *argv[]);

void sort(float values[], int arraylength);
void swap(float values[], int i, int j); 
int check(float values[], int arraylength);
void print_array(float values[], int arraylength); 

/********************************************************************/
int main (int argc, char *argv[]) {

  // this initializes the filename string
  char filename[MAXFILENAME];

  // this declares the array, which will hold values from the file
  float values[ARRAYSIZE];

  // this is the loop counter
  int i;

  // declare any additional variables here
  int arraylength;
  float min, max;
  // this initializes the filename string from the command line arguments
  get_filename_from_commandline(filename, argc, argv);

  // open file for reading 
  open_file_and_check(filename);

  // read in file header to get number of floats needed
	// check if the input file contains the info needed
  if(read_int(&arraylength) || read_float(&min) || read_float(&max)) {
    printf("You've reached the end of file.\n");
    exit(2);
  }
	//check if the array is too long to sort
  if(arraylength > ARRAYSIZE) {
    printf("You have too many numbers to sort.\n");
    exit(3);
  }

  // tell the u
  // (1) the name of the file being read
  // (2) how many float values  there are
  // (3) the range of values
  printf("\nReading file named %s \n", filename);
  printf("The file contains %d floating point numbers, ranging from %.2f to"
         " %.2f \n", arraylength, min, max);


  // read in floats from file, and check if reaching the end of the file
  for (i = 0; i < arraylength; i++) {
    if(read_float(&values[i])) {
      printf("End of the file, you gave the wrong array size.\n");
      exit(2);
    }
  }

  // print out information read in from the file
  // print out the unsorted values in array
  // on a single line in the terminal window
  printf("The unsorted values are: ");
  print_array(values, arraylength);

  // sort the array in place using a function
  sort(values, arraylength);

  // print out sorted values in the array
  printf("The sorted values are: ");
  print_array(values, arraylength);
 
  // close file and return
  close_file();
  return 0;
}

//********************************************************************
// THE FOLLOWING FUNCTIONS ARE PROVIDED FOR YOU
// DO NOT MODIFY, OR DO SO AT YOUR OWN RISK!
//
//********************************************************************

// This is just a function to open a file using the readfile library
// and double-check that it has opened properly. You will need 
// to call this in your program.  There is no reason to modify 
// this function, in general.  Please refrain from doing so, or 
// modify at your own risk.
void open_file_and_check(char *filename){

  int ret;
  
  ret = open_file(filename);  // try to open the integer file
  if(ret == -1) {
    printf("bad error: can't open %s\n", filename);
    exit(1);
  }
  
}

// I'm giving you this complete function: DO NOT modify it
//  This function gets the filename passed in as a command line option
//  and copies it into the filename parameter. It exits with an error 
//  message if the command line is badly formed.
//   filename: the string to fill with the passed filename
//   argc, argv: the command line parameters from main 
//               (number and strings array)
void get_filename_from_commandline(char filename[], int argc, char *argv[]) {

  if(argc != 2) {
    printf("Usage: ./exp integer_file\n");
    // exit function: quits the program immediately...some errors are not 
    // recoverable by the program, so exiting with an error message is 
    // reasonable error handling option in this case
    exit(1);   
  }
  if(strlen(argv[1]) >= MAXFILENAME) { 
    printf("Filename, %s, is too long, cp to shorter name and try again\n",
        filename);
    exit(1);
  }
  strcpy(filename, argv[1]);
}

// selection sort the array
void sort(float values[], int arraylength) {
  // this is the loop counters
  int i, j;

  // this stores the temporary minimum value in the loop
  float min;

  // this stores the index of the temporary minimum value in the loop
  int index;

  for (i = 0; i < arraylength; i++) {
    min = values[i];
    index = i;
    for (j = i + 1; j < arraylength; j++)
      if (values[j] < min) {
        min = values[j];
        index = j;
      }
    swap(values, index, i);
  }

  if (!check(values, arraylength)) printf("Unsorted! \n");
}
 // swap the value of ith and jth entry in values
void swap(float values[], int i, int j) {
  // this temporarily stores the value that will be swapped
  float temp;
  
  temp = values[i];
  values[i] = values[j];
  values[j] = temp;
}

// check if the array is sorted
// return 1 if sorted, 0 if not
int check(float values[], int arraylength) {
  // this is the loop counter
  int i;

  for(i = 0; i < arraylength - 1; i++) 
    if (values[i] > values[i+1]) 
      return 0;
  return 1;
}

// print out the array
void print_array(float values[], int arraylength) {
  // this is the loop counter
  int i;
  
  for(i = 0; i < arraylength; i++) {
    printf("%.2f ", values[i]);
  }
  printf("\n");
} 
