#
# ECE 18-447 Makefile.
#
# - History:
#   - 2011: Joshua Wise
#   - 2013: Yoongu Kim
#   - 2014: Xiao Bo Zhao
#	- 2023: Hojan Young
#


###################################################################
### Setup
###################################################################

TIMESTAMP := outputs/$(shell date +%m%d-%H%M%S)
OUTPUT ?= $(TIMESTAMP)

INPUT ?= 447inputs/addiu.s
REGDUMP := $(INPUT:.s=.reg)

BATCH ?= 447inputs/addiu.s

SYNTHDIR = dc
SYNTH = lab
TB = testbench

###################################################################
### Constants
###################################################################

# text attributes: normal, bold, underline
n=\e[0m
b=\e[1m
u=\e[4m

# bold+green
g=\e[1;32m

# bold+red
r=\e[1;31m

# debug message
m=$g18-477 Makefile: $n


###################################################################
### Help Screen
###################################################################

default:
	@echo "$mHELP SCREEN"
	@echo ""
	@echo "$bUSAGE:$n"
	@echo "\tmake $uTARGET$n [$uVARIABLE$n]..."
	@echo ""
	@echo ""
	@echo "$bTARGET:$n"
	@echo "\t$bsim$n"
	@echo "\t    compiles Verilog found in $usrc$n directory;"
	@echo "\t    simulates Verilog using $bINPUT$n as input;"
	@echo "\t    places all outputs (logs, waveforms, regdump, etc.) in $bOUTPUT$n"
	@echo ""
	@echo "\t$bverify$n"
	@echo "\t    compiles Verilog found in $usrc$n directory;"
	@echo "\t    simulates Verilog using $bINPUT$n as input;"
	@echo "\t    places all outputs (logs, waveforms, regdump, etc.) in $bOUTPUT$n"
	@echo "\t    verifies the simulation register-dump against the reference"
	@echo ""
	@echo "\t$bbatch-verify$n"
	@echo "\t    similar to $bverify$n, but uses $bBATCH$n as input"
	@echo ""
	@echo "\t$bsynth$n"
	@echo "\t    compiles Verilog found in $usrc$n directory;"
	@echo "\t    synthesizes Verilog"
	@echo "\t    places all outputs (reports, schematics, etc.) in $bOUTPUT$n"
	@echo ""
	@echo "\t$bauto$n"
	@echo "\t    automatically \"wires\" up Verilog modules found in $usrc/*.v$n;"
	@echo "\t    (not mandatory -- merely for your convenience)"
	@echo ""
	@echo ""
	@echo "$bVARIABLE:$n"
	@echo "\t$bINPUT$n=$uASSEMBLY$n"
	@echo "\t    for $bsim$n and $bverify$n, specifies an input ARM assemply file;"
	@echo "\t    if unspecified, defaults to $u447inputs/addiu.s$n"
	@echo ""
	@echo "\t$bBATCH$n=$uASSEMBLY FILES$n"
	@echo "\t    for $bbatch-verify$n, specifies one or more input ARM assemply files;"
	@echo "\t    if unspecified, defaults to $u447inputs/addiu.s$n"
	@echo ""
	@echo "\t$bOUTPUT$n=$uDIR$n"
	@echo "\t    for all targets, specifies the output directory;"
	@echo "\t    if unspecified, defaults to $uoutputs/current-timestamp$n;"
	@echo ""
	@echo ""
	@echo "$bEXAMPLES:$n"
	@echo "\tmake sim"
	@echo "\tmake sim INPUT=inputs/mytest.s OUTPUT=outputs/myoutput"
	@echo "\tmake verify"
	@echo "\tmake verify INPUT=inputs/mytest.s OUTPUT=outputs/myoutput"
	@echo "\tmake batch-verify BATCH=447inputs/*.s"
	@echo "\tmake batch-verify BATCH=447inputs/*.s OUTPUT=outputs/myoutput"
	@echo "\tmake synth"
	@echo "\tmake synth OUTPUT=outputs/myoutput"
	@echo ""
	@echo "(END)"
	@exit 0


###################################################################
### Compile Verilog
###################################################################

compile:
	@echo "$mCopying Verilog into $(OUTPUT)..."
	@mkdir -p $(OUTPUT)/sim/src
	@cp `find src 447src -iname '*.sv' -o -iname '*.v' -o -iname '*.vh'`	$(OUTPUT)/sim/src
	@echo "$mCompiling Verilog..."
	cd $(OUTPUT)/sim; iverilog -g2012 -o verilog_sim src/*.v src/*.sv -I src/


###################################################################
### Assemble MIPS Assembly Input
###################################################################

assemble:
	@echo "$mCopying $(INPUT) into $(OUTPUT)..."
	@mkdir -p $(OUTPUT)/sim
	@([ -f $(INPUT) ] && cp $(INPUT) $(OUTPUT)/sim) || \
		(echo "$mERROR -- test input not found: $(INPUT)"; exit 1)
	@echo "$mAssemblying input in $(OUTPUT)..."
	spim447 -notrap -vasm $(INPUT) $(OUTPUT)/sim/mem;


###################################################################
### Simulate Verilog
###################################################################

sim: compile assemble
	@echo "$mCopying NCSim configuration into $(OUTPUT)..."
	@cp ncsim/* $(OUTPUT)/sim
	@cp 447ncsim/* $(OUTPUT)/sim
	@echo "$mSimulating Verilog in $(OUTPUT)..."
	cd $(OUTPUT)/sim; ncsim worklib.$(TB):module -input 447ncsim.tcl -input ncsim.tcl
	@if grep '*W' $(OUTPUT)/sim/ncsim.log >/dev/null; \
		then echo '$m$rSimulator log has warnings!$n'; fi
	@echo "$mSimulation of $(INPUT) has completed in $(OUTPUT)."
	@echo "$mTo view waveforms, execute the following command: $bsimvision $(OUTPUT)/sim/waveforms.shm$n"


###################################################################
### Verify Simulation
###################################################################

verify: sim
	@([ -f $(REGDUMP) ] && cp $(REGDUMP) $(OUTPUT)/sim) || \
		(echo "$mERROR -- reference register-dump not found: $(REGDUMP)"; exit 1)
	447util/tester.pl $(OUTPUT)/sim/$(notdir $(REGDUMP)) $(OUTPUT)/sim/regdump.txt
	@echo "$mVerification of $(INPUT) has completed in $(OUTPUT)."
	@echo "$mTo view the simulation register-dump, execute the following command: $bcat $(OUTPUT)/sim/regdump.txt$n"

batch-verify:
	for i in $(BATCH); do \
		make verify INPUT=$$i OUTPUT=$(OUTPUT); \
	done


###################################################################
### Synthesize Verilog
###################################################################

synth:
	@mkdir -p $(OUTPUT)/synth/src
	@mkdir -p $(OUTPUT)/synth/447src
	@cp `find src -iname '*.v' -o -iname '*.sv' -o -iname '*.vh'` $(OUTPUT)/synth/src
	@cp `find 447src -iname '*.v' -o -iname '*.sv' -o -iname '*.vh'` $(OUTPUT)/synth/447src
	@cp $(SYNTHDIR)/$(SYNTH).dc $(OUTPUT)/synth
	@cd $(OUTPUT)/synth; dc_shell-xg-t -f $(SYNTH).dc
	@echo "$mSynthesis has completed in $(OUTPUT)."
	@echo "Timing results can be found at $(OUTPUT)/synth/timing_mips_core.rpt"


###################################################################
### Misc.
###################################################################

auto:
	@emacs -l /afs/ece/class/ece447/elisp/quicklaunch.el -l /afs/ece/class/ece447/elisp/verilog-mode.el --batch `find src -iname '*.v' -o -iname '*.sv'` -f verilog-batch-auto
