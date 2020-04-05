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
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  li $s0, 10
  addi $sp, $sp, -40
  addi $sp, $sp, -4
  lw $s1, -48($fp)
  la $s1, -44($fp)
  addi $sp, $sp, -4
  lw $s2, -52($fp)
  li $s2, 0
  addi $sp, $sp, -4
  lw $s3, -56($fp)
  li $s3, 0
  sw $s0, -4($fp)
  sw $s1, -48($fp)
  sw $s2, -52($fp)
  sw $s3, -56($fp)
label1:
  lw $s0, -52($fp)
  li $v1, 10
  blt $s0, $v1, label2
  sw $s0, -52($fp)
  j label3
label2:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -60($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -64($fp)
  lw $s0, -48($fp)
  move $t1, $s0
  addi $sp, $sp, -4
  lw $t2, -68($fp)
  lw $s1, -52($fp)
  li $v0, 4
  mul $t2, $v0, $s1
  addi $sp, $sp, -4
  lw $t3, -72($fp)
  add $t3, $t1, $t2
  addi $sp, $sp, -4
  lw $t4, -76($fp)
  move $t4, $t3
  sw $t0, 0($t4)
  addi $sp, $sp, -4
  lw $t5, -80($fp)
  addi $t5, $s1, 1
  move $s1, $t5
  sw $s0, -48($fp)
  sw $s1, -52($fp)
  sw $t0, -60($fp)
  sw $t1, -64($fp)
  sw $t2, -68($fp)
  sw $t3, -72($fp)
  sw $t4, -76($fp)
  sw $t5, -80($fp)
  j label1
label3:
  addi $sp, $sp, -4
  lw $t0, -84($fp)
  lw $s0, -4($fp)
  li $v1, 2
  div $s0, $v1
  mflo $t0
  addi $sp, $sp, -4
  lw $s1, -88($fp)
  move $s1, $t0
  sw $s0, -4($fp)
  sw $s1, -88($fp)
  sw $t0, -84($fp)
label4:
  lw $s0, -88($fp)
  li $v1, 0
  bge $s0, $v1, label5
  sw $s0, -88($fp)
  j label6
label5:
  lw $s0, -56($fp)
  li $s0, 0
  lw $s1, -52($fp)
  lw $s2, -88($fp)
  move $s1, $s2
  sw $s0, -56($fp)
  sw $s1, -52($fp)
  sw $s2, -88($fp)
label7:
  lw $s0, -56($fp)
  li $v1, 0
  beq $s0, $v1, label8
  sw $s0, -56($fp)
  j label9
label8:
  lw $s0, -56($fp)
  li $s0, 1
  addi $sp, $sp, -4
  lw $t0, -92($fp)
  lw $s1, -52($fp)
  li $v1, 2
  mul $t0, $s1, $v1
  addi $sp, $sp, -4
  lw $t1, -96($fp)
  addi $t1, $t0, 1
  addi $sp, $sp, -4
  lw $s2, -100($fp)
  move $s2, $t1
  addi $sp, $sp, -4
  lw $t2, -104($fp)
  li $v1, 2
  mul $t2, $s1, $v1
  addi $sp, $sp, -4
  lw $t3, -108($fp)
  addi $t3, $t2, 2
  addi $sp, $sp, -4
  lw $s3, -112($fp)
  move $s3, $t3
  sw $s0, -56($fp)
  sw $s1, -52($fp)
  sw $s2, -100($fp)
  sw $s3, -112($fp)
  sw $t0, -92($fp)
  sw $t1, -96($fp)
  sw $t2, -104($fp)
  sw $t3, -108($fp)
  lw $s0, -100($fp)
  lw $s1, -4($fp)
  blt $s0, $s1, label10
  sw $s0, -100($fp)
  sw $s1, -4($fp)
  j label11
label10:
  addi $sp, $sp, -4
  lw $t0, -116($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -120($fp)
  lw $s1, -100($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -124($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -128($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $s2, -132($fp)
  move $s2, $t3
  sw $s0, -48($fp)
  sw $s1, -100($fp)
  sw $s2, -132($fp)
  sw $t0, -116($fp)
  sw $t1, -120($fp)
  sw $t2, -124($fp)
  sw $t3, -128($fp)
  lw $s0, -112($fp)
  lw $s1, -4($fp)
  blt $s0, $s1, label14
  sw $s0, -112($fp)
  sw $s1, -4($fp)
  j label13
label14:
  addi $sp, $sp, -4
  lw $t0, -136($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -140($fp)
  lw $s1, -112($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -144($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -148($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -152($fp)
  move $t4, $s0
  addi $sp, $sp, -4
  lw $t5, -156($fp)
  lw $s2, -100($fp)
  li $v0, 4
  mul $t5, $v0, $s2
  addi $sp, $sp, -4
  lw $t6, -160($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -164($fp)
  lw $t7, 0($t6)
  sw $s0, -48($fp)
  sw $s1, -112($fp)
  sw $s2, -100($fp)
  sw $t0, -136($fp)
  sw $t1, -140($fp)
  sw $t2, -144($fp)
  sw $t3, -148($fp)
  sw $t4, -152($fp)
  sw $t5, -156($fp)
  sw $t6, -160($fp)
  sw $t7, -164($fp)
  lw $t0, -148($fp)
  lw $t1, -164($fp)
  blt $t0, $t1, label12
  sw $t0, -148($fp)
  sw $t1, -164($fp)
  j label13
label12:
  addi $sp, $sp, -4
  lw $t0, -168($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -172($fp)
  lw $s1, -112($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -176($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -180($fp)
  lw $t3, 0($t2)
  lw $s2, -132($fp)
  move $s2, $t3
  lw $s3, -100($fp)
  move $s3, $s1
  sw $s0, -48($fp)
  sw $s1, -112($fp)
  sw $s2, -132($fp)
  sw $s3, -100($fp)
  sw $t0, -168($fp)
  sw $t1, -172($fp)
  sw $t2, -176($fp)
  sw $t3, -180($fp)
label13:
  addi $sp, $sp, -4
  lw $t0, -184($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -188($fp)
  lw $s1, -52($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -192($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -196($fp)
  lw $t3, 0($t2)
  sw $s0, -48($fp)
  sw $s1, -52($fp)
  sw $t0, -184($fp)
  sw $t1, -188($fp)
  sw $t2, -192($fp)
  sw $t3, -196($fp)
  lw $t0, -196($fp)
  lw $s0, -132($fp)
  bgt $t0, $s0, label15
  sw $s0, -132($fp)
  sw $t0, -196($fp)
  j label16
label15:
  lw $s0, -56($fp)
  li $s0, 0
  addi $sp, $sp, -4
  lw $t0, -200($fp)
  lw $s1, -48($fp)
  move $t0, $s1
  addi $sp, $sp, -4
  lw $t1, -204($fp)
  lw $s2, -52($fp)
  li $v0, 4
  mul $t1, $v0, $s2
  addi $sp, $sp, -4
  lw $t2, -208($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -212($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -216($fp)
  move $t4, $s1
  addi $sp, $sp, -4
  lw $t5, -220($fp)
  lw $s3, -100($fp)
  li $v0, 4
  mul $t5, $v0, $s3
  addi $sp, $sp, -4
  lw $t6, -224($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -228($fp)
  move $t7, $t6
  sw $t3, 0($t7)
  addi $sp, $sp, -4
  lw $t8, -232($fp)
  move $t8, $s1
  addi $sp, $sp, -4
  lw $t9, -236($fp)
  li $v0, 4
  mul $t9, $v0, $s2
  addi $sp, $sp, -4
  sw $t0, -200($fp)
  lw $t0, -240($fp)
  add $t0, $t8, $t9
  addi $sp, $sp, -4
  sw $t1, -204($fp)
  lw $t1, -244($fp)
  move $t1, $t0
  lw $s4, -132($fp)
  sw $s4, 0($t1)
  move $s2, $s3
  sw $s0, -56($fp)
  sw $s1, -48($fp)
  sw $s2, -52($fp)
  sw $s3, -100($fp)
  sw $s4, -132($fp)
  sw $t0, -240($fp)
  sw $t1, -244($fp)
  sw $t2, -208($fp)
  sw $t3, -212($fp)
  sw $t4, -216($fp)
  sw $t5, -220($fp)
  sw $t6, -224($fp)
  sw $t7, -228($fp)
  sw $t8, -232($fp)
  sw $t9, -236($fp)
label16:
label11:
  j label7
label9:
  addi $sp, $sp, -4
  lw $t0, -248($fp)
  lw $s0, -88($fp)
  addi $t0, $s0, -1
  move $s0, $t0
  sw $s0, -88($fp)
  sw $t0, -248($fp)
  j label4
label6:
  lw $s0, -52($fp)
  li $s0, 0
  sw $s0, -52($fp)
label17:
  lw $s0, -52($fp)
  lw $s1, -4($fp)
  blt $s0, $s1, label18
  sw $s0, -52($fp)
  sw $s1, -4($fp)
  j label19
label18:
  addi $sp, $sp, -4
  lw $t0, -252($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -256($fp)
  lw $s1, -52($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -260($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -264($fp)
  lw $t3, 0($t2)
  sw $s0, -48($fp)
  sw $s1, -52($fp)
  sw $t0, -252($fp)
  sw $t1, -256($fp)
  sw $t2, -260($fp)
  sw $t3, -264($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t3
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -268($fp)
  lw $s0, -52($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  sw $s0, -52($fp)
  sw $t0, -268($fp)
  j label17
label19:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
