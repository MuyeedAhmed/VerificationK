from z3 import *


def ground_truth(s, x, y, r, x1, y1, d_gt, output_msg_gt):
    s.add(Or(output_msg_gt == "Point is inside the Circle.", output_msg_gt == "Point is on the Circle.", output_msg_gt == "Point is outside the Circle."))
    s.add(d_gt>=0)
    s.add(d_gt**2 == (x1-x)**2 + (y1-y)**2)
    
    s.add(Implies(d_gt < r, output_msg_gt == "Point is inside the Circle."))
    s.add(Implies(d_gt == r, output_msg_gt == "Point is on the Circle."))
    s.add(Implies(d_gt > r, output_msg_gt == "Point is outside the Circle."))
    
    
def student_a(s, x, y, r, x1, y1, d, output_msg):
    s.add(Or(output_msg == "Point is inside the Circle.", output_msg == "Point is on the Circle.", output_msg == "Point is outside the Circle."))
    
    s.add(d == (x-x1)*(x-x1)+(y-y1)*(y-y1)-r)
    
    s.add(Implies(d < r, output_msg == "Point is inside the Circle."))
    s.add(Implies(d == r, output_msg == "Point is on the Circle."))
    s.add(Implies(d > r, output_msg == "Point is outside the Circle."))

#271205_buggy
def student_b(s, x, y, r, x1, y1, d, output_msg):
    s.add(Or(output_msg == "Point is inside the Circle.", output_msg == "Point is on the Circle.", output_msg == "Point is outside the Circle."))
    
    s.add(d>=0)
    s.add(d**2 == (x-x1)*(x-x1)+(y-y1)*(y-y1))
    
    # s.add(Implies(d < r, output_msg == "Point is inside the Circle."))
    
    s.add(If(d > r, output_msg == "Point is outside the Circle.", output_msg == "Point is on the Circle."))
    
#271213_buggy
def student_c(s, x, y, r, x1, y1, d, output_msg):
    s.add(Or(output_msg == "Point is inside the Circle.", output_msg == "Point is on the Circle.", output_msg == "Point is outside the Circle."))
    s.add(d>=0)
    s.add(d**2 == (x-x1)*(x-x1)+(y-y1)*(y-y1))
    
    s.add(Implies(r > d, output_msg == "Point is inside the Circle."))
    s.add(Implies(r == d, output_msg == "Point is on the Circle."))
    s.add(Implies(r < d, output_msg == "Point is outside the Circle."))


        
def Verify(student):  
    s = Solver()

    x = Real("x")
    y = Real("y")
    r = Real("r")
    x1 = Real("x1")
    y1 = Real("y1")
    
    # s.add(x==-1.0, y==-2.0, r==5.0, x1== 1.5, y1== 2.0)
    # Ground Truth
    d_gt = Real("d_gt")
    output_msg_gt = String("output_msg_gt")
    
    ground_truth(s, x, y, r, x1, y1, d_gt, output_msg_gt)

    # Test
    d = Real("d")
    output_msg = String("output_msg")
    
    if student == 'a':
        student_a(s, x, y, r, x1, y1, d, output_msg)
    elif student == 'b':
        student_b(s, x, y, r, x1, y1, d, output_msg)
    elif student == 'c':
        student_c(s, x, y, r, x1, y1, d, output_msg)

    # Check 
    # s.push()
    s.add(output_msg_gt!= output_msg)
    
    if s.check() == sat:
        print("Sat")
        model = s.model()
        print("(x, y, r, x1, y1) : ", model[x], model[y], model[r], model[x1], model[y1])
        print("GT:   (d, msg) : ", model[d_gt], model[output_msg_gt])
        print("Test: (d, msg) : ", model[d], model[output_msg])
        return
    else:
        print("unsat")
    # s.pop()
    
    # s.push()
    # s.add(output_msg_gt == output_msg)
    # if s.check() == sat:
    #     print("unsat")
    # else:
    #     model = s.model()

    #     print("(x, y, r, x1, y1) : ", model[x], model[y], model[r], model[x1], model[y1])
    #     print("GT:   (d, msg) : ", model[d_gt], model[output_msg_gt])
    #     print("Test: (d, msg) : ", model[d], model[output_msg])
    #     print("Conflicting Condition")
    # s.pop()
        
        
if __name__=="__main__":
    print("Student a")
    Verify('a')
    print("Student b")
    Verify('b')
    print("Student c")
    Verify('c')