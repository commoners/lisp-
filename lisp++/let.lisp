;let
(setf x 'outside)
(let ((x 'inside)
 (y x))
  (list x y))
;ret (inside outside)

(let ((a 3)
(b 4)
(c 5))
(* (+ a b)c))
;ret 35

(setq a 10)
(let ((a 3)
(m a))
(+m a))
;ret 13

;let*
(setf x 'outside)
(let* ((x 'inside)
(y x))
(list x y))
;;ret (inside inside)


;;let double test
(setq my-pi 3.1415)
(let ((my-pi 3) (old-pi my-pi))
(list my-pi old-pi))
;=> (3 3.1415)

;;let* double test
(setq my-pi 3.1415)

(let* ((my-pi 3) (old-pi my-pi))
(list my-pi old-pi))
;=> (3 3)