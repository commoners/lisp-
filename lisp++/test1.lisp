(define (p) (p)) 
(define (test x y) (if (= x 0) 0 y)) 
;(test 0 (p))
(define (info exp e) (begin (display e) (display exp) (newline) exp))
(define (add exp) (+ exp))
(add 2)
(display "hello,world!")
(newline)
(display (add 2222))
(newline)


