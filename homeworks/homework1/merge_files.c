#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
// Validate program arguments.
// *** YOUR CODE HERE. ***
  if (argc < 3 || argc > 4) {
    printf("Invalid number of arguments\n");
    return 1;
  }

// Open two files to be merged.
FILE *fp1 = fopen(argv[1], "r");
FILE *fp2 = fopen(argv[2], "r");


// Define third file, to be written to.
FILE *fp3;

// Open third file to store the result.
// If only given 2 input filenames, third filename is "myfile.out" by default.
// If given 3 input filenames, third filename is the final argv.
// *** YOUR CODE HERE. ***
char *output_filename = (argc == 4) ? argv[3] : "myfile.out";
fp3 = fopen(output_filename, "w");


// Ensure all files exist.
if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
{
printf("No such file or directory.");
exit(0);
}

// Copy contents of first file to file3.txt.
// *** YOUR CODE HERE. ***
char ch;
while ((ch = fgetc(fp1)) != EOF) {
  fputc(ch, fp3);
}
while ((ch = fgetc(fp2)) != EOF) {
  fputc(ch, fp3);
}

// Copy contents of second file to file3.txt.
// *** YOUR CODE HERE. ***
// Close all files.
fclose(fp1);
fclose(fp2);
fclose(fp3);
return 0;
}
