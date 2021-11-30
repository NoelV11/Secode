// main function of Secode
header.h
admin_menu.cpp
    

int main()
{
    char ch;
    intro();
    do
    {
        clrscr();
        cout<<"\n\n\n\tMAIN MENU";
        cout<<"\n\n\t01. C Insecure Functions";
          cout<<"\n\n\t02. EXIT";
          cout<<"\n\n\tPlease Select Your Option (1-2) ";
          ch=getche();
          switch(ch)
          {
            case '1':clrscr();
                 C_ruleset();
                    break;
              case '2':exit(0);
              default :cout<<"Wrong Option";
        }
        }while(ch!='2');
}
