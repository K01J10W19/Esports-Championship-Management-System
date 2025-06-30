#include "Tournament.hpp"
using namespace std;

void Tournament::registerPlayer(){
    players.add("PLAYER.txt");
}
void Tournament::checkInPlayer(){
    char id[10];
    cout << "Enter player ID: ";
    cin >> id;
    players.checkIn(id, "PLAYER.txt");
}
void Tournament::withdrawPlayer(){
    char id[10];
    cout << "Enter player ID to withdraw: ";
    cin >> id;
    players.withdraw(id, "PLAYER.txt");
}

void Tournament::SpectatorOperating(){
    spectators.addSpectatorToFile("SPECTATOR.txt");
}
void Tournament::DisplaySpectator(){
    spectators.loadCSV_spectator("SPECTATOR.txt");
    spectators.displaySeating();
    spectators.processWaitingQueue();
    spectators.displaySeating();
}
void Tournament::displayMatch_Schedule(){
    players.displayTournament();
}
void Tournament::recordMatch_Result(){
    players.initialize("PLAYER.txt");
    players.displayTournament();
    players.runTournament();
    players.saveTournament("MATCHES.csv");
}
void Tournament::displayMatches_History(){
    players.displayMatchesHistory("MATCHES.csv");
}
void Tournament::Record_Players_Performance(){
    players.displayPlayerPerformance("PLAYER.txt");
}