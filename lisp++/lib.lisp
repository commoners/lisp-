;;sys
(define (find-proc name)
    (sys:find name (sys:top-env)))
(define (update-proc name proc)
    (sys:update name proc (sys:top-env)))

;;trace

(define (print-level indent char)
(if (< indent 1) (display "")
(begin
 (display char)
 (print-level (- indent 1) char))))

;(print-level 10 "--")

(define nil '())

(define *trace-functions nil)
(define *trace-level* 1)

(define (trace name)
    (let ((proc (find-proc name))
	  (new-proc nil )
	  (result nil))
      (set! new-proc (lambda x
		       (begin (print-level *trace-level* "-") (display name) (display x) (display "\n")
			      (set! *trace-level* (+ *trace-level* 1))
			      (set! result (proc (car x) (car (cdr x)) ))
;                                (set! result (invoke (cons proc  x )))
			      (set! *trace-level* (- *trace-level* 1)) (print-level *trace-level* "-") (display result) (display "\n") result)) )
;(display (cons proc new-proc))
      (update-proc name new-proc)))


;;add1 sub1
(define (add1 n)
(+ n 1))
(define (sub1 n)
(- n 1))

;(define (zero? n)
;(= 0 n))

(define (atom? x) (not (pair? x)))

;(define (map fn l)
;(if (pair? l)
;(cons (fn (car l)) (map fn (cdr l)))
;'()))

;;cadr
(define (cadr o)
    (car (cdr o)))
;;caar
(define (caar o)
    (car (car o)))
;;cdar
(define (cdar o)
    (cdr (car o)))
;;cddr
(define (cddr o)
    (cdr (cdr o)))

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
;;map
(define (map proc items)
  (if (null? items)
      nil
      (cons (proc (car items))
            (map proc (cdr items)))))
;(map abs '( -10 2.5 -11.6 17))
;nil '()
;(define nil '())
;exp
;; Linear recursion
(define (expt b n)
  (if (= n 0)
      1
      (* b (expt b (- n 1)))))


;; Linear iteration
(define (expt b n)
  (expt-iter b n 1))

(define (expt-iter b counter product)
  (if (= counter 0)
      product
      (expt-iter b
                (- counter 1)
                (* b product)))) 
;(expt 2.1 10)

;;;list fun
(define  list
  (lambda values values) ) 

;(list 'd 'a 1 'ee '(a b c d))
;(list 3 4 'a (car '(b . c)) (+ 6 -2))
;(list (list 1 2) (list 3 4))

;;append
(define (append x y)
  (if (null? x)
      y
      (cons (car x) (append (cdr x) y))))
;(trace 'append)
;(trace 'null?)
;(append '(a b c d e) 'b )



