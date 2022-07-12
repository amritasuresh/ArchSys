TP Architecture et système
==========================

Enseignements de première année, Département informatique de l'ENS de Cachan

- Introduction, Shell ;
- Rappels de C ;
- Processus et signaux, Gestion mémoire ;
- Système de fichiers, Entrées/Sorties ;
- Threads, Exclusion mutuelle.

Structure of files
------------------
BSD make is required, it is often called `bmake` on Linux distributions.

Each (dated) directory contains a Makefile and a latex source file as
`main.tex`. The Makefile should start by including `mk/latex.mk` which
sets `main.pdf` as the default target and provides an inference rule to build
PDF files for TeX ones. The Makefiles along the sources only set additional
dependencies and must set the `SRC` variable (which is always --currently-- 
`main.tex`).
The Make variable `ANSWERS` can be set to `1` to print
the answers in the resulting PDF.
To build all files, just call (BSD) `make` at the root of the repository.
_Id est_, to build all files with answers (and use parallelism on 4 cpus),
```shell
make -j 4 ANSWERS=1
```

