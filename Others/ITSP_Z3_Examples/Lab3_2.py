from z3 import *


def ground_truth(s, a1, b1, a2, b2, d_gt, a_gt, b_gt, msg_gt):
    
    s.add(Or(msg_gt == "INF", msg_gt == "Not INF"))
    
    s.add(d_gt == a1*b2 - a2*b1)
    s.add(If(d_gt==0, msg_gt=="INF", And(a_gt == (b2-b1)/d_gt, b_gt == (a1-a2)/d_gt, msg_gt=="Not INF")))
    
def student_a(s, a1, b1, a2, b2, d, a, b, msg):
    s.add(Or(msg == "INF", msg == "Not INF"))
    s.add(If((a1/a2==b1/b2), msg=="INF", And(b==(a2-a1)/(a2*b1-b2*a1), a==(b2-b1)/(b2*a1-b1*a2), msg=="Not INF")))

def student_b(s, a1, b1, a2, b2, d, a, b, msg):
    s.add(Or(msg == "INF", msg == "Not INF"))
    s.add(If(a1*b2-a2*b1!=0, And(a==(b1-b2)/(a2*b1-a1*b2), b==(a2-a1)/(a2*b1-a1*b2), msg == "Not INF"), msg == "INF"))

def student_c(s, a1, b1, a2, b2, d, a, b, msg):
    s.add(Or(msg == "INF", msg == "Not INF"))
    s.add(a ==(b2-b1)/((b1*a2)-(a1*b2)))
    s.add(b ==(a2-a1)/((b1*a2)-(a1*b2)))
    
    s.add(If(a1*b2==a2*b1, msg == "INF", msg == "Not INF"))


        
def Verify(student):  
    s = Solver()

    a1 = Real("a1")
    b1 = Real("b1")
    a2 = Real("a2")
    b2 = Real("b2")
    
    # Ground Truth
    d_gt = Real("d_gt")
    a_gt = Real("a_gt")
    b_gt = Real("b_gt")
    msg_gt = String("msg_gt")
    ground_truth(s, a1, b1, a2, b2, d_gt, a_gt, b_gt, msg_gt)

    # Test
    d = Real("d")
    a = Real("a")
    b = Real("b")
    msg = String("msg")
    
    if student == 'a':
        student_a(s, a1, b1, a2, b2, d, a, b, msg)
    elif student == 'b':
        student_b(s, a1, b1, a2, b2, d, a, b, msg)
    elif student == 'c':
        student_c(s, a1, b1, a2, b2, d, a, b, msg)

    # Check 
    s.add(
        Or(
            msg_gt!= msg,
            And(
                msg_gt == "Not INF",
                Or(
                    a_gt != a, 
                    b_gt != b
                )
            )
        )
    )
    
    if s.check() == sat:
        print("Sat")
        model = s.model()
        print("(a1, b1, a2, b2) : ", model[a1], model[b1], model[a2], model[b2])
        print("GT:   (a, b, d, msg) : ", model[a_gt], model[b_gt], model[d_gt], model[msg_gt])
        print("Test: (a, b, d, msg) : ", model[a], model[b], model[d], model[msg])
    else:
        print("unsat")


if __name__=="__main__":
    print("Student a")
    Verify('a')
    print("Student b")
    Verify('b')
    print("Student c")
    Verify('c')