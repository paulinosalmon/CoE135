# include <stdio.h> 
# include <string.h> 

FILE *filePointer; 
char string[] = "KIM CHAEWON BEST GIRL";
char buffer[100];

void fileOpen() {
    printf("FOPEN starting...\n");
    filePointer = fopen("inputC.txt", "w") ; 
    filePointer == NULL ? printf("File failed to open.\n") : printf("FOPEN done!\n\n"); ; 
}

void fileWrite() {
    printf("FWRITE starting...\n");
    printf("Writing \"%s\" in inputC.txt...\n", string);
    fwrite(string, 1, sizeof(string), filePointer);
    printf("FWRITE done!\n\n");
}

void fileRead() {
    printf("FREAD starting...\n");
    fread(&buffer, strlen(string) + 1, 1, filePointer);
    printf("String inside file is: \"%s\"\n", string); 
    printf("FREAD done!\n");
}

void fileClose() {
    printf("FCLOSE starting...\n");
    fclose(filePointer); 
    printf("FCLOSE done!\n\n");
}
   
int main() { 

    fileOpen();
    fileWrite();
    fileClose();
    filePointer = fopen("inputC.txt", "r") ; 
    fileRead();
    fclose(filePointer); 

    return 0;         
} 