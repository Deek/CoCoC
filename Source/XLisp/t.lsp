; A simple turtle graphics program for VT100 compatible terminals
; By David Betz with some stuff borrowed from a similar program
; by Mark Mallett

; :::::::::::::::::::::
; :: Misc. Functions ::
; :::::::::::::::::::::

; Note:  The following functions assume that this program is running on
; a VT100 compatible terminal.

; Clear the screen
(defun clear ()
    (princ "\e[H\e[J"))

; Move the cursor
(defun setpos (x y)
    (princ "\e[" y ";" x "H"))

; Kill the remainder of the screen
(defun kill ()
    (princ "\e[J"))

; Set the scrolling region
(defun setscroll (t b)
    (princ "\e[" t ";" b "r"))

; Move the cursor to the currently set bottom position and clear the line
;  under it
(defun bottom ()
    (setpos 0 by)
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
(Turtle 'ivars '(xpos ypos xvel yvel heading trail char))

; Answer "isnew" by initing a position and char and displaying.
(Turtle 'answer 'isnew '(x y) '(
    (setq xpos x)       ; set the turtle's x position
    (setq ypos y)       ; set the turtle's y position
    (setq xvel 0)       ; set the turtle's x velocity
    (setq yvel -1)      ; set the turtle's y velocity
    (setq char "^")     ; set the turtle's character representation
    (setq trail ".")    ; set the turtle's trail character
    (setq heading 0)    ; set the turtle's heading
    (self 'display)))   ; display the new turtle

; Message "display" prints its char at its current position
(Turtle 'answer 'display '() '(
    (setpos xpos ypos)
    (princ char)
    (bottom)
    self))

; Message "trail" sets the turtle's trail character to its arg
;  and redisplays the turtle
(Turtle 'answer 'trail '(c) '(
    (setq trail c)
    (self 'display)))

; Message "goto" goes to a new place after clearing old one
(Turtle 'answer 'goto '(x y) '(
    (setpos xpos ypos)
    (princ trail)
    (setq xpos (max 1 (min x 80)))
    (setq ypos (max 1 (min y by)))
    (self 'display)))

; Message "left" turns the turtle left
(Turtle 'answer 'left '() '(
    (turn 3)
    (self 'display)))

; Message "right" turns the turtle right
(Turtle 'answer 'right '() '(
    (turn 1)
    (self 'display)))

; Function "turn" turns a turtle
(defun turn (inc)
    (setq heading (% (+ heading inc) 4))
    (cond
        ((== heading 0)         ; heading north
            (setq xvel 0)
            (setq yvel -1)
            (setq char "^"))
        ((== heading 1)         ; heading east
            (setq xvel 1)
            (setq yvel 0)
            (setq char ">"))
        ((== heading 2)         ; heading south
            (setq xvel 0)
            (setq yvel 1)
            (setq char "v"))
        ((== heading 3)         ; heading west
            (setq xvel -1)
            (setq yvel 0)
            (setq char "<"))))

; Message "forward" moves the turtle forward
(Turtle 'answer 'forward '() '(
    (self 'goto (+ xpos xvel) (+ ypos yvel))))

; Message "backward" moves the turtle backward
(Turtle 'answer 'backward '() '(
    (self 'goto (- xpos xvel) (- ypos yvel))))


; :::::::::::::::
; :: Main Code ::
; :::::::::::::::

; Initialize
(setq by 20)
(setscroll (+ by 1) 24)

; Create some programmable turtles
(cb)
(setq t1 (Turtle 'new 40 10))
(setq t2 (Turtle 'new 41 10))