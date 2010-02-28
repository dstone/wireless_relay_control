RX_DIR = rx
TX_DIR = tx
SUBDIRS = $(RX_DIR) $(TX_DIR)


all:
	@echo "Building all targets"
	for i in $(SUBDIRS) ; do \
		( cd $$i ; make ) ; \
		done

rx: 
	@cd $(RX_DIR) ; make

tx:
	@cd $(TX_DIR) ; make

clean:
	for i in $(SUBDIRS) ; do \
		(cd $$i ; make clean); \
		done

.PHONY: all rx tx clean

#%: force
#	@$(MAKE) -f TX.mk $@
#	@$(MAKE) -f RX.mk $@

#force: ;
