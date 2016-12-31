#=======================================================#
#														#
#	PIC C30 Makefile 				      				#
#														#
#=======================================================#
TARGET		=	farm

DEVICE		=	24EP256GU810

BUILD		=	release

VPATH		=

ASOURCES	=

CSOURCES	=	main.c \
				ck4_comm.c \
				ck4_fpga.c \
				ck4_gpio.c \
				ck4_gvar.c \
				ck4_i2c.c \
				ck4_serial.c \
				ck4_timer.c \
				ck4_uart.c

PSOURCES	=

USER_LIBS	=

LDSCRIPT	=

USER_DEFS	=

INC_SYS		=

INC_APP		=

OPTIMIZE	=	-O1 -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data

MCU_TARGET  =   -mcpu=$(DEVICE)

CC_OPT		=	-Wall -Werror \
				-Wno-unused-variable \
				-fno-exceptions
#CC_OPT		=	-x c

CP_OPT		=	-Wall -Werror \
				-Wno-unused-variable \
				-Wno-unused-function \
				-fno-exceptions

SYSINCS		=	$(addprefix -I, $(INC_SYS))
APPINCS		=	$(addprefix -I, $(INC_APP))
AINCS		=	$(SYSINCS) $(APPINCS)
CINCS		=	$(SYSINCS) $(APPINCS)
PINCS		=	$(SYSINCS) $(APPINCS)
LIBINCS		=	$(addprefix -L, $(LIB_ROOT))
DEFS		=	$(addprefix -D, $(USER_DEFS))
LIBS		=	$(addprefix -l, $(USER_LIBS))

# You should not have to change anything below here.
AS			=	pic30-as
CC			=	pic30-gcc
CP			=
AR			=	pic30-ar
LD			=	pic30-ld
OBJCOPY		=
OBJDUMP		=	pic30-objdump
SIZE		=
HEX			=   pic30-bin2hex

# AFLAGS        = -Wa,-adhlns=$(<:.s=.lst),-gstabs
# AFLAGS        =	-Wa,-adhlns=$(<:.s=.lst)
# ALL_ASFLAGS    = -x assembler-with-cpp $(ASFLAGS) $(DEFS)
ALL_ASFLAGS    = $(AFLAGS) $(MCU_TARGET) $(DEFS)

# Override is only needed by avr-lib build system.

CFLAGS		=	$(CC_OPT) $(OPTIMIZE) $(MCU_TARGET) $(DEFS)
PFLAGS		=	-std=c++14 $(CP_OPT) $(OPTIMIZE) $(MCU_TARGET) $(DEFS)

# override LDFLAGS = $(MCU_TARGET) -nostartfiles -Wl,-Map,$(TARGET).map -T $(LDSCRIPT)
# override LDFLAGS = $(MCU_TARGET) -T$(DEVICE).gld,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--heap=1024,--stack=1024,-Map="VTS-10a.map"
override LDFLAGS = $(MCU_TARGET) -T p$(DEVICE).gld --heap=1024 --stack=1024

OBJCOPY_OPT	=	--srec-forceS3 --srec-len 32

OBJECTS	=	$(addprefix $(BUILD)/,$(patsubst %.s,%.o,$(ASOURCES))) \
			$(addprefix $(BUILD)/,$(patsubst %.c,%.o,$(CSOURCES))) \
			$(addprefix $(BUILD)/,$(patsubst %.cpp,%.o,$(PSOURCES)))

DOBJECTS =	$(addprefix $(BUILD)/,$(patsubst %.c,%.o,$(CSOURCES))) \
			$(addprefix $(BUILD)/,$(patsubst %.cpp,%.o,$(PSOURCES)))

DEPENDS =   $(patsubst %.o,%.d, $(DOBJECTS))

.PHONY: all clean
.SUFFIXES :
.SUFFIXES : .rc .hpp .s .h .c .cpp .d .o

all: $(BUILD) $(TARGET).elf

$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(LDFLAGS) $(LIBINCS) -o $@ $(OBJECTS) $(LIBS)
	$(HEX) $@ $(TARGET).hex

$(BUILD)/%.o: %.s
	mkdir -p $(dir $@); \
	$(AS) -c $(AOPT) $(AFLAGS) $(AINCS) -o $@ $<

$(BUILD)/%.o : %.c
	mkdir -p $(dir $@); \
	$(CC) -c $(COPT) $(CFLAGS) $(CINCS) $(CCWARN) -o $@ $<

$(BUILD)/%.o : %.cpp
	mkdir -p $(dir $@); \
	$(CP) -c $(POPT) $(PFLAGS) $(PINCS) $(CPWARN) -o $@ $<

$(BUILD)/%.d: %.c
	mkdir -p $(dir $@); \
	$(CC) -MM -DDEPEND_ESCAPE $(COPT) $(CFLAGS) $(APPINCS) $< \
	| sed 's/$(notdir $*)\.o:/$(subst /,\/,$(patsubst %.d,%.o,$@) $@):/' > $@ ; \
	[ -s $@ ] || rm -f $@

$(BUILD)/%.d: %.cpp
	mkdir -p $(dir $@); \
	$(CP) -MM -DDEPEND_ESCAPE $(POPT) $(PFLAGS) $(APPINCS) $< \
	| sed 's/$(notdir $*)\.o:/$(subst /,\/,$(patsubst %.d,%.o,$@) $@):/' > $@ ; \
	[ -s $@ ] || rm -f $@

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).mot $(TARGET).lst $(TARGET).map

clean_depend:
	rm -f $(DEPENDS)

lst:  $(TARGET).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: mot lst

bin: $(TARGET).bin
mot: $(TARGET).mot
lst: $(TARGET).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.mot: %.elf
	$(OBJCOPY) $(OBJCOPY_OPT) -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
#	$(OBJCOPY) -j .vects -j .text -j .data -O binary $< $@

tarball:
	tar cfvz $(subst .elf,,$(TARGET))_$(shell date +%Y%m%d%H).tgz \
	*.[hc]pp Makefile ../common/*/*.[hc]pp ../common/*/*.[hc]

bin_zip:
	$(LK) $(LFLAGS) $(LIBS) $(OBJECTS) $(ICON_OBJ) $(LIBN) -mwindows -o $(TARGET) 
	rm -f $(subst .exe,,$(TARGET))_$(shell date +%Y%m%d%H)_bin.zip
	zip $(subst .exe,,$(TARGET))_$(shell date +%Y%m%d%H)_bin.zip *.exe *.dll res/*.*

run:
	rx_prog -d RX24T --progress --write --verify $(TARGET).mot

-include $(DEPENDS)
