#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

struct Spectator {
    int id;
    string name;
    int priority; //4 = Streamer, 3 = General, 2 = Influencer, 1 = VIP
    bool is_seated;
    int slot_number; // -1 for non-streamers, 1-5 for streamers
};


// Priority Queue for VIPs and Influencers
class PriorityQueue{
    private:
        struct QueueItem{
            Spectator spectator;
            int priority;
        };
        QueueItem* items;
        int size;
        int count;
    public:
        PriorityQueue(int capacity){
            size = capacity;
            items = new QueueItem[capacity];
            count = 0;
        }
        ~PriorityQueue(){
            delete[] items;
        }
        bool isFull();
        bool isEmpty();
        bool enqueue(const Spectator& spec);
        Spectator dequeue(){
            if(!isEmpty()){
                Spectator spec = items[0].spectator;
                for(int i = 0; i < count - 1; i++){
                    items[i] = items[i + 1];
                }
                count--;
                return spec;
            }
            return Spectator{-1, "", 0, false, -1};
        }
};

// Circular Queue for General Spectators, Streamers, and Waiting Queue
class CircularQueue{
    private:
        Spectator* items;
        int size;
        int front;
        int rear;
        int count;
    public:
        CircularQueue(int capacity){
            size = capacity;
            items = new Spectator[capacity];
            front = 0;
            rear = -1;
            count = 0;
        }
        ~CircularQueue(){
            delete[] items;
        }
        bool isFull();
        bool isEmpty();
        bool enqueue(const Spectator& item);
        Spectator dequeue() {
            if (!isEmpty()) {
                Spectator item = items[front];
                items[front] = {-1, "", 0, false, -1};
                front = (front + 1) % size;
                count--;
                return item;
            }
            return {-1, "", 0, false, -1};
        }
};

class SpectatorManager{
    private:
        PriorityQueue priorityQ;
        CircularQueue generalQ;
        CircularQueue streamerQ;
        CircularQueue waitingQ;
        string* vipSeats; // 30 seats
        string* influencerSeats; // 10 seats
        string* generalSeats; // 60 seats
        string** streamerSlots; // 5 slots, each with 5 seats
        int vipCapacity;
        int influencerCapacity;
        int generalCapacity;
        int streamserSlotCapacity; // Seats per slot
        int totalStreamerSlots; // Number of slots
    public:
        SpectatorManager()
            : priorityQ(40), generalQ(60), waitingQ(120), streamerQ(25){
                vipCapacity = 30;
                influencerCapacity = 10;
                generalCapacity = 60;
                streamserSlotCapacity = 5;
                totalStreamerSlots = 5;

                vipSeats = new string[vipCapacity];
                influencerSeats = new string[influencerCapacity];
                generalSeats = new string[generalCapacity];
                streamerSlots = new string*[totalStreamerSlots];
                for (int i = 0; i < totalStreamerSlots; i++) {
                    streamerSlots[i] = new string[streamserSlotCapacity];
                    for (int j = 0; j < streamserSlotCapacity; j++) {
                        streamerSlots[i][j] = "";
                    }
                }
                for (int i = 0; i < vipCapacity; i++) vipSeats[i] = "";
                for (int i = 0; i < influencerCapacity; i++) influencerSeats[i] = "";
                for (int i = 0; i < generalCapacity; i++) generalSeats[i] = "";
            }
        ~SpectatorManager() {
            delete[] vipSeats;
            delete[] influencerSeats;
            delete[] generalSeats;
            for (int i = 0; i < totalStreamerSlots; i++) {
                delete[] streamerSlots[i];
            }
            delete[] streamerSlots;
        }
        void loadCSV_spectator(const string& filename);
        void addSpectator(const Spectator& spec);
        void assignVipSeat(const Spectator& spec);
        void assignInfluencerSeat(const Spectator& spec);
        void assignGeneralSeat(const Spectator& spec);
        void assignStreamerSeat(const Spectator& spec);
        void processWaitingQueue();
        void displaySeating();
        bool addSpectatorToFile(const string& filename);
};