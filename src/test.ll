; ModuleID = 'module'
source_filename = "module"

@input = global [100 x i32] zeroinitializer
@0 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@1 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@2 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@3 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@4 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@5 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@6 = private unnamed_addr constant [2 x i8] c"\0A\00"
@7 = private unnamed_addr constant [4 x i8] c"%d \00"
@8 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@9 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@10 = private unnamed_addr constant [4 x i8] c"%d \00"
@11 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"

define private void @bubble(i32 %n) {
entry:
  %i48 = alloca i32
  %j = alloca i32
  %i = alloca i32
  %temp = alloca i32
  %n1 = alloca i32
  store i32 %n, i32* %n1
  store i32 0, i32* %temp
  %n2 = load i32, i32* %n1
  store i32 0, i32* %i
  %0 = icmp slt i32 0, %n2
  br i1 %0, label %for_i, label %for_after_i58

for_i:                                            ; preds = %for_internal_i57, %entry
  %n3 = load i32, i32* %n1
  store i32 1, i32* %j
  %1 = icmp slt i32 1, %n3
  br i1 %1, label %for_j, label %for_after_j

for_j:                                            ; preds = %for_internal_j, %for_i
  %j4 = load i32, i32* %j
  %binop_sub = sub i32 %j4, 1
  %gep_check_lt_length = icmp slt i32 %binop_sub, 100
  %gep_check_nonneg = icmp sge i32 %binop_sub, 0
  %gep_bounds_check = and i1 %gep_check_lt_length, %gep_check_nonneg
  br i1 %gep_bounds_check, label %gep_succesful_bounds_check, label %gep_bounds_check_fail

gep_bounds_check_fail:                            ; preds = %for_j
  %2 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @0, i32 0, i32 0))
  call void @exit(i32 1)
  ret void

gep_succesful_bounds_check:                       ; preds = %for_j
  %"indexed location" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %binop_sub
  %"input[]" = load i32, i32* %"indexed location"
  %j5 = load i32, i32* %j
  %gep_check_lt_length6 = icmp slt i32 %j5, 100
  %gep_check_nonneg7 = icmp sge i32 %j5, 0
  %gep_bounds_check8 = and i1 %gep_check_lt_length6, %gep_check_nonneg7
  br i1 %gep_bounds_check8, label %gep_succesful_bounds_check10, label %gep_bounds_check_fail9

gep_bounds_check_fail9:                           ; preds = %gep_succesful_bounds_check
  %3 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @1, i32 0, i32 0))
  call void @exit(i32 1)
  ret void

gep_succesful_bounds_check10:                     ; preds = %gep_succesful_bounds_check
  %"indexed location11" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %j5
  %"input[]12" = load i32, i32* %"indexed location11"
  %binop_gt = icmp sgt i32 %"input[]", %"input[]12"
  br i1 %binop_gt, label %then, label %if_then_merge

then:                                             ; preds = %gep_succesful_bounds_check10
  %j13 = load i32, i32* %j
  %binop_sub14 = sub i32 %j13, 1
  %gep_check_lt_length15 = icmp slt i32 %binop_sub14, 100
  %gep_check_nonneg16 = icmp sge i32 %binop_sub14, 0
  %gep_bounds_check17 = and i1 %gep_check_lt_length15, %gep_check_nonneg16
  br i1 %gep_bounds_check17, label %gep_succesful_bounds_check19, label %gep_bounds_check_fail18

gep_bounds_check_fail18:                          ; preds = %then
  %4 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @2, i32 0, i32 0))
  call void @exit(i32 1)
  ret void

gep_succesful_bounds_check19:                     ; preds = %then
  %"indexed location20" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %binop_sub14
  %"input[]21" = load i32, i32* %"indexed location20"
  store i32 %"input[]21", i32* %temp
  %j22 = load i32, i32* %j
  %gep_check_lt_length23 = icmp slt i32 %j22, 100
  %gep_check_nonneg24 = icmp sge i32 %j22, 0
  %gep_bounds_check25 = and i1 %gep_check_lt_length23, %gep_check_nonneg24
  br i1 %gep_bounds_check25, label %gep_succesful_bounds_check27, label %gep_bounds_check_fail26

gep_bounds_check_fail26:                          ; preds = %gep_succesful_bounds_check19
  %5 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @3, i32 0, i32 0))
  call void @exit(i32 1)
  ret void

gep_succesful_bounds_check27:                     ; preds = %gep_succesful_bounds_check19
  %"indexed location28" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %j22
  %"input[]29" = load i32, i32* %"indexed location28"
  %j30 = load i32, i32* %j
  %binop_sub31 = sub i32 %j30, 1
  %gep_check_lt_length32 = icmp slt i32 %binop_sub31, 100
  %gep_check_nonneg33 = icmp sge i32 %binop_sub31, 0
  %gep_bounds_check34 = and i1 %gep_check_lt_length32, %gep_check_nonneg33
  br i1 %gep_bounds_check34, label %gep_succesful_bounds_check36, label %gep_bounds_check_fail35

gep_bounds_check_fail35:                          ; preds = %gep_succesful_bounds_check27
  %6 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @4, i32 0, i32 0))
  call void @exit(i32 1)
  ret void

gep_succesful_bounds_check36:                     ; preds = %gep_succesful_bounds_check27
  %"indexed location37" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %binop_sub31
  store i32 %"input[]29", i32* %"indexed location37"
  %temp38 = load i32, i32* %temp
  %j39 = load i32, i32* %j
  %gep_check_lt_length40 = icmp slt i32 %j39, 100
  %gep_check_nonneg41 = icmp sge i32 %j39, 0
  %gep_bounds_check42 = and i1 %gep_check_lt_length40, %gep_check_nonneg41
  br i1 %gep_bounds_check42, label %gep_succesful_bounds_check44, label %gep_bounds_check_fail43

gep_bounds_check_fail43:                          ; preds = %gep_succesful_bounds_check36
  %7 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @5, i32 0, i32 0))
  call void @exit(i32 1)
  ret void

gep_succesful_bounds_check44:                     ; preds = %gep_succesful_bounds_check36
  %"indexed location45" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %j39
  store i32 %temp38, i32* %"indexed location45"
  br label %if_then_merge

if_then_merge:                                    ; preds = %gep_succesful_bounds_check44, %gep_succesful_bounds_check10
  br label %for_internal_j

for_internal_j:                                   ; preds = %if_then_merge
  %8 = load i32, i32* %j
  %9 = add i32 %8, 1
  store i32 %9, i32* %j
  %10 = icmp slt i32 %9, %n3
  br i1 %10, label %for_j, label %for_after_j

for_after_j:                                      ; preds = %for_internal_j, %for_i
  %11 = call i32 bitcast (i32 (i8*)* @printf to i32 (i8*, i32)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @6, i32 0, i32 0), i32 1)
  %n46 = load i32, i32* %n1
  store i32 0, i32* %i48
  %12 = icmp slt i32 0, %n46
  br i1 %12, label %for_i47, label %for_after_i

for_i47:                                          ; preds = %for_internal_i, %for_after_j
  %i49 = load i32, i32* %i48
  %gep_check_lt_length50 = icmp slt i32 %i49, 100
  %gep_check_nonneg51 = icmp sge i32 %i49, 0
  %gep_bounds_check52 = and i1 %gep_check_lt_length50, %gep_check_nonneg51
  br i1 %gep_bounds_check52, label %gep_succesful_bounds_check54, label %gep_bounds_check_fail53

gep_bounds_check_fail53:                          ; preds = %for_i47
  %13 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @8, i32 0, i32 0))
  call void @exit(i32 1)
  ret void

gep_succesful_bounds_check54:                     ; preds = %for_i47
  %"indexed location55" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i49
  %"input[]56" = load i32, i32* %"indexed location55"
  %14 = call i32 bitcast (i32 (i8*)* @printf to i32 (i8*, i32)*)(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @7, i32 0, i32 0), i32 %"input[]56")
  br label %for_internal_i

for_internal_i:                                   ; preds = %gep_succesful_bounds_check54
  %15 = load i32, i32* %i48
  %16 = add i32 %15, 1
  store i32 %16, i32* %i48
  %17 = icmp slt i32 %16, %n46
  br i1 %17, label %for_i47, label %for_after_i

for_after_i:                                      ; preds = %for_internal_i, %for_after_j
  br label %for_internal_i57

for_internal_i57:                                 ; preds = %for_after_i
  %18 = load i32, i32* %i
  %19 = add i32 %18, 1
  store i32 %19, i32* %i
  %20 = icmp slt i32 %19, %n2
  br i1 %20, label %for_i, label %for_after_i58

for_after_i58:                                    ; preds = %for_internal_i57, %entry
  ret void
}

declare i32 @printf(i8*)

declare void @exit(i32)

define private i32 @getInt() {
entry:
  %thisLanguageHasNoWhileLoop = alloca i32
  %input = alloca i32
  %ret = alloca i32
  store i32 0, i32* %ret
  store i32 0, i32* %input
  store i32 0, i32* %ret
  store i32 1, i32* %thisLanguageHasNoWhileLoop
  br i1 true, label %for_thisLanguageHasNoWhileLoop, label %for_after_thisLanguageHasNoWhileLoop

for_thisLanguageHasNoWhileLoop:                   ; preds = %for_internal_thisLanguageHasNoWhileLoop, %entry
  %thisLanguageHasNoWhileLoop1 = load i32, i32* %thisLanguageHasNoWhileLoop
  %minus_assign = sub i32 %thisLanguageHasNoWhileLoop1, 1
  store i32 %minus_assign, i32* %thisLanguageHasNoWhileLoop
  %0 = call i32 @getchar()
  store i32 %0, i32* %input
  %input2 = load i32, i32* %input
  %binop_ge = icmp sge i32 %input2, 48
  %input3 = load i32, i32* %input
  %binop_le = icmp sle i32 %input3, 57
  %binop_and = and i1 %binop_ge, %binop_le
  br i1 %binop_and, label %then, label %else

then:                                             ; preds = %for_thisLanguageHasNoWhileLoop
  %ret4 = load i32, i32* %ret
  %binop_mult = mul i32 10, %ret4
  %input5 = load i32, i32* %input
  %binop_add = add i32 %binop_mult, %input5
  %binop_sub = sub i32 %binop_add, 48
  store i32 %binop_sub, i32* %ret
  br label %if_then_merge

else:                                             ; preds = %for_thisLanguageHasNoWhileLoop
  br label %for_after_thisLanguageHasNoWhileLoop

if_then_merge:                                    ; preds = %then
  br label %for_internal_thisLanguageHasNoWhileLoop

for_internal_thisLanguageHasNoWhileLoop:          ; preds = %if_then_merge
  %1 = load i32, i32* %thisLanguageHasNoWhileLoop
  %2 = add i32 %1, 1
  store i32 %2, i32* %thisLanguageHasNoWhileLoop
  %3 = icmp slt i32 %2, 2
  br i1 %3, label %for_thisLanguageHasNoWhileLoop, label %for_after_thisLanguageHasNoWhileLoop

for_after_thisLanguageHasNoWhileLoop:             ; preds = %for_internal_thisLanguageHasNoWhileLoop, %else, %entry
  %ret6 = load i32, i32* %ret
  ret i32 %ret6
}

declare i32 @getchar()

define i32 @main() {
entry:
  %i = alloca i32
  %n = alloca i32
  store i32 0, i32* %n
  %call = call i32 @getInt()
  store i32 %call, i32* %n
  %n1 = load i32, i32* %n
  store i32 0, i32* %i
  %0 = icmp slt i32 0, %n1
  br i1 %0, label %for_i, label %for_after_i

for_i:                                            ; preds = %for_internal_i, %entry
  %call2 = call i32 @getInt()
  %i3 = load i32, i32* %i
  %gep_check_lt_length = icmp slt i32 %i3, 100
  %gep_check_nonneg = icmp sge i32 %i3, 0
  %gep_bounds_check = and i1 %gep_check_lt_length, %gep_check_nonneg
  br i1 %gep_bounds_check, label %gep_succesful_bounds_check, label %gep_bounds_check_fail

gep_bounds_check_fail:                            ; preds = %for_i
  %1 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @9, i32 0, i32 0))
  call void @exit(i32 1)
  ret i32 0

gep_succesful_bounds_check:                       ; preds = %for_i
  %"indexed location" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i3
  store i32 %call2, i32* %"indexed location"
  %i4 = load i32, i32* %i
  %gep_check_lt_length5 = icmp slt i32 %i4, 100
  %gep_check_nonneg6 = icmp sge i32 %i4, 0
  %gep_bounds_check7 = and i1 %gep_check_lt_length5, %gep_check_nonneg6
  br i1 %gep_bounds_check7, label %gep_succesful_bounds_check9, label %gep_bounds_check_fail8

gep_bounds_check_fail8:                           ; preds = %gep_succesful_bounds_check
  %2 = call i32 @printf(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @11, i32 0, i32 0))
  call void @exit(i32 1)
  ret i32 0

gep_succesful_bounds_check9:                      ; preds = %gep_succesful_bounds_check
  %"indexed location10" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i4
  %"input[]" = load i32, i32* %"indexed location10"
  %3 = call i32 bitcast (i32 (i8*)* @printf to i32 (i8*, i32)*)(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @10, i32 0, i32 0), i32 %"input[]")
  br label %for_internal_i

for_internal_i:                                   ; preds = %gep_succesful_bounds_check9
  %4 = load i32, i32* %i
  %5 = add i32 %4, 1
  store i32 %5, i32* %i
  %6 = icmp slt i32 %5, %n1
  br i1 %6, label %for_i, label %for_after_i

for_after_i:                                      ; preds = %for_internal_i, %entry
  %n11 = load i32, i32* %n
  call void @bubble(i32 %n11)
  ret i32 0
}
