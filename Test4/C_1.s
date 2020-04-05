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
  addi $sp, $sp, -40
  addi $sp, $sp, -4
  lw $s2, -92($fp)
  la $s2, -88($fp)
  addi $sp, $sp, -4
  lw $s3, -96($fp)
  li $s3, 0
  addi $sp, $sp, -4
  lw $s4, -100($fp)
  li $s4, 0
  sw $s0, -4($fp)
  sw $s1, -48($fp)
  sw $s2, -92($fp)
  sw $s3, -96($fp)
  sw $s4, -100($fp)
label1:
  lw $s0, -96($fp)
  li $v1, 10
  blt $s0, $v1, label2
  sw $s0, -96($fp)
  j label3
label2:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -104($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -108($fp)
  lw $s0, -48($fp)
  move $t1, $s0
  addi $sp, $sp, -4
  lw $t2, -112($fp)
  lw $s1, -96($fp)
  li $v0, 4
  mul $t2, $v0, $s1
  addi $sp, $sp, -4
  lw $t3, -116($fp)
  add $t3, $t1, $t2
  addi $sp, $sp, -4
  lw $t4, -120($fp)
  move $t4, $t3
  sw $t0, 0($t4)
  addi $sp, $sp, -4
  lw $t5, -124($fp)
  addi $t5, $s1, 1
  move $s1, $t5
  sw $s0, -48($fp)
  sw $s1, -96($fp)
  sw $t0, -104($fp)
  sw $t1, -108($fp)
  sw $t2, -112($fp)
  sw $t3, -116($fp)
  sw $t4, -120($fp)
  sw $t5, -124($fp)
  j label1
label3:
  addi $sp, $sp, -4
  lw $t0, -128($fp)
  lw $s0, -4($fp)
  li $v1, 2
  div $s0, $v1
  mflo $t0
  addi $sp, $sp, -4
  lw $s1, -132($fp)
  move $s1, $t0
  sw $s0, -4($fp)
  sw $s1, -132($fp)
  sw $t0, -128($fp)
label4:
  lw $s0, -132($fp)
  li $v1, 0
  bge $s0, $v1, label5
  sw $s0, -132($fp)
  j label6
label5:
  lw $s0, -100($fp)
  li $s0, 0
  lw $s1, -96($fp)
  lw $s2, -132($fp)
  move $s1, $s2
  sw $s0, -100($fp)
  sw $s1, -96($fp)
  sw $s2, -132($fp)
label7:
  lw $s0, -100($fp)
  li $v1, 0
  beq $s0, $v1, label8
  sw $s0, -100($fp)
  j label9
label8:
  lw $s0, -100($fp)
  li $s0, 1
  addi $sp, $sp, -4
  lw $t0, -136($fp)
  lw $s1, -96($fp)
  li $v1, 2
  mul $t0, $s1, $v1
  addi $sp, $sp, -4
  lw $t1, -140($fp)
  addi $t1, $t0, 1
  addi $sp, $sp, -4
  lw $s2, -144($fp)
  move $s2, $t1
  addi $sp, $sp, -4
  lw $t2, -148($fp)
  li $v1, 2
  mul $t2, $s1, $v1
  addi $sp, $sp, -4
  lw $t3, -152($fp)
  addi $t3, $t2, 2
  addi $sp, $sp, -4
  lw $s3, -156($fp)
  move $s3, $t3
  sw $s0, -100($fp)
  sw $s1, -96($fp)
  sw $s2, -144($fp)
  sw $s3, -156($fp)
  sw $t0, -136($fp)
  sw $t1, -140($fp)
  sw $t2, -148($fp)
  sw $t3, -152($fp)
  lw $s0, -144($fp)
  lw $s1, -4($fp)
  blt $s0, $s1, label10
  sw $s0, -144($fp)
  sw $s1, -4($fp)
  j label11
label10:
  addi $sp, $sp, -4
  lw $t0, -160($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -164($fp)
  lw $s1, -144($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -168($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -172($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $s2, -176($fp)
  move $s2, $t3
  sw $s0, -48($fp)
  sw $s1, -144($fp)
  sw $s2, -176($fp)
  sw $t0, -160($fp)
  sw $t1, -164($fp)
  sw $t2, -168($fp)
  sw $t3, -172($fp)
  lw $s0, -156($fp)
  lw $s1, -4($fp)
  blt $s0, $s1, label14
  sw $s0, -156($fp)
  sw $s1, -4($fp)
  j label13
label14:
  addi $sp, $sp, -4
  lw $t0, -180($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -184($fp)
  lw $s1, -156($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -188($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -192($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -196($fp)
  move $t4, $s0
  addi $sp, $sp, -4
  lw $t5, -200($fp)
  lw $s2, -144($fp)
  li $v0, 4
  mul $t5, $v0, $s2
  addi $sp, $sp, -4
  lw $t6, -204($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -208($fp)
  lw $t7, 0($t6)
  sw $s0, -48($fp)
  sw $s1, -156($fp)
  sw $s2, -144($fp)
  sw $t0, -180($fp)
  sw $t1, -184($fp)
  sw $t2, -188($fp)
  sw $t3, -192($fp)
  sw $t4, -196($fp)
  sw $t5, -200($fp)
  sw $t6, -204($fp)
  sw $t7, -208($fp)
  lw $t0, -192($fp)
  lw $t1, -208($fp)
  blt $t0, $t1, label12
  sw $t0, -192($fp)
  sw $t1, -208($fp)
  j label13
label12:
  addi $sp, $sp, -4
  lw $t0, -212($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -216($fp)
  lw $s1, -156($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -220($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -224($fp)
  lw $t3, 0($t2)
  lw $s2, -176($fp)
  move $s2, $t3
  lw $s3, -144($fp)
  move $s3, $s1
  sw $s0, -48($fp)
  sw $s1, -156($fp)
  sw $s2, -176($fp)
  sw $s3, -144($fp)
  sw $t0, -212($fp)
  sw $t1, -216($fp)
  sw $t2, -220($fp)
  sw $t3, -224($fp)
label13:
  addi $sp, $sp, -4
  lw $t0, -228($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -232($fp)
  lw $s1, -96($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -236($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -240($fp)
  lw $t3, 0($t2)
  sw $s0, -48($fp)
  sw $s1, -96($fp)
  sw $t0, -228($fp)
  sw $t1, -232($fp)
  sw $t2, -236($fp)
  sw $t3, -240($fp)
  lw $t0, -240($fp)
  lw $s0, -176($fp)
  bgt $t0, $s0, label15
  sw $s0, -176($fp)
  sw $t0, -240($fp)
  j label16
label15:
  lw $s0, -100($fp)
  li $s0, 0
  addi $sp, $sp, -4
  lw $t0, -244($fp)
  lw $s1, -48($fp)
  move $t0, $s1
  addi $sp, $sp, -4
  lw $t1, -248($fp)
  lw $s2, -96($fp)
  li $v0, 4
  mul $t1, $v0, $s2
  addi $sp, $sp, -4
  lw $t2, -252($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -256($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -260($fp)
  move $t4, $s1
  addi $sp, $sp, -4
  lw $t5, -264($fp)
  lw $s3, -144($fp)
  li $v0, 4
  mul $t5, $v0, $s3
  addi $sp, $sp, -4
  lw $t6, -268($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -272($fp)
  move $t7, $t6
  sw $t3, 0($t7)
  addi $sp, $sp, -4
  lw $t8, -276($fp)
  move $t8, $s1
  addi $sp, $sp, -4
  lw $t9, -280($fp)
  li $v0, 4
  mul $t9, $v0, $s2
  addi $sp, $sp, -4
  sw $t0, -244($fp)
  lw $t0, -284($fp)
  add $t0, $t8, $t9
  addi $sp, $sp, -4
  sw $t1, -248($fp)
  lw $t1, -288($fp)
  move $t1, $t0
  lw $s4, -176($fp)
  sw $s4, 0($t1)
  move $s2, $s3
  sw $s0, -100($fp)
  sw $s1, -48($fp)
  sw $s2, -96($fp)
  sw $s3, -144($fp)
  sw $s4, -176($fp)
  sw $t0, -284($fp)
  sw $t1, -288($fp)
  sw $t2, -252($fp)
  sw $t3, -256($fp)
  sw $t4, -260($fp)
  sw $t5, -264($fp)
  sw $t6, -268($fp)
  sw $t7, -272($fp)
  sw $t8, -276($fp)
  sw $t9, -280($fp)
label16:
label11:
  j label7
label9:
  addi $sp, $sp, -4
  lw $t0, -292($fp)
  lw $s0, -132($fp)
  addi $t0, $s0, -1
  move $s0, $t0
  sw $s0, -132($fp)
  sw $t0, -292($fp)
  j label4
label6:
  lw $s0, -132($fp)
  li $s0, 10
  lw $s1, -96($fp)
  li $s1, 0
  sw $s0, -132($fp)
  sw $s1, -96($fp)
label17:
  lw $s0, -96($fp)
  lw $s1, -4($fp)
  blt $s0, $s1, label18
  sw $s0, -96($fp)
  sw $s1, -4($fp)
  j label19
label18:
  addi $sp, $sp, -4
  lw $t0, -296($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -300($fp)
  li $v0, 4
  li $v1, 0
  mul $t1, $v0, $v1
  addi $sp, $sp, -4
  lw $t2, -304($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -308($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -312($fp)
  lw $s1, -92($fp)
  move $t4, $s1
  addi $sp, $sp, -4
  lw $t5, -316($fp)
  lw $s2, -96($fp)
  li $v0, 4
  mul $t5, $v0, $s2
  addi $sp, $sp, -4
  lw $t6, -320($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -324($fp)
  move $t7, $t6
  sw $t3, 0($t7)
  addi $sp, $sp, -4
  lw $t8, -328($fp)
  addi $t8, $s2, 1
  move $s2, $t8
  addi $sp, $sp, -4
  lw $t9, -332($fp)
  move $t9, $s0
  addi $sp, $sp, -4
  sw $t0, -296($fp)
  lw $t0, -336($fp)
  lw $s3, -132($fp)
  addi $t0, $s3, -1
  addi $sp, $sp, -4
  sw $t1, -300($fp)
  lw $t1, -340($fp)
  li $v0, 4
  mul $t1, $v0, $t0
  addi $sp, $sp, -4
  sw $t2, -304($fp)
  lw $t2, -344($fp)
  add $t2, $t9, $t1
  addi $sp, $sp, -4
  sw $t3, -308($fp)
  lw $t3, -348($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  sw $t4, -312($fp)
  lw $t4, -352($fp)
  move $t4, $s0
  addi $sp, $sp, -4
  sw $t5, -316($fp)
  lw $t5, -356($fp)
  li $v0, 4
  li $v1, 0
  mul $t5, $v0, $v1
  addi $sp, $sp, -4
  sw $t6, -320($fp)
  lw $t6, -360($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  sw $t7, -324($fp)
  lw $t7, -364($fp)
  move $t7, $t6
  sw $t3, 0($t7)
  lw $s4, -100($fp)
  li $s4, 0
  addi $sp, $sp, -4
  sw $t8, -328($fp)
  lw $t8, -368($fp)
  addi $t8, $s3, -1
  move $s3, $t8
  addi $sp, $sp, -4
  lw $s5, -372($fp)
  li $s5, 0
  sw $s0, -48($fp)
  sw $s1, -92($fp)
  sw $s2, -96($fp)
  sw $s3, -132($fp)
  sw $s4, -100($fp)
  sw $s5, -372($fp)
  sw $t0, -336($fp)
  sw $t1, -340($fp)
  sw $t2, -344($fp)
  sw $t3, -348($fp)
  sw $t4, -352($fp)
  sw $t5, -356($fp)
  sw $t6, -360($fp)
  sw $t7, -364($fp)
  sw $t8, -368($fp)
  sw $t9, -332($fp)
label20:
  lw $s0, -100($fp)
  li $v1, 0
  beq $s0, $v1, label21
  sw $s0, -100($fp)
  j label22
label21:
  lw $s0, -100($fp)
  li $s0, 1
  addi $sp, $sp, -4
  lw $t0, -376($fp)
  lw $s1, -372($fp)
  li $v1, 2
  mul $t0, $s1, $v1
  addi $sp, $sp, -4
  lw $t1, -380($fp)
  addi $t1, $t0, 1
  lw $s2, -144($fp)
  move $s2, $t1
  addi $sp, $sp, -4
  lw $t2, -384($fp)
  li $v1, 2
  mul $t2, $s1, $v1
  addi $sp, $sp, -4
  lw $t3, -388($fp)
  addi $t3, $t2, 2
  lw $s3, -156($fp)
  move $s3, $t3
  sw $s0, -100($fp)
  sw $s1, -372($fp)
  sw $s2, -144($fp)
  sw $s3, -156($fp)
  sw $t0, -376($fp)
  sw $t1, -380($fp)
  sw $t2, -384($fp)
  sw $t3, -388($fp)
  lw $s0, -144($fp)
  lw $s1, -132($fp)
  blt $s0, $s1, label23
  sw $s0, -144($fp)
  sw $s1, -132($fp)
  j label24
label23:
  addi $sp, $sp, -4
  lw $t0, -392($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -396($fp)
  lw $s1, -144($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -400($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -404($fp)
  lw $t3, 0($t2)
  lw $s2, -176($fp)
  move $s2, $t3
  sw $s0, -48($fp)
  sw $s1, -144($fp)
  sw $s2, -176($fp)
  sw $t0, -392($fp)
  sw $t1, -396($fp)
  sw $t2, -400($fp)
  sw $t3, -404($fp)
  lw $s0, -156($fp)
  lw $s1, -132($fp)
  blt $s0, $s1, label27
  sw $s0, -156($fp)
  sw $s1, -132($fp)
  j label26
label27:
  addi $sp, $sp, -4
  lw $t0, -408($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -412($fp)
  lw $s1, -156($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -416($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -420($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -424($fp)
  move $t4, $s0
  addi $sp, $sp, -4
  lw $t5, -428($fp)
  lw $s2, -144($fp)
  li $v0, 4
  mul $t5, $v0, $s2
  addi $sp, $sp, -4
  lw $t6, -432($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -436($fp)
  lw $t7, 0($t6)
  sw $s0, -48($fp)
  sw $s1, -156($fp)
  sw $s2, -144($fp)
  sw $t0, -408($fp)
  sw $t1, -412($fp)
  sw $t2, -416($fp)
  sw $t3, -420($fp)
  sw $t4, -424($fp)
  sw $t5, -428($fp)
  sw $t6, -432($fp)
  sw $t7, -436($fp)
  lw $t0, -420($fp)
  lw $t1, -436($fp)
  blt $t0, $t1, label25
  sw $t0, -420($fp)
  sw $t1, -436($fp)
  j label26
label25:
  addi $sp, $sp, -4
  lw $t0, -440($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -444($fp)
  lw $s1, -156($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -448($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -452($fp)
  lw $t3, 0($t2)
  lw $s2, -176($fp)
  move $s2, $t3
  lw $s3, -144($fp)
  move $s3, $s1
  sw $s0, -48($fp)
  sw $s1, -156($fp)
  sw $s2, -176($fp)
  sw $s3, -144($fp)
  sw $t0, -440($fp)
  sw $t1, -444($fp)
  sw $t2, -448($fp)
  sw $t3, -452($fp)
label26:
  addi $sp, $sp, -4
  lw $t0, -456($fp)
  lw $s0, -48($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -460($fp)
  lw $s1, -372($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -464($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -468($fp)
  lw $t3, 0($t2)
  sw $s0, -48($fp)
  sw $s1, -372($fp)
  sw $t0, -456($fp)
  sw $t1, -460($fp)
  sw $t2, -464($fp)
  sw $t3, -468($fp)
  lw $t0, -468($fp)
  lw $s0, -176($fp)
  bgt $t0, $s0, label28
  sw $s0, -176($fp)
  sw $t0, -468($fp)
  j label29
label28:
  lw $s0, -100($fp)
  li $s0, 0
  addi $sp, $sp, -4
  lw $t0, -472($fp)
  lw $s1, -48($fp)
  move $t0, $s1
  addi $sp, $sp, -4
  lw $t1, -476($fp)
  lw $s2, -372($fp)
  li $v0, 4
  mul $t1, $v0, $s2
  addi $sp, $sp, -4
  lw $t2, -480($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -484($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -488($fp)
  move $t4, $s1
  addi $sp, $sp, -4
  lw $t5, -492($fp)
  lw $s3, -144($fp)
  li $v0, 4
  mul $t5, $v0, $s3
  addi $sp, $sp, -4
  lw $t6, -496($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -500($fp)
  move $t7, $t6
  sw $t3, 0($t7)
  addi $sp, $sp, -4
  lw $t8, -504($fp)
  move $t8, $s1
  addi $sp, $sp, -4
  lw $t9, -508($fp)
  li $v0, 4
  mul $t9, $v0, $s2
  addi $sp, $sp, -4
  sw $t0, -472($fp)
  lw $t0, -512($fp)
  add $t0, $t8, $t9
  addi $sp, $sp, -4
  sw $t1, -476($fp)
  lw $t1, -516($fp)
  move $t1, $t0
  lw $s4, -176($fp)
  sw $s4, 0($t1)
  move $s2, $s3
  sw $s0, -100($fp)
  sw $s1, -48($fp)
  sw $s2, -372($fp)
  sw $s3, -144($fp)
  sw $s4, -176($fp)
  sw $t0, -512($fp)
  sw $t1, -516($fp)
  sw $t2, -480($fp)
  sw $t3, -484($fp)
  sw $t4, -488($fp)
  sw $t5, -492($fp)
  sw $t6, -496($fp)
  sw $t7, -500($fp)
  sw $t8, -504($fp)
  sw $t9, -508($fp)
label29:
label24:
  j label20
label22:
  j label17
label19:
  lw $s0, -96($fp)
  li $s0, 0
  sw $s0, -96($fp)
label30:
  lw $s0, -96($fp)
  lw $s1, -4($fp)
  blt $s0, $s1, label31
  sw $s0, -96($fp)
  sw $s1, -4($fp)
  j label32
label31:
  addi $sp, $sp, -4
  lw $t0, -520($fp)
  lw $s0, -92($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -524($fp)
  lw $s1, -96($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -528($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -532($fp)
  lw $t3, 0($t2)
  sw $s0, -92($fp)
  sw $s1, -96($fp)
  sw $t0, -520($fp)
  sw $t1, -524($fp)
  sw $t2, -528($fp)
  sw $t3, -532($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t3
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -536($fp)
  lw $s0, -96($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  sw $s0, -96($fp)
  sw $t0, -536($fp)
  j label30
label32:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
