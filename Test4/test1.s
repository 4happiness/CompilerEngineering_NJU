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

main:
  addi $sp, $sp, -4
  sw $fp, ($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  li $s0, 0
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  li $s1, 1
  addi $sp, $sp, -4
  lw $s2, -12($fp)
  li $s2, 0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -16($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $s3, -20($fp)
  move $s3, $t0
label1:
  blt $s2, $s3, label2
  j label3
label2:
  addi $sp, $sp, -4
  lw $t1, -24($fp)
  add $t1, $s0, $s1
  addi $sp, $sp, -4
  lw $s4, -28($fp)
  move $s4, $t1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s1
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $s0, $s1
  move $s1, $s4
  addi $sp, $sp, -4
  lw $t2, -32($fp)
  addi $t2, $s2, 1
  move $s2, $t2
  j label1
label3:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra