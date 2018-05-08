LATEXMK = ./latexrun.py 
LATEX = pdflatex
READ = evince
TIKZ := $(wildcard tikz/*.tex)
SOURCES := morusAC.tex Note_on_Linear_Biases_in_MORUS.tex
PDFS := $(notdir $(SOURCES:.tex=.pdf))
PNGS := $(notdir $(SOURCES:.tex=.png))

NO_COLOR="\033[0m"
RED = "\033[31m"
BRIGHTRED = "\033[91m"
YELLOW = "\033[93m"
ORANGE = "\033[33m"
GRAY = "\033[37m"
GREEN = "\033[32m"
BOLD = "\033[1m"

.PHONY: FORCE

all: $(PDFS)

cleanpdf:
	@rm png/*.png 2> /dev/null || true
	@rm *.pdf 2> /dev/null || true

%.pdf: FORCE %.tex $(TIKZ)
	@echo ""
	@echo $(BOLD)$(YELLOW)"Building $*.tex"$(NO_COLOR)
	@$(LATEXMK) $*.tex

open: $(PDFS)
	${READ} $(PDFS)


clean:
	@echo cleaning...
	@rm -r latex.out
	@rm *.aux 2> /dev/null || true
	@rm *.bbl 2> /dev/null || true
	@rm *.blg 2> /dev/null || true
	@rm *.brf 2> /dev/null || true
	@rm *.dvi 2> /dev/null || true
	@rm *.fdb_latexmk 2> /dev/null || true
	@rm *.fls 2> /dev/null || true
	@rm *.idx 2> /dev/null || true
	@rm *.ilg 2> /dev/null || true
	@rm *.ind 2> /dev/null || true
	@rm *.lfm 2> /dev/null || true
	@rm *.lof 2> /dev/null || true
	@rm *.log 2> /dev/null || true
	@rm *.lol 2> /dev/null || true
	@rm *.lot 2> /dev/null || true
	@rm *.nav 2> /dev/null || true
	@rm *.out 2> /dev/null || true
	@rm *.ps 2> /dev/null || true
	@rm *.snm 2> /dev/null || true
	@rm *.toc 2> /dev/null || true
	@rm *.vrb 2> /dev/null || true

png: $(PDFS) $(PNGS)

%.png: %.pdf
	convert -density 300 $*.pdf -quality 90 $*.png
