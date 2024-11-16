; ModuleID = 'TestSet/Test.c'
source_filename = "TestSet/Test.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

@N = global i32 0, align 4
@T = global i32 0, align 4
@array = global [30 x i32] zeroinitializer, align 4
@.str = private unnamed_addr constant [5 x i8] c"%d%d\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@.str.3 = private unnamed_addr constant [4 x i8] c"YES\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"NO\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @groupSum(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store i32 %1, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = load i32, ptr @N, align 4
  %10 = sub nsw i32 %9, 1
  %11 = icmp eq i32 %8, %10
  br i1 %11, label %16, label %12

12:                                               ; preds = %2
  %13 = load i32, ptr %5, align 4
  %14 = load i32, ptr @T, align 4
  %15 = icmp sgt i32 %13, %14
  br i1 %15, label %16, label %17

16:                                               ; preds = %12, %2
  store i32 0, ptr %3, align 4
  br label %44

17:                                               ; preds = %12
  %18 = load i32, ptr %5, align 4
  %19 = load i32, ptr @T, align 4
  %20 = icmp eq i32 %18, %19
  br i1 %20, label %21, label %22

21:                                               ; preds = %17
  store i32 1, ptr %3, align 4
  br label %44

22:                                               ; preds = %17
  %23 = load i32, ptr %4, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, ptr %4, align 4
  %25 = load i32, ptr %4, align 4
  %26 = load i32, ptr %5, align 4
  %27 = call i32 @groupSum(i32 noundef %25, i32 noundef %26)
  store i32 %27, ptr %6, align 4
  %28 = load i32, ptr %4, align 4
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds [30 x i32], ptr @array, i64 0, i64 %29
  %31 = load i32, ptr %30, align 4
  %32 = load i32, ptr %5, align 4
  %33 = add nsw i32 %31, %32
  store i32 %33, ptr %5, align 4
  %34 = load i32, ptr %4, align 4
  %35 = load i32, ptr %5, align 4
  %36 = call i32 @groupSum(i32 noundef %34, i32 noundef %35)
  store i32 %36, ptr %7, align 4
  %37 = load i32, ptr %7, align 4
  %38 = icmp ne i32 %37, 0
  br i1 %38, label %42, label %39

39:                                               ; preds = %22
  %40 = load i32, ptr %6, align 4
  %41 = icmp ne i32 %40, 0
  br i1 %41, label %42, label %43

42:                                               ; preds = %39, %22
  store i32 1, ptr %3, align 4
  br label %44

43:                                               ; preds = %39
  store i32 0, ptr %3, align 4
  br label %44

44:                                               ; preds = %43, %42, %21, %16
  %45 = load i32, ptr %3, align 4
  ret i32 %45
}

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %3 = call i32 (ptr, ...) @scanf(ptr noundef @.str, ptr noundef @N, ptr noundef @T)
  store i32 0, ptr %2, align 4
  br label %4

4:                                                ; preds = %13, %0
  %5 = load i32, ptr %2, align 4
  %6 = load i32, ptr @N, align 4
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %16

8:                                                ; preds = %4
  %9 = load i32, ptr %2, align 4
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds [30 x i32], ptr @array, i64 0, i64 %10
  %12 = call i32 (ptr, ...) @scanf(ptr noundef @.str.1, ptr noundef %11)
  br label %13

13:                                               ; preds = %8
  %14 = load i32, ptr %2, align 4
  %15 = add nsw i32 %14, 1
  store i32 %15, ptr %2, align 4
  br label %4, !llvm.loop !5

16:                                               ; preds = %4
  %17 = call i32 @groupSum(i32 noundef 0, i32 noundef 0)
  %18 = icmp eq i32 %17, 1
  %19 = zext i1 %18 to i64
  %20 = select i1 %18, ptr @.str.3, ptr @.str.4
  %21 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, ptr noundef %20)
  ret i32 0
}

declare i32 @scanf(ptr noundef, ...) #1

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 1}
!3 = !{i32 7, !"frame-pointer", i32 1}
!4 = !{!"Homebrew clang version 19.1.3"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
