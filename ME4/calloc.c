#include <stdio.h> 
#include <stdlib.h> 
  
int main() {
   int *arr; 
   arr = (int *)calloc(5, sizeof(int)); 
   printf("Allocated the memory for 5 integers.\n");
   free(arr); 
   printf("Memory freed.\n");
}   