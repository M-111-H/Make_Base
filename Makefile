TARGET = Basemake

# 存放构建时生成的文件的目录
BUILD_DIR = build

# ARM GCC 工具链
TOOLCHAIN  = arm-none-eabi-
CC  := $(TOOLCHAIN)gcc
CP  := $(TOOLCHAIN)objcopy
AS  := $(TOOLCHAIN)gcc -x assembler-with-cpp
SZ  := $(TOOLCHAIN)size
HEX := $(CP) -O ihex
BIN := $(CP) -O binary -S

########################################### 标准库源码 #################################################
ASM_SRC := ./Libraries/CMSIS/Start_up/startup_stm32f10x_hd.s
C_SRC   := ./Libraries/CMSIS/CoreSupport/core_cm3.c
C_SRC   += ./Libraries/CMSIS/DeviceSupport/system_stm32f10x.c
C_SRC   += $(wildcard ./Libraries/STM32F10x_FWLib/src/*.c)
############################################ 用户源码 ##################################################
C_SRC += $(wildcard ./User/*.c)
C_SRC += $(wildcard ./HARDWARE/LED/*.c)
C_SRC += $(wildcard ./FreeRTOS/*.c)
C_SRC += $(wildcard ./FreeRTOS/portable/GCC/ARM_CM3/*.c)
C_SRC += $(wildcard ./FreeRTOS/portable/MemMang/*.c)
C_SRC += $(wildcard ./HARDWARE/LED/*.c)
C_SRC += $(wildcard ./SYSTEM/delay/*.c)
C_SRC += $(wildcard ./SYSTEM/sys/*.c)
C_SRC += $(wildcard ./SYSTEM/usart/*.c)
########################################### 头文件路径 #################################################
INC_DIR := ./Libraries/CMSIS/CoreSupport/
INC_DIR += ./Libraries/CMSIS/DeviceSupport/
INC_DIR += ./Libraries/STM32F10x_FWLib/inc/
INC_DIR += ./User/
INC_DIR += ./HARDWARE/LED/
INC_DIR += ./FreeRTOS/include/
INC_DIR += ./FreeRTOS/portable/GCC/ARM_CM3/
INC_DIR += ./SYSTEM/delay/
INC_DIR += ./SYSTEM/sys/
INC_DIR += ./SYSTEM/usart/


# 内核架构
CPU := cortex-m3
# 预定义宏
DEFS := -D STM32F10X_HD  -D USE_STDPERIPH_DRIVER
# 链接脚本
LINK_SCRIPT := ./STM32F103ZETx_FLASH.ld
# 编译参数
CP_FLAGS := $(DEFS) $(addprefix -I,$(INC_DIR)) -mcpu=$(CPU) -g -gdwarf-2 -mthumb -Os -Wall -Wextra -fdata-sections -ffunction-sections -fmessage-length=0 -MMD -MP
# 汇编参数
AS_FLAGS := $(CP_FLAGS)
# 链接参数
LD_FLAGS := -mcpu=$(CPU) -T $(LINK_SCRIPT) -specs=nano.specs --specs=nosys.specs -Wl,--no-warn-rwx-segments,-Map=$(BUILD_DIR)/$(TARGET).map,--cref,--gc-sections,--print-memory-usage




# 设置文件搜索路径，.h 文件路径由 gcc -I 参数指定
vpath %.c $(sort $(dir $(C_SRC)))
vpath %.s $(sort $(dir $(ASM_SRC)))
# 保留 .o 文件，make 可能会删除生成的 .o 文件
.PRECIOUS: $(BUILD_DIR)/%.o




.PHONY: all
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

%.elf: $(addprefix $(BUILD_DIR)/,$(notdir $(C_SRC:.c=.o))) $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SRC:.s=.o)))
	$(CC) $(LD_FLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $(CP_FLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s
	$(AS) -c $(AS_FLAGS) $< -o $@

%.hex: %.elf
	$(HEX) $< $@

%.bin: %.elf
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@




clean:
	rm $(BUILD_DIR)/ -rf

st-flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash --format binary write $(BUILD_DIR)/$(TARGET).bin 0x8000000

st-erase:
	st-flash erase

isp-flash: $(BUILD_DIR)/$(TARGET).bin
	stm32flash -b 115200 -i '-dtr&rts,dtr:-dtr&-rts,dtr' -v -w $(BUILD_DIR)/$(TARGET).bin /dev/ttyUSB0

isp-erase:
	stm32flash -b 115200 -i '-dtr&rts,dtr:-dtr&-rts,dtr' -o /dev/ttyUSB0

isp-info:
	stm32flash -b 115200 -i '-dtr&rts,dtr:-dtr&-rts,dtr' /dev/ttyUSB0

rebuild: clean all

install: st-flash




# 包含由 gcc -MMD 参数生成的 .d 文件
-include $(BUILD_DIR)/*.d