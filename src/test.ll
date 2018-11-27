; ModuleID = 'module'
source_filename = "module"

@input = global [100 x i32] zeroinitializer
@0 = private unnamed_addr constant [3 x i8] c"%d\00"
@1 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"
@2 = private unnamed_addr constant [4 x i8] c"%d \00"
@3 = private unnamed_addr constant [54 x i8] c"\0Aruntime error: array access to input out of bounds.\0A\00"

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
  %callout_call = call i32 @getchar()
  store i32 %callout_call, i32* %input
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
  %0 = load i32, i32* %thisLanguageHasNoWhileLoop
  %1 = add i32 %0, 1
  store i32 %1, i32* %thisLanguageHasNoWhileLoop
  %2 = icmp slt i32 %1, 2
  br i1 %2, label %for_thisLanguageHasNoWhileLoop, label %for_after_thisLanguageHasNoWhileLoop

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
  %callout_call = call i32 @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 %n1)
  %n2 = load i32, i32* %n
  store i32 0, i32* %i
  %0 = icmp slt i32 0, %n2
  br i1 %0, label %for_i, label %for_after_i

for_i:                                            ; preds = %for_internal_i, %entry
  %call3 = call i32 @getInt()
  %i4 = load i32, i32* %i
  %gep_check_lt_length = icmp slt i32 %i4, 100
  %gep_check_nonneg = icmp sgt i32 %i4, 0
  %gep_bounds_check = and i1 %gep_check_lt_length, %gep_check_nonneg
  br i1 %gep_bounds_check, label %gep_succesful_bounds_check, label %gep_bounds_check_fail

gep_bounds_check_fail:                            ; preds = %for_i
  %callout_call5 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @1, i32 0, i32 0))
  %callout_call6 = call i32 @exit(i32 1)
  ret i32 0

gep_succesful_bounds_check:                       ; preds = %for_i
  %"indexed location" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i4
  store i32 %call3, i32* %"indexed location"
  %i7 = load i32, i32* %i
  %gep_check_lt_length8 = icmp slt i32 %i7, 100
  %gep_check_nonneg9 = icmp sgt i32 %i7, 0
  %gep_bounds_check10 = and i1 %gep_check_lt_length8, %gep_check_nonneg9
  br i1 %gep_bounds_check10, label %gep_succesful_bounds_check14, label %gep_bounds_check_fail11

gep_bounds_check_fail11:                          ; preds = %gep_succesful_bounds_check
  %callout_call12 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([54 x i8], [54 x i8]* @3, i32 0, i32 0))
  %callout_call13 = call i32 @exit(i32 1)
  ret i32 0

gep_succesful_bounds_check14:                     ; preds = %gep_succesful_bounds_check
  %"indexed location15" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i7
  %"input[]" = load i32, i32* %"indexed location15"
  %callout_call16 = call i32 @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %"input[]")
  br label %for_internal_i

for_internal_i:                                   ; preds = %gep_succesful_bounds_check14
  %1 = load i32, i32* %i
  %2 = add i32 %1, 1
  store i32 %2, i32* %i
  %3 = icmp slt i32 %2, %n2
  br i1 %3, label %for_i, label %for_after_i

for_after_i:                                      ; preds = %for_internal_i, %entry
  ret i32 0
}

declare i32 @printf(i8*, i32)

declare i32 @exit(i32)
