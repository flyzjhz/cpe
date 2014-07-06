ifeq (TRUE, $(IN_ACL))
  MODULE_CFLAG += -DIN_ACL
endif

ifeq (TRUE, $(IN_FDB))
  MODULE_CFLAG += -DIN_FDB
endif

ifeq (TRUE, $(IN_IGMP))
  MODULE_CFLAG += -DIN_IGMP
endif

ifeq (TRUE, $(IN_LEAKY))
  MODULE_CFLAG += -DIN_LEAKY
endif

ifeq (TRUE, $(IN_LED))
  MODULE_CFLAG += -DIN_LED
endif

ifeq (TRUE, $(IN_MIB))
  MODULE_CFLAG += -DIN_MIB
endif

ifeq (TRUE, $(IN_MIRROR))
  MODULE_CFLAG += -DIN_MIRROR
endif

ifeq (TRUE, $(IN_MISC))
  MODULE_CFLAG += -DIN_MISC
endif

ifeq (TRUE, $(IN_PORTCONTROL))
  MODULE_CFLAG += -DIN_PORTCONTROL
endif

ifeq (TRUE, $(IN_PORTVLAN))
  MODULE_CFLAG += -DIN_PORTVLAN
endif

ifeq (TRUE, $(IN_QOS))
  MODULE_CFLAG += -DIN_QOS
endif

ifeq (TRUE, $(IN_RATE))
  MODULE_CFLAG += -DIN_RATE
endif

ifeq (TRUE, $(IN_STP))
  MODULE_CFLAG += -DIN_STP
endif

ifeq (TRUE, $(IN_VLAN))
  MODULE_CFLAG += -DIN_VLAN
endif

ifeq (TRUE, $(IN_REDUCED_ACL))
  MODULE_CFLAG += -DIN_REDUCED_ACL
endif

ifeq (TRUE, $(IN_COSMAP))
  MODULE_CFLAG += -DIN_COSMAP
endif

ifeq (TRUE, $(IN_IP))
  MODULE_CFLAG += -DIN_IP
endif

ifeq (TRUE, $(IN_NAT))
  MODULE_CFLAG += -DIN_NAT
endif

ifeq (TRUE, $(IN_TRUNK))
  MODULE_CFLAG += -DIN_TRUNK
endif

ifeq (TRUE, $(IN_SEC))
  MODULE_CFLAG += -DIN_SEC
endif

ifeq (TRUE, $(IN_NAT_HELPER))
  MODULE_CFLAG += -DIN_NAT_HELPER
endif

ifeq (TRUE, $(IN_INTERFACECONTROL))
  MODULE_CFLAG += -DIN_INTERFACECONTROL
endif

ifneq (TRUE, $(FAL))
  MODULE_CFLAG += -DHSL_STANDALONG
endif

ifeq (TRUE, $(UK_IF))
  MODULE_CFLAG += -DUK_IF
endif

#ifdef UK_NL_PROT
  MODULE_CFLAG += -DUK_NL_PROT=$(UK_NL_PROT)
#endif

#ifdef UK_MINOR_DEV
  MODULE_CFLAG += -DUK_MINOR_DEV=$(UK_MINOR_DEV)
#endif

ifeq (TRUE, $(API_LOCK))
  MODULE_CFLAG += -DAPI_LOCK
endif

ifeq (TRUE, $(REG_ACCESS_SPEEDUP))
  MODULE_CFLAG += -DREG_ACCESS_SPEEDUP
endif

ifeq (TRUE, $(DEBUG_ON))
  MODULE_CFLAG += -g 
endif

MODULE_CFLAG += $(OPT_FLAG) -Wall -DVERSION=\"$(VERSION)\" -DBUILD_DATE=\"$(BUILD_DATE)\" -DCPU=\"$(CPU)\" -DOS=\"$(OS)\"

MODULE_INC += -I$(PRJ_PATH)/include \
                   -I$(PRJ_PATH)/include/common \
                   -I$(PRJ_PATH)/include/api \
                   -I$(PRJ_PATH)/include/fal \
                   -I$(PRJ_PATH)/include/hsl \
                   -I$(PRJ_PATH)/include/hsl/phy \
                   -I$(PRJ_PATH)/include/sal/os \
                   -I$(PRJ_PATH)/include/sal/sd \
                   -I$(PRJ_PATH)/include/sal/sd/linux/hydra_howl \
                   -I$(PRJ_PATH)/include/sal/sd/linux/uk_interface \
                   -I$(PRJ_PATH)/include/init

ifneq (,$(findstring ATHENA, $(SUPPORT_CHIP)))
  MODULE_INC   += -I$(PRJ_PATH)/include/hsl/athena
  MODULE_CFLAG += -DATHENA
endif

ifneq (,$(findstring GARUDA, $(SUPPORT_CHIP)))
  MODULE_INC   += -I$(PRJ_PATH)/include/hsl/garuda
  MODULE_CFLAG += -DGARUDA
endif

ifneq (,$(findstring SHIVA, $(SUPPORT_CHIP)))
  MODULE_INC   += -I$(PRJ_PATH)/include/hsl/shiva
  MODULE_CFLAG += -DSHIVA
endif

ifneq (,$(findstring HORUS, $(SUPPORT_CHIP)))
  MODULE_INC   += -I$(PRJ_PATH)/include/hsl/horus
  MODULE_CFLAG += -DHORUS
endif

ifneq (,$(findstring ISIS, $(SUPPORT_CHIP)))
  ifneq (ISISC, $(SUPPORT_CHIP))
     MODULE_INC   += -I$(PRJ_PATH)/include/hsl/isis
     MODULE_CFLAG += -DISIS
  endif
endif

ifneq (,$(findstring ISISC, $(SUPPORT_CHIP)))
  MODULE_INC   += -I$(PRJ_PATH)/include/hsl/isisc
  MODULE_CFLAG += -DISISC
endif

# check for GCC version
ifeq (4, $(GCC_VER))
  MODULE_CFLAG += -DGCCV4
endif

ifeq (KSLIB, $(MODULE_TYPE))
  ifeq (2_6, $(OS_VER))
	  MODULE_CFLAG += -DKVER26
          # check for Linux subversion
          LNX_GT_26_22 := $(shell [ $(OS_SUB) -ge 22 ] && echo TRUE)
          ifeq ($(LNX_GT_26_22), TRUE)
	    MODULE_CFLAG += -DLNX26_22
	    MODULE_INC += -I$(SYS_PATH) \
              -I$(SYS_PATH)/include \
              -I$(SYS_PATH)/arch/mips/include \
	      -I$(SYS_PATH)/arch/mips/include/asm/mach-ar7240 \
	      -I$(SYS_PATH)/arch/mips/include/asm/mach-generic \
	      -I$(SYS_PATH)/usr/include
	  else
	    MODULE_INC += -I$(SYS_PATH) \
              -I$(SYS_PATH)/include \
	      -I$(SYS_PATH)/include/asm/mach-generic
	  endif

	  ifeq (mips, $(CPU))
	    #CPU_CFLAG    = -G 0 -mno-abicalls -fno-pic -pipe -mabi=32 -march=mips32r2
	    ifndef CPU_CFLAG
	    CPU_CFLAG    = -Wstrict-prototypes -fomit-frame-pointer -G 0 -mno-abicalls -fno-strict-aliasing \
                     -O2 -fno-pic -pipe -mabi=32 -march=mips32r2 -DMODULE -mlong-calls -DEXPORT_SYMTAB
            endif
          endif
	  
	  
  else
	  MODULE_CFLAG += -DKVER24
	  ifeq (mips, $(CPU))
	    ifndef CPU_CFLAG
	    CPU_CFLAG     = -Wall -Wstrict-prototypes -Wall -Wundef -fomit-frame-pointer -G 0 -mno-abicalls -Wno-trigraphs \
	                    -fno-strict-aliasing -fno-common -ffreestanding -O2 -fno-pic -pipe -mabi=32 -march=r4600 -Wa,-32 -Wa,-march=r4600 -Wa,--trap -DMODULE -mlong-calls -DEXPORT_SYMTAB
	    endif
	  endif
	  
	  MODULE_INC += -I$(SYS_PATH) \
              -I$(SYS_PATH)/include \
	      -I$(SYS_PATH)/include/asm/mach-generic
  endif

  MODULE_CFLAG += -D__KERNEL__ -DKERNEL_MODULE $(CPU_CFLAG)

  
endif

ifeq (SHELL, $(MODULE_TYPE))
  MODULE_INC += -I$(PRJ_PATH)/include/shell

    ifeq (2_6, $(OS_VER))
  	  MODULE_CFLAG += -DKVER26
    else
   	  MODULE_CFLAG += -DKVER24
    endif
  
    ifeq (TRUE, $(KERNEL_MODE))
      MODULE_CFLAG += -static
    else
      MODULE_CFLAG += -static -DUSER_MODE
    endif
endif

ifneq (TRUE, $(KERNEL_MODE))
  ifneq (SHELL, $(MODULE_TYPE))
    MODULE_CFLAG +=  -DUSER_MODE
  endif
endif

CFLAGS += $(MODULE_INC) $(MODULE_CFLAG)
