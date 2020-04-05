.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

hanoi:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a3
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  move $s1, $a2
  addi $sp, $sp, -4
  lw $s2, -12($fp)
  move $s2, $a1
  addi $sp, $sp, -4
  lw $s3, -16($fp)
  move $s3, $a0
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  sw $s2, -12($fp)
  sw $s3, -16($fp)
  lw $s0, -4($fp)
  li $v1, 1
  beq $s0, $v1, label1
  sw $s0, -4($fp)
  j label2
label1:
  addi $sp, $sp, -4
  lw $t0, -20($fp)
  lw $s0, -8($fp)
  li $v1, 1000000000
  mul $t0, $s0, $v1
  addi $sp, $sp, -4
  lw $t1, -24($fp)
  lw $s1, -16($fp)
  add $t1, $t0, $s1
  sw $s0, -8($fp)
  sw $s1, -16($fp)
  sw $t0, -20($fp)
  sw $t1, -24($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t1
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label3
label2:
  addi $sp, $sp, -4
  lw $t0, -28($fp)
  lw $s0, -4($fp)
  addi $t0, $s0, -1
  sw $s0, -4($fp)
  sw $t0, -28($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $s1, -12($fp)
  move $a0, $s1
  lw $s2, -16($fp)
  move $a1, $s2
  lw $s3, -8($fp)
  move $a2, $s3
  move $a3, $t0
  jal hanoi
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -32($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -36($fp)
  lw $s0, -8($fp)
  li $v1, 1000000000
  mul $t1, $s0, $v1
  addi $sp, $sp, -4
  lw $t2, -40($fp)
  lw $s1, -16($fp)
  add $t2, $t1, $s1
  sw $s0, -8($fp)
  sw $s1, -16($fp)
  sw $t0, -32($fp)
  sw $t1, -36($fp)
  sw $t2, -40($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t2
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -44($fp)
  lw $s0, -4($fp)
  addi $t0, $s0, -1
  sw $s0, -4($fp)
  sw $t0, -44($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $s1, -16($fp)
  move $a0, $s1
  lw $s2, -8($fp)
  move $a1, $s2
  lw $s3, -12($fp)
  move $a2, $s3
  move $a3, $t0
  jal hanoi
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -48($fp)
  move $t0, $v0
  sw $t0, -48($fp)
label3:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -4($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $s0, -8($fp)
  move $s0, $t0
  sw $s0, -8($fp)
  sw $t0, -4($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  li $v1, 3
  move $a0, $v1
  li $v1, 2
  move $a1, $v1
  li $v1, 1
  move $a2, $v1
  move $a3, $s0
  jal hanoi
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -12($fp)
  move $t0, $v0
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
