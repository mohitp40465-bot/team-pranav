#include "WaitingQueue.h"
#include <fstream>
#include <sstream>
#include <vector>

static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

WaitingQueue::WaitingQueue() : front(nullptr), rear(nullptr), count(0) {}

WaitingQueue::~WaitingQueue() {
    clear();
}

void WaitingQueue::clear() {
    while (front != nullptr) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
    count = 0;
}

void WaitingQueue::enqueue(const std::string& cId, const std::string& cName, const std::string& vId) {
    WaitingRequest req(cId, cName, vId);
    QueueNode* newNode = new QueueNode(req);

    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    count++;
    std::cout << "[INFO] Customer " << cName << " (" << cId << ") added to Waiting Queue for Vehicle ID: " << vId << ".\n";
}

bool WaitingQueue::dequeue(WaitingRequest& outRequest) {
    if (isEmpty()) {
        return false;
    }

    QueueNode* temp = front;
    outRequest = temp->data;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
    count--;
    return true;
}

bool WaitingQueue::peek(WaitingRequest& outRequest) const {
    if (isEmpty()) return false;
    outRequest = front->data;
    return true;
}

bool WaitingQueue::isEmpty() const {
    return front == nullptr;
}

int WaitingQueue::getCount() const {
    return count;
}

void WaitingQueue::displayQueue() const {
    if (isEmpty()) {
        std::cout << "\n=========================================================\n";
        std::cout << "                 WAITING QUEUE IS EMPTY                  \n";
        std::cout << "=========================================================\n";
        return;
    }

    std::cout << "\n=========================================================\n";
    std::cout << "               CURRENT WAITING QUEUE (FIFO)              \n";
    std::cout << "=========================================================\n";
    std::cout << std::left 
              << std::setw(6)  << "Pos"
              << std::setw(14) << "Customer ID"
              << std::setw(22) << "Customer Name"
              << std::setw(15) << "Requested ID"
              << "\n";
    std::cout << "---------------------------------------------------------\n";

    QueueNode* curr = front;
    int pos = 1;
    while (curr != nullptr) {
        std::cout << std::left 
                  << std::setw(6)  << pos++
                  << std::setw(14) << curr->data.customerId
                  << std::setw(22) << curr->data.customerName
                  << std::setw(15) << curr->data.requestedVehicleId
                  << "\n";
        curr = curr->next;
    }
    std::cout << "---------------------------------------------------------\n";
    std::cout << "Total waiting requests: " << count << "\n";
}

bool WaitingQueue::hasWaitingCustomerForVehicle(const std::string& vehicleId) const {
    QueueNode* curr = front;
    while (curr != nullptr) {
        if (curr->data.requestedVehicleId == vehicleId) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool WaitingQueue::serveCustomerForVehicle(const std::string& vehicleId, WaitingRequest& outRequest) {
    if (isEmpty()) return false;

    QueueNode* curr = front;
    QueueNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.requestedVehicleId == vehicleId) {
            // Found eligible customer
            outRequest = curr->data;

            // Remove node from linked list
            if (curr == front) {
                front = front->next;
                if (front == nullptr) rear = nullptr;
            } else {
                prev->next = curr->next;
                if (curr == rear) {
                    rear = prev;
                }
            }

            delete curr;
            count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

void WaitingQueue::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return;

    QueueNode* curr = front;
    while (curr != nullptr) {
        outFile << curr->data.customerId << "|"
                << curr->data.customerName << "|"
                << curr->data.requestedVehicleId << "\n";
        curr = curr->next;
    }
    outFile.close();
}

void WaitingQueue::loadFromFile(const std::string& filename) {
    clear();
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        std::vector<std::string> tokens = split(line, '|');
        if (tokens.size() >= 3) {
            enqueue(tokens[0], tokens[1], tokens[2]);
        }
    }
    inFile.close();
}
