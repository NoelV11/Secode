int Analyzer()
{
  // Get And open File
  // Here, performance is critical for our software, thus we consider using the C language. This code is 4-5 times faster than the C++ versions
  FILE* fp = fopen(FILENAME, "r");
if (fp == NULL)
    exit(EXIT_FAILURE);

char* line = NULL;
size_t len = 0;
while ((getline(&line, &len, fp)) != -1) {
    // using printf() in all tests for consistency
    printf("%s", line);
}
fclose(fp);
if (line)
    free(line);
   

// scan if .c file 
  // Here, R_OK, W_OK, and X_OK to check for read permission and execute permission respectively.
if( access( fname, R_OK | X_OK ) == 0 ) {
    // file exists in C
} else {
    // file doesn't exist in C
}
  
// Scan header's

// ignore Comment's

// Lexical Analyzer 
  ruleset();
  
  // Conditional Statement's
}
