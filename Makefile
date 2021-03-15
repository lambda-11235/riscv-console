
all:
	cd tos; $(MAKE)
	cd kernel; $(MAKE)
	cd program; $(MAKE)

clean:
	cd tos; $(MAKE) clean
	cd kernel; $(MAKE) clean
	cd program; $(MAKE) clean
