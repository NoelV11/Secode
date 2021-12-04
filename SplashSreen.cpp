int SplashScreen()
{
    // Cleaning up the screen and memory to avoid any memory leakage or anything like that
    clrscr();
    
    //  Goto lcation 35, 11  
    gotoxy(35,11);
    cout<<"Secode: Insecure Code Detector";
    
    //  Goto lcation 35, 14 
    gotoxy(35,14);
    cout<<"Made By Ali Asgar, oorja Rungat, Shirshak Kumar and Noel Varghese";
    
    //  Goto lcation 35, 17 
    gotoxy(35,17);
    cout<<"VIT'B Project Exhibition - 2";
    
     //  Goto lcation 35, 19
    gotoxy(35,19);
    cout<<"\n\n Supervised By: Sir Thangavel M";
    
    
    cout<<"\n\n Press any key to continue";
    
    getch();
}
