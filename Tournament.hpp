#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "Player.hpp"
#include "SpectatorQueue.hpp"
using namespace std;
// Tournament Class
class Tournament {
    private:
        Player players;
        // MatchHistory history;
        SpectatorManager spectators;
    
    public:
    
        void registerPlayer();
    
        void checkInPlayer();
        void withdrawPlayer();
    
        void SpectatorOperating();
        void DisplaySpectator();

        void displayPlayersRank_Ascending() { players.display("PLAYER.txt", 1); }
        void displayPlayersRank_Descending() { players.display("PLAYER.txt", 2); }
        void displayPlayersCheck_IN_T(){ players.displaycheckIn("PLAYER.txt", 1); }
        void displayPlayersCheck_IN_F(){ players.displaycheckIn("PLAYER.txt", 2); }
        void displayPlayersPriority(){ players.displaypriority("PLAYER.txt"); }
        void displayMatch_Schedule();
        void recordMatch_Result();
        void displayMatches_History();
        void Record_Players_Performance();

        // void displayHistory() { history.display(); }
        // void scheduleMatch(const char* p1_id, const char* p2_id);
    
        // void recordMatchResult(const char* p1_id, const char* p2_id, int score1, int score2);
        // void displayBracket();

};