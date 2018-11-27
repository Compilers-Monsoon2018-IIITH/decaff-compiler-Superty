; ModuleID = 'module'
source_filename = "module"

@input = global [100 x i32] zeroinitializer
@0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@1 = private unnamed_addr constant [4 x i8] c"%d \00"
@2 = private unnamed_addr constant [4 x i8] c"%d \00"

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
  br i1 %0, label %"for i", label %"for_after i59"

"for i":                                          ; preds = %"for_internal i58", %entry
  %n3 = load i32, i32* %n1
  store i32 1, i32* %j
  %1 = icmp slt i32 1, %n3
  br i1 %1, label %"for j", label %"for_after j"

"for j":                                          ; preds = %"for_internal j", %"for i"
  %j4 = load i32, i32* %j
  %binop_sub = sub i32 %j4, 1
  %gep_check_lt_length = icmp slt i32 %binop_sub, 100
  %gep_check_nonneg = icmp sgt i32 %binop_sub, 0
  %gep_bounds_check = and i1 %gep_check_lt_length, %gep_check_nonneg
  br i1 %gep_bounds_check, label %gep_succesful_bounds_check, label %gep_bounds_check_fail

gep_bounds_check_fail:                            ; preds = %"for j"
  br label %gep_succesful_bounds_check

gep_succesful_bounds_check:                       ; preds = %gep_bounds_check_fail, %"for j"
  %"indexed location" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %binop_sub
  %"input[]" = load i32, i32* %"indexed location"
  %j5 = load i32, i32* %j
  %gep_check_lt_length6 = icmp slt i32 %j5, 100
  %gep_check_nonneg7 = icmp sgt i32 %j5, 0
  %gep_bounds_check8 = and i1 %gep_check_lt_length6, %gep_check_nonneg7
  br i1 %gep_bounds_check8, label %gep_succesful_bounds_check10, label %gep_bounds_check_fail9

gep_bounds_check_fail9:                           ; preds = %gep_succesful_bounds_check
  br label %gep_succesful_bounds_check10

gep_succesful_bounds_check10:                     ; preds = %gep_bounds_check_fail9, %gep_succesful_bounds_check
  %"indexed location11" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %j5
  %"input[]12" = load i32, i32* %"indexed location11"
  %binop_gt = icmp sgt i32 %"input[]", %"input[]12"
  br i1 %binop_gt, label %then, label %if_then_merge

then:                                             ; preds = %gep_succesful_bounds_check10
  %j13 = load i32, i32* %j
  %binop_sub14 = sub i32 %j13, 1
  %gep_check_lt_length15 = icmp slt i32 %binop_sub14, 100
  %gep_check_nonneg16 = icmp sgt i32 %binop_sub14, 0
  %gep_bounds_check17 = and i1 %gep_check_lt_length15, %gep_check_nonneg16
  br i1 %gep_bounds_check17, label %gep_succesful_bounds_check19, label %gep_bounds_check_fail18

gep_bounds_check_fail18:                          ; preds = %then
  br label %gep_succesful_bounds_check19

gep_succesful_bounds_check19:                     ; preds = %gep_bounds_check_fail18, %then
  %"indexed location20" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %binop_sub14
  %"input[]21" = load i32, i32* %"indexed location20"
  store i32 %"input[]21", i32* %temp
  %j22 = load i32, i32* %j
  %gep_check_lt_length23 = icmp slt i32 %j22, 100
  %gep_check_nonneg24 = icmp sgt i32 %j22, 0
  %gep_bounds_check25 = and i1 %gep_check_lt_length23, %gep_check_nonneg24
  br i1 %gep_bounds_check25, label %gep_succesful_bounds_check27, label %gep_bounds_check_fail26

gep_bounds_check_fail26:                          ; preds = %gep_succesful_bounds_check19
  br label %gep_succesful_bounds_check27

gep_succesful_bounds_check27:                     ; preds = %gep_bounds_check_fail26, %gep_succesful_bounds_check19
  %"indexed location28" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %j22
  %"input[]29" = load i32, i32* %"indexed location28"
  %j30 = load i32, i32* %j
  %binop_sub31 = sub i32 %j30, 1
  %gep_check_lt_length32 = icmp slt i32 %binop_sub31, 100
  %gep_check_nonneg33 = icmp sgt i32 %binop_sub31, 0
  %gep_bounds_check34 = and i1 %gep_check_lt_length32, %gep_check_nonneg33
  br i1 %gep_bounds_check34, label %gep_succesful_bounds_check36, label %gep_bounds_check_fail35

gep_bounds_check_fail35:                          ; preds = %gep_succesful_bounds_check27
  br label %gep_succesful_bounds_check36

gep_succesful_bounds_check36:                     ; preds = %gep_bounds_check_fail35, %gep_succesful_bounds_check27
  %"indexed location37" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %binop_sub31
  store i32 %"input[]29", i32* %"indexed location37"
  %temp38 = load i32, i32* %temp
  %j39 = load i32, i32* %j
  %gep_check_lt_length40 = icmp slt i32 %j39, 100
  %gep_check_nonneg41 = icmp sgt i32 %j39, 0
  %gep_bounds_check42 = and i1 %gep_check_lt_length40, %gep_check_nonneg41
  br i1 %gep_bounds_check42, label %gep_succesful_bounds_check44, label %gep_bounds_check_fail43

gep_bounds_check_fail43:                          ; preds = %gep_succesful_bounds_check36
  br label %gep_succesful_bounds_check44

gep_succesful_bounds_check44:                     ; preds = %gep_bounds_check_fail43, %gep_succesful_bounds_check36
  %"indexed location45" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %j39
  store i32 %temp38, i32* %"indexed location45"
  br label %if_then_merge

if_then_merge:                                    ; preds = %gep_succesful_bounds_check44, %gep_succesful_bounds_check10
  br label %"for_internal j"

"for_internal j":                                 ; preds = %if_then_merge
  %2 = load i32, i32* %j
  %3 = add i32 %2, 1
  store i32 %3, i32* %j
  %4 = icmp slt i32 %3, %n3
  br i1 %4, label %"for j", label %"for_after j"

"for_after j":                                    ; preds = %"for_internal j", %"for i"
  %callout_call = call i32 @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @0, i32 0, i32 0), i32 1)
  %n46 = load i32, i32* %n1
  store i32 0, i32* %i48
  %5 = icmp slt i32 0, %n46
  br i1 %5, label %"for i47", label %"for_after i"

"for i47":                                        ; preds = %"for_internal i", %"for_after j"
  %i49 = load i32, i32* %i48
  %gep_check_lt_length50 = icmp slt i32 %i49, 100
  %gep_check_nonneg51 = icmp sgt i32 %i49, 0
  %gep_bounds_check52 = and i1 %gep_check_lt_length50, %gep_check_nonneg51
  br i1 %gep_bounds_check52, label %gep_succesful_bounds_check54, label %gep_bounds_check_fail53

gep_bounds_check_fail53:                          ; preds = %"for i47"
  br label %gep_succesful_bounds_check54

gep_succesful_bounds_check54:                     ; preds = %gep_bounds_check_fail53, %"for i47"
  %"indexed location55" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i49
  %"input[]56" = load i32, i32* %"indexed location55"
  %callout_call57 = call i32 @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %"input[]56")
  br label %"for_internal i"

"for_internal i":                                 ; preds = %gep_succesful_bounds_check54
  %6 = load i32, i32* %i48
  %7 = add i32 %6, 1
  store i32 %7, i32* %i48
  %8 = icmp slt i32 %7, %n46
  br i1 %8, label %"for i47", label %"for_after i"

"for_after i":                                    ; preds = %"for_internal i", %"for_after j"
  br label %"for_internal i58"

"for_internal i58":                               ; preds = %"for_after i"
  %9 = load i32, i32* %i
  %10 = add i32 %9, 1
  store i32 %10, i32* %i
  %11 = icmp slt i32 %10, %n2
  br i1 %11, label %"for i", label %"for_after i59"

"for_after i59":                                  ; preds = %"for_internal i58", %entry
  ret void
}

declare i32 @printf(i8*, i32)

define private i32 @getInt() {
entry:
  %thisLanguageHasNoWhileLoop = alloca i32
  %input = alloca i32
  %ret = alloca i32
  store i32 0, i32* %ret
  store i32 0, i32* %input
  store i32 0, i32* %ret
  store i32 1, i32* %thisLanguageHasNoWhileLoop
  br i1 true, label %"for thisLanguageHasNoWhileLoop", label %"for_after thisLanguageHasNoWhileLoop"

"for thisLanguageHasNoWhileLoop":                 ; preds = %"for_internal thisLanguageHasNoWhileLoop", %entry
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

then:                                             ; preds = %"for thisLanguageHasNoWhileLoop"
  %ret4 = load i32, i32* %ret
  %binop_mult = mul i32 10, %ret4
  %input5 = load i32, i32* %input
  %binop_add = add i32 %binop_mult, %input5
  %binop_sub = sub i32 %binop_add, 48
  store i32 %binop_sub, i32* %ret
  br label %if_then_merge

else:                                             ; preds = %"for thisLanguageHasNoWhileLoop"
  br label %"for_after thisLanguageHasNoWhileLoop"

if_then_merge:                                    ; preds = %then
  br label %"for_internal thisLanguageHasNoWhileLoop"

"for_internal thisLanguageHasNoWhileLoop":        ; preds = %if_then_merge
  %0 = load i32, i32* %thisLanguageHasNoWhileLoop
  %1 = add i32 %0, 1
  store i32 %1, i32* %thisLanguageHasNoWhileLoop
  %2 = icmp slt i32 %1, 2
  br i1 %2, label %"for thisLanguageHasNoWhileLoop", label %"for_after thisLanguageHasNoWhileLoop"

"for_after thisLanguageHasNoWhileLoop":           ; preds = %"for_internal thisLanguageHasNoWhileLoop", %else, %entry
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
  br i1 %0, label %"for i", label %"for_after i"

"for i":                                          ; preds = %"for_internal i", %entry
  %call2 = call i32 @getInt()
  %i3 = load i32, i32* %i
  %gep_check_lt_length = icmp slt i32 %i3, 100
  %gep_check_nonneg = icmp sgt i32 %i3, 0
  %gep_bounds_check = and i1 %gep_check_lt_length, %gep_check_nonneg
  br i1 %gep_bounds_check, label %gep_succesful_bounds_check, label %gep_bounds_check_fail

gep_bounds_check_fail:                            ; preds = %"for i"
  br label %gep_succesful_bounds_check

gep_succesful_bounds_check:                       ; preds = %gep_bounds_check_fail, %"for i"
  %"indexed location" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i3
  store i32 %call2, i32* %"indexed location"
  %i4 = load i32, i32* %i
  %gep_check_lt_length5 = icmp slt i32 %i4, 100
  %gep_check_nonneg6 = icmp sgt i32 %i4, 0
  %gep_bounds_check7 = and i1 %gep_check_lt_length5, %gep_check_nonneg6
  br i1 %gep_bounds_check7, label %gep_succesful_bounds_check9, label %gep_bounds_check_fail8

gep_bounds_check_fail8:                           ; preds = %gep_succesful_bounds_check
  br label %gep_succesful_bounds_check9

gep_succesful_bounds_check9:                      ; preds = %gep_bounds_check_fail8, %gep_succesful_bounds_check
  %"indexed location10" = getelementptr [100 x i32], [100 x i32]* @input, i32 0, i32 %i4
  %"input[]" = load i32, i32* %"indexed location10"
  %callout_call = call i32 @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %"input[]")
  br label %"for_internal i"

"for_internal i":                                 ; preds = %gep_succesful_bounds_check9
  %1 = load i32, i32* %i
  %2 = add i32 %1, 1
  store i32 %2, i32* %i
  %3 = icmp slt i32 %2, %n1
  br i1 %3, label %"for i", label %"for_after i"

"for_after i":                                    ; preds = %"for_internal i", %entry
  %n11 = load i32, i32* %n
  call void @bubble(i32 %n11)
  ret i32 0
}
