(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 16))
(declare-const v1 (_ BitVec 16))
(assert (= #b1 (bvnot (ite (= (bvnot v1) ((_ extract 15 0) (bvnot (concat v0 v1)))) #b1 #b0))))
(check-sat)