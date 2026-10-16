bits 64
default rel

extern printf
global run_coa_calculation
global roadA
global roadB
global roadC
global roadD
global total
global highest
global signalTime

%ifdef STANDALONE_COA
global main
%else
global coa_main
%endif

section .data

roadA dq 40
roadB dq 15
roadC dq 25
roadD dq 10

signalMultiplier dq 2

msgA db "Road A = %llu", 10, 0
msgB db "Road B = %llu", 10, 0
msgC db "Road C = %llu", 10, 0
msgD db "Road D = %llu", 10, 0

msgTotal db "Total Traffic = %llu", 10, 0
msgSignal db "Signal Time for Highest Traffic Road = %llu seconds", 10, 0
msgHigh db "Highest Traffic = %llu", 10, 0

section .bss

total resq 1
highest resq 1
signalTime resq 1

section .text

run_coa_calculation:
    ; --------------------------------
    ; Operation 1: Calculate Total Traffic
    ; Total = A + B + C + D
    ; --------------------------------

    mov rax, [roadA]
    add rax, [roadB]
    add rax, [roadC]
    add rax, [roadD]

    mov [total], rax


    ; --------------------------------
    ; Find Highest Traffic Road
    ; --------------------------------

    mov rax, [roadA]

    cmp rax, [roadB]
    jge checkC
    mov rax, [roadB]

checkC:
    cmp rax, [roadC]
    jge checkD
    mov rax, [roadC]

checkD:
    cmp rax, [roadD]
    jge saveHighest
    mov rax, [roadD]

saveHighest:
    mov [highest], rax


    ; --------------------------------
    ; Operation 2: Calculate Signal Time
    ; Signal Time = Highest Traffic × 2
    ; --------------------------------

    mov rax, [highest]
    mov rbx, [signalMultiplier]

    imul rax, rbx

    mov [signalTime], rax

    ret

%ifdef STANDALONE_COA
main:
%else
coa_main:
%endif
    sub rsp, 40

    call run_coa_calculation

    ; --------------------------------
    ; Print Road A
    ; --------------------------------
    lea rcx, [msgA]
    mov rdx, [roadA]
    call printf

    ; --------------------------------
    ; Print Road B
    ; --------------------------------
    lea rcx, [msgB]
    mov rdx, [roadB]
    call printf

    ; --------------------------------
    ; Print Road C
    ; --------------------------------
    lea rcx, [msgC]
    mov rdx, [roadC]
    call printf

    ; --------------------------------
    ; Print Road D
    ; --------------------------------
    lea rcx, [msgD]
    mov rdx, [roadD]
    call printf

    ; --------------------------------
    ; Print Total Traffic
    ; --------------------------------
    lea rcx, [msgTotal]
    mov rdx, [total]
    call printf

    ; --------------------------------
    ; Print Highest Traffic
    ; --------------------------------
    lea rcx, [msgHigh]
    mov rdx, [highest]
    call printf

    ; --------------------------------
    ; Print Signal Time
    ; --------------------------------
    lea rcx, [msgSignal]
    mov rdx, [signalTime]
    call printf

    add rsp, 40
    xor eax, eax
    ret
