; ModuleID = 'module'
source_filename = "module"

@input = global [100 x i32] zeroinitializer
@0 = private unnamed_addr constant [3 x i8] c"%d\00"
@1 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@2 = private unnamed_addr constant [4 x i8] c"%d \00"
@3 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"

define private i32 @getInt() {
entry:
  %idx = alloca i32
  %input = alloca i32
  %ret = alloca i32
  store i32 0, i32* %ret
  store i32 0, i32* %input
  store i32 0, i32* %ret
  store i32 1, i32* %idx
  br i1 true, label %for_idx, label %for_after_idx

for_idx:                                          ; preds = %for_internal_idx, %entry
  %idx1 = load i32, i32* %idx
  %minus_assign = sub i32 %idx1, 1
  store i32 %minus_assign, i32* %idx
  %0 = call i32 @getchar()
  store i32 %0, i32* %input
  %input2 = load i32, i32* %input
  %binop_ge = icmp sge i32 %input2, 48
  %input3 = load i32, i32* %input
  %binop_le = icmp sle i32 %input3, 57
  %binop_and = and i1 %binop_ge, %binop_le
  br i1 %binop_and, label %then, label %else

then:                                             ; preds = %for_idx
  %ret4 = load i32, i32* %ret
  %binop_mult = mul i32 10, %ret4
  %input5 = load i32, i32* %input
  %binop_add = add i32 %binop_mult, %input5
  %binop_sub = sub i32 %binop_add, 48
  store i32 %binop_sub, i32* %ret
  br label %if_then_merge

else:                                             ; preds = %for_idx
  br label %for_after_idx

if_then_merge:                                    ; preds = %then
  br label %for_internal_idx

for_internal_idx:                                 ; preds = %if_then_merge
  %1 = load i32, i32* %idx
  %2 = add i32 %1, 1
  store i32 %2, i32* %idx
  %3 = icmp slt i32 %2, 2
  br i1 %3, label %for_idx, label %for_after_idx

for_after_idx:                                    ; preds = %for_internal_idx, %else, %entry
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
  %0 = call i32 @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 %n1)
  %n2 = load i32, i32* %n
  store i32 0, i32* %i
  %1 = icmp slt i32 0, %n2
  br i1 %1, label %for_i, label %for_after_i

for_i:                                            ; preds = %for_internal_i, %entry
  %call3 = call i32 @getInt()
  %i4 = load i32, i32* %i
  %gep_check_lt_length = icmp slt i32 %i4, 100
  %gep_check_nonneg = icmp sge i32 %i4, 0
  %gep_bounds_check = and i1 %gep_check_lt_length, %gep_check_nonneg
  br i1 %gep_bounds_check, label %gep_succesful_bounds_check, label %gep_bounds_check_fail

gep_bounds_check_fail:                            ; preds = %for_i
  %2 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @1, i32 0, i32 0))
  call void @exit(i32 1)
  ret i32 0

gep_succesful_bounds_check:                       ; preds = %for_i
  %"indexed location" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i4
  store i32 %call3, i32* %"indexed location"
  %i5 = load i32, i32* %i
  %gep_check_lt_length6 = icmp slt i32 %i5, 100
  %gep_check_nonneg7 = icmp sge i32 %i5, 0
  %gep_bounds_check8 = and i1 %gep_check_lt_length6, %gep_check_nonneg7
  br i1 %gep_bounds_check8, label %gep_succesful_bounds_check10, label %gep_bounds_check_fail9

gep_bounds_check_fail9:                           ; preds = %gep_succesful_bounds_check
  %3 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @3, i32 0, i32 0))
  call void @exit(i32 1)
  ret i32 0

gep_succesful_bounds_check10:                     ; preds = %gep_succesful_bounds_check
  %"indexed location11" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i5
  %"input[]" = load i32, i32* %"indexed location11"
  %4 = call i32 @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %"input[]")
  br label %for_internal_i

for_internal_i:                                   ; preds = %gep_succesful_bounds_check10
  %5 = load i32, i32* %i
  %6 = add i32 %5, 1
  store i32 %6, i32* %i
  %7 = icmp slt i32 %6, %n2
  br i1 %7, label %for_i, label %for_after_i

for_after_i:                                      ; preds = %for_internal_i, %entry
  ret i32 0
}

declare i32 @printf(i8*, i32)

declare void @exit(i32)
