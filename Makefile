

.PHONY: all
all: software sim

.PHONY: software
software:
	@+$(MAKE) -C software --no-print-directory

.PHONY: sim
sim:
	@odatix sim --overwrite -y