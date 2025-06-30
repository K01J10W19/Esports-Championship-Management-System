#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;
// Dynamic Array for Players
class Player{
    private:
        struct player{
            char id[10];
            char name[50];
            int rank;
            bool checked_in;
            int kill;
            int death;
            int wins;
            int losses;
            float winrate;
            string prioritytype;
        };
        struct Match {
            string matchID;
            char player1_id[10];
            char player2_id[10];
            char winner_id[10];
            int winner_kills;
            int winner_deaths;
            int loser_kills;
            int loser_deaths;
            Match* left_child;  // Previous round match
            Match* right_child; // Previous round match
            Match* parent; // Next round match
            int round; // 1 - Quarterfinals, 2 - Semifinals, 3 - Final
        };
        player* data;
        int capacity;
        int size;
        player* all_data;
        int all_data_size;
        static const int initial_capacity = 8;
        bool loadCSV(const string& filename);
        bool saveCSV(const string& filename);
        void swap(player& a, player& b);
        void swap(int& a, int& b);
        int partitionbyRank(player* arr, int low, int high, int num);
        int partitionbyId(player* arr, int low, int high);
        int partitionbyInt(int* arr, int low, int high);
        void quickSort(int* arr, int low, int high);
        void quickSort(player* arr, int low, int high, int num, bool bywho);
        void sortByRank(player* arr, int n);
        void sortById(player* arr, int n);
        //---------------------------------------------------
        Match* root;
        int match_count;
        bool loadPlayers(const string& filename);
        int partitionPlayers(player* arr, int low, int high);
        void quickSortPlayers(player* arr, int low, int high);
        void swap(Match& a, Match& b);
        void sortByRound(Match arr[], int size);
        Match* createMatch(int round){
            Match* match = new Match;
            match->player1_id[0] = '\0';
            match->player2_id[0] = '\0';
            match->winner_id[0] = '\0';
            match->left_child = nullptr;
            match->right_child = nullptr;
            match->parent = nullptr;
            match->round = round;
            match_count++;
            return match;
        }
        void buildTree(int depth, Match*& node, int round);
        void assignPlayersToLeaves(Match* node, int& player_index);
        void updateParent(Match* node);
        bool resolveMatch(Match* node);
        void processRound(Match* node, int round);
        void displayMatch(const Match* node, const string& prefix);
        void displayBracket(Match* node, int round, bool first);
        void saveMatches(const std::string& filename, Match* node);
        void savePlayers(const string& filename);
        void deleteTree(Match* node);
        const player* findPlayerById(const char* id) const;
        player* findPlayerById(const char* id);
    public:
        // Constructor
        Player() {
            data = new player[initial_capacity];
            all_data = new player[initial_capacity];
            size = 0;
            all_data_size = 0;
            capacity = initial_capacity;
            root = nullptr;
            match_count = 0;
        }
        ~Player() {
            delete[] data;
            delete[] all_data;
            deleteTree(root);
        }
        void clear();
        bool add(const string& filename);
        void display(const string& filename, int NUM);
        void checkIn(const char* id, const string& filename);   
        void withdraw(const char* id, const std::string& filename);
        void displaycheckIn(const string&filename, int NUM);
        void displaypriority(const string&filename);
        void print();
        player* get(int index) {return (index >=0 && index < size) ? &data[index] : nullptr;}
        int getSize();
        void updateStats(const char* id, bool won);
        //----------------------------------------------------------
        void initialize(const string& player_file);
        void runTournament();
        void displayTournament();
        void saveTournament(const string& filename);
        void debugPrint(Match* node);
        void displayMatchesHistory(const string& filename);
        void displayPlayerPerformance(const string& filename);

};