.PHONY: prog
prog:
	@sudo make -f makefile.prog
	
.PHONY: test
test:
	@sudo make -f makefile.test

.PHONY: clean
clean:
	@sudo rm -rf bin/ lib/ obj/ inc/
	@echo Cleaning...

.PHONY: push
push:
	@sudo git add .
	@sudo git commit -m "some changes"
	@sudo git push origin master

.PHONY: help
help:
	@echo type make for compiling
	@echo type make clean for cleaning
	@echo type make push for fast pushing to repo
	@echo type make test to build the tests
	@echo type make prog to build the program