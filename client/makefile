CFLAGS := -c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS := -lboost_system -lpthread
BIN_DIR := bin

# Default target
all: clean_objects Client StompWCIClient

# Clean object files in bin directory before compilation
.PHONY: clean_objects
clean_objects:
	@echo "Cleaning up object files..."
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/*.d

# Client target
Client: $(BIN_DIR)/StompClient.o $(BIN_DIR)/ConnectionHandler.o $(BIN_DIR)/StompProtocol.o $(BIN_DIR)/ThreadSafeHashMap_future.o $(BIN_DIR)/keyboardInput.o $(BIN_DIR)/OutputHandler.o $(BIN_DIR)/Frame.o $(BIN_DIR)/ThreadSafeQueue.o $(BIN_DIR)/DataHandler.o $(BIN_DIR)/event.o
	mkdir -p $(BIN_DIR)
	g++ -o $(BIN_DIR)/Client $^ $(LDFLAGS)

# StompWCIClient target
StompWCIClient: $(BIN_DIR)/ConnectionHandler.o $(BIN_DIR)/StompClient.o $(BIN_DIR)/event.o $(BIN_DIR)/ThreadSafeQueue.o $(BIN_DIR)/Frame.o $(BIN_DIR)/StompProtocol.o $(BIN_DIR)/ThreadSafeHashMap_future.o $(BIN_DIR)/keyboardInput.o $(BIN_DIR)/OutputHandler.o $(BIN_DIR)/DataHandler.o
	mkdir -p $(BIN_DIR)
	g++ -o $(BIN_DIR)/StompWCIClient $^ $(LDFLAGS)

# Generic rule to compile .cpp files into .o files
$(BIN_DIR)/%.o: src/%.cpp
	mkdir -p $(BIN_DIR)
	g++ $(CFLAGS) -MMD -MF $(@:.o=.d) -o $@ $<

# Include generated dependency files
-include $(BIN_DIR)/*.d

# Clean up all build files (bin directory)
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
