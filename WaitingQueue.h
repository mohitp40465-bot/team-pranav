#ifndef WAITINGQUEUE_H
#define WAITINGQUEUE_H

#include <iostream>
#include <string>
#include <iomanip>

/**
 * Structure representing a request in the waiting queue.
 */
struct WaitingRequest {
    std::string customerId;
    std::string customerName;
    std::string requestedVehicleId;

    WaitingRequest() : customerId(""), customerName(""), requestedVehicleId("") {}
    WaitingRequest(const std::string& cId, const std::string& cName, const std::string& vId)
        : customerId(cId), customerName(cName), requestedVehicleId(vId) {}
};

/**
 * Node for the Queue implementation
 */
struct QueueNode {
    WaitingRequest data;
    QueueNode* next;

    QueueNode(const WaitingRequest& req) : data(req), next(nullptr) {}
};

/**
 * ============================================================================
 * CLASS: WaitingQueue (Manual FIFO Queue Implementation)
 * Demonstrates: DSA - Linked Queue (Dynamic FIFO), Node management,
 *               Memory deallocation in destructor
 * ============================================================================
 */
class WaitingQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int count;

public:
    WaitingQueue();
    ~WaitingQueue();

    // Core Queue Operations
    void enqueue(const std::string& cId, const std::string& cName, const std::string& vId);
    bool dequeue(WaitingRequest& outRequest);
    bool peek(WaitingRequest& outRequest) const;
    bool isEmpty() const;
    int getCount() const;

    // Display
    void displayQueue() const;

    // Search and serve eligible customer for a specific vehicle
    bool hasWaitingCustomerForVehicle(const std::string& vehicleId) const;
    bool serveCustomerForVehicle(const std::string& vehicleId, WaitingRequest& outRequest);

    // Serialization
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void clear();
};

#endif // WAITINGQUEUE_H
