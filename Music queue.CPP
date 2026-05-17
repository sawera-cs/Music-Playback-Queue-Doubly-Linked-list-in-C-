#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

struct Node {
    string song;
    Node* next;
    Node* prev;

    Node(string name) {
        song = name;
        next = prev = nullptr;
    }
};

class PlaybackQueue {
private:
    Node* head;
    Node* tail;
    Node* current;

public:
    PlaybackQueue() {
        head = tail = current = nullptr;
    }

    // Insert song right after currently playing node
    void playNext(string song) {
        Node* newNode = new Node(song);

        // Empty queue — this song becomes everything
        if (head == nullptr) {
            head = tail = current = newNode;
            return;
        }

        // current is at the end, just append
        if (current->next == nullptr) {
            newNode->prev = current;
            current->next = newNode;
            tail = newNode;
            return;
        }

        // current is somewhere in the middle
        // need to squeeze newNode between current and current->next
        Node* temp = current->next;
        current->next = newNode;
        newNode->prev = current;
        newNode->next = temp;
        temp->prev = newNode;
    }

    // Move to the next song
    void skip() {
        if (current == nullptr) {
            cout << "Queue is empty.\n";
            return;
        }
        if (current->next == nullptr) {
            cout << "Already at the last song.\n";
            return;
        }
        current = current->next;
        cout << "Now playing: " << current->song << "\n";
    }

    // Remove any song by name
    void remove(string song) {
        Node* temp = head;

        while (temp != nullptr) {
            if (temp->song == song) {

                // If removing the currently playing song, move current forward
                if (temp == current) {
                    if (current->next != nullptr)
                        current = current->next;
                    else if (current->prev != nullptr)
                        current = current->prev;
                    else
                        current = nullptr;
                }

                // Fix neighbor pointers
                if (temp->prev != nullptr)
                    temp->prev->next = temp->next;
                else
                    head = temp->next; // was the head

                if (temp->next != nullptr)
                    temp->next->prev = temp->prev;
                else
                    tail = temp->prev; // was the tail

                delete temp;
                cout << "Removed: " << song << "\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Song not found: " << song << "\n";
    }

    // Shuffle everything except the current song
    void shuffle() {
        if (head == nullptr || head == tail) return;

        // Collect all nodes except current into a vector
        vector<Node*> nodes;
        Node* temp = head;
        while (temp != nullptr) {
            if (temp != current)
                nodes.push_back(temp);
            temp = temp->next;
        }

        // Shuffle the vector
        random_device rd;
        mt19937 g(rd());
        std::shuffle(nodes.begin(), nodes.end(), g);

        // Rebuild list: current stays in its position
        // Put current in the middle-ish, rest around it
        int mid = nodes.size() / 2;
        nodes.insert(nodes.begin() + mid, current);

        // Relink everything
        head = nodes[0];
        head->prev = nullptr;
        for (int i = 1; i < nodes.size(); i++) {
            nodes[i - 1]->next = nodes[i];
            nodes[i]->prev = nodes[i - 1];
        }
        tail = nodes.back();
        tail->next = nullptr;

        cout << "Queue shuffled. Current song unchanged: " << current->song << "\n";
    }

    // Delete everything after current song
    void clearAhead() {
        if (current == nullptr || current->next == nullptr) {
            cout << "Nothing ahead to clear.\n";
            return;
        }

        Node* temp = current->next;
        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }

        current->next = nullptr;
        tail = current;
        cout << "Cleared everything after: " << current->song << "\n";
    }

    // Print the full queue, marking current song
    void display() {
        if (head == nullptr) {
            cout << "Queue is empty.\n";
            return;
        }

        Node* temp = head;
        cout << "\n--- Playback Queue ---\n";
        while (temp != nullptr) {
            if (temp == current)
                cout << "  >> " << temp->song << " (now playing)\n";
            else
                cout << "     " << temp->song << "\n";
            temp = temp->next;
        }
        cout << "----------------------\n\n";
    }

    // Free memory on destruction
    ~PlaybackQueue() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
    }
};

int main() {
    PlaybackQueue q;

    q.playNext("Pasoori");
    q.playNext("Kana Yaari");
    q.playNext("Coke Studio Season 14");
    q.playNext("Tu Jhoom");
    q.playNext("Afreen Afreen");

    q.display();

    q.skip();
    q.skip();
    q.display();

    q.playNext("Bayaan");
    q.display();

    q.remove("Kana Yaari");
    q.display();

    q.shuffle();
    q.display();

    q.clearAhead();
    q.display();

    return 0;
}
