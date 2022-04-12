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
APP_EXE          = orchestra
SRC_ROOT_DIR     = $(PWD)/src
BUILD_ROOT_DIR   = $(PWD)/build
APP_EXE_DIR      = $(PWD)
CONFIG_FILES_DIR = $(SRC_ROOT_DIR)/ConfigurationFiles

INSTALL_EXE_DIR  = /usr/bin/
INSTALL_CONF_DIR = /etc/$(APP_EXE)

CORE_ROOT_DIR                         = Core
CORE_COMMON_DIR                       = $(CORE_ROOT_DIR)/Common
CORE_CONNECTIVITY_DIR                 = $(CORE_ROOT_DIR)/Connectivity
CORE_CRYPTO_DIR                       = $(CORE_ROOT_DIR)/Crypto
CORE_EXECUTION_MODULES_MANAGEMENT_DIR = $(CORE_ROOT_DIR)/ExecutionModulesManagement
CORE_ORCHESTRATOR_DIR                 = $(CORE_ROOT_DIR)/Orchestrator
EXECUTION_MODULES_ROOT_DIR            = ExecutionModules
EXECUTION_MODULES_LINUX_DIR           = $(EXECUTION_MODULES_ROOT_DIR)/Linux

APP_SRCS_DIRS =                              \
    .                                        \
    $(CORE_ROOT_DIR)                         \
    $(CORE_COMMON_DIR)                       \
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
SRCS_C   := $(foreach dir, $(APP_SRCS_DIRS), $(shell ls $(SRC_ROOT_DIR)/$(dir)/*.c   2>/dev/null | sed "s|.*/|$(SRC_ROOT_DIR)/$(dir)/|g"))
SRCS_CPP := $(foreach dir, $(APP_SRCS_DIRS), $(shell ls $(SRC_ROOT_DIR)/$(dir)/*.cpp 2>/dev/null | sed "s|.*/|$(SRC_ROOT_DIR)/$(dir)/|g"))

OBJS_C   := $(foreach file, $(SRCS_C:c=o),     $(shell echo $(file) | sed "s|$(SRC_ROOT_DIR)/|$(BUILD_ROOT_DIR)/|g"))
OBJS_CPP := $(foreach file, $(SRCS_CPP:cpp=O), $(shell echo $(file) | sed "s|$(SRC_ROOT_DIR)/|$(BUILD_ROOT_DIR)/|g"))

APP_OBJECTS := $(OBJS_C) $(OBJS_CPP)

default: application

################################################################################
#    Defining Compilation Rules for App and Drivers                            #
################################################################################
APP_C_FLAGS          = -fPIC -Wall -std=c11 $(INCLUDE_DIRS) -lpthread
APP_CPP_FLAGS        = -Wall -std=c++11 $(INCLUDE_DIRS) -lpthread
APP_LINKING_FLAGS    = -Wall -std=c++11 $(INCLUDE_DIRS) -lpthread

#######################################
# Rules for ".c" and ".cpp"
#######################################
# SourceRoot/
$(BUILD_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/%.O: $(SRC_ROOT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/Core/
$(BUILD_ROOT_DIR)/$(CORE_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_ROOT_DIR)/%.O: $(SRC_ROOT_DIR)/$(CORE_ROOT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/Core/Common/
$(BUILD_ROOT_DIR)/$(CORE_COMMON_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_COMMON_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_COMMON_DIR)/%.O: $(SRC_ROOT_DIR)/$(CORE_COMMON_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/Core/Connectivity/
$(BUILD_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.O: $(SRC_ROOT_DIR)/$(CORE_CONNECTIVITY_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/Core/Crypto/
$(BUILD_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.O: $(SRC_ROOT_DIR)/$(CORE_CRYPTO_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/Core/ExecutionModulesManagement/
$(BUILD_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.O: $(SRC_ROOT_DIR)/$(CORE_EXECUTION_MODULES_MANAGEMENT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/Core/Orchestrator/
$(BUILD_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.o: $(SRC_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.O: $(SRC_ROOT_DIR)/$(CORE_ORCHESTRATOR_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/ExecutionModules/
$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.o: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.O: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_ROOT_DIR)/%.cpp
	@echo [Compiling] $<
	@$(GXX) -c $(APP_CPP_FLAGS) $< -o $@

# SourceRoot/ExecutionModules/Linux/
$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.o: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.c
	@echo [Compiling] $<
	@$(GCC) -c $(APP_C_FLAGS) $< -o $@

$(BUILD_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.O: $(SRC_ROOT_DIR)/$(EXECUTION_MODULES_LINUX_DIR)/%.cpp
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
#    Installing Application                                                    #
################################################################################
install:
	@echo "[Installing Orchestra]"
	@echo ""

	@echo "Installing Executables ..."
	@if [ -f $(APP_EXE_DIR)/$(APP_EXE) ];                       \
	then                                                        \
		sudo cp -f $(APP_EXE_DIR)/$(APP_EXE) $(INSTALL_EXE_DIR); \
	else                                                        \
		echo "Please build the project first";                  \
	fi

	@echo "Copying Configuration Files ..."
	$(shell [ -d $(INSTALL_CONF_DIR) ] || sudo mkdir $(INSTALL_CONF_DIR))
	@sudo cp -f $(CONFIG_FILES_DIR)/*.conf $(INSTALL_CONF_DIR)

	@echo "Completed"

uninstall:
	@echo "[Uninstalling Orchestra]"
	@echo ""

	@echo "Removing Executables ..."
	@sudo rm -f $(INSTALL_EXE_DIR)/$(APP_EXE);

	@echo "Removing Configuration Files ..."
	@sudo rm -rf $(INSTALL_CONF_DIR)

	@echo "Completed"

################################################################################
#                            ----------------------                            #
################################################################################
