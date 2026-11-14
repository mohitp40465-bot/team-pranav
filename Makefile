# Makefile for Smart Vehicle Rental Management System
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRCS = main.cpp Vehicle.cpp Customer.cpp Booking.cpp WaitingQueue.cpp RentalHistory.cpp RentalSystem.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = SmartVehicleRental

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
