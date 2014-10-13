(define count 110)
(define f (lambda () (if (< count 1000000) (begin (set! count (+ count 1)) (f)) count)))
(f)
