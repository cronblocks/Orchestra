################################################################################
# Project:    Orchestra
# Author:     Usama Mahmood
# Created On: 12 July, 2021
#
#
#
################################################################################

GCC    = gcc
GXX    = g++
LDGCC  = gcc
LD     = $(LDGCC)

#######################################
# Defining Paths
#######################################
APP_EXE        = orchestra
SRC_ROOT_DIR   = $(PWD)/src
BUILD_ROOT_DIR = $(PWD)/build
APP_EXE_DIR    = $(PWD)

CORE_ROOT_DIR                         = Core
CORE_CONFIGURATION_DIR                = $(CORE_ROOT_DIR)/Configuration
CORE_CONNECTIVITY_DIR                 = $(CORE_ROOT_DIR)/Connectivity
CORE_CRYPTO_DIR                       = $(CORE_ROOT_DIR)/Crypto
CORE_EXECUTION_MODULES_MANAGEMENT_DIR = $(CORE_ROOT_DIR)/ExecutionModulesManagement
CORE_ORCHESTRATOR_DIR                 = $(CORE_ROOT_DIR)/Orchestrator
EXECUTION_MODULES_ROOT_DIR            = ExecutionModules
EXECUTION_MODULES_LINUX_DIR           = $(EXECUTION_MODULES_ROOT_DIR)/Linux

APP_SRCS_DIRS =                              \
    $(CORE_ROOT_DIR)                         \
    $(CORE_CONFIGURATION_DIR)                \
    $(CORE_CONNECTIVITY_DIR)                 \
    $(CORE_CRYPTO_DIR)                       \
    $(CORE_EXECUTION_MODULES_MANAGEMENT_DIR) \
    $(CORE_ORCHESTRATOR_DIR)                 \
    $(EXECUTION_MODULES_ROOT_DIR)            \
    $(EXECUTION_MODULES_LINUX_DIR)

INCLUDE_DIRS := $(foreach dir, $(APP_SRCS_DIRS), -I$(SRC_ROOT_DIR)/$(dir))

#######################################
# Creating Output Directories
#######################################
$(shell [ -d $(BUILD_ROOT_DIR) ] || mkdir $(BUILD_ROOT_DIR))
$(foreach dir, $(APP_SRCS_DIRS), $(shell [ -d $(BUILD_ROOT_DIR)/$(dir) ] || mkdir $(BUILD_ROOT_DIR)/$(dir)))

#######################################
# Defining Object Files
#######################################
SRCS_C   := $(foreach dir, $(APP_SRCS_DIRS), $(shell ls $(SRC_ROOT_DIR)/$(dir)/*.c 2>/dev/null | sed "s|.*/|$(SRC_ROOT_DIR)/$(dir)/|g"))
SRCS_CPP := $(foreach dir, $(APP_SRCS_DIRS), $(shell ls $(SRC_ROOT_DIR)/$(dir)/*.cpp 2>/dev/null | sed "s|.*/|$(SRC_ROOT_DIR)/$(dir)/|g"))

OBJS_C   := $(foreach file, $(SRCS_C:c=o), $(shell echo $(file) | sed "s|$(SRC_ROOT_DIR)/|$(BUILD_ROOT_DIR)/|g"))
OBJS_CPP := $(foreach file, $(SRCS_CPP:cpp=o), $(shell echo $(file) | sed "s|$(SRC_ROOT_DIR)/|$(BUILD_ROOT_DIR)/|g"))

APP_OBJECTS := $(OBJS_C) $(OBJS_CPP)

default: application

################################################################################
#    Defining Compilation Rules for App and Drivers                            #
################################################################################
APP_C_FLAGS          = -fPIC -Wall -std=c11 $(INCLUDE_DIRS) -lpthread
APP_CPP_FLAGS        = -Wall -std=c++11 $(INCLUDE_DIRS) -lpthread
APP_LINKING_FLAGS    = -Wall -std=c++11 $(INCLUDE_DIRS) -lpthread

#######################################
# Rules for ".c"
#######################################
$(BUILD_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONFIGURATION_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONFIGURATION_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.o: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

#######################################
# Rules for ".cpp"
#######################################
$(BUILD_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_ROOT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONFIGURATION_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONFIGURATION_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.o: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

################################################################################
#    Building Application                                                      #
################################################################################
application: $(APP_OBJECTS)
	@$(LDGCC) -o $(APP_EXE_DIR)/$(APP_EXE) $(APP_OBJECTS) $(APP_LINKING_FLAGS)
	@echo "*****************************************************************"
	@echo "*** Build Complete"
	@echo "*****************************************************************"
	@echo "**********************************************************"

################################################################################
#    Cleaning Solution                                                         #
################################################################################
clean:
	$(foreach dir, $(APP_SRCS_DIRS), $(shell rm -f $(BUILD_ROOT_DIR)/$(dir)/*.o))
	@rm -f $(APP_EXE_DIR)/$(APP_EXE)
	@echo "Project cleaned ..."

################################################################################
#                            ----------------------                            #
################################################################################
