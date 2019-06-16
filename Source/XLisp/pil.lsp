;;; The following is a tiny Prolog interpreter in MacLisp
;; written by Ken Kahn and modified for XLISP by David Betz.
;; It was inspired by other tiny Lisp-based Prologs of
;; Par Emanuelson and Martin Nilsson.
;; There are no side-effects anywhere in the implementation.
;; Though it is VERY slow of course.

(defun prolog (database / goal) ;; a top-level loop for Prolog
  (while (setq goal (read))
    (prove (list (rename-variables goal '(0)))
           '((bottom-of-environment))
           database
           1)))

(defun prove (list-of-goals environment database level)
  ;; proves the conjunction of the list-of-goals
  ;; in the current environment
  (cond ((null list-of-goals)
         ;; succeeded since there are no goals
         (print-bindings environment environment)
          ;; the user answers "y" or "n" to "More?"
         (! (y-or-n-p "More?")))
        (t (try-each database database
                     (tail list-of-goals) (head list-of-goals)
                     environment level))))

(defun try-each (database-left database goals-left goal environment level 
                 / assertion new-enviroment)
 (cond ((null database-left)
        ()) ;; fail since nothing left in database
       (t (setq assertion
                 ;; level is used to uniquely rename variables
                (rename-variables (head database-left)
                                   (list level)))
          (setq new-environment
                (unify goal (head assertion) environment))
          (cond ((null new-environment) ;; failed to unify
                 (try-each (tail database-left) database
                           goals-left goal
                           environment level))
                ((prove (append (tail assertion) goals-left)
                        new-environment
                        database
                        (+ 1 level)))
                (t (try-each (tail database-left) database
                             goals-left goal
                             environment level))))))

(defun unify (x y environment / new-environment)
  (setq x (value x environment))
  (setq y (value y environment))
  (cond ((variable-p x) (cons (list x y) environment))
        ((variable-p y) (cons (list y x) environment))
        ((|| (atom x) (atom y))
         (cond ((equal x y) environment)
               (t nil)))
        (t (setq new-environment (unify (head x) (head y) environment))
           (cond (new-environment (unify (tail x) (tail y) new-environment))
                 (t nil)))))

(defun value (x environment / binding)
  (cond ((variable-p x)
         (setq binding (assoc x environment))
         (cond ((null binding) x)
               (t (value (nth 2 binding) environment))))
        (t x)))

(defun variable-p (x) ;; a variable is a list beginning with "?"
  (&& x (listp x) (eq (head x) '?)))

(defun rename-variables (term list-of-level)
  (cond ((variable-p term) (append term list-of-level))
        ((atom term) term)
        (t (cons (rename-variables (head term)
                                   list-of-level)
                 (rename-variables (tail term)
                                   list-of-level)))))

(defun print-bindings (environment-left environment)
  (cond ((tail environment-left)
         (cond ((== 0
                    (nth 3 (head (head environment-left))))
                (print
                 (nth 2 (head (head environment-left))))
                (princ " = ")
                (print (value (head (head environment-left))
                              environment))
                (princ "\n")))
         (print-bindings (tail environment-left) environment))))

;; a sample database:
(setq db '(((father jack ken))
           ((father jack karen))
           ((grandparent (? grandparent) (? grandchild))
            (parent (? grandparent) (? parent))
            (parent (? parent) (? grandchild)))
           ((mother el ken))
           ((mother cele jack))
           ((parent (? parent) (? child))
            (mother (? parent) (? child)))
           ((parent (? parent) (? child))
            (father (? parent) (? child)))))

;; the following are utilities
(defun assoc (key env)
  (cond ((null env) nil)
        ((equal (head (head env)) key) (head env))
        (t (assoc key (tail env)))))

(defun y-or-n-p (prompt)
  (princ prompt)
  (cond ((eq (read) 'y) t)
        (t nil)))