#include <iostram>
#include <conio.h>
#include <stdio.h>
int get_csv_token(char *token)
int c;
int quoted=0;
int len=0;
while(c=getchar())
{
  if(c== -1)
  {
    break;
  }
  if(len==0 && c == '"' && quoted==0)
  {
    quoted=1;
  continue;
  }
  
if(c='"')
{
  quoted=0;
continue;
}
*token = c;
  len++;
 token++;
}
*token= '\0';
return (-1);
}
