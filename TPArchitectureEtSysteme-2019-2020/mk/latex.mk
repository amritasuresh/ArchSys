LC       = lualatex
LC_FLAGS = -shell-escape
LTXHTML       = pandoc --from latex --to html
mathjax_url   = https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.7/MathJax.js?config=TeX-MML-AM_CHTML
LTXHTML_FLAGS = --mathjax=${mathjax_url}

# Set to 1 to print the answers
ANSWERS  = 0
SRC     ?=

.SUFFIXES:

.PHONY: clean
clean:
	rm -f ${SRC:S/.tex/.out/}
	rm -f ${SRC:S/.tex/.log/}
	rm -f ${SRC:S/.tex/.aux/}

.PHONY: clean
clobber: clean
	rm -f ${SRC:S/.tex/.pdf/}

.SUFFIXES: .pdf .tex .html

.tex.pdf:
	${LC} ${LC_FLAGS} '\def\answers{${ANSWERS}}\input{$<}'
	${LC} ${LC_FLAGS} '\def\answers{${ANSWERS}}\input{$<}'

.tex.html:
	${LTXHTML} ${LTX_HTML_FLAGS} --output=$@ $<
