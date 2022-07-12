#!/usr/bin/guile \
-e main -s
!#

(define (main args)
  (let ((pid (primitive-fork)))
    (if (= pid 0)
        (child)
        (begin
          (sleep 3)
          (kill pid SIGINT)))))

(define (child)
  (let ((pid (primitive-fork)))
    (if (= pid 0)
        (write-bullshit)
        (begin
          (sleep 1)
          (child)))))

(define (write-bullshit)
  (execl "/usr/games/fortune" "-e"))
