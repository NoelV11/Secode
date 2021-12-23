//Sensitive Data Exposure

#include <cstdio>

int main(int argc, char * argv[])
{
    char buffer[16];
    std::printf("Enter a test string [available length %i]\n",
           sizeof(buffer));
    std::scanf("%s", buffer);
    char secret_data[21] = "My super secret data";

    std::printf("You entered %s\n", buffer);
    return 0;
}

Solution

#include <cstdio>

int main(int argc, char * argv[])
{
    char buffer[17] = "****************";
	std::printf("Enter a test string [available length %i]\n",
           sizeof(buffer) - 1);
    
	char fmt[6];
	sprintf(fmt, "%%%ds", sizeof(buffer)/sizeof(buffer[0]) - 1);
	std::scanf(fmt, buffer);
    
    char secret_data[21] = "My super secret data";

    std::printf("You entered %s\n", buffer);
    return 0;
}
