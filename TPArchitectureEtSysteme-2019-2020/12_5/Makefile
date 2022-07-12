#
# MAKEFILE pour les sujets de TP Architecture Système
#

MAIN=$(wildcard *.tex)
BASE=$(patsubst %.tex,%, $(MAIN))
ETUDIANTS=$(patsubst %.tex,%-students.pdf, $(MAIN))
ENSEIGNANTS=$(patsubst %.tex,%-teachers.pdf, $(MAIN))

# Commandes
PDFLATEX=xelatex
PDFLATEX_OPTIONS= -shell-escape -halt-on-error
# -interaction=batchmode

.PHONY:all
	
all: $(ETUDIANTS) $(ENSEIGNANTS)

# -------------------------------------------------------------
# Instructions pour compiler avec xelatex
$(ETUDIANTS): $(MAIN)
	@echo "\n\nPremiere passe !\n\n"
	$(PDFLATEX) $(PDFLATEX_OPTIONS) '\def\public{etudiants}\input{$<}'
	@echo "\n\nDeuxieme passe !\n\n"
	$(PDFLATEX) -shell-escape '\def\public{etudiants}\input{$<}'
	@echo "\n\nTroisieme passe !\n\n"
	$(PDFLATEX) -shell-escape '\def\public{etudiants}\input{$<}'
	mv $(BASE).pdf $(ETUDIANTS)

$(ENSEIGNANTS): $(MAIN)
	@echo "\n\nPremiere passe !\n\n"
	$(PDFLATEX) $(PDFLATEX_OPTIONS) '\def\public{enseignants}\input{$<}'
	@echo "\n\nDeuxieme passe !\n\n"
	$(PDFLATEX) -shell-escape '\def\public{enseignants}\input{$<}'
	@echo "\n\nTroisieme passe !\n\n"
	$(PDFLATEX) -shell-escape '\def\public{enseignants}\input{$<}'
	mv $(BASE).pdf $(ENSEIGNANTS)
	
clean:
	@echo "Nettoyage !\n\n"
	rm -f $(BASE).log
	rm -f $(BASE).aux
	rm -f $(BASE).tmp
	rm -f $(BASE).fdb_latexmk

superclean: clean
	@echo "Nettoyage (y compris des PDF) !\n\n"
	rm -f $(ETUDIANTS)
	rm -f $(ENSEIGNANTS)
