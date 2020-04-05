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
  addi $sp, $sp, -20
  addi $sp, $sp, -4
  lw $s0, -24($fp)
  la $s0, -20($fp)
  addi $sp, $sp, -4
  lw $s1, -28($fp)
  li $s1, 0
label1:
  li $v1, 5
  blt $s1, $v1, label2
  j label3
label2:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -32($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -36($fp)
  move $t1, $s0
  addi $sp, $sp, -4
  lw $t2, -40($fp)
  li $v0, 4
  mul $t2, $v0, $s1
  addi $sp, $sp, -4
  lw $t3, -44($fp)
  add $t3, $t1, $t2
  addi $sp, $sp, -4
  lw $t4, -48($fp)
  move $t4, $t3
  sw $t0, 0($t4)
  addi $sp, $sp, -4
  lw $t5, -52($fp)
  addi $t5, $s1, 1
  move $s1, $t5
  j label1
label3:
  addi $sp, $sp, -4
  lw $s2, -56($fp)
  li $s2, 1
label4:
  li $v1, 1
  beq $s2, $v1, label5
  j label6
label5:
  li $s2, 0
  li $s1, 1
label7:
  li $v1, 5
  blt $s1, $v1, label8
  j label9
label8:
  addi $sp, $sp, -4
  lw $s3, -60($fp)
  move $s3, $s1
label10:
  li $v1, 0
  bgt $s3, $v1, label13
  j label12
label13:
  addi $sp, $sp, -4
  lw $t6, -64($fp)
  move $t6, $s0
  addi $sp, $sp, -4
  lw $t7, -68($fp)
  li $v0, 4
  mul $t7, $v0, $s3
  addi $sp, $sp, -4
  lw $t8, -72($fp)
  add $t8, $t6, $t7
  addi $sp, $sp, -4
  lw $t9, -76($fp)
  lw $t9, 0($t8)
  addi $sp, $sp, -4
  sw $t0, -32($fp)
  lw $t0, -80($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  sw $t1, -36($fp)
  lw $t1, -84($fp)
  addi $t1, $s3, -1
  addi $sp, $sp, -4
  sw $t2, -40($fp)
  lw $t2, -88($fp)
  li $v0, 4
  mul $t2, $v0, $t1
  addi $sp, $sp, -4
  sw $t3, -44($fp)
  lw $t3, -92($fp)
  add $t3, $t0, $t2
  addi $sp, $sp, -4
  sw $t4, -48($fp)
  lw $t4, -96($fp)
  lw $t4, 0($t3)
  blt $t9, $t4, label11
  j label12
label11:
  addi $sp, $sp, -4
  sw $t5, -52($fp)
  lw $t5, -100($fp)
  move $t5, $s0
  addi $sp, $sp, -4
  sw $t6, -64($fp)
  lw $t6, -104($fp)
  li $v0, 4
  mul $t6, $v0, $s3
  addi $sp, $sp, -4
  sw $t7, -68($fp)
  lw $t7, -108($fp)
  add $t7, $t5, $t6
  addi $sp, $sp, -4
  sw $t8, -72($fp)
  lw $t8, -112($fp)
  lw $t8, 0($t7)
  addi $sp, $sp, -4
  lw $s4, -116($fp)
  move $s4, $t8
  addi $sp, $sp, -4
  sw $t9, -76($fp)
  lw $t9, -120($fp)
  move $t9, $s0
  addi $sp, $sp, -4
  sw $t0, -80($fp)
  lw $t0, -124($fp)
  addi $t0, $s3, -1
  addi $sp, $sp, -4
  sw $t1, -84($fp)
  lw $t1, -128($fp)
  li $v0, 4
  mul $t1, $v0, $t0
  addi $sp, $sp, -4
  sw $t2, -88($fp)
  lw $t2, -132($fp)
  add $t2, $t9, $t1
  addi $sp, $sp, -4
  sw $t3, -92($fp)
  lw $t3, -136($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  sw $t4, -96($fp)
  lw $t4, -140($fp)
  move $t4, $s0
  addi $sp, $sp, -4
  sw $t5, -100($fp)
  lw $t5, -144($fp)
  li $v0, 4
  mul $t5, $v0, $s3
  addi $sp, $sp, -4
  sw $t6, -104($fp)
  lw $t6, -148($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  sw $t7, -108($fp)
  lw $t7, -152($fp)
  move $t7, $t6
  sw $t3, 0($t7)
  addi $sp, $sp, -4
  sw $t8, -112($fp)
  lw $t8, -156($fp)
  move $t8, $s0
  addi $sp, $sp, -4
  sw $t9, -120($fp)
  lw $t9, -160($fp)
  addi $t9, $s3, -1
  addi $sp, $sp, -4
  sw $t0, -124($fp)
  lw $t0, -164($fp)
  li $v0, 4
  mul $t0, $v0, $t9
  addi $sp, $sp, -4
  sw $t1, -128($fp)
  lw $t1, -168($fp)
  add $t1, $t8, $t0
  addi $sp, $sp, -4
  sw $t2, -132($fp)
  lw $t2, -172($fp)
  move $t2, $t1
  sw $s4, 0($t2)
  li $s2, 1
  addi $sp, $sp, -4
  sw $t3, -136($fp)
  lw $t3, -176($fp)
  addi $t3, $s3, -1
  move $s3, $t3
  j label10
label12:
  addi $sp, $sp, -4
  sw $t4, -140($fp)
  lw $t4, -180($fp)
  addi $t4, $s1, 1
  move $s1, $t4
  j label7
label9:
  j label4
label6:
  li $s1, 0
label14:
  li $v1, 5
  blt $s1, $v1, label15
  j label16
label15:
  addi $sp, $sp, -4
  sw $t5, -144($fp)
  lw $t5, -184($fp)
  move $t5, $s0
  addi $sp, $sp, -4
  sw $t6, -148($fp)
  lw $t6, -188($fp)
  li $v0, 4
  mul $t6, $v0, $s1
  addi $sp, $sp, -4
  sw $t7, -152($fp)
  lw $t7, -192($fp)
  add $t7, $t5, $t6
  addi $sp, $sp, -4
  sw $t8, -156($fp)
  lw $t8, -196($fp)
  lw $t8, 0($t7)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t8
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  sw $t9, -160($fp)
  lw $t9, -200($fp)
  addi $t9, $s1, 1
  move $s1, $t9
  j label14
label16:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
