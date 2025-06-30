#include "Player.hpp"


void Player::clear(){
    delete[] data;
    delete[] all_data;
    data = new player[initial_capacity];
    all_data = new player[initial_capacity];
    size = 0;
    capacity = initial_capacity;
}

bool Player::loadCSV(const string& filename){
    clear();

    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        return false;
    }
    string line;
    getline(file, line);
    while(getline(file, line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string id, name, rank, checkedin, wins, losses, prioritytype,winsrate,kill,death;
        getline(ss, id, ',');
        getline(ss, name, ',');
        if (!name.empty() && name.front() == '"') {
            name = name.substr(1, name.size() - 2);
        }
        getline(ss, rank, ',');
        getline(ss, checkedin, ',');
        getline(ss, wins, ',');
        getline(ss, losses, ',');
        getline(ss, prioritytype, ',');
        getline(ss, winsrate, ',');
        getline(ss, kill, ',');
        getline(ss, death, ',');
        if (size == capacity) {
            player* temp = new player[capacity * 2];
            for (int i = 0; i < size; ++i) {
                temp[i] = data[i];
            }
            delete[] data;
            data = temp;
            capacity *= 2;
        }
        try {
            strcpy(data[size].id, id.c_str());
            strcpy(data[size].name, name.c_str());
            data[size].rank = stoi(rank);
            data[size].checked_in = (checkedin == "true");
            data[size].wins = stoi(wins);
            data[size].losses = stoi(losses);
            data[size].prioritytype = prioritytype.c_str();
            data[size].winrate = stof(winsrate);
            data[size].kill = stoi(kill);
            data[size].death = stoi(death);
            ++size;
        } catch (...) {
            cerr << "Invalid data in CSV line: " << line << endl;
            continue;
        }
    }
    file.close();
    return true;
}

bool Player::saveCSV(const string& filename){
    ofstream file(filename, ios::out);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << std::endl;
        return false;
    }

    file << "ID,Name,Ranking,CheckedIn,Wins,Losses,PriorityType,WinsRate,Kill,Death\n";

    for (int i = 0; i < size; ++i){
        string name = data[i].name;
        string escaped;
        bool quotes = false;
        for (char c : name) {
            if (c == ',' || c == '"') {
                quotes = true;
            }
            if (c == '"') {
                escaped += '"';
            }
            escaped += c;
        }
        file << data[i].id << ",";
        if (quotes) {
            file << "\"" << escaped << "\"";
        } else {
            file << escaped;
        }
        file << "," << data[i].rank << "," << (data[i].checked_in ? "true" : "false")
                << "," << data[i].wins << "," << data[i].losses << "," << data[i].prioritytype
                << "," << data[i].winrate << "," << data[i].kill << "," << data[i].death << "\n";
    }
    file.close();
    return true;    
}

bool Player::add(const string& filename){
    clear();
    // attribute
    string id, name, prioritytype;
    bool checkin = false;
    int rank, wins = 0, losses = 0, kill = 0, death = 0;
    float winrate = 0.0;
    // Get input from Player
    cout << "Enter player ID: ";
    getline(cin, id);
    cout << "Enter player name: ";
    getline(cin, name);
    cout << "Enter player ranking [1 - 100]: ";
    cin >> rank;
    // Validate rank
    if (rank < 1 || rank > 100) {
        cerr << "Rank must be between 1 and 100." << endl;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

    if (rank >=1 && rank <= 30){
        prioritytype = "Wildcard";
    }else if (stoi(id) <= 20){
        prioritytype = "EarlyBird";
    }else{
        prioritytype = "Regular";
    }
    // Resize
    if(size == capacity){
        player* temp = new player[capacity * 2];
        for (int i = 0; i < size; ++i) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
        capacity *= 2;
    }
    // store into array
    strcpy(data[size].id, id.c_str());
    strcpy(data[size].name, name.c_str());
    data[size].rank = rank;
    data[size].checked_in = checkin;
    data[size].wins = wins;
    data[size].losses = losses;
    data[size].prioritytype, prioritytype.c_str();
    data[size].winrate = winrate;
    data[size].kill = kill;
    data[size].death = death;
    ++size;

    // check and write into CSV File
    bool fileExists = ifstream(filename).good();
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << std::endl;
        return false;
    }

    if (!fileExists) {
        file << "ID,Name,Ranking,CheckedIn,Wins,Losses,PriorityType,WinsRate,Kill,Death\n";
    }
    // quotes validation of name
    string escaped;
    bool quotes = false;
    for (char c : name) {
        if (c == ',' || c == '"') {
            quotes = true;
        }
        if (c == '"') {
            escaped += '"';
        }
        escaped += c;
    }

    file << id << ",";
    if (quotes) {
        file << "\"" << escaped << "\"";
    } else {
        file << escaped;
    }
    file << "," << rank << "," << (checkin ? "true" : "false")
            << "," << wins << "," << losses << "," << prioritytype 
            << "," << winrate << "," << kill << "," << death << "\n";
    // Print Registration Result
    cout << "Player " << name << " registered.\n";
    file.close();
    return true;
}
void Player::withdraw(const char* id, const std::string& filename) {
    // Load current player data
    if (!loadCSV(filename)) {
        cout << "Failed to load player data.\n";
        return;
    }
    bool found = false;
    for (int i = 0; i < size; ++i) {
        if (strcmp(data[i].id, id) == 0) {
            data[i].checked_in = false;
            cout << "Player " << data[i].name << " withdrawn successfully.\n";
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Player not found.\n";
        return;
    }
    // Save changes back to file
    if (!saveCSV(filename)) {
        cerr << "Failed to save updated data to CSV.\n";
    }
}


// - - - - - Quick Sort - - - - -
void Player::swap(player& a, player& b){
    player temp = a;
    a = b;
    b = temp;
}
void Player::swap(Match& a, Match& b){
    Match temp = a;
    a = b;
    b = temp;
}
void Player::swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}
int Player::partitionbyRank(player* arr, int low, int high, int num){
    int pivot = arr[high].rank;
    int i = low - 1;
    if (num == 1){
        for (int j = low; j < high; ++j){
            if (arr[j].rank < pivot){
                ++i;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i+1], arr[high]);
        return i + 1;
    }else if (num == 2){
        for (int j = low; j < high; ++j){
            if (arr[j].rank > pivot){
                ++i;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i+1], arr[high]);
        return i + 1;
    }
}
int Player::partitionbyId(player* arr, int low, int high){
    int pivot = atoi(arr[high].id);
    int i = low - 1;
    for (int j = low; j < high; ++j){
        if (atoi(arr[j].id) < pivot){
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i + 1;
}
int Player::partitionbyInt(int* arr, int low, int high){
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
void Player::quickSort(player* arr, int low, int high, int num, bool bywho){
    if (low < high){
        int pi = bywho ? partitionbyRank(arr, low, high, num) : partitionbyId(arr, low, high);
        quickSort(arr, low, pi - 1, num, bywho);
        quickSort(arr, pi + 1, high, num, bywho);
    }
}
void Player::quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partitionbyInt(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
int Player::partitionPlayers(player* arr, int low, int high) {
    int pivotRank = arr[high].rank;
    const char* pivotId = arr[high].id;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j].rank < pivotRank ||
            (arr[j].rank == pivotRank && strcmp(arr[j].id, pivotId) < 0)) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
void Player::quickSortPlayers(player* arr, int low, int high) {
    if (low < high) {
        int pi = partitionPlayers(arr, low, high);
        quickSortPlayers(arr, low, pi - 1);
        quickSortPlayers(arr, pi + 1, high);
    }
}
// - - - - - - - - - - - - - - - -
void Player::display(const string& filename,int NUM){
    // Check File Exists or not
    ifstream countFile(filename);
    if(!countFile.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        return;
    }
    // Check Data Exists or not
    int lineCount = 0;
    string line;
    getline(countFile, line);
    while(getline(countFile, line)){
        if(!line.empty()){
            ++lineCount;
        }
    }
    countFile.close();
    if(lineCount == 0){
        cout <<"No players exists."<<endl;
        return;
    }
    // Allocate Array
    player* players = new player[lineCount];
    int index = 0;
    // Read CSV into Array
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        delete[] players;
        return;
    }
    getline(file, line);
    while(getline(file, line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string id, name, rank, checkedin, wins, losses;
        getline(ss, id, ',');
        getline(ss, name, ',');
        if (!name.empty() && name.front() == '"') {
            name = name.substr(1, name.size() - 2);
        }
        getline(ss, rank, ',');
        getline(ss, checkedin, ',');
        getline(ss, wins, ',');
        getline(ss, losses, ',');
        // Correctly Data Type of Each
        try {
            strcpy(players[index].id, id.c_str());
            strcpy(players[index].name, name.c_str());
            players[index].rank = stoi(rank);
            players[index].checked_in = (checkedin == "true");
            players[index].wins = stoi(wins);
            players[index].losses = stoi(losses);
            ++index;
        } catch (...) {
            cerr << "Invalid data in CSV line: " << line << endl;
            continue;
        }
    }
    file.close();
    // Using Quick Sorting to sort * data
    if(NUM == 1){
        quickSort(players, 0, index - 1,NUM,true);
    }else if(NUM == 2){
        quickSort(players, 0, index - 1,NUM,true);
    }
    // Display Player Lists
    cout << left
        << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Rank"
        << setw(12) << "Checked-In"
        << setw(8) << "Wins"
        << setw(8) << "Losses"
        << endl;
    cout << string(68, '-') << endl;
    for (int i = 0; i < index; ++i) {
        cout << left
                << setw(10) << players[i].id
                << setw(20) << players[i].name
                << setw(10) << players[i].rank
                << setw(12) << (players[i].checked_in ? "true" : "false")
                << setw(8) << players[i].wins
                << setw(8) << players[i].losses
                << endl;
    }
    // Summary table
    cout << "\nSummary:\n";
    cout << string(240, '-') << endl;
    cout << left
        << setw(25) << "Total Players"
        << ": " << index << endl;

    // Collect unique ranks
    int* ranks = new int[index];
    int rankCount = 0;
    for (int i = 0; i < index; ++i) {
        bool exists = false;
        for (int j = 0; j < rankCount; ++j) {
            if (ranks[j] == players[i].rank) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            ranks[rankCount++] = players[i].rank;
        }
    }

    // Sort ranks
    quickSort(ranks, 0, rankCount - 1);

    // Display ranks
    cout << setw(25) << "Available Ranks";
    cout << ": ";
    for (int i = 0; i < rankCount; ++i) {
        cout << ranks[i];
        if (i < rankCount - 1) cout << ", ";
    }
    cout << endl;

    // Count checked-in
    int checkedInTrue = 0, checkedInFalse = 0;
    for (int i = 0; i < index; ++i) {
        if (players[i].checked_in) {
            ++checkedInTrue;
        } else {
            ++checkedInFalse;
        }
    }

    cout << setw(25) << "Checked-In (True)"
            << ": " << checkedInTrue << endl;
    cout << setw(25) << "Checked-In (False)"
            << ": " << checkedInFalse << endl;
    cout << string(240, '-') << endl;

    // Free memory
    delete[] ranks;
    delete[] players;
}

void Player::checkIn(const char* id, const string& filename){
    // Load CSV into data array and check if empty{
    if (!loadCSV(filename)) {
        cout << "Failed to load player data.\n";
        return;
    }
    bool found = false;
    // Find player and update checked_in
    for (int i = 0; i < size; ++i) {
        if (strcmp(data[i].id, id) == 0) {
            data[i].checked_in = true;
            cout << "Player " << data[i].name << " checked-in successful.\n";
            found = true;
            break;
        }
    }
    if(!found){
        cout << "Player not found.\n";
        return;
    }
    // Save updated array to CSV
    if (!saveCSV(filename)) {
        cerr << "Failed to save updated data to CSV.\n";
    }
}

void Player::displaycheckIn(const string&filename, int NUM){
    // Check File Exists or not
    ifstream countFile(filename);
    if(!countFile.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        return;
    }
    // Check Data Exists or not
    int lineCount = 0;
    string line;
    getline(countFile, line);
    while(getline(countFile, line)){
        if(!line.empty()){
            ++lineCount;
        }
    }
    countFile.close();
    if(lineCount == 0){
        cout <<"No players exists."<<endl;
        return;
    }
    // Allocate Array
    player* players = new player[lineCount];
    int index = 0;
    // Read CSV into Array
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        delete[] players;
        return;
    }
    getline(file, line);
    while(getline(file, line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string id, name, rank, checkedin, wins, losses;
        getline(ss, id, ',');
        getline(ss, name, ',');
        if (!name.empty() && name.front() == '"') {
            name = name.substr(1, name.size() - 2);
        }
        getline(ss, rank, ',');
        getline(ss, checkedin, ',');
        getline(ss, wins, ',');
        getline(ss, losses, ',');
        
        if(NUM == 1){
            if(checkedin == "true"){
                try{
                    strcpy(players[index].id, id.c_str());
                    strcpy(players[index].name, name.c_str());
                    players[index].rank = std::stoi(rank);
                    players[index].checked_in = true;
                    players[index].wins = std::stoi(wins);
                    players[index].losses = std::stoi(losses);
                    ++index;
                }catch (...) {
                    std::cerr << "Invalid data in CSV line: " << line << std::endl;
                    continue;
                }
            }
        }else if(NUM == 2){
            if(checkedin == "false"){
                try{
                    strcpy(players[index].id, id.c_str());
                    strcpy(players[index].name, name.c_str());
                    players[index].rank = std::stoi(rank);
                    players[index].checked_in = false;
                    players[index].wins = std::stoi(wins);
                    players[index].losses = std::stoi(losses);
                    ++index;
                }catch (...) {
                    cerr << "Invalid data in CSV line: " << line << std::endl;
                    continue;
                }
            }
        }
    }
    file.close();
    quickSort(players, 0, index - 1,0,false);
    cout << std::left
        << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Rank"
        << setw(12) << "Checked-In"
        << setw(8) << "Wins"
        << setw(8) << "Losses"
        << endl;
    cout << string(68, '-') << endl;

    for (int i = 0; i < index; ++i) {
        cout << left
            << setw(10) << players[i].id
            << setw(20) << players[i].name
            << setw(10) << players[i].rank
            << setw(12) << (players[i].checked_in ? "true" : "false")
            << setw(8) << players[i].wins
            << setw(8) << players[i].losses
            << endl;
    }

    if (index == 0) {
        cout << "No checked-in players found." << std::endl;
    }
    // Summary table
    cout << "\nSummary:\n";
    cout << string(240, '-') << std::endl;
    cout << left
        << setw(25) << "Total Players"
        << ": " << index << endl;
    // Collect unique ranks
    int* ranks = new int[index];
    int rankCount = 0;
    for (int i = 0; i < index; ++i) {
        bool exists = false;
        for (int j = 0; j < rankCount; ++j) {
            if (ranks[j] == players[i].rank) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            ranks[rankCount++] = players[i].rank;
        }
    }
    // Sort ranks
    quickSort(ranks, 0, rankCount - 1);
    // Display ranks
    cout << setw(25) << "Available Ranks";
    cout << ": ";
    for (int i = 0; i < rankCount; ++i) {
        cout << ranks[i];
        if (i < rankCount - 1) cout << ", ";
    }
    cout << endl;
    // Count checked-in
    int checkedInTrue = 0, checkedInFalse = 0;
    for (int i = 0; i < index; ++i) {
        if (players[i].checked_in) {
            ++checkedInTrue;
        } else {
            ++checkedInFalse;
        }
    }
    cout << setw(25) << "Checked-In (True)"
            << ": " << checkedInTrue << endl;
    cout << setw(25) << "Checked-In (False)"
            << ": " << checkedInFalse << endl;
    cout << string(240, '-') << endl;
    // Free memory
    delete[] ranks;
    delete[] players;
}

// - - - - - Bubble Sort - - - - -
void Player::sortByRank(player* arr, int n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j].rank > arr[j + 1].rank)
                swap(arr[j], arr[j + 1]);
}

void Player::sortById(player* arr, int n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (stoi(arr[j].id) > stoi(arr[j + 1].id))
                swap(arr[j], arr[j + 1]);
}
void Player::sortByRound(Match arr[], int size){
    for (int i = 0; i < size - 1; ++i)
        for (int j = 0; j < size - i - 1; ++j)
            if (arr[j].round > arr[j + 1].round)
                swap(arr[j], arr[j + 1]);
}
// - - - - - - - - - - - - - - - -

void Player::displaypriority(const string&filename){
    // Check File Exists or not
    ifstream countFile(filename);
    if(!countFile.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        return;
    }
    // Check Data Exists or not
    int lineCount = 0;
    string line;
    getline(countFile, line);
    while(getline(countFile, line)){
        if(!line.empty()){
            ++lineCount;
        }
    }
    countFile.close();
    if(lineCount == 0){
        cout <<"No players exists."<<endl;
        return;
    }
    // Allocate Array
    player* players = new player[lineCount];
    int index = 0;
    // Read CSV into Array
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        delete[] players;
        return;
    }
    getline(file, line);
    while(getline(file, line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string id, name, rank, checkedin, wins, losses, prioritytype;
        getline(ss, id, ',');
        getline(ss, name, ',');
        if (!name.empty() && name.front() == '"') {
            name = name.substr(1, name.size() - 2);
        }
        getline(ss, rank, ',');
        getline(ss, checkedin, ',');
        getline(ss, wins, ',');
        getline(ss, losses, ',');
        getline(ss, prioritytype, ',');
        try{
            strcpy(players[index].id, id.c_str());
            strcpy(players[index].name, name.c_str());
            players[index].rank = stoi(rank);
            players[index].checked_in = (checkedin == "true");
            players[index].wins = stoi(wins);
            players[index].losses = stoi(losses);
            players[index].prioritytype = prioritytype.c_str();
            ++index;
        }catch (...) {
            cerr << "Invalid data in CSV line: " << line << endl;
            continue;
        }
    }
    file.close();
    player* wildcard = new player[lineCount];
    player* earlybird = new player[lineCount];
    player* regular = new player[lineCount];
    int wCount = 0, eCount = 0, rCount = 0;
    for (int i = 0; i < index; ++i){
        if(players[i].prioritytype == "Wildcard"){
            wildcard[wCount++] = players[i];
        }else if(players[i].prioritytype == "EarlyBird"){
            earlybird[eCount++] = players[i];
        }else{
            regular[rCount++] = players[i];
        }
    }
    sortByRank(wildcard, wCount);
    sortById(earlybird, eCount);
    sortById(regular, rCount);
    const char* GREEN = "\033[32m";
    const char* RESET = "\033[0m";
    bool Color = true;
    cout << left
        << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Rank"
        << setw(12) << "Checked-In"
        << setw(8) << "Wins"
        << setw(8) << "Losses"
        << setw(12) << "Priority-Type"
        << endl;
    cout << string(78, '-') << endl;

    auto displayGroup = [Color, GREEN, RESET](player* arr, int n) {
        for (int i = 0; i < n; ++i) {
            string displayName = arr[i].name;
            if(!Color && arr[i].checked_in){
                displayName += "*";
            }
            if(Color && arr[i].checked_in){
                cout << GREEN;
            }
            cout << left
                << setw(10) << arr[i].id
                << setw(20) << arr[i].name
                << setw(10) << arr[i].rank
                << setw(12) << (arr[i].checked_in ? "true" : "false")
                << setw(8) << arr[i].wins
                << setw(8) << arr[i].losses
                << setw(12) << arr[i].prioritytype
                << "\n";
            if(Color && arr[i].checked_in){
                cout << RESET;
            }
        }
    };

    displayGroup(wildcard, wCount);
    displayGroup(earlybird, eCount);
    displayGroup(regular, rCount);

    // Summary table
    cout << "\nSummary:\n";
    cout << string(240, '-') << std::endl;
    cout << left
        << setw(25) << "Total Players"
        << ": " << index << endl;

    // Collect unique ranks
    int* ranks = new int[index];
    int rankCount = 0;
    for (int i = 0; i < index; ++i) {
        bool exists = false;
        for (int j = 0; j < rankCount; ++j) {
            if (ranks[j] == players[i].rank) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            ranks[rankCount++] = players[i].rank;
        }
    }

    // Sort ranks
    quickSort(ranks, 0, rankCount - 1);

    // Display ranks
    cout << setw(25) << "Available Ranks";
    cout << ": ";
    for (int i = 0; i < rankCount; ++i) {
        cout << ranks[i];
        if (i < rankCount - 1) cout << ", ";
    }
    cout << endl;

    // Count checked-in
    int checkedInTrue = 0, checkedInFalse = 0;
    for (int i = 0; i < index; ++i) {
        if (players[i].checked_in) {
            ++checkedInTrue;
        } else {
            ++checkedInFalse;
        }
    }

    cout << setw(25) << "Checked-In (True)"
            << ": " << checkedInTrue <<" [GREEN]"<<endl;
    cout << setw(25) << "Checked-In (False)"
            << ": " << checkedInFalse <<" [DEFAULT]"<<endl;

    // Count Priority
    cout << setw(25) << "WildCard Priority Type"
            << ": " << wCount <<" times"<<endl;
    cout << setw(25) << "EarlyBird Priority Type"
            << ": " << eCount <<" times"<<endl;
    cout << setw(25) << "Regular Priority Type"
            << ": " << rCount <<" times"<<endl;
    cout << string(240, '-') << endl;
    // Free Memory again
    delete[] ranks;
    delete[] wildcard;
    delete[] earlybird;
    delete[] regular;
    delete[] players;
}

void Player::updateStats(const char* id, bool won){
    for(int i = 0; i < size; ++i){
        if (strcmp(data[i].id, id) == 0){
            if (won) ++data[i].wins;
            else ++data[i].losses;
            return;
        }
    }
}

void Player::print(){
    for (int i = 0; i < size; ++i) {
        std::cout << data[i].id << ", " << data[i].name << ", " << data[i].rank
                << ", " << (data[i].checked_in ? "true" : "false") << ", "
                << data[i].wins << ", " << data[i].losses << ", " << data[i].prioritytype << endl;
    }
}

int Player::getSize(){
    return size;
}



// --------- Match Function -----------------------------------------------------------
bool Player::loadPlayers(const string& filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return false;
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, ranking_str, checkedin_str, prioritytype_str, kill_str, death_str, wins_str, losses_str, winrate_str;

        getline(ss, id, ',');
        getline(ss, name, ',');
        if (!name.empty() && name.front() == '"') {
            name = name.substr(1, name.size() - 2);
        }
        getline(ss, ranking_str, ',');
        getline(ss, checkedin_str, ',');
        getline(ss, wins_str, ',');
        getline(ss, losses_str, ',');
        getline(ss, prioritytype_str, ',');
        getline(ss, winrate_str, ',');
        getline(ss, kill_str, ',');
        getline(ss, death_str, ',');
        if (all_data_size == capacity) {
            player* temp = new player[capacity * 2];
            for (int i = 0; i < all_data_size; ++i) {
                temp[i] = all_data[i];
            }
            delete[] all_data;
            all_data = temp;
            capacity *= 2;
        }

        try {
            strcpy(all_data[all_data_size].id, id.c_str());
            strcpy(all_data[all_data_size].name, name.c_str());
            all_data[all_data_size].rank = stoi(ranking_str);
            all_data[all_data_size].checked_in = (checkedin_str == "true");
            all_data[all_data_size].wins = wins_str.empty() ? 0 : stoi(wins_str);
            all_data[all_data_size].losses = losses_str.empty() ? 0 : stoi(losses_str);
            all_data[all_data_size].prioritytype = prioritytype_str.c_str(); 
            all_data[all_data_size].winrate = winrate_str.empty() ? 0 : stof(winrate_str);
            all_data[all_data_size].kill = kill_str.empty() ? 0 : stoi(kill_str);
            all_data[all_data_size].death = death_str.empty() ? 0 : stoi(death_str);
            ++all_data_size;
        } catch (...) {
            cerr << "Invalid data in CSV line: " << line << endl;
            continue;
        }
    }
    file.close();
    cout << "Loaded " << all_data_size << " players from " << filename << "\n";
    return true;
}
void Player::buildTree(int depth, Match*& node, int round){
    if (depth == 0) return;
    node = createMatch(round);
    buildTree(depth - 1, node->left_child, round - 1);
    buildTree(depth - 1, node->right_child, round - 1);
    if (node->left_child) node->left_child->parent = node;
    if (node->right_child) node->right_child->parent = node;
}
void Player::assignPlayersToLeaves(Match* node, int& player_index){
    if (!node) return;
    if (!node->left_child && !node->right_child) { // Leaf node
        if (player_index < 4) {
            strcpy(node->player1_id, data[player_index].id);
            int opponent_index = 7 - player_index;
            if (opponent_index < 8) {
                strcpy(node->player2_id, data[opponent_index].id);
            } else {
                node->player2_id[0] = '\0'; // Bye (shouldn’t happen with 8 players)
            }
            cout << "[DEBUG] Assigned: " << node->player1_id << " vs. " << node->player2_id << "\n";
            player_index++;
        }
    } else {
        assignPlayersToLeaves(node->left_child, player_index);
        assignPlayersToLeaves(node->right_child, player_index);
    }
}
void Player::updateParent(Match* node){
    if (!node || !node->parent || node->winner_id[0] == '\0') return;
    if (node->parent->left_child == node) {
        strcpy(node->parent->player1_id, node->winner_id);
    } else {
        strcpy(node->parent->player2_id, node->winner_id);
    }
    node->parent->winner_id[0] = '\0'; // Clear parent's winner
    cout << "[DEBUG] Updated parent: P1=" << node->parent->player1_id << ", P2=" << node->parent->player2_id << "\n";
}
bool Player::resolveMatch(Match* node){
    if (!node || node->player1_id[0] == '\0'){
        cerr << "[DEBUG] Invalid Match node\n";
        return false;
    }
    cout << "[DEBUG] Resolving match: round = " << node->round
    << ", player1_id = " << node->player1_id
    << ", player2_id = " << node->player2_id << endl;
    cout << "Match " << node->round << ": " << node->player1_id;
    if (node->player2_id[0] != '\0') {
        cout << " vs. " << node->player2_id;
    } else {
        cout << " (Bye)";
        strcpy(node->winner_id, node->player1_id);
        player* winner = findPlayerById(node->winner_id);
        if(winner){
            winner->wins++;
        }
        updateParent(node);
        return true;
    }
    cout << "\nEnter winner ID: ";
    string winner_id;
    getline(cin, winner_id);
    if (winner_id.empty()) {
        cerr << "[DEBUG] Empty input received\n";
        return false;
    }
    winner_id.erase(0, winner_id.find_first_not_of(" \t"));
    winner_id.erase(winner_id.find_last_not_of(" \t") + 1);
    cout << "[DEBUG] Received input: '" << winner_id << "'\n";
    if (strcmp(winner_id.c_str(), node->player1_id) == 0 ||
        strcmp(winner_id.c_str(), node->player2_id) == 0) {
        strcpy(node->winner_id, winner_id.c_str());
        player* winner = findPlayerById(node->winner_id);
        player* loser = findPlayerById(
            strcmp(winner_id.c_str(), node->player1_id) == 0 ? node->player2_id : node->player1_id
        );
        int winner_kills, winner_deaths, loser_kills, loser_deaths;
        cout << "Enter kills for " << winner->name << ": ";
        cin >> winner_kills;
        cout << "Enter deaths for " << winner->name << ": ";
        cin >> winner_deaths;
        cout << "Enter kills for " << loser->name << ": ";
        cin >> loser_kills;
        cout << "Enter deaths for " << loser->name << ": ";
        cin >> loser_deaths;
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        if(winner){
            winner->wins++;
            winner->kill += winner_kills;
            winner->death += winner_deaths;
        }
        if(loser){
            loser->losses++;
            loser->kill += loser_kills;
            loser->death += loser_deaths;
        }
        node->winner_kills = winner_kills;
        node->winner_deaths = winner_deaths;
        node->loser_kills = loser_kills;
        node->loser_deaths = loser_deaths;
        updateParent(node);
        return true;
    }
    cerr << "Invalid winner ID.\n";
    return false;
}
void Player::processRound(Match* node, int round){
    if (!node) return;
    processRound(node->left_child, round);
    processRound(node->right_child, round);
    if (node->round == round) {
        cout << "[DEBUG] Processing node: Round " << node->round
            << ", P1=" << node->player1_id << ", P2=" << node->player2_id << endl;
        if (!resolveMatch(node)) {
            cout << "[DEBUG] Failed to resolve match\n";
        }
    }
}
void Player::displayMatch(const Match* node, const string& prefix = ""){
    if (!node) return;
    cout << prefix << "Round " << node->round << ": ";
    const player* p1 = findPlayerById(node->player1_id);
    if (p1) {
        cout << left << setw(5) << p1->id << "("
                << setw(10) << p1->name << "[Rank: " << setw(2) << p1->rank << "])";
    } else {
        cout << setw(20) << node->player1_id;
    }
    if (node->player2_id[0] != '\0') {
        const player* p2 = findPlayerById(node->player2_id);
        cout << " vs. ";
        if (p2) {
            cout << setw(5) << p2->id << "("
                    <<setw(10) << p2->name << "[Rank:" << setw(2) << p2->rank << "])" << setw(2);
        } else {
            cout << setw(20) << node->player2_id;
        }
    } else {
        cout << " vs. " << setw(20) << "-";
    }
    cout << " -> Winner: ";
    if (node->winner_id[0] != '\0') {
        const player* winner = findPlayerById(node->winner_id);
        if (winner) {
            cout << setw(5) << winner->id << "(" << winner->name << ")";
        } else {
            cout << setw(15) << node->winner_id;
        }
    } else {
        cout << setw(15) << "TBD";
    }
    cout << endl;
}
void Player::displayBracket(Match* node, int round, bool first = true){
    if (!node) return;
    if (node->round == round && first) {
        string round_name = round == 1 ? "QuarterFinals" : round == 2 ? "SemiFinals" : "Final";
        cout << "\n"<< round_name << ":\n";
        cout << string(90, '-') << endl;
    }
    if (node->round == round) {
        displayMatch(node, "");
    } else {
        displayBracket(node->left_child, round, first);
        displayBracket(node->right_child, round, false);
    }
}
void Player::saveMatches(const string& filename, Match* node){
    if (!node) return;
    ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing\n";
        return;
    }
    file << "match" << match_count << "," << node->player1_id << ","
        << node->player2_id << "," << node->winner_id << "," << node->round << ","
        << node->winner_kills << "," << node->winner_deaths << "," << node->loser_kills << ","
        << node->loser_deaths << "\n";
    file.close();
    match_count--;
    saveMatches(filename, node->left_child);
    saveMatches(filename, node->right_child);
}
void Player::savePlayers(const string& filename){
    ofstream out(filename, ios::out);
    if (!out.is_open()) {
        cerr << "Failed to open " << filename << " for writing\n";
        return;
    }
    out << "ID,Name,Ranking,CheckedIn,Wins,Losses,PriorityType,WinsRate,Kill,Death\n";
    for (int i = 0; i < all_data_size; ++i) {
        out << all_data[i].id << "," << all_data[i].name << ","
            << all_data[i].rank << "," << (data[i].checked_in ? "true" : "false") << ","
            << all_data[i].wins << "," << all_data[i].losses << ","
            << all_data[i].prioritytype << "," << all_data[i].winrate << ","
            << all_data[i].kill << "," << all_data[i].death << "\n";
    }
    out.close();
    cout << "Updated player data in " << filename << "\n";
}
void Player::deleteTree(Match* node){
    if (!node) return;
    deleteTree(node->left_child);
    deleteTree(node->right_child);
    delete node;
}
void Player::initialize(const string& player_file){
    if (!loadPlayers(player_file)) return;
    if (all_data_size < 8) {
        cerr << "Need at least 8 players for tournament.\n";
        return;
    }
    int checked_in_count = 0;
    for (int i = 0; i < all_data_size; ++i) {
        if (all_data[i].checked_in) checked_in_count++;
    }
    cout << "[DEBUG] Total checked-in players: " << checked_in_count << "\n";
    if (checked_in_count < 8) {
        cerr << "Need at least 8 checked-in players for tournament.\n";
        return;
    }
    quickSortPlayers(all_data, 0, all_data_size - 1);
    delete[] data;
    data = new player[8];
    size = 0;
    for (int i = 0; i < all_data_size && size < 8; ++i) {
        if(all_data[i].checked_in){
            data[size] = all_data[i];
            size++;
        }
    }
    if (size < 8) {
        cerr << "Not enough checked-in players after filtering.\n";
        delete[] data;
        data = nullptr;
        return;
    }
    cout << "[DEBUG] Sorted players:\n";
    for (int i = 0; i < size; ++i) {
        cout << "ID: " << data[i].id << ", Rank: " << data[i].rank << ", CheckedIn: "
        << (data[i].checked_in ? "true" : "false") << "\n";
    }
    deleteTree(root);
    root = nullptr;
    match_count = 0;
    buildTree(3, root, 3); // 3 levels: final, semifinals, quarterfinals
    int player_index = 0;
    assignPlayersToLeaves(root, player_index);
}
void Player::runTournament(){
    if (!root) {
        cout << "Tournament not initialized.\n";
        return;
    }
    debugPrint(root);
    for (int round = 1; round <= 3; ++round) {
        cout << "\nStarting Round " << round << "\n";
        cout << "[DEBUG] Displaying round " << round << endl;
        displayBracket(root, round, true);
        cout << "[DEBUG] Displaying round " << round << endl;
        processRound(root, round);
        cout << "[DEBUG] Displaying round " << round << " after processing\n";
        displayBracket(root, round, true);
        savePlayers("PLAYER.txt");
    }
}
void Player::displayTournament(){
    if (!root) {
        cout << "Tournament not initialized.\n";
        return;
    }
    for (int round = 1; round <= 3; ++round) {
        displayBracket(root, round, true);
    }
}
void Player::saveTournament(const string& filename){
    ofstream file(filename, ios::out);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << " for writing\n";
        return;
    }
    file << "MatchID,Player1ID,Player2ID,WinnerID,Round,WinnerKills,WinnerDeaths,LoserKills,LoserDeaths\n";
    file.close();
    match_count = 7; // Matches numbered 1–7
    saveMatches(filename, root);
    cout << "Saved tournament to " << filename << "\n";
}
const Player::player* Player::findPlayerById(const char* id) const{
    for (int i = 0; i < all_data_size; ++i) {
        if (strcmp(all_data[i].id, id) == 0) {
            return &all_data[i];
        }
    }
    return nullptr;
}
Player::player* Player::findPlayerById(const char* id){
    for (int i = 0; i < all_data_size; ++i) {
        if (strcmp(all_data[i].id, id) == 0) {
            return &all_data[i];
        }
    }
    return nullptr;
}
void Player::debugPrint(Match* node){
    if (!node) return;
    cout << "[DEBUG] Match Round: " << node->round 
        << ", P1=" << node->player1_id 
        << ", P2=" << node->player2_id << endl;
    debugPrint(node->left_child);
    debugPrint(node->right_child);
}
void Player::displayMatchesHistory(const string& filename){
    Match matches[100];
    int count = 0;
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open file." << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line) && count < 100) {
        stringstream ss(line);
        string token;
        getline(ss, matches[count].matchID, ',');
        getline(ss, token, ',');
        strcpy(matches[count].player1_id, token.c_str());
        getline(ss, token, ',');
        strcpy(matches[count].player2_id, token.c_str());
        getline(ss, token, ',');
        strcpy(matches[count].winner_id, token.c_str());
        getline(ss, token, ',');
        matches[count].round = atoi(token.c_str());
        getline(ss, token, ',');
        matches[count].winner_kills = atoi(token.c_str());
        getline(ss, token, ',');
        matches[count].winner_deaths = atoi(token.c_str());
        getline(ss, token, ',');
        matches[count].loser_kills = atoi(token.c_str());
        getline(ss, token);
        matches[count].loser_deaths = atoi(token.c_str());
        ++count;
    }
    file.close();
    sortByRound(matches, count);
    cout << left << setw(10) << "MatchID"
        << setw(12) << "Player1ID"
        << setw(12) << "Player2ID"
        << setw(10) << "WinnerID"
        << setw(6) << "Round"
        << setw(16) << "Winner Kills"
        << setw(16) << "Winner Deaths"
        << setw(16) << "Loser Kills"
        << setw(16) << "Loser Deaths"<<endl;
    for (int i = 0; i < count; ++i) {
        cout << left << setw(10) << matches[i].matchID
            << setw(12) << matches[i].player1_id
            << setw(12) << matches[i].player2_id
            << setw(10) << matches[i].winner_id
            << setw(6)  << matches[i].round
            << setw(16) << matches[i].winner_kills
            << setw(16) << matches[i].winner_deaths
            << setw(16) << matches[i].loser_kills
            << setw(16) << matches[i].loser_deaths << endl;
    }
}
void Player::displayPlayerPerformance(const string& filename){
    ifstream countFile(filename);
    if(!countFile.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        return;
    }
    int lineCount = 0;
    string line;
    getline(countFile, line);
    while(getline(countFile,line)){
        if(!line.empty()){
            ++lineCount;
        }
    }
    countFile.close();
    if(lineCount == 0){
        cout << "No players exists."<<endl;
        return;
    }
    player* players = new player[lineCount];
    int index = 0;
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"Failed to open"<<filename<<endl;
        delete[] players;
        return;
    }
    getline(file, line);
    while(getline(file, line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string id, name, rank, checkedin, wins, losses, prioritytype, winsrate, kill, death;
        getline(ss, id, ',');
        getline(ss, name, ',');
        if (!name.empty() && name.front() == '"') {
            name = name.substr(1, name.size() - 2);
        }
        getline(ss, rank, ',');
        getline(ss, checkedin, ',');
        getline(ss, wins, ',');
        getline(ss, losses, ',');
        getline(ss, prioritytype, ',');
        getline(ss, winsrate, ',');
        getline(ss, kill, ',');
        getline(ss, death, ',');

        try {
            strcpy(players[index].id, id.c_str());
            strcpy(players[index].name, name.c_str());
            players[index].rank = stoi(rank);
            players[index].checked_in = (checkedin == "true");
            players[index].wins = wins.empty() ? 0 : stoi(wins);
            players[index].losses = losses.empty() ? 0 : stoi(losses);
            players[index].prioritytype = prioritytype;
            players[index].winrate = winsrate.empty() ? 0.0f : stof(winsrate);
            players[index].kill = kill.empty() ? 0 : stoi(kill);
            players[index].death = death.empty() ? 0 : stoi(death);
            ++index;
        } catch (...) {
            cerr << "Invalid data in CSV line: " << line << "\n";
            continue;
        }
    }
    file.close();
    cout << "\nPlayer Performance \n";
    cout << left
        << setw(10) << "PlayerID"
        << setw(10) << "Name"
        << setw(10) << "Rank"
        << setw(10) << "Win"
        << setw(10) << "Lose"
        << setw(10) << "WinsRate"
        << setw(10) << "Kill"
        << setw(10) << "Death"
        << "\n";
    cout << string(88, '-') << endl;
    for (int i = 0; i < index; ++i) {
        double winrate = 0.0;
        int totalGames = players[i].wins + players[i].losses;
        if(totalGames > 0){
            winrate = (players[i].wins * 100.0) / totalGames;
        }
        cout << left
            << setw(10) << players[i].id
            << setw(10) << players[i].name
            << setw(10) << players[i].rank
            << setw(10) << players[i].wins
            << setw(10) << players[i].losses
            << setw(10) << fixed << setprecision(1) << winrate
            << setw(10) << players[i].kill
            << setw(10) << players[i].death
            << endl;
    }
    cout << string(88, '-') << endl;

    for (int i = 0; i < index; ++i) {
        double winrate = 0.0;
        int totalGames = players[i].wins + players[i].losses;
        if (totalGames > 0) {
            winrate = (players[i].wins * 100.0) / totalGames;
        }

        if (players[i].wins >= 3 && players[i].losses == 0) {
            cout << "\033[32m"  // green
                    << "✅ Player " << players[i].name << " is on a win streak "
                    << players[i].wins << " !" <<"\033[0m" << endl;
        }

        if (players[i].losses >= 3 && players[i].wins == 0) {
            cout << "\033[31m"  // red
                    << "❌ Player " << players[i].name << " is on a losing streak "
                    << players[i].losses << " !" << "\033[0m" << endl;
        }

        if (winrate == 100.0) {
            cout << "\033[32m"
                    << "🏆 Player " << players[i].name << " has a perfect win rate!"
                    << "\033[0m" << endl;
        }

        if (players[i].kill >= 100) {
            cout << "\033[36m"
                    << "⚔️ Player " << players[i].name << " has over 100 kills!"
                    << "\033[0m" << endl;
        } else if (players[i].kill >= 50) {
            cout << "\033[36m"
                    << "⚔️ Player " << players[i].name << " has over 50 kills!"
                    << "\033[0m" << endl;
        }

        if (players[i].death >= 100) {
            cout << "\033[33m"
                    << "💀 Player " << players[i].name << " has over 100 deaths!"
                    << "\033[0m" << endl;
        } else if (players[i].death >= 50) {
            cout << "\033[33m"
                        << "💀 Player " << players[i].name << " has over 50 deaths!"
                        << "\033[0m" << endl;
        }
    }
    cout << string(88, '-') << endl;
}
