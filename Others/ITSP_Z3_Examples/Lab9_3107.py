from z3 import *

# int groupSum(int len_covered, int partial_sum)
# {
# 	if(len_covered == N)
# 		return partial_sum == T;
# 	else if(partial_sum == T)
# 	    return 1;
# 	else
# 		return ( groupSum(len_covered+1, partial_sum+array[len_covered]) || groupSum(len_covered+1, partial_sum) );
# }


def ground_truth(s, array, N, T, PartialSum_gt):
    def groupSum(len_covered, partial_sum):
        if len_covered == N:
            return partial_sum == T
        else:
            include_current = groupSum(len_covered + 1, partial_sum + Select(array, len_covered))
            exclude_current = groupSum(len_covered + 1, partial_sum)
            return Or(include_current, exclude_current)
    s.add(If(groupSum(0, 0), PartialSum_gt == "YES", PartialSum_gt == "NO"))
    
#300581
def student_a(s, array, N, T, PartialSum):
    T_a = Int('T_a')
    def groupSum(len_covered, partial_sum):
        if partial_sum == T_a:
            return True
        if len_covered == N:
            return partial_sum == T_a
        else:
            partial_sum=partial_sum + Select(array, len_covered)
            len_covered += 1
            include_current = groupSum(len_covered, partial_sum - Select(array, (len_covered-1)))
            exclude_current = groupSum(len_covered, partial_sum)
            return Or(include_current, exclude_current)
    s.add(If(groupSum(0, 0), PartialSum == "YES", PartialSum == "NO"))

# int groupSum(int len_covered, int partial_sum){
#     if(partial_sum==0){
#         return 1;
#     }
#     if(len_covered>=N){
#         return 0;
#     }
#     int x=0;
#     if(array[len_covered]<partial_sum){
#         x= groupSum(++len_covered,partial_sum-array[len_covered]);
#     }
#     return groupSum(++len_covered,partial_sum)||x;
# }

#300486
def student_b(s, array, N, T, PartialSum):
    def groupSum(len_covered, partial_sum):
        if partial_sum == T:
            return True
        if len_covered >= N:
            return partial_sum == T
        
        include_current = groupSum(len_covered + 1, partial_sum - Select(array, len_covered))
        exclude_current = groupSum(len_covered + 1, partial_sum)
        return If(Select(array, len_covered) < partial_sum, Or(include_current, exclude_current), exclude_current)

    s.add(If(groupSum(0, 0), PartialSum == "YES", PartialSum == "NO"))
    s.add(PartialSum == "NO")



        
def Verify(student, N):  
    s = Solver()
    array = Array('array', IntSort(), IntSort())

    T = Int('T')
    
    # Ground Truth
    PartialSum_gt = String("PartialSum_gt")
    ground_truth(s, array, N, T, PartialSum_gt)

    # Test
    PartialSum = String("PartialSum")
    
    if student == 'a':
        student_a(s, array, N, T, PartialSum)
    elif student == 'b':
        student_b(s, array, N, T, PartialSum)

    # Check 
    s.add(PartialSum_gt != PartialSum)
    
    # for constraint in s.assertions():
    #     print(constraint)
    if s.check() == sat:
        print("Sat")
        
        model = s.model()
        result_array = []
        for i in range(N):
            result_array.append(model.evaluate(Select(array, i)))
        print("Array:", result_array)

        print("T: ", model[T])
        print("GT   : ", model[PartialSum_gt])
        print("Test : ", model[PartialSum])
    else:
        print("unsat")


if __name__=="__main__":
    print("Student a")
    Verify('a', 3)
    print("Student b")
    Verify('b', 3)