SRC  = 11_22/main.tex
SRC += 11_29/main.tex
SRC += 12_5/main.tex
SRC += 12_13/main.tex
SRC += 01_10/main.tex

.SUFFIXES:

.MAIN: ${SRC:S/.tex/.pdf/} ${SRC:.tex=.html}

.PHONY: clean
clean:
.	for d in ${SRC:H}
		${MAKE} -C "$d" clean
.	endfor

.PHONY: clobber
clobber: clean
.	for d in ${SRC:H}
		${MAKE} -C "$d" clobber
.	endfor

.SUFFIXES: .tex .pdf .html

.tex.html:
	${MAKE} -C ${<:H} ${@:T}

.tex.pdf:
	${MAKE} -C ${<:H} ${@:T}
