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

CORE_ROOT_DIR                   = Core
CORE_CONFIGURATION_PROVIDER_DIR = $(CORE_ROOT_DIR)/ConfigurationProvider
CORE_CONNECTION_MANAGER_DIR     = $(CORE_ROOT_DIR)/ConnectionManager
CORE_CRYPTO_DIR                 = $(CORE_ROOT_DIR)/Crypto
CORE_MODULES_MANAGER_DIR        = $(CORE_ROOT_DIR)/ModulesManager
CORE_ORCHESTRATOR_DIR           = $(CORE_ROOT_DIR)/Orchestrator
EXECUTION_MODULES_ROOT_DIR      = ExecutionModules
EXECUTION_MODULES_LINUX_DIR     = $(EXECUTION_MODULES_ROOT_DIR)/Linux

APP_SRCS_DIRS =                                     \
                $(CORE_ROOT_DIR)/                   \
                $(CORE_CONFIGURATION_PROVIDER_DIR)/ \
                $(CORE_CONNECTION_MANAGER_DIR)/     \
                $(CORE_CRYPTO_DIR)/                 \
                $(CORE_MODULES_MANAGER_DIR)/        \
                $(CORE_ORCHESTRATOR_DIR)/           \
				$(EXECUTION_MODULES_ROOT_DIR)/      \
                $(EXECUTION_MODULES_LINUX_DIR)/

INCLUDE_DIRS := $(foreach dir, $(APP_SRCS_DIRS), -I$(SRC_ROOT_DIR)/$(dir))

#######################################
# Creating Output Directories
#######################################
$(shell [ -d $(BUILD_ROOT_DIR) ] || mkdir $(BUILD_ROOT_DIR))
$(foreach dir, $(APP_SRCS_DIRS), $(shell [ -d $(BUILD_ROOT_DIR)/$(dir) ] || mkdir $(BUILD_ROOT_DIR)/$(dir)))

#######################################
# Defining Object Files
#######################################
SRCS_C   := $(foreach dir, $(APP_SRCS_DIRS), $(shell ls $(dir)/*.c 2>/dev/null | sed "s/.*[/]/$(dir)\//"))
SRCS_CPP := $(foreach dir, $(APP_SRCS_DIRS), $(shell ls $(dir)/*.cpp 2>/dev/null | sed "s/.*[/]/$(dir)\//"))

OBJS_C   := $(foreach file, $(SRCS_C:.c=.o), $(BUILD_ROOT_DIR)/$(file))
OBJS_CPP := $(foreach file, $(SRCS_CPP:.cpp=.o), $(BUILD_ROOT_DIR)/$(file))

APP_OBJECTS  = $(OBJS_C) $(OBJS_CPP)

default: application

################################################################################
#    Defining Compilation Rules for App and Drivers                            #
################################################################################
APP_C_FLAGS          = -fPIC -Wall -std=c11 $(INCLUDE_DIRS) -lpthread
APP_CPP_FLAGS        = -Wall -std=c++11 $(INCLUDE_DIRS) -lpthread

#######################################
# Rules for ".c"
#######################################
$(BUILD_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONFIGURATION_PROVIDER_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONFIGURATION_PROVIDER_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONNECTION_MANAGER_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONNECTION_MANAGER_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_MODULES_MANAGER_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_MODULES_MANAGER_DIR)/%.c
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

$(BUILD_ROOT_DIR)/$(CORE_CONFIGURATION_PROVIDER_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONFIGURATION_PROVIDER_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONNECTION_MANAGER_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONNECTION_MANAGER_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_MODULES_MANAGER_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_MODULES_MANAGER_DIR)/%.cpp
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
	@$(LDGCC) -o $(BUILD_ROOT_DIR)/$(APP_EXE) $(APP_OBJECTS) $(APP_CPP_FLAGS)
	@echo "*****************************************************************"
	@echo "*** Build Complete"
	@echo "*****************************************************************"
	@echo "**********************************************************"

################################################################################
#    Cleaning Solution                                                         #
################################################################################
clean:
	$(foreach dir, $(APP_SRCS_DIRS), $(shell rm -f $(BUILD_ROOT_DIR)/$(dir)/*.o))
	@rm -f $(BUILD_ROOT_DIR)/$(APP_EXE)
	@echo "Project cleaned ..."

################################################################################
#                            ----------------------                            #
################################################################################
