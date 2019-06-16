; This is a sample XLISP program.
; It implements a simple form of programmable turtle for VT100 compatible
; terminals.

; To run it:

;       A>xlisp pt

; This should cause the screen to be cleared and two turtles to appear.
; They should each execute their simple programs and then the prompt
; should return.  Look at the code to see how all of this works.

; Clear the screen
(defun clear ()
    (princ "\e[H\e[J"))

; Move the cursor
(defun setpos (x y)
    (princ "\e[" y ";" x "H"))

; Kill the remainder of the line
(defun kill ()
    (princ "\e[K"))

; Move the cursor to the currently set bottom position and clear the line
;  under it
(defun bottom ()
    (setpos bx (+ by 1))
    (kill)
    (setpos bx by)
    (kill))

; Clear the screen and go to the bottom
(defun cb ()
    (clear)
    (bottom))


; ::::::::::::
; :: Turtle ::
; ::::::::::::

; Define "Turtle" class
(setq Turtle (Class 'new))

; Define instance variables
(Turtle 'ivars '(xpos ypos char))

; Answer "isnew" by initing a position and char and displaying.
(Turtle 'answer 'isnew '() '(
    (setq xpos (setq newx (+ newx 1)))
    (setq ypos 12)
    (setq char "*")
    (self 'display)
    self))

; Message "display" prints its char at its current position
(Turtle 'answer 'display '() '(
    (setpos xpos ypos)
    (princ char)
    (bottom)
    self))

; Message "char" sets char to its arg and displays it
(Turtle 'answer 'char '(c) '(
    (setq char c)
    (self 'display)))

; Message "goto" goes to a new place after clearing old one
(Turtle 'answer 'goto '(x y) '(
    (setpos xpos ypos) (princ " ")
    (setq xpos x)
    (setq ypos y)
    (self 'display)))

; Message "up" moves up if not at top
(Turtle 'answer 'up '() '(
    (if (> ypos 0) (
        (self 'goto xpos (- ypos 1)))
    (
        (bottom)))))

; Message "down" moves down if not at bottom
(Turtle 'answer 'down '() '(
    (if (< ypos by) (
        (self 'goto xpos (+ ypos 1)))
    (
        (bottom)))))

; Message "right" moves right if not at right
(Turtle 'answer 'right '() '(
    (if (< xpos 80) (
        (self 'goto (+ xpos 1) ypos))
    (
        (bottom)))))

; Message "left" moves left if not at left
(Turtle 'answer 'left '() '(
    (if (> xpos 0) (
        (self 'goto (- xpos 1) ypos))
    (
        (bottom)))))


; :::::::::::::
; :: PTurtle ::
; :::::::::::::

; Define "DPurtle" programable turtle class
(setq PTurtle (Class 'new Turtle))

; Define instance variables
(PTurtle 'ivars '(prog pc))

; Message "program" stores a program
(PTurtle 'answer 'program '(p) '(
    (setq prog p)
    (setq pc prog)
    self))

; Message "step" executes a single program step
(PTurtle 'answer 'step '() '(
    (if (null pc) (
        (setq pc prog)))
    (if pc (
        (self (head pc))
        (setq pc (tail pc))))
    self))


; ::::::::::::::
; :: PTurtles ::
; ::::::::::::::

; Define "PTurtles" class
(setq PTurtles (Class 'new))

; Define instance variables
(PTurtles 'ivars '(turtles))

; Message "make" makes a programable turtle and adds it to the collection
(PTurtles 'answer 'make '(x y / newturtle) '(
    (setq newturtle (PTurtle 'new))
    (newturtle 'goto x y)
    (setq turtles (cons newturtle turtles))
    newturtle))

; Message "step" steps each turtle program once
(PTurtles 'answer 'step '() '(
    (foreach turtle turtles
        (turtle 'step))
    self))

; Message "step:" steps each turtle program n times
(PTurtles 'answer 'step: '(n) '(
    (while n
        (self 'step)
        (setq n (- n 1)))
    self))


; Initialize things and start up
(setq bx 0)
(setq by 20)
(setq newx 0)

; Create some programmable turtles
(cb)
(setq turtles (PTurtles 'new))
(setq t1 (turtles 'make 40 10))
(setq t2 (turtles 'make 41 10))
(t1 'program '(left right up down))
(t2 'program '(right left down up))