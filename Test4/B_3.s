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

isPrime:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a0
  addi $sp, $sp, -4
  lw $t0, -8($fp)
  li $v1, 4
  div $s0, $v1
  mflo $t0
  addi $sp, $sp, -4
  lw $s1, -12($fp)
  move $s1, $t0
  addi $sp, $sp, -4
  lw $s2, -16($fp)
  li $s2, 2
  sw $s0, -4($fp)
  sw $s1, -12($fp)
  sw $s2, -16($fp)
  sw $t0, -8($fp)
label1:
  lw $s0, -16($fp)
  lw $s1, -12($fp)
  blt $s0, $s1, label2
  sw $s0, -16($fp)
  sw $s1, -12($fp)
  j label3
label2:
  addi $sp, $sp, -4
  lw $t0, -20($fp)
  lw $s0, -4($fp)
  lw $s1, -16($fp)
  div $s0, $s1
  mflo $t0
  addi $sp, $sp, -4
  lw $t1, -24($fp)
  mul $t1, $t0, $s1
  sw $s0, -4($fp)
  sw $s1, -16($fp)
  sw $t0, -20($fp)
  sw $t1, -24($fp)
  lw $s0, -4($fp)
  lw $t0, -24($fp)
  beq $s0, $t0, label4
  sw $s0, -4($fp)
  sw $t0, -24($fp)
  j label5
label4:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
label5:
  addi $sp, $sp, -4
  lw $t0, -28($fp)
  lw $s0, -16($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  sw $s0, -16($fp)
  sw $t0, -28($fp)
  j label1
label3:
  li $v0, 1
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

isRever:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a0
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  move $s1, $s0
  addi $sp, $sp, -40
  addi $sp, $sp, -4
  lw $s2, -52($fp)
  la $s2, -48($fp)
  addi $sp, $sp, -4
  lw $s3, -56($fp)
  li $s3, 0
  addi $sp, $sp, -4
  lw $s4, -60($fp)
  li $s4, 0
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  sw $s2, -52($fp)
  sw $s3, -56($fp)
  sw $s4, -60($fp)
label6:
  lw $s0, -8($fp)
  li $v1, 0
  bne $s0, $v1, label7
  sw $s0, -8($fp)
  j label8
label7:
  addi $sp, $sp, -4
  lw $t0, -64($fp)
  lw $s0, -8($fp)
  li $v1, 10
  div $s0, $v1
  mflo $t0
  addi $sp, $sp, -4
  lw $t1, -68($fp)
  li $v1, 10
  mul $t1, $t0, $v1
  addi $sp, $sp, -4
  lw $t2, -72($fp)
  sub $t2, $s0, $t1
  addi $sp, $sp, -4
  lw $t3, -76($fp)
  lw $s1, -52($fp)
  move $t3, $s1
  addi $sp, $sp, -4
  lw $t4, -80($fp)
  lw $s2, -56($fp)
  li $v0, 4
  mul $t4, $v0, $s2
  addi $sp, $sp, -4
  lw $t5, -84($fp)
  add $t5, $t3, $t4
  addi $sp, $sp, -4
  lw $t6, -88($fp)
  move $t6, $t5
  sw $t2, 0($t6)
  addi $sp, $sp, -4
  lw $t7, -92($fp)
  li $v1, 10
  div $s0, $v1
  mflo $t7
  move $s0, $t7
  addi $sp, $sp, -4
  lw $t8, -96($fp)
  addi $t8, $s2, 1
  move $s2, $t8
  sw $s0, -8($fp)
  sw $s1, -52($fp)
  sw $s2, -56($fp)
  sw $t0, -64($fp)
  sw $t1, -68($fp)
  sw $t2, -72($fp)
  sw $t3, -76($fp)
  sw $t4, -80($fp)
  sw $t5, -84($fp)
  sw $t6, -88($fp)
  sw $t7, -92($fp)
  sw $t8, -96($fp)
  j label6
label8:
  addi $sp, $sp, -4
  lw $t0, -100($fp)
  lw $s0, -56($fp)
  addi $t0, $s0, -1
  move $s0, $t0
  sw $s0, -56($fp)
  sw $t0, -100($fp)
label9:
  lw $s0, -60($fp)
  lw $s1, -56($fp)
  bne $s0, $s1, label10
  sw $s0, -60($fp)
  sw $s1, -56($fp)
  j label11
label10:
  addi $sp, $sp, -4
  lw $t0, -104($fp)
  lw $s0, -52($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -108($fp)
  lw $s1, -60($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -112($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -116($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -120($fp)
  move $t4, $s0
  addi $sp, $sp, -4
  lw $t5, -124($fp)
  lw $s2, -56($fp)
  li $v0, 4
  mul $t5, $v0, $s2
  addi $sp, $sp, -4
  lw $t6, -128($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -132($fp)
  lw $t7, 0($t6)
  sw $s0, -52($fp)
  sw $s1, -60($fp)
  sw $s2, -56($fp)
  sw $t0, -104($fp)
  sw $t1, -108($fp)
  sw $t2, -112($fp)
  sw $t3, -116($fp)
  sw $t4, -120($fp)
  sw $t5, -124($fp)
  sw $t6, -128($fp)
  sw $t7, -132($fp)
  lw $t0, -116($fp)
  lw $t1, -132($fp)
  bne $t0, $t1, label12
  sw $t0, -116($fp)
  sw $t1, -132($fp)
  j label13
label12:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
label13:
  addi $sp, $sp, -4
  lw $t0, -136($fp)
  lw $s0, -60($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  addi $sp, $sp, -4
  lw $t1, -140($fp)
  lw $s1, -56($fp)
  addi $t1, $s1, -1
  move $s1, $t1
  sw $s0, -60($fp)
  sw $s1, -56($fp)
  sw $t0, -136($fp)
  sw $t1, -140($fp)
  j label9
label11:
  li $v0, 1
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  li $s0, 100
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  li $s1, 110
  addi $sp, $sp, -4
  lw $s2, -12($fp)
  move $s2, $s0
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  sw $s2, -12($fp)
label14:
  lw $s0, -12($fp)
  lw $s1, -8($fp)
  blt $s0, $s1, label15
  sw $s0, -12($fp)
  sw $s1, -8($fp)
  j label16
label15:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $s0, -12($fp)
  move $a0, $s0
  jal isPrime
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -16($fp)
  move $t0, $v0
  sw $t0, -16($fp)
  lw $t0, -16($fp)
  li $v1, 1
  beq $t0, $v1, label17
  sw $t0, -16($fp)
  j label18
label17:
  lw $s0, -12($fp)
  sw $s0, -12($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label18:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $s0, -12($fp)
  move $a0, $s0
  jal isRever
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -20($fp)
  move $t0, $v0
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  li $v1, 1
  beq $t0, $v1, label19
  sw $t0, -20($fp)
  j label20
label19:
  addi $sp, $sp, -4
  lw $t0, -24($fp)
  lw $s0, -12($fp)
  li $v1, 0
  sub $t0, $v1, $s0
  sw $s0, -12($fp)
  sw $t0, -24($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label20:
  addi $sp, $sp, -4
  lw $t0, -28($fp)
  lw $s0, -12($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  sw $s0, -12($fp)
  sw $t0, -28($fp)
  j label14
label16:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
