(load "lib.lisp")
;;<=
(define (<= x y)
(or (< x y) (= x y)))

(define loop
  (lambda(x y)
    (if (<= x y)
        (begin (display x) 
               (display " ") 
               (set! x (+ x 1))
               (loop x y)) )))
;(trace 'loop)
;(trace 'if)
(loop 1 10)

(load "./test/ch4-mceval.scm")
(load "./test/ch4.scm")
