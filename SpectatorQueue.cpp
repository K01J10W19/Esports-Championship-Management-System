#include "SpectatorQueue.hpp"

// - - - - - Class Priority Queue - - - - -
bool PriorityQueue::isFull(){
    return count == size;
}
bool PriorityQueue::isEmpty(){
    return count == 0;
}
bool PriorityQueue::enqueue(const Spectator& spec){
    if(!isFull()){
        items[count] = {spec, spec.priority};
        count++;
        for(int i = count - 1; i > 0; i--){
            if(items[i].priority < items[i - 1].priority){
                QueueItem temp = items[i];
                items[i] = items[i-1];
                items[i-1] = temp;
            } else {
                break;
            }
        }
        return true;
    }
    return false;
}
// - - - - - Class Circular Queue - - - - -
bool CircularQueue::isFull(){
    return count == size;
}
bool CircularQueue::isEmpty(){
    return count == 0;
}
bool CircularQueue::enqueue(const Spectator& item){
    if (!isFull()) {
            rear = (rear + 1) % size;
            items[rear] = item;
            count++;
            return true;
        }
        return false;
}
// - - - - - Class SpectatorManager - - - - -
void SpectatorManager::loadCSV_spectator(const string& filename){
    ifstream file(filename);
    if(!file.is_open()){
        cout << "Error: Could not open file " << filename << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, name, priorityStr, isSeatedStr, slotNumberStr;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priorityStr, ',');
        getline(ss, isSeatedStr, ',');
        getline(ss, slotNumberStr, ',');

        try{
            if (idStr.empty() || priorityStr.empty() || isSeatedStr.empty() || slotNumberStr.empty()) {
                cerr << "Skipping malformed line: " << line << endl;
                continue;
            }
            Spectator spec;
            spec.id = stoi(idStr);
            spec.name = name;
            spec.priority = stoi(priorityStr);
            spec.is_seated = (isSeatedStr == "true");
            spec.slot_number = stoi(slotNumberStr);

            if (spec.is_seated) {
                addSpectator(spec);
            }
        } catch (const exception& e){
            cerr << "Error parsing line: " << line << " => " << e.what() << endl;
        }
    }
    file.close();
}
void SpectatorManager::addSpectator(const Spectator& spec){
    if(spec.priority == 4){
        if(streamerQ.enqueue(spec)){
            cout << "Streamer " << spec.name << " (ID: " << spec.id << ") added to streamer queue." << endl;
            assignStreamerSeat(spec);
        }else{
            cout << "Streamer " << spec.name << " (ID: " << spec.id << ") cannot be added: all streaming slots are full." << endl;
        }
    }else if(spec.priority == 1 || spec.priority == 2){
        if(priorityQ.enqueue(spec)){
            cout <<(spec.priority == 1 ? "VIP" : "Influencer") << " " << spec.name 
                << " (ID: " << spec.id << ") added to priority queue." << endl;
            if (spec.priority == 1) {
                assignVipSeat(spec);
            } else {
                assignInfluencerSeat(spec);
            }
        } else{
            cout << (spec.priority == 1 ? "VIP" : "Influencer") << " " << spec.name 
                << " (ID: " << spec.id << ") cannot be added: priority queue is full." << endl;
        }
    }else{
        if(generalQ.enqueue(spec)){
            cout << "General spectator " << spec.name << " (ID: " << spec.id << ") added to general queue." << endl;
            assignGeneralSeat(spec);
        }else{
            if (waitingQ.enqueue(spec)) {
                cout << "General spectator " << spec.name << " (ID: " << spec.id 
                        << ") added to waiting queue due to overflow." << endl;
            } else {
                cout << "General spectator " << spec.name << " (ID: " << spec.id 
                        << ") cannot be added: waiting queue is full." << endl;
            }
        }
    }
}
void SpectatorManager::assignVipSeat(const Spectator& spec){
    for (int i = 0; i < vipCapacity; i++) {
        if (vipSeats[i].empty()) {
            vipSeats[i] = spec.name + " (ID: " + to_string(spec.id) + ")";
            cout << "\033[32mVIP " << spec.name << " (ID: " << spec.id << ") assigned to VIP seat " << (i + 1) << ".\033[0m" << endl;
            priorityQ.dequeue();
            return;
        }
    }
    cout << "\033[31mNo available VIP seats for " << spec.name << " (ID: " << spec.id << ").\033[0m" << endl;
}
void SpectatorManager::assignInfluencerSeat(const Spectator& spec){
    for (int i = 0; i < influencerCapacity; i++) {
        if (influencerSeats[i].empty()) {
            influencerSeats[i] = spec.name + " (ID: " + to_string(spec.id) + ")";
            cout << "\033[32mInfluencer " << spec.name << " (ID: " << spec.id << ") assigned to influencer seat " << (i + 1) << ".\033[0m" << endl;
            priorityQ.dequeue();
            return;
        }
    }
    cout << "\033[31mNo available influencer seats for " << spec.name << " (ID: " << spec.id << ").\033[0m" << endl;
}
void SpectatorManager::assignGeneralSeat(const Spectator& spec){
    for (int i = 0; i < generalCapacity; i++) {
        if (generalSeats[i].empty()) {
            generalSeats[i] = spec.name + " (ID: " + to_string(spec.id) + ")";
            cout << "\033[32mGeneral spectator " << spec.name << " (ID: " << spec.id << ") assigned to seat " << (i + 1) << ".\033[0m" << endl;
            generalQ.dequeue();
            return;
        }
    }
    cout << "\033[31mNo available general seats for " << spec.name << " (ID: " << spec.id << "). Moving to waiting queue.\033[0m" << endl;
    generalQ.dequeue();
    waitingQ.enqueue(spec);
}
void SpectatorManager::assignStreamerSeat(const Spectator& spec){
    int preferredSlot = (spec.slot_number >= 1 && spec.slot_number <= totalStreamerSlots) ? spec.slot_number - 1 : -1;
    if (preferredSlot != -1) {
        for (int j = 0; j < streamserSlotCapacity; j++) {
            if (streamerSlots[preferredSlot][j].empty()) {
                streamerSlots[preferredSlot][j] = spec.name + " (ID: " + to_string(spec.id) + ")";
                cout << "\033[32mStreamer " << spec.name << " (ID: " << spec.id << ") assigned to streaming slot " 
                        << (preferredSlot + 1) << ", seat " << (j + 1) << ".\033[0m" << endl;
                streamerQ.dequeue();
                return;
            }
        }
    }
    // Try any available slot if preferred slot is full
    for (int i = 0; i < totalStreamerSlots; i++) {
        for (int j = 0; j < streamserSlotCapacity; j++) {
            if (streamerSlots[i][j].empty()) {
                streamerSlots[i][j] = spec.name + " (ID: " + to_string(spec.id) + ")";
                cout << "\033[32mStreamer " << spec.name << " (ID: " << spec.id << ") assigned to streaming slot " 
                        << (i + 1) << ", seat " << (j + 1) << ".\033[0m" << endl;
                streamerQ.dequeue();
                return;
            }
        }
    }
    cout << "\033[31mNo available streaming slots for " << spec.name << " (ID: " << spec.id << ").\033[0m" << endl;
}
void SpectatorManager::processWaitingQueue(){
    cout << "\n\033[37mProcessing waiting queue...\033[0m" << endl;
    while (!waitingQ.isEmpty()) {
        Spectator spec = waitingQ.dequeue();
        if (generalQ.enqueue(spec)) {
            cout << "\033[37mProcessing waiting queue: " << spec.name << " (ID: " << spec.id << ") moved to general queue.\033[0m" << endl;
            assignGeneralSeat(spec);
        } else {
            cout << "\033[37mProcessing waiting queue: " << spec.name << " (ID: " << spec.id << ") cannot be moved, general queue full.\033[0m" << endl;
            waitingQ.enqueue(spec);
            break;
        }
    }
}
void SpectatorManager::displaySeating(){
    cout << "\nCurrent Seating Arrangement:" << endl;
    cout << "\033[37m" <<string(250, '-') << "\033[0m" <<endl;
    cout << "\033[33mVIP Seats (" << vipCapacity << "):\033[0m\n";
    for(int i = 0; i < vipCapacity; i++){
        cout << "Seat " << (i+1) <<": "<< (vipSeats[i].empty() ? "\033[32mEmpty\033[0m" : vipSeats[i]) << " | ";
    }
    cout << "\n\033[37m" <<string(250, '-') << "\033[0m" <<endl;
    cout << "\n\033[33mInfluencer Seats (" << influencerCapacity << "):\033[0m\n";
    for (int i = 0; i < influencerCapacity; i++) {
        cout << "Seat " << (i + 1) << ": " << (influencerSeats[i].empty() ? "\033[32mEmpty\033[0m" : influencerSeats[i]) << " | ";
    }
    cout << "\n\033[37m" <<string(250, '-') << "\033[0m" <<endl;
    cout << "\n\033[33mGeneral Seats (" << generalCapacity << "):\033[0m\n";
    for (int i = 0; i < generalCapacity; i++) {
        cout << "Seat " << (i + 1) << ": " << (generalSeats[i].empty() ? "\033[32mEmpty\033[0m" : generalSeats[i]) << " | ";
    }
    cout << "\n\033[37m" <<string(250, '-') << "\033[0m" <<endl;
    cout << "\n\033[33mStreamer Slots (" << totalStreamerSlots << " slots, " << streamserSlotCapacity << " seats each):\033[0m\n";
    for (int i = 0; i < totalStreamerSlots; i++) {
        cout << "  Slot " << (i + 1) << ": ";
        for (int j = 0; j < streamserSlotCapacity; j++) {
            cout << "Seat " << (j + 1) << ": " << (streamerSlots[i][j].empty() ? "\033[32mEmpty\033[0m" : streamerSlots[i][j]) << " | ";
        }
        cout << endl;
    }
    cout << "\n\033[37m" <<string(250, '-') << "\033[0m" <<endl;
}
bool SpectatorManager::addSpectatorToFile(const string& filename){
    int newId = 0;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        getline(file, line); 
        while (getline(file, line)) {
            stringstream ss(line);
            string idStr;
            getline(ss, idStr, ',');
            int id = stoi(idStr);
            if (id >= newId) {
                newId = id + 1;
            }
        }
        file.close();
    } else {
        cout << "Note: File " << filename << " does not exist, creating new file." << endl;
    }
    string name;
    bool is_seated;
    int priority, slot, num;
    cout << "Enter Spectator Name: ";
    getline(cin, name);
    cout << "1. VIP\t2. Influencer\t3. General\t4. Streamer\n";
    cin >> priority;
    if(priority == 4){
        cout <<"Enter the slot [1-5] for streamer";
        cin >> slot;
    }
    cout << "Is Seated ?\n[1. True / 2. False] : ";
    cin >> num;
    if(num == 1){
        is_seated = true;
    }else{
        is_seated = false;
    }
    if (priority < 1 || priority > 4) {
        cout << "Error: Invalid priority " << priority << " for " << name << ". Priority must be 1-4." << endl;
        return false;
    }
    if (priority == 4 && (slot < -1 || slot == 0 || slot > totalStreamerSlots)) {
        cout << "Error: Invalid slot number " << slot << " for streamer " << name << "." << endl;
        return false;
    }
    ofstream outfile(filename, ios::app);
    if (!outfile.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return false;
    }
    outfile << newId << "," << name << "," << priority << "," << (is_seated ? "true" : "false")
            << "," << slot << "\n";
    outfile.close();
    cout << "Player " << name << " registered.\n";
    if(is_seated){
        Spectator spec{newId, name, priority, is_seated, slot};
        addSpectator(spec);
    }
    return true;
}
