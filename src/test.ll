; ModuleID = 'module'
source_filename = "module"

@0 = private unnamed_addr constant [3 x i8] c"%d\00"
@1 = private unnamed_addr constant [9 x i8] c"\0Asdfasdf\00"
@2 = private unnamed_addr constant [63 x i8] c"\0Aruntime error: reached end of function with non-void return.\0A\00"

define i32 @main() {
entry:
  %callout_call = call i32 @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 1)
  %callout_call1 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @1, i32 0, i32 0))
  %callout_call2 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([63 x i8], [63 x i8]* @2, i32 0, i32 0))
  %callout_call3 = call i32 @exit(i32 1)
  ret i32 0
}

declare i32 @printf(i8*, i32)

declare i32 @exit(i32)
