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
  li $s0, 12
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  li $s1, 4
  addi $sp, $sp, -4
  lw $s2, -12($fp)
  li $s2, 5
  addi $sp, $sp, -4
  lw $t0, -16($fp)
  mul $t0, $s0, $s1
  addi $sp, $sp, -4
  lw $s3, -20($fp)
  move $s3, $t0
  addi $sp, $sp, -4
  lw $t1, -24($fp)
  add $t1, $s3, $s2
  addi $sp, $sp, -4
  lw $t2, -28($fp)
  sub $t2, $t1, $s1
  addi $sp, $sp, -4
  lw $s4, -32($fp)
  move $s4, $t2
  addi $sp, $sp, -4
  lw $t3, -36($fp)
  mul $t3, $s4, $s1
  addi $sp, $sp, -4
  lw $t4, -40($fp)
  add $t4, $s3, $t3
  addi $sp, $sp, -4
  lw $s5, -44($fp)
  move $s5, $t4
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s5
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t5, -48($fp)
  add $t5, $s3, $s4
  addi $sp, $sp, -4
  lw $t6, -52($fp)
  mul $t6, $s5, $t5
  addi $sp, $sp, -4
  lw $t7, -56($fp)
  li $v1, 25
  div $t6, $v1
  mflo $t7
  move $s5, $t7
  addi $sp, $sp, -4
  lw $t8, -60($fp)
  add $t8, $s4, $s5
  addi $sp, $sp, -4
  lw $t9, -64($fp)
  div $s3, $s5
  mflo $t9
  addi $sp, $sp, -4
  sw $t0, -16($fp)
  lw $t0, -68($fp)
  sub $t0, $t8, $t9
  addi $sp, $sp, -4
  sw $t1, -24($fp)
  lw $t1, -72($fp)
  li $v1, 12
  mul $t1, $s1, $v1
  addi $sp, $sp, -4
  sw $t2, -28($fp)
  lw $t2, -76($fp)
  add $t2, $t0, $t1
  move $s4, $t2
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s5
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s4
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
