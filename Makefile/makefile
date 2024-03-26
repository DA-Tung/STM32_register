
ifeq ($(MODULE),)
    $(error export MODULE before starting test : exmple export MODULE= gpio, ...)
endif

ifeq ($(TEST),)
    $(error export TEST before starting test : exmple export TEST= TS_0xx, ...)
endif 

include ../test/gpio_test/make/$(TEST)

PRO_DIR     := .
PATH_OUTPUT := ../output
PROJ_NAME   := $(MODULE)

# path to all include folder
INCLUDE_DIR += $(PRO_DIR)/include

# path to all source  folder
SRC_DIRS += $(PRO_DIR)/source

# File startup
# STARTUP_DIR = $(PRO_DIR)/startup_file

# search  all file in source folder
SRC_FILES     := $(foreach SRC_DIRS,$(SRC_DIRS),$(wildcard $(SRC_DIRS)/*))

# search  all file in include folder
INCLUDE_FILES := $(foreach INCLUDE_DIR,$(INCLUDE_DIR),$(wildcard $(INCLUDE_DIR)/*))

# path to linker file microchip 
LINKER_FILE   := $(PRO_DIR)/linker/STM32F407VGTx_FLASH.ld

# path to linker folder
COMPILER_DIR  := D:/KEIL_C/gcc_arm/linker

#path to compiler
CC			  := $(COMPILER_DIR)/bin/arm-none-eabi-gcc
# ASM			  := $(COMPILER_DIR)/bin/arm-none-eabi-as
LD			  := $(COMPILER_DIR)/bin/arm-none-eabi-ld

INCLUDE_DIRS_OTP := $(foreach INCLUDE_DIR,$(INCLUDE_DIR),-I$(INCLUDE_DIR))

# FILE_TO_LINK	:= $(PATH_OUTPUT)/startup_stm32f407xx.o

#complier option
	# -mcpu=cortex-m4 : select core for microcontroller
	# -mthumb : select script Thumb
	# - o0 : select level optimize
CC_OPT		  := -mcpu=cortex-m4 -c -O0 -g -mthumb $(INCLUDE_DIRS_OTP)
# ASM_OPT		  := -mcpu=cortex-m4 -c -mthumb
	# -Map : get log Memory map
LD_OPT        := -T $(LINKER_FILE) -Map $(PATH_OUTPUT)/$(PROJ_NAME).map

# package all file in source folder
OJB_FILES     := $(notdir $(SRC_FILES))

# convert file .c to .o
OJB_FILES     := $(subst .c,.o,$(OJB_FILES))

# search all file
PATH_OJB      := $(foreach OJB_FILES,$(OJB_FILES),$(PATH_OUTPUT)/$(OJB_FILES))

# package all file .c in source folder
vpath %.c $(SRC_DIRS)

# package all file .h in include folder
vpath %.h $(INCLUDE_DIR)

build: $(OJB_FILES) $(LINKER_FILE)
	# create file elf
	$(LD) $(LD_OPT) $(PATH_OJB) -o $(PATH_OUTPUT)/$(PROJ_NAME).elf
	
	# objcopy.exe convert elf to hex or hex to elf ...
	$(COMPILER_DIR)/arm-none-eabi/bin/objcopy.exe -O ihex "$(PATH_OUTPUT)/$(PROJ_NAME).elf" "$(PATH_OUTPUT)/$(PROJ_NAME).hex"
	
	# cacular size elf
	size $(PATH_OUTPUT)/$(PROJ_NAME).elf

# build2 : $(FILE_TO_LINK) $(LINKER_FILE)
	# $(LD) $(LD_OPT) -o $(PATH_OUTPUT)/$(PROJ_NAME).elf

# $(PATH_OUTPUT)/startup_stm32f407xx.o : $(STARTUP_DIR)/startup_stm32f407xx.s
	# $(ASM) $(ASM_OPT) $(STARTUP_DIR)/startup_stm32f407xx.s -o $(PATH_OUTPUT)/startup_stm32f407xx.o
	
%.o: %.c $(INCLUDE_FILES)
	# $(CC) : build file C
	$(CC) $(CC_OPT) -c $< -o $(PATH_OUTPUT)/$@
	
.PHONY: run	
run:
	$(PRO_DIR)/ST-LINK_Utility/ST-LINK_CLI.exe -p "../output/$(PROJ_NAME).hex" 0x08000000
	$(PRO_DIR)/ST-LINK_Utility/ST-LINK_CLI.exe -rst
	# $(PRO_DIR)/ST-LINK_Utility/ST-LINK_CLI.exe -r32 0x08000000 4 | grep -i B083B480 > $(PATH_OUTPUT)/result.log
	
# .PHONY: report	
# report:	
	# if [[ -s $(PATH_OUTPUT)/result.log ]]; then \
		# echo "Test $(TEST) Passed"; \
	# else \
		# echo "Test $(TEST) Failed"; \
	# fi
	
.PHONY: inform	
inform:
	@ls ../test/$(MODULE)_test/make
	
.PHONY: clean	
clean:
	# rm -rf $(PATH_OUTPUT)
	rm $(PATH_OUTPUT)/*
	
print-%:
	@echo $($(subst print-,,$@))