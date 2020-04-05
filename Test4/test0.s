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

print:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  lw $s0, 4($fp)
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  lw $s1, 8($fp)
  addi $sp, $sp, -4
  lw $s2, -12($fp)
  lw $s2, 12($fp)
  addi $sp, $sp, -4
  lw $s3, -16($fp)
  lw $s3, 16($fp)
  addi $sp, $sp, -4
  lw $s4, -20($fp)
  move $s4, $a3
  addi $sp, $sp, -4
  lw $s5, -24($fp)
  move $s5, $a2
  addi $sp, $sp, -4
  lw $s6, -28($fp)
  move $s6, $a1
  addi $sp, $sp, -4
  lw $s7, -32($fp)
  move $s7, $a0
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  sw $s2, -12($fp)
  sw $s3, -16($fp)
  sw $s4, -20($fp)
  sw $s5, -24($fp)
  sw $s6, -28($fp)
  sw $s7, -32($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -8($fp)
  sw $s0, -8($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -12($fp)
  sw $s0, -12($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -16($fp)
  sw $s0, -16($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -20($fp)
  sw $s0, -20($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -24($fp)
  sw $s0, -24($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -28($fp)
  sw $s0, -28($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -32($fp)
  sw $s0, -32($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
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
  li $v1, 8
  move $a0, $v1
  li $v1, 7
  move $a1, $v1
  li $v1, 6
  move $a2, $v1
  li $v1, 5
  move $a3, $v1
  addi $sp, $sp, -4
  li $v1, 4
  sw $v1, 0($sp)
  addi $sp, $sp, -4
  li $v1, 3
  sw $v1, 0($sp)
  addi $sp, $sp, -4
  li $v1, 2
  sw $v1, 0($sp)
  addi $sp, $sp, -4
  li $v1, 1
  sw $v1, 0($sp)
  jal print
  addi $sp, $sp, 16
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -4($fp)
  move $t0, $v0
  sw $t0, -4($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  li $v1, 16
  move $a0, $v1
  li $v1, 15
  move $a1, $v1
  li $v1, 14
  move $a2, $v1
  li $v1, 13
  move $a3, $v1
  addi $sp, $sp, -4
  li $v1, 12
  sw $v1, 0($sp)
  addi $sp, $sp, -4
  li $v1, 11
  sw $v1, 0($sp)
  addi $sp, $sp, -4
  li $v1, 10
  sw $v1, 0($sp)
  addi $sp, $sp, -4
  li $v1, 9
  sw $v1, 0($sp)
  jal print
  addi $sp, $sp, 16
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -8($fp)
  move $t0, $v0
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
