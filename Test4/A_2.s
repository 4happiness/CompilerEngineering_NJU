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
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t1, -12($fp)
  move $t1, $v0
  addi $sp, $sp, -4
  lw $s1, -16($fp)
  move $s1, $t1
  bgt $s0, $s1, label1
  j label2
label1:
  addi $sp, $sp, -4
  lw $t2, -20($fp)
  li $v1, 2
  mul $t2, $s1, $v1
  bgt $s0, $t2, label3
  j label4
label3:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label5
label4:
  addi $sp, $sp, -4
  lw $t3, -24($fp)
  li $v1, 2
  mul $t3, $s1, $v1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t3
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label5:
  j label6
label2:
  beq $s0, $s1, label7
  j label8
label7:
  li $v1, 100
  bgt $s1, $v1, label9
  j label10
label9:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  li $v1, 100
  move $a0, $v1
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label11
label10:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s1
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label11:
  j label12
label8:
  addi $sp, $sp, -4
  lw $t4, -28($fp)
  li $v1, 2
  mul $t4, $s0, $v1
  blt $t4, $s1, label13
  j label14
label13:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s1
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label15
label14:
  addi $sp, $sp, -4
  lw $t5, -32($fp)
  li $v1, 2
  mul $t5, $s0, $v1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t5
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label15:
label12:
label6:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
