int SplashScreen()
{
    // Cleaning up the screen and memory to avoid any memory leakage or anything like that
    clrscr();

    int x, y;     // gotoxy initialization 
    gotoxy(x, y); // gotoxy definition 

    //  Goto lcation 20, 11
    gotoxy(20, 11);
    cout<< "Secode: Insecure Code Detector";
    
    //  Goto lcation 10, 14
    gotoxy(10, 14);
    cout<< "Made By Ali Asgar, Oorja Rungta, Shirshak Kumar and Noel Varghese";
    
    //  Goto lcation 20, 17 
    gotoxy(20,17);
    cout<< "VIT'B Project Exhibition - 2";
    
     //  Goto lcation 25, 19
    gotoxy(25,19);
    cout<< "Supervised By: Sir Thangavel M";
}
