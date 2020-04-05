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

_add:
  addi $sp, $sp, -4
  sw $fp, ($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a0
  addi $sp, $sp, -4
  lw $t0, -8($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -12($fp)
  addi $t1, $t0, 0
  addi $sp, $sp, -4
  lw $t2, -16($fp)
  lw $t2, 0($t1)
  addi $sp, $sp, -4
  lw $t3, -20($fp)
  move $t3, $s0
  addi $sp, $sp, -4
  lw $t4, -24($fp)
  addi $t4, $t3, 4
  addi $sp, $sp, -4
  lw $t5, -28($fp)
  lw $t5, 0($t4)
  addi $sp, $sp, -4
  lw $t6, -32($fp)
  add $t6, $t2, $t5
  move $v0, $t6
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, ($sp)
  move $fp, $sp
  addi $sp, $sp, -8
  addi $sp, $sp, -4
  lw $s1, -12($fp)
  la $s1, -8($fp)
  addi $sp, $sp, -4
  lw $t7, -16($fp)
  move $t7, $s1
  addi $sp, $sp, -4
  lw $t8, -20($fp)
  addi $t8, $t7, 0
  li $v1,  1
  sw $v1, 0($t8)
  addi $sp, $sp, -4
  lw $t9, -24($fp)
  move $t9, $s1
  addi $sp, $sp, -4
  lw $t0, -28($fp)
  addi $t0, $t9, 4
  li $v1,  2
  sw $v1, 0($t0)
  addi $sp, $sp, -4
  lw $t1, -32($fp)
  move $t1, $s1
  sw $s1, -12($fp)
  sw $t0, -28($fp)
  sw $t1, -32($fp)
  sw $t7, -16($fp)
  sw $t8, -20($fp)
  sw $t9, -24($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t1
  jal _add
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t2, -36($fp)
  move $t2, $v0
  addi $sp, $sp, -4
  lw $s2, -40($fp)
  move $s2, $t2
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s2
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
