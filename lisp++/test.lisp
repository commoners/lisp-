(define (add a b) (+ a b))
(define x 1000)
(add 1 x)
(add (+ 10 11) (add 100 1))
(define (p) (p))
;(p)
(cond ((= 1 2) (+ 1 3))
      ((= 3 1) 'less)
      ((= 1 2) (+ 1 4))
      (2))
(and (= 1 1 ) (< 2 1))
(or (= 2 2) (> 2 1))
(or (= 2 2) (< 2 1))
(or (< 2 2) (< 2 1))
(or (< 2 2) (< 2 1) (or (= 2 2) (< 2 1)))

(cond ((= 1 2) (+ 1 3))
      ((= 3 1) 'more)
      ((= 1 2) (+ 1 4))
      (else (+ 91 (+ 1 2 (+ 4 5)))))

(+ 1 2)
(> -5 0)
(> 5 0)

