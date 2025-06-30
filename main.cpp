#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include "Tournament.hpp"
using namespace std;

void displayMenu();
void titleMenu();

int main(){
    system("chcp 65001");
    Tournament tm;
    int choice, c;
    while(true){
        displayMenu();
        cin >> choice;
        cin.ignore();

        if(choice == 1){
            tm.registerPlayer();
        }else if(choice == 2){
            tm.checkInPlayer();
        }else if (choice == 3){ 
            tm.withdrawPlayer();
        }else if(choice == 4){
            tm.displayMatch_Schedule();
        }else if(choice == 5){
            tm.recordMatch_Result();
        }else if(choice == 6){
            tm.SpectatorOperating();
        }else if(choice == 7){
            bool flag = true;
            while(flag){
                titleMenu();
                cin >> c;
                cin.ignore();
                if(c == 1){
                    tm.displayPlayersRank_Ascending();
                }else if(c == 2){
                    tm.displayPlayersRank_Descending();
                }else if(c == 3){
                    tm.displayPlayersCheck_IN_T();
                }else if(c == 4){
                    tm.displayPlayersCheck_IN_F();
                }else if(c == 5){
                    tm.displayPlayersPriority();
                }else if(c == 6){
                    flag = false;
                }
            }
        }else if(choice == 8){
            tm.displayMatches_History();
        }else if(choice == 9){
            tm.DisplaySpectator();
        }else if(choice == 10){
            tm.Record_Players_Performance();
        }else if(choice == 11){
            cout << "Exiting..........\n";
            break;
        }else{
            cout << "Invalid Choice.\n";
        }
    }

    return 0;
}

void displayMenu(){
    cout << "\neSports Championship System\n"
        << "1. Register Player\n"
        << "2. Check-in Player\n"
        << "3. Withdraw Player\n"
        << "4. Schedule Match\n"
        << "5. Record Match Result\n"
        << "6. Add Spectator\n"
        << "7. Display Players\n"
        << "8. Display Match History\n"
        << "9. Display Spectators\n"
        << "10. Record Players Performance\n"
        << "11. Exit\n"
        << "Enter choice: ";
}

void titleMenu(){
    cout << "1. Display by Rank ! (Ascending)\n"
        << "2. Display by Rank ! (Descending)\n"
        << "3. Display by Checked-In ! (True)\n"
        << "4. Display by Checked-In ! (False)\n"
        << "5. Display by Prioritize Entries !\n"
        << "6. Exit\n"
        << "Enter choice: ";
}