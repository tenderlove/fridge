(define (cal-Rb Vs Vc Rl icCurrent)
  (let ((lc 100) ;lc in mA

        (hFE 100))
    (/ (* Vc hFE) (* 5 lc))))

(define Vs 3.3)
(define Vc 1.1)
(define Rl 6500)

(define lc (* (/ Vs Rl))) ; in mA
(define hFE (* 5 (/ lc 1.5)))
(define Rb (/ (* Vc hFE) (* 5 lc)))


(print (cal-Rb 6 6 100 5))
