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
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -8($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $s1, -12($fp)
  move $s1, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t1, -16($fp)
  move $t1, $v0
  addi $sp, $sp, -4
  lw $s2, -20($fp)
  move $s2, $t1
  bgt $s1, $s2, label1
  j label2
label1:
  addi $sp, $sp, -4
  lw $s3, -24($fp)
  move $s3, $s1
  j label3
label2:
  move $s3, $s2
label3:
label4:
  li $v1, 0
  beq $s0, $v1, label5
  j label6
label5:
  addi $sp, $sp, -4
  lw $t2, -28($fp)
  div $s3, $s1
  mflo $t2
  addi $sp, $sp, -4
  lw $t3, -32($fp)
  mul $t3, $t2, $s1
  beq $s3, $t3, label7
  j label8
label7:
  addi $sp, $sp, -4
  lw $t4, -36($fp)
  div $s3, $s2
  mflo $t4
  addi $sp, $sp, -4
  lw $t5, -40($fp)
  mul $t5, $t4, $s2
  beq $s3, $t5, label9
  j label10
label9:
  addi $sp, $sp, -4
  lw $s4, -44($fp)
  move $s4, $s3
  li $s0, 1
  j label11
label10:
  addi $sp, $sp, -4
  lw $t6, -48($fp)
  addi $t6, $s3, 1
  move $s3, $t6
label11:
  j label12
label8:
  addi $sp, $sp, -4
  lw $t7, -52($fp)
  addi $t7, $s3, 1
  move $s3, $t7
label12:
  j label4
label6:
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
