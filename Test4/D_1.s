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

process:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a0
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  li $s1, 3
  addi $sp, $sp, -4
  lw $t0, -12($fp)
  li $v0, 11
  li $v1, 3
  mul $t0, $v0, $v1
  addi $sp, $sp, -4
  lw $t1, -16($fp)
  addi $t1, $t0, -2
  addi $sp, $sp, -4
  lw $t2, -20($fp)
  addi $t2, $t1, 5
  move $s1, $t2
  addi $sp, $sp, -4
  lw $t3, -24($fp)
  li $v1, 321
  mul $t3, $s0, $v1
  addi $sp, $sp, -4
  lw $t4, -28($fp)
  li $v1, 2
  mul $t4, $t3, $v1
  addi $sp, $sp, -4
  lw $t5, -32($fp)
  mul $t5, $s0, $s1
  addi $sp, $sp, -4
  lw $t6, -36($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -40($fp)
  sub $t7, $t6, $s0
  addi $sp, $sp, -4
  lw $t8, -44($fp)
  mul $t8, $s1, $s0
  addi $sp, $sp, -4
  lw $t9, -48($fp)
  add $t9, $t7, $t8
  addi $sp, $sp, -4
  sw $t0, -12($fp)
  lw $t0, -52($fp)
  lw $s2, -8($fp)
  mul $t0, $s2, $s1
  addi $sp, $sp, -4
  sw $t1, -16($fp)
  lw $t1, -56($fp)
  add $t1, $t9, $t0
  addi $sp, $sp, -4
  sw $t2, -20($fp)
  lw $t2, -60($fp)
  add $t2, $t1, $s0
  addi $sp, $sp, -4
  sw $t3, -24($fp)
  lw $t3, -64($fp)
  add $t3, $t2, $s0
  addi $sp, $sp, -4
  sw $t4, -28($fp)
  lw $t4, -68($fp)
  addi $t4, $t3, -23
  addi $sp, $sp, -4
  sw $t5, -32($fp)
  lw $t5, -72($fp)
  addi $t5, $t4, 45
  move $s1, $t5
  addi $sp, $sp, -4
  sw $t6, -36($fp)
  lw $t6, -76($fp)
  li $v1, 3
  div $s1, $v1
  mflo $t6
  addi $sp, $sp, -4
  sw $t7, -40($fp)
  lw $t7, -80($fp)
  li $v0, 14
  li $v1, 24
  mul $t7, $v0, $v1
  addi $sp, $sp, -4
  sw $t8, -44($fp)
  lw $t8, -84($fp)
  add $t8, $t6, $t7
  addi $sp, $sp, -4
  sw $t9, -48($fp)
  lw $t9, -88($fp)
  li $v1, 12
  mul $t9, $s0, $v1
  addi $sp, $sp, -4
  sw $t0, -52($fp)
  lw $t0, -92($fp)
  li $v1, 4
  div $t9, $v1
  mflo $t0
  addi $sp, $sp, -4
  sw $t1, -56($fp)
  lw $t1, -96($fp)
  sub $t1, $t8, $t0
  addi $sp, $sp, -4
  sw $t2, -60($fp)
  lw $t2, -100($fp)
  li $v0, 20
  li $v1, 3
  mul $t2, $v0, $v1
  addi $sp, $sp, -4
  sw $t3, -64($fp)
  lw $t3, -104($fp)
  sub $t3, $t1, $t2
  addi $sp, $sp, -4
  sw $t4, -68($fp)
  lw $t4, -108($fp)
  li $v1, 12
  div $s1, $v1
  mflo $t4
  addi $sp, $sp, -4
  sw $t5, -72($fp)
  lw $t5, -112($fp)
  li $v1, 24
  mul $t5, $t4, $v1
  addi $sp, $sp, -4
  sw $t6, -76($fp)
  lw $t6, -116($fp)
  add $t6, $t3, $t5
  addi $sp, $sp, -4
  sw $t7, -80($fp)
  lw $t7, -120($fp)
  li $v0, 12
  li $v1, 3
  mul $t7, $v0, $v1
  addi $sp, $sp, -4
  sw $t8, -84($fp)
  lw $t8, -124($fp)
  add $t8, $t6, $t7
  addi $sp, $sp, -4
  sw $t9, -88($fp)
  lw $t9, -128($fp)
  li $v0, 3
  li $v1, 2
  div $v0, $v1
  mflo $t9
  addi $sp, $sp, -4
  sw $t0, -92($fp)
  lw $t0, -132($fp)
  add $t0, $t8, $t9
  move $s1, $t0
  addi $sp, $sp, -4
  sw $t1, -96($fp)
  lw $t1, -136($fp)
  li $v0, 4
  li $v1, 6
  mul $t1, $v0, $v1
  addi $sp, $sp, -4
  sw $t2, -100($fp)
  lw $t2, -140($fp)
  add $t2, $s0, $t1
  addi $sp, $sp, -4
  sw $t3, -104($fp)
  lw $t3, -144($fp)
  li $v0, 3
  li $v1, 2
  div $v0, $v1
  mflo $t3
  addi $sp, $sp, -4
  sw $t4, -108($fp)
  lw $t4, -148($fp)
  add $t4, $t2, $t3
  move $s1, $t4
  move $v0, $s1
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $t0, -4($fp)
  li $v0, 5
  li $v1, 2
  div $v0, $v1
  mflo $t0
  addi $sp, $sp, -4
  lw $t1, -8($fp)
  addi $t1, $t0, 14
  addi $sp, $sp, -4
  lw $t2, -12($fp)
  addi $t2, $t1, -3
  addi $sp, $sp, -4
  lw $s0, -16($fp)
  move $s0, $t2
  addi $sp, $sp, -4
  lw $t3, -20($fp)
  li $v0, 7
  li $v1, 5
  mul $t3, $v0, $v1
  addi $sp, $sp, -4
  lw $t4, -24($fp)
  li $v1, 2
  div $t3, $v1
  mflo $t4
  addi $sp, $sp, -4
  lw $t5, -28($fp)
  addi $t5, $t4, 3
  addi $sp, $sp, -4
  lw $s1, -32($fp)
  move $s1, $t5
  addi $sp, $sp, -4
  lw $t6, -36($fp)
  li $v1, 4
  addi $t6, $v1, 5
  addi $sp, $sp, -4
  lw $t7, -40($fp)
  addi $t7, $t6, 6
  addi $sp, $sp, -4
  lw $t8, -44($fp)
  li $v0, 1
  li $v1, 2
  div $v0, $v1
  mflo $t8
  addi $sp, $sp, -4
  lw $t9, -48($fp)
  sub $t9, $t7, $t8
  addi $sp, $sp, -4
  lw $s2, -52($fp)
  move $s2, $t9
  addi $sp, $sp, -4
  sw $t0, -4($fp)
  lw $t0, -56($fp)
  add $t0, $s0, $s1
  addi $sp, $sp, -4
  sw $t1, -8($fp)
  lw $t1, -60($fp)
  add $t1, $t0, $s2
  addi $sp, $sp, -4
  lw $s3, -64($fp)
  move $s3, $t1
  addi $sp, $sp, -4
  sw $t2, -12($fp)
  lw $t2, -68($fp)
  mul $t2, $s0, $s1
  addi $sp, $sp, -4
  sw $t3, -20($fp)
  lw $t3, -72($fp)
  li $v1, 2
  div $s2, $v1
  mflo $t3
  addi $sp, $sp, -4
  sw $t4, -24($fp)
  lw $t4, -76($fp)
  add $t4, $t2, $t3
  addi $sp, $sp, -4
  lw $s4, -80($fp)
  move $s4, $t4
  addi $sp, $sp, -4
  sw $t5, -28($fp)
  lw $t5, -84($fp)
  sub $t5, $s0, $s1
  addi $sp, $sp, -4
  sw $t6, -36($fp)
  lw $t6, -88($fp)
  sub $t6, $t5, $s2
  addi $sp, $sp, -4
  lw $s5, -92($fp)
  move $s5, $t6
  addi $sp, $sp, -4
  lw $s6, -96($fp)
  li $s6, 42
  addi $sp, $sp, -4
  lw $s7, -100($fp)
  li $s7, 0
  addi $sp, $sp, -4
  sw $t7, -40($fp)
  lw $t7, -104($fp)
  add $t7, $s0, $s1
  addi $sp, $sp, -4
  sw $t8, -44($fp)
  lw $t8, -108($fp)
  add $t8, $t7, $s2
  addi $sp, $sp, -4
  sw $t9, -48($fp)
  lw $t9, -112($fp)
  li $v0, 1000
  li $v1, 2
  mul $t9, $v0, $v1
  addi $sp, $sp, -4
  sw $t0, -56($fp)
  lw $t0, -116($fp)
  add $t0, $t8, $t9
  addi $sp, $sp, -4
  sw $t1, -60($fp)
  lw $t1, -120($fp)
  sub $t1, $t0, $s5
  move $s5, $t1
  sw $s0, -16($fp)
  sw $s1, -32($fp)
  sw $s2, -52($fp)
  sw $s3, -64($fp)
  sw $s4, -80($fp)
  sw $s5, -92($fp)
  sw $s6, -96($fp)
  sw $s7, -100($fp)
  sw $t0, -116($fp)
  sw $t1, -120($fp)
  sw $t2, -68($fp)
  sw $t3, -72($fp)
  sw $t4, -76($fp)
  sw $t5, -84($fp)
  sw $t6, -88($fp)
  sw $t7, -104($fp)
  sw $t8, -108($fp)
  sw $t9, -112($fp)
label1:
  addi $sp, $sp, -4
  lw $t0, -124($fp)
  lw $s0, -16($fp)
  lw $s1, -32($fp)
  add $t0, $s0, $s1
  sw $s0, -16($fp)
  sw $s1, -32($fp)
  sw $t0, -124($fp)
  lw $t0, -124($fp)
  lw $s0, -92($fp)
  blt $t0, $s0, label2
  sw $s0, -92($fp)
  sw $t0, -124($fp)
  j label3
label2:
  addi $sp, $sp, -4
  lw $t0, -128($fp)
  lw $s0, -100($fp)
  li $v1, 12
  mul $t0, $s0, $v1
  addi $sp, $sp, -4
  lw $t1, -132($fp)
  lw $s1, -96($fp)
  add $t1, $s1, $t0
  addi $sp, $sp, -4
  lw $t2, -136($fp)
  addi $t2, $t1, 4
  addi $sp, $sp, -4
  lw $t3, -140($fp)
  addi $t3, $t2, 5
  addi $sp, $sp, -4
  lw $t4, -144($fp)
  li $v0, 7
  li $v1, 3
  div $v0, $v1
  mflo $t4
  addi $sp, $sp, -4
  lw $t5, -148($fp)
  add $t5, $t3, $t4
  move $s1, $t5
  sw $s0, -100($fp)
  sw $s1, -96($fp)
  sw $t0, -128($fp)
  sw $t1, -132($fp)
  sw $t2, -136($fp)
  sw $t3, -140($fp)
  sw $t4, -144($fp)
  sw $t5, -148($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $s2, -92($fp)
  move $a0, $s2
  jal process
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -152($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -156($fp)
  lw $s0, -16($fp)
  li $v0, 2
  mul $t1, $v0, $s0
  addi $sp, $sp, -4
  lw $t2, -160($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -164($fp)
  lw $s1, -92($fp)
  sub $t3, $t2, $s1
  addi $sp, $sp, -4
  lw $t4, -168($fp)
  lw $s2, -52($fp)
  lw $s3, -64($fp)
  mul $t4, $s2, $s3
  addi $sp, $sp, -4
  lw $t5, -172($fp)
  add $t5, $t3, $t4
  addi $sp, $sp, -4
  lw $s4, -176($fp)
  move $s4, $t5
  addi $sp, $sp, -4
  lw $t6, -180($fp)
  lw $s5, -100($fp)
  lw $s6, -100($fp)
  add $t6, $s6, $s5
  move $s5, $t6
  addi $sp, $sp, -4
  lw $t7, -184($fp)
  lw $s7, -100($fp)
  add $t7, $s7, $s5
  move $s5, $t7
  addi $sp, $sp, -4
  lw $t8, -188($fp)
  sw $s0, -16($fp)
  lw $s0, -100($fp)
  add $t8, $s0, $s5
  move $s0, $t8
  addi $sp, $sp, -4
  lw $t9, -192($fp)
  sw $s1, -92($fp)
  lw $s1, -100($fp)
  add $t9, $s1, $s0
  move $s0, $t9
  addi $sp, $sp, -4
  sw $t0, -152($fp)
  lw $t0, -196($fp)
  sw $s2, -52($fp)
  lw $s2, -100($fp)
  add $t0, $s2, $s0
  move $s0, $t0
  addi $sp, $sp, -4
  sw $t1, -156($fp)
  lw $t1, -200($fp)
  addi $t1, $s0, 3
  addi $sp, $sp, -4
  sw $s3, -64($fp)
  lw $s3, -204($fp)
  move $s3, $t1
  addi $sp, $sp, -4
  sw $t2, -160($fp)
  lw $t2, -208($fp)
  addi $t2, $s3, -1
  move $s3, $t2
  addi $sp, $sp, -4
  sw $t3, -164($fp)
  lw $t3, -212($fp)
  addi $t3, $s3, 3
  move $s3, $t3
  addi $sp, $sp, -4
  sw $t4, -168($fp)
  lw $t4, -216($fp)
  li $v0, 3
  li $v1, 2
  mul $t4, $v0, $v1
  addi $sp, $sp, -4
  sw $t5, -172($fp)
  lw $t5, -220($fp)
  sub $t5, $s3, $t4
  move $s3, $t5
  sw $s0, -100($fp)
  sw $s1, -100($fp)
  sw $s2, -100($fp)
  sw $s3, -204($fp)
  sw $s4, -176($fp)
  sw $s5, -100($fp)
  sw $s6, -100($fp)
  sw $s7, -100($fp)
  sw $t0, -196($fp)
  sw $t1, -200($fp)
  sw $t2, -208($fp)
  sw $t3, -212($fp)
  sw $t4, -216($fp)
  sw $t5, -220($fp)
  sw $t6, -180($fp)
  sw $t7, -184($fp)
  sw $t8, -188($fp)
  sw $t9, -192($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  sw $s4, -176($fp)
  lw $s4, -16($fp)
  move $a0, $s4
  jal process
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -224($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -228($fp)
  lw $s0, -16($fp)
  addi $t1, $s0, 3
  addi $sp, $sp, -4
  lw $t2, -232($fp)
  addi $t2, $t1, -2
  addi $sp, $sp, -4
  lw $t3, -236($fp)
  addi $t3, $t2, -1
  sw $s0, -16($fp)
  sw $t0, -224($fp)
  sw $t1, -228($fp)
  sw $t2, -232($fp)
  sw $t3, -236($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t3
  jal process
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -240($fp)
  move $t0, $v0
  sw $t0, -240($fp)
  lw $t0, -224($fp)
  lw $t1, -240($fp)
  beq $t0, $t1, label4
  sw $t0, -224($fp)
  sw $t1, -240($fp)
  j label5
label4:
  addi $sp, $sp, -4
  lw $t0, -244($fp)
  lw $s0, -92($fp)
  addi $t0, $s0, -2
  addi $sp, $sp, -4
  lw $t1, -248($fp)
  addi $t1, $t0, 1
  move $s0, $t1
  sw $s0, -92($fp)
  sw $t0, -244($fp)
  sw $t1, -248($fp)
label5:
  addi $sp, $sp, -4
  lw $t0, -252($fp)
  lw $s0, -16($fp)
  addi $t0, $s0, 2
  addi $sp, $sp, -4
  lw $t1, -256($fp)
  addi $t1, $t0, 1
  move $s0, $t1
  sw $s0, -16($fp)
  sw $t0, -252($fp)
  sw $t1, -256($fp)
  j label1
label3:
  addi $sp, $sp, -4
  lw $t0, -260($fp)
  li $v0, 3
  li $v1, 4
  mul $t0, $v0, $v1
  addi $sp, $sp, -4
  lw $t1, -264($fp)
  lw $s0, -96($fp)
  sub $t1, $s0, $t0
  lw $s1, -204($fp)
  move $s1, $t1
  sw $s0, -96($fp)
  sw $s1, -204($fp)
  sw $t0, -260($fp)
  sw $t1, -264($fp)
label6:
  lw $s0, -204($fp)
  lw $s1, -96($fp)
  blt $s0, $s1, label7
  sw $s0, -204($fp)
  sw $s1, -96($fp)
  j label8
label7:
  addi $sp, $sp, -4
  lw $t0, -268($fp)
  li $v0, 15
  li $v1, 4
  mul $t0, $v0, $v1
  addi $sp, $sp, -4
  lw $t1, -272($fp)
  addi $t1, $t0, -2
  addi $sp, $sp, -4
  lw $t2, -276($fp)
  lw $s0, -16($fp)
  add $t2, $t1, $s0
  lw $s1, -92($fp)
  move $s1, $t2
  addi $sp, $sp, -4
  lw $t3, -280($fp)
  lw $s2, -96($fp)
  addi $t3, $s2, -12
  lw $s3, -176($fp)
  move $s3, $t3
  addi $sp, $sp, -4
  lw $t4, -284($fp)
  lw $s4, -204($fp)
  addi $t4, $s4, 1
  move $s4, $t4
  move $s3, $s2
  addi $sp, $sp, -4
  lw $t5, -288($fp)
  lw $s5, -32($fp)
  add $t5, $s0, $s5
  lw $s6, -100($fp)
  move $s6, $t5
  addi $sp, $sp, -4
  lw $t6, -292($fp)
  add $t6, $s0, $s5
  lw $s7, -52($fp)
  move $s7, $t6
  sw $s0, -16($fp)
  sw $s1, -92($fp)
  sw $s2, -96($fp)
  sw $s3, -176($fp)
  sw $s4, -204($fp)
  sw $s5, -32($fp)
  sw $s6, -100($fp)
  sw $s7, -52($fp)
  sw $t0, -268($fp)
  sw $t1, -272($fp)
  sw $t2, -276($fp)
  sw $t3, -280($fp)
  sw $t4, -284($fp)
  sw $t5, -288($fp)
  sw $t6, -292($fp)
  j label6
label8:
  lw $s0, -92($fp)
  sw $s0, -92($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -296($fp)
  lw $s0, -16($fp)
  lw $s1, -32($fp)
  add $t0, $s0, $s1
  move $s0, $t0
  addi $sp, $sp, -4
  lw $t1, -300($fp)
  add $t1, $s0, $s1
  move $s1, $t1
  addi $sp, $sp, -4
  lw $t2, -304($fp)
  add $t2, $s0, $s1
  lw $s2, -52($fp)
  move $s2, $t2
  addi $sp, $sp, -4
  lw $t3, -308($fp)
  add $t3, $s0, $s1
  lw $s3, -92($fp)
  move $s3, $t3
  addi $sp, $sp, -4
  lw $t4, -312($fp)
  add $t4, $s0, $s1
  lw $s4, -176($fp)
  move $s4, $t4
  addi $sp, $sp, -4
  lw $t5, -316($fp)
  add $t5, $s2, $s3
  addi $sp, $sp, -4
  lw $t6, -320($fp)
  add $t6, $t5, $s4
  sw $s0, -16($fp)
  sw $s1, -32($fp)
  sw $s2, -52($fp)
  sw $s3, -92($fp)
  sw $s4, -176($fp)
  sw $t0, -296($fp)
  sw $t1, -300($fp)
  sw $t2, -304($fp)
  sw $t3, -308($fp)
  sw $t4, -312($fp)
  sw $t5, -316($fp)
  sw $t6, -320($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t6
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
