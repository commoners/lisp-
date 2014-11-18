;;sys
(define (find-proc name)
(sys-find name (sys-top-env)))
(define (update-proc name proc)
(sys-update name proc (sys-top-env)))

;;trace

(define (print-level indent char)
(if (< indent 1) (display "")
(begin (display char) (print-level (- indent 1) char))))

;(print-level 10 "--")


(define *trace-functions nil)
(define *trace-level* 1)
(define (trace name)
(let ((proc (find-proc name))
(new-proc nil )
(result nil)
)
(set! new-proc (lambda x
(begin (print-level *trace-level* "-") (display name) (display x) (display "\n")
(set! *trace-level* (+ *trace-level* 1))
(set! result (proc (car x) (car (cdr x)) ))
;                                (set! result (invoke (cons proc  x )))
(set! *trace-level* (- *trace-level* 1)) (print-level *trace-level* "-") (display result) (display "\n") result)) )
;(display (cons proc new-proc))
(update-proc name new-proc)
))


;;add1 sub1
(define (add1 n)
(+ n 1))
(define (sub1 n)
(- n 1))
(define (zero? n)
(= 0 n))

(define (atom? x) (not (pair? x)))

(define (map fn l)
(if (pair? l)
(cons (fn (car l)) (map fn (cdr l)))
'()))

;;cadr
(define (cadr o)
(car (cdr o)))

;;factorial
(define (factorial n)
(if (= n 1)
1(* n (factorial (- n 1)))))

;;abs
(define (abs x)
(cond ((< x 0) (- x))
(else x)))

;;<=
(define (<= x y)
(or (< x y) (= x y)))

;;>=
(define (>= x y)
(or (> x y) (= x y)))


