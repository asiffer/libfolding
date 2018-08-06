### libfolding makefile
#
# @author = asr

### INSTALL DIRECTORIES
DESTDIR =
###


VERSION = 1.0

# Current directory
CURDIR = $(realpath .)

# folder of the headers
INC_DIR = $(CURDIR)/include

# folder of the sources
SRC_DIR = $(CURDIR)/src

#folder of the built sources
OBJ_DIR = $(CURDIR)/build

#folder of the shared library
LIB_DIR = $(CURDIR)/lib

#folder for the testspot
TEST_DIR = $(CURDIR)/test


###
INSTALL_HEAD_DIR = $(DESTDIR)/usr/include/libfolding
INSTALL_LIB_DIR = $(DESTDIR)/usr/lib
###

EXPORT = @export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIB_DIR)

# compiler & flags
CXX = @c++
CXXFLAGS = -std=c++11 -Wall -pedantic
LFLAGS = -L/usr/lib -L/usr/lib/x86_64-linux-gnu -larmadillo

# all the files (header, sources, build)
FILES = pivot.h armaring.h streamfolding.h folding.h
DEPS = $(foreach n,$(FILES),$(INC_DIR)/$(n))
SRCS = $(foreach n,$(FILES:.h=.cpp),$(SRC_DIR)/$(n)) $(SRC_DIR)/interface.cpp
OBJS = $(FILES:.h=.o) interface.o


# library file
TARGET = libfolding.so


### MAKEFILE TARGETS
all: checkdir $(TARGET)

# create lib/ and build/ directories
checkdir:
	@echo
	@echo "==== libfolding" $(VERSION) "===="
	@echo
	@echo "Checking the library directory ("$(LIB_DIR)")"
	@mkdir -p $(LIB_DIR)
	@echo "Checking the object directory ("$(OBJ_DIR)")"
	@mkdir -p $(OBJ_DIR)
	@echo
	@echo "[Building sources]"

# create the shared library
$(TARGET): $(OBJS)
	@echo
	@echo "[Building library]"
	@echo "Building" $@ "..."
	$(CXX) $(CXXFLAGS) -shared $(foreach n,$^,$(OBJ_DIR)/$(n)) -o $(LIB_DIR)/$@ -fPIC $(LFLAGS);
	@echo "[done]"

# build source files
%.o: $(SRC_DIR)/%.cpp
	@echo "Building" $@ "..."
	$(CXX) $(CXXFLAGS) $(EXT_INC_DIR) -I $(INC_DIR) -c $< -o $(OBJ_DIR)/$@ -fPIC $(LFLAGS)


# test programs
%.run: $(TEST_DIR)/%.cpp
	@echo "Building test $@ ..."
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -L$(LIB_DIR) -o $(TEST_DIR)/$@ $< -lfolding $(LFLAGS)
	@echo "Running test ..."
	$(EXPORT); $(TEST_DIR)/$@



## INSTALL
install:
	@echo "Checking the headers installation directory ("$(INSTALL_HEAD_DIR)")"
	@mkdir -p $(INSTALL_HEAD_DIR)
	@echo "Checking the library installation directory ("$(INSTALL_LIB_DIR)")"
	@mkdir -p $(INSTALL_LIB_DIR)
	@echo "Installing the shared library ("$(TARGET)")"
	@install $(LIB_DIR)/*.so $(INSTALL_LIB_DIR)
	@echo "Installing the headers"
	@install $(INC_DIR)/*.h $(INSTALL_HEAD_DIR) 
	@echo "[done]"

test_header:
	@echo
	@echo "[Testing]"


test: exp_uni.run norm_uni.run mvnorm_uni.run norm_multi.run perf.run drift.run

clean:
	@rm -rfd $(OBJ_DIR)
	@rm -rfd $(LIB_DIR)
	@rm -rf $(TEST_DIR)/*.run


