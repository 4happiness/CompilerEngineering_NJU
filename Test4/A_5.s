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

compare:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a1
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  move $s1, $a0
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  lw $s0, -4($fp)
  lw $s1, -8($fp)
  bgt $s0, $s1, label1
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  j label2
label1:
  lw $s0, -4($fp)
  move $v0, $s0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  sw $s0, -4($fp)
label2:
  lw $s0, -4($fp)
  lw $s1, -8($fp)
  blt $s0, $s1, label3
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  j label4
label3:
  lw $s0, -8($fp)
  move $v0, $s0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  sw $s0, -8($fp)
label4:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

_add:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a1
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  move $s1, $a0
  addi $sp, $sp, -4
  lw $t0, -12($fp)
  add $t0, $s0, $s1
  move $v0, $t0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -40
  addi $sp, $sp, -4
  lw $s0, -44($fp)
  la $s0, -40($fp)
  addi $sp, $sp, -20
  addi $sp, $sp, -4
  lw $s1, -68($fp)
  la $s1, -64($fp)
  addi $sp, $sp, -4
  lw $s2, -72($fp)
  li $s2, 0
  sw $s0, -44($fp)
  sw $s1, -68($fp)
  sw $s2, -72($fp)
label5:
  lw $s0, -72($fp)
  li $v1, 10
  blt $s0, $v1, label6
  sw $s0, -72($fp)
  j label7
label6:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -76($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -80($fp)
  lw $s0, -44($fp)
  move $t1, $s0
  addi $sp, $sp, -4
  lw $t2, -84($fp)
  lw $s1, -72($fp)
  li $v0, 4
  mul $t2, $v0, $s1
  addi $sp, $sp, -4
  lw $t3, -88($fp)
  add $t3, $t1, $t2
  addi $sp, $sp, -4
  lw $t4, -92($fp)
  move $t4, $t3
  sw $t0, 0($t4)
  addi $sp, $sp, -4
  lw $t5, -96($fp)
  addi $t5, $s1, 1
  move $s1, $t5
  sw $s0, -44($fp)
  sw $s1, -72($fp)
  sw $t0, -76($fp)
  sw $t1, -80($fp)
  sw $t2, -84($fp)
  sw $t3, -88($fp)
  sw $t4, -92($fp)
  sw $t5, -96($fp)
  j label5
label7:
  lw $s0, -72($fp)
  li $s0, 0
  addi $sp, $sp, -4
  lw $s1, -100($fp)
  li $s1, 0
  sw $s0, -72($fp)
  sw $s1, -100($fp)
label8:
  lw $s0, -72($fp)
  li $v1, 10
  blt $s0, $v1, label9
  sw $s0, -72($fp)
  j label10
label9:
  addi $sp, $sp, -4
  lw $t0, -104($fp)
  lw $s0, -44($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -108($fp)
  lw $s1, -72($fp)
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
  addi $t5, $s1, 1
  addi $sp, $sp, -4
  lw $t6, -128($fp)
  li $v0, 4
  mul $t6, $v0, $t5
  addi $sp, $sp, -4
  lw $t7, -132($fp)
  add $t7, $t4, $t6
  addi $sp, $sp, -4
  lw $t8, -136($fp)
  lw $t8, 0($t7)
  sw $s0, -44($fp)
  sw $s1, -72($fp)
  sw $t0, -104($fp)
  sw $t1, -108($fp)
  sw $t2, -112($fp)
  sw $t3, -116($fp)
  sw $t4, -120($fp)
  sw $t5, -124($fp)
  sw $t6, -128($fp)
  sw $t7, -132($fp)
  sw $t8, -136($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t8
  move $a1, $t3
  jal compare
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -140($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -144($fp)
  lw $s0, -68($fp)
  move $t1, $s0
  addi $sp, $sp, -4
  lw $t2, -148($fp)
  lw $s1, -100($fp)
  li $v0, 4
  mul $t2, $v0, $s1
  addi $sp, $sp, -4
  lw $t3, -152($fp)
  add $t3, $t1, $t2
  addi $sp, $sp, -4
  lw $t4, -156($fp)
  move $t4, $t3
  sw $t0, 0($t4)
  addi $sp, $sp, -4
  lw $t5, -160($fp)
  lw $s2, -72($fp)
  addi $t5, $s2, 2
  move $s2, $t5
  addi $sp, $sp, -4
  lw $t6, -164($fp)
  addi $t6, $s1, 1
  move $s1, $t6
  sw $s0, -68($fp)
  sw $s1, -100($fp)
  sw $s2, -72($fp)
  sw $t0, -140($fp)
  sw $t1, -144($fp)
  sw $t2, -148($fp)
  sw $t3, -152($fp)
  sw $t4, -156($fp)
  sw $t5, -160($fp)
  sw $t6, -164($fp)
  j label8
label10:
  lw $s0, -72($fp)
  li $s0, 0
  sw $s0, -72($fp)
label11:
  lw $s0, -72($fp)
  li $v1, 5
  blt $s0, $v1, label12
  sw $s0, -72($fp)
  j label13
label12:
  addi $sp, $sp, -4
  lw $t0, -168($fp)
  lw $s0, -68($fp)
  move $t0, $s0
  addi $sp, $sp, -4
  lw $t1, -172($fp)
  lw $s1, -72($fp)
  li $v0, 4
  mul $t1, $v0, $s1
  addi $sp, $sp, -4
  lw $t2, -176($fp)
  add $t2, $t0, $t1
  addi $sp, $sp, -4
  lw $t3, -180($fp)
  lw $t3, 0($t2)
  addi $sp, $sp, -4
  lw $t4, -184($fp)
  lw $s2, -44($fp)
  move $t4, $s2
  addi $sp, $sp, -4
  lw $t5, -188($fp)
  li $v0, 4
  mul $t5, $v0, $s1
  addi $sp, $sp, -4
  lw $t6, -192($fp)
  add $t6, $t4, $t5
  addi $sp, $sp, -4
  lw $t7, -196($fp)
  lw $t7, 0($t6)
  sw $s0, -68($fp)
  sw $s1, -72($fp)
  sw $s2, -44($fp)
  sw $t0, -168($fp)
  sw $t1, -172($fp)
  sw $t2, -176($fp)
  sw $t3, -180($fp)
  sw $t4, -184($fp)
  sw $t5, -188($fp)
  sw $t6, -192($fp)
  sw $t7, -196($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t7
  move $a1, $t3
  jal _add
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -200($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -204($fp)
  lw $s0, -68($fp)
  move $t1, $s0
  addi $sp, $sp, -4
  lw $t2, -208($fp)
  lw $s1, -72($fp)
  li $v0, 4
  mul $t2, $v0, $s1
  addi $sp, $sp, -4
  lw $t3, -212($fp)
  add $t3, $t1, $t2
  addi $sp, $sp, -4
  lw $t4, -216($fp)
  move $t4, $t3
  sw $t0, 0($t4)
  addi $sp, $sp, -4
  lw $t5, -220($fp)
  move $t5, $s0
  addi $sp, $sp, -4
  lw $t6, -224($fp)
  li $v0, 4
  mul $t6, $v0, $s1
  addi $sp, $sp, -4
  lw $t7, -228($fp)
  add $t7, $t5, $t6
  addi $sp, $sp, -4
  lw $t8, -232($fp)
  lw $t8, 0($t7)
  sw $s0, -68($fp)
  sw $s1, -72($fp)
  sw $t0, -200($fp)
  sw $t1, -204($fp)
  sw $t2, -208($fp)
  sw $t3, -212($fp)
  sw $t4, -216($fp)
  sw $t5, -220($fp)
  sw $t6, -224($fp)
  sw $t7, -228($fp)
  sw $t8, -232($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t8
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -236($fp)
  lw $s0, -72($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  sw $s0, -72($fp)
  sw $t0, -236($fp)
  j label11
label13:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
