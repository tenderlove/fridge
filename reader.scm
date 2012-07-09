(use posix)

(define (make-serialport file)
  (let ((fileno
         (file-open
           file
           (bitwise-ior open/rdwr open/noctty open/nonblock))))

    ; Enable blocking reads on the FD
    (let ((current-fl (file-control fileno fcntl/getfl 0)))
      (file-control fileno fcntl/setfl
                    (bitwise-and current-fl (bitwise-not open/nonblock)))
      (open-input-file* fileno))))

; Split bytes in to pairs
(define (split l)
  (if (null? l)
      '()
      (let ((pair (take l 2)) (rest (drop l 2)))
        (cons pair (split rest)))))

; Convert the list of pairs to 16 bit ints and divide by 10
(define (decode s)
  (map (lambda (pair)
         (let ((l (car pair)) (r (cadr pair)))
           (/ (+ (arithmetic-shift l 8) r) 10)))
       (split (map char->integer (string->list s)))))

(define (read-sausage) (decode (read-line)))

(define (print-sausage) (port-for-each print read-sausage))

(define sausage-port (make-serialport "/dev/tty.uart-AEFF467AF9D70234"))

(with-input-from-port sausage-port print-sausage)

