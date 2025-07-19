CXX = g++
CXXFLAGS = -std=c++11 -Wall -fPIC
LDFLAGS = -L/usr/lib/x86_64-linux-gnu
LDLIBS = -lmariadbcpp
INCLUDE_PATHS = -I/usr/include

BUILD_DIR = build

SRCS = \
	Main.cpp \
	Controller.cpp \
	Menu.cpp \
	Carteira.cpp \
	Date.cpp \
	Entry.cpp \
	EntryDBDAO.cpp \
	InMemoryEntryDAO.cpp \
	ServerDBConnection.cpp \
	SistemaFTCoin.cpp \
	SysInfo.cpp \
	TextFromFile.cpp \
	Utils.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

TARGET = ftcoin

.PHONY: all clean $(BUILD_DIR)

all: $(BUILD_DIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)