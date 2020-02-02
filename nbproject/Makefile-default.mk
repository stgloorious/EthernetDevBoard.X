#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=--mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/enc424j600/ENC424J600.c src/eth/ethernet.c src/eth/mac.c src/mem/memoryController.c src/stack/backgroundTasks.c src/stack/stack.c src/system/main.c src/system/sevenseg.c src/system/uart.c src/stack/protocols/arp.c src/stack/protocols/ipv4.c src/stack/time.c src/stack/protocols/icmp.c src/stack/protocols/tcp.c src/stack/protocols/udp.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/enc424j600/ENC424J600.p1 ${OBJECTDIR}/src/eth/ethernet.p1 ${OBJECTDIR}/src/eth/mac.p1 ${OBJECTDIR}/src/mem/memoryController.p1 ${OBJECTDIR}/src/stack/backgroundTasks.p1 ${OBJECTDIR}/src/stack/stack.p1 ${OBJECTDIR}/src/system/main.p1 ${OBJECTDIR}/src/system/sevenseg.p1 ${OBJECTDIR}/src/system/uart.p1 ${OBJECTDIR}/src/stack/protocols/arp.p1 ${OBJECTDIR}/src/stack/protocols/ipv4.p1 ${OBJECTDIR}/src/stack/time.p1 ${OBJECTDIR}/src/stack/protocols/icmp.p1 ${OBJECTDIR}/src/stack/protocols/tcp.p1 ${OBJECTDIR}/src/stack/protocols/udp.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/src/enc424j600/ENC424J600.p1.d ${OBJECTDIR}/src/eth/ethernet.p1.d ${OBJECTDIR}/src/eth/mac.p1.d ${OBJECTDIR}/src/mem/memoryController.p1.d ${OBJECTDIR}/src/stack/backgroundTasks.p1.d ${OBJECTDIR}/src/stack/stack.p1.d ${OBJECTDIR}/src/system/main.p1.d ${OBJECTDIR}/src/system/sevenseg.p1.d ${OBJECTDIR}/src/system/uart.p1.d ${OBJECTDIR}/src/stack/protocols/arp.p1.d ${OBJECTDIR}/src/stack/protocols/ipv4.p1.d ${OBJECTDIR}/src/stack/time.p1.d ${OBJECTDIR}/src/stack/protocols/icmp.p1.d ${OBJECTDIR}/src/stack/protocols/tcp.p1.d ${OBJECTDIR}/src/stack/protocols/udp.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/enc424j600/ENC424J600.p1 ${OBJECTDIR}/src/eth/ethernet.p1 ${OBJECTDIR}/src/eth/mac.p1 ${OBJECTDIR}/src/mem/memoryController.p1 ${OBJECTDIR}/src/stack/backgroundTasks.p1 ${OBJECTDIR}/src/stack/stack.p1 ${OBJECTDIR}/src/system/main.p1 ${OBJECTDIR}/src/system/sevenseg.p1 ${OBJECTDIR}/src/system/uart.p1 ${OBJECTDIR}/src/stack/protocols/arp.p1 ${OBJECTDIR}/src/stack/protocols/ipv4.p1 ${OBJECTDIR}/src/stack/time.p1 ${OBJECTDIR}/src/stack/protocols/icmp.p1 ${OBJECTDIR}/src/stack/protocols/tcp.p1 ${OBJECTDIR}/src/stack/protocols/udp.p1

# Source Files
SOURCEFILES=src/enc424j600/ENC424J600.c src/eth/ethernet.c src/eth/mac.c src/mem/memoryController.c src/stack/backgroundTasks.c src/stack/stack.c src/system/main.c src/system/sevenseg.c src/system/uart.c src/stack/protocols/arp.c src/stack/protocols/ipv4.c src/stack/time.c src/stack/protocols/icmp.c src/stack/protocols/tcp.c src/stack/protocols/udp.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F27K42
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/enc424j600/ENC424J600.p1: src/enc424j600/ENC424J600.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/enc424j600" 
	@${RM} ${OBJECTDIR}/src/enc424j600/ENC424J600.p1.d 
	@${RM} ${OBJECTDIR}/src/enc424j600/ENC424J600.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/enc424j600/ENC424J600.p1 src/enc424j600/ENC424J600.c 
	@-${MV} ${OBJECTDIR}/src/enc424j600/ENC424J600.d ${OBJECTDIR}/src/enc424j600/ENC424J600.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/enc424j600/ENC424J600.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eth/ethernet.p1: src/eth/ethernet.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/eth" 
	@${RM} ${OBJECTDIR}/src/eth/ethernet.p1.d 
	@${RM} ${OBJECTDIR}/src/eth/ethernet.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/eth/ethernet.p1 src/eth/ethernet.c 
	@-${MV} ${OBJECTDIR}/src/eth/ethernet.d ${OBJECTDIR}/src/eth/ethernet.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eth/ethernet.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eth/mac.p1: src/eth/mac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/eth" 
	@${RM} ${OBJECTDIR}/src/eth/mac.p1.d 
	@${RM} ${OBJECTDIR}/src/eth/mac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/eth/mac.p1 src/eth/mac.c 
	@-${MV} ${OBJECTDIR}/src/eth/mac.d ${OBJECTDIR}/src/eth/mac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eth/mac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/mem/memoryController.p1: src/mem/memoryController.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/mem" 
	@${RM} ${OBJECTDIR}/src/mem/memoryController.p1.d 
	@${RM} ${OBJECTDIR}/src/mem/memoryController.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/mem/memoryController.p1 src/mem/memoryController.c 
	@-${MV} ${OBJECTDIR}/src/mem/memoryController.d ${OBJECTDIR}/src/mem/memoryController.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/mem/memoryController.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/backgroundTasks.p1: src/stack/backgroundTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack" 
	@${RM} ${OBJECTDIR}/src/stack/backgroundTasks.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/backgroundTasks.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/backgroundTasks.p1 src/stack/backgroundTasks.c 
	@-${MV} ${OBJECTDIR}/src/stack/backgroundTasks.d ${OBJECTDIR}/src/stack/backgroundTasks.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/backgroundTasks.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/stack.p1: src/stack/stack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack" 
	@${RM} ${OBJECTDIR}/src/stack/stack.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/stack.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/stack.p1 src/stack/stack.c 
	@-${MV} ${OBJECTDIR}/src/stack/stack.d ${OBJECTDIR}/src/stack/stack.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/stack.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system/main.p1: src/system/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/system" 
	@${RM} ${OBJECTDIR}/src/system/main.p1.d 
	@${RM} ${OBJECTDIR}/src/system/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/system/main.p1 src/system/main.c 
	@-${MV} ${OBJECTDIR}/src/system/main.d ${OBJECTDIR}/src/system/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system/sevenseg.p1: src/system/sevenseg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/system" 
	@${RM} ${OBJECTDIR}/src/system/sevenseg.p1.d 
	@${RM} ${OBJECTDIR}/src/system/sevenseg.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/system/sevenseg.p1 src/system/sevenseg.c 
	@-${MV} ${OBJECTDIR}/src/system/sevenseg.d ${OBJECTDIR}/src/system/sevenseg.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system/sevenseg.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system/uart.p1: src/system/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/system" 
	@${RM} ${OBJECTDIR}/src/system/uart.p1.d 
	@${RM} ${OBJECTDIR}/src/system/uart.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/system/uart.p1 src/system/uart.c 
	@-${MV} ${OBJECTDIR}/src/system/uart.d ${OBJECTDIR}/src/system/uart.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system/uart.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/arp.p1: src/stack/protocols/arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/arp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/arp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/arp.p1 src/stack/protocols/arp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/arp.d ${OBJECTDIR}/src/stack/protocols/arp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/arp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/ipv4.p1: src/stack/protocols/ipv4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/ipv4.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/ipv4.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/ipv4.p1 src/stack/protocols/ipv4.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/ipv4.d ${OBJECTDIR}/src/stack/protocols/ipv4.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/ipv4.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/time.p1: src/stack/time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack" 
	@${RM} ${OBJECTDIR}/src/stack/time.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/time.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/time.p1 src/stack/time.c 
	@-${MV} ${OBJECTDIR}/src/stack/time.d ${OBJECTDIR}/src/stack/time.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/time.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/icmp.p1: src/stack/protocols/icmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/icmp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/icmp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/icmp.p1 src/stack/protocols/icmp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/icmp.d ${OBJECTDIR}/src/stack/protocols/icmp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/icmp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/tcp.p1: src/stack/protocols/tcp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/tcp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/tcp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/tcp.p1 src/stack/protocols/tcp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/tcp.d ${OBJECTDIR}/src/stack/protocols/tcp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/tcp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/udp.p1: src/stack/protocols/udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/udp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/udp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=pickit3  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/udp.p1 src/stack/protocols/udp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/udp.d ${OBJECTDIR}/src/stack/protocols/udp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/udp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/enc424j600/ENC424J600.p1: src/enc424j600/ENC424J600.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/enc424j600" 
	@${RM} ${OBJECTDIR}/src/enc424j600/ENC424J600.p1.d 
	@${RM} ${OBJECTDIR}/src/enc424j600/ENC424J600.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/enc424j600/ENC424J600.p1 src/enc424j600/ENC424J600.c 
	@-${MV} ${OBJECTDIR}/src/enc424j600/ENC424J600.d ${OBJECTDIR}/src/enc424j600/ENC424J600.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/enc424j600/ENC424J600.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eth/ethernet.p1: src/eth/ethernet.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/eth" 
	@${RM} ${OBJECTDIR}/src/eth/ethernet.p1.d 
	@${RM} ${OBJECTDIR}/src/eth/ethernet.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/eth/ethernet.p1 src/eth/ethernet.c 
	@-${MV} ${OBJECTDIR}/src/eth/ethernet.d ${OBJECTDIR}/src/eth/ethernet.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eth/ethernet.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eth/mac.p1: src/eth/mac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/eth" 
	@${RM} ${OBJECTDIR}/src/eth/mac.p1.d 
	@${RM} ${OBJECTDIR}/src/eth/mac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/eth/mac.p1 src/eth/mac.c 
	@-${MV} ${OBJECTDIR}/src/eth/mac.d ${OBJECTDIR}/src/eth/mac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eth/mac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/mem/memoryController.p1: src/mem/memoryController.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/mem" 
	@${RM} ${OBJECTDIR}/src/mem/memoryController.p1.d 
	@${RM} ${OBJECTDIR}/src/mem/memoryController.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/mem/memoryController.p1 src/mem/memoryController.c 
	@-${MV} ${OBJECTDIR}/src/mem/memoryController.d ${OBJECTDIR}/src/mem/memoryController.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/mem/memoryController.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/backgroundTasks.p1: src/stack/backgroundTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack" 
	@${RM} ${OBJECTDIR}/src/stack/backgroundTasks.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/backgroundTasks.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/backgroundTasks.p1 src/stack/backgroundTasks.c 
	@-${MV} ${OBJECTDIR}/src/stack/backgroundTasks.d ${OBJECTDIR}/src/stack/backgroundTasks.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/backgroundTasks.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/stack.p1: src/stack/stack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack" 
	@${RM} ${OBJECTDIR}/src/stack/stack.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/stack.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/stack.p1 src/stack/stack.c 
	@-${MV} ${OBJECTDIR}/src/stack/stack.d ${OBJECTDIR}/src/stack/stack.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/stack.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system/main.p1: src/system/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/system" 
	@${RM} ${OBJECTDIR}/src/system/main.p1.d 
	@${RM} ${OBJECTDIR}/src/system/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/system/main.p1 src/system/main.c 
	@-${MV} ${OBJECTDIR}/src/system/main.d ${OBJECTDIR}/src/system/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system/sevenseg.p1: src/system/sevenseg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/system" 
	@${RM} ${OBJECTDIR}/src/system/sevenseg.p1.d 
	@${RM} ${OBJECTDIR}/src/system/sevenseg.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/system/sevenseg.p1 src/system/sevenseg.c 
	@-${MV} ${OBJECTDIR}/src/system/sevenseg.d ${OBJECTDIR}/src/system/sevenseg.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system/sevenseg.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/system/uart.p1: src/system/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/system" 
	@${RM} ${OBJECTDIR}/src/system/uart.p1.d 
	@${RM} ${OBJECTDIR}/src/system/uart.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/system/uart.p1 src/system/uart.c 
	@-${MV} ${OBJECTDIR}/src/system/uart.d ${OBJECTDIR}/src/system/uart.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/system/uart.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/arp.p1: src/stack/protocols/arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/arp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/arp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/arp.p1 src/stack/protocols/arp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/arp.d ${OBJECTDIR}/src/stack/protocols/arp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/arp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/ipv4.p1: src/stack/protocols/ipv4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/ipv4.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/ipv4.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/ipv4.p1 src/stack/protocols/ipv4.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/ipv4.d ${OBJECTDIR}/src/stack/protocols/ipv4.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/ipv4.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/time.p1: src/stack/time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack" 
	@${RM} ${OBJECTDIR}/src/stack/time.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/time.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/time.p1 src/stack/time.c 
	@-${MV} ${OBJECTDIR}/src/stack/time.d ${OBJECTDIR}/src/stack/time.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/time.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/icmp.p1: src/stack/protocols/icmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/icmp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/icmp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/icmp.p1 src/stack/protocols/icmp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/icmp.d ${OBJECTDIR}/src/stack/protocols/icmp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/icmp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/tcp.p1: src/stack/protocols/tcp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/tcp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/tcp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/tcp.p1 src/stack/protocols/tcp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/tcp.d ${OBJECTDIR}/src/stack/protocols/tcp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/tcp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/stack/protocols/udp.p1: src/stack/protocols/udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/stack/protocols" 
	@${RM} ${OBJECTDIR}/src/stack/protocols/udp.p1.d 
	@${RM} ${OBJECTDIR}/src/stack/protocols/udp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/src/stack/protocols/udp.p1 src/stack/protocols/udp.c 
	@-${MV} ${OBJECTDIR}/src/stack/protocols/udp.d ${OBJECTDIR}/src/stack/protocols/udp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/stack/protocols/udp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.map  -D__DEBUG=1  --debugger=pickit3  -DXPRJ_default=$(CND_CONF)  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"        $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  --double=24 --float=24 --emi=wordwrite --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=free -P -N255 --warn=-3 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/EthernetDevBoard.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
