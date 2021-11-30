// main function of Secode

// Calling Files
header.h
IntroSreen.cpp 
Options.cpp
    

int main()
{
    char ch;
    
    // Invoking IntroSreen.cpp 
    intro();
    do
        {
            clrscr();

            // Options.cpp
            cout<<"\n\n\n\t Options";
            cout<<"\n\n\t   01. C Insecure Functions";
              cout<<"\n\n\t 02. EXIT";
              cout<<"\n\n\t Please Select Your Option (1-2) ";

            // getting Options
              ch=getche();

            // Using Switch Case
              switch(ch)
              {
                case '1':clrscr();
                     C_ruleset();
                        break;
                  case '2':exit(0);
                  default: cout<<"Wrong Option";
            }

            }
    while(ch!='2');
}
