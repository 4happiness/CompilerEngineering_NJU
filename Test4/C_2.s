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

mod:
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
  div $s0, $s1
  mflo $t0
  addi $sp, $sp, -4
  lw $t1, -16($fp)
  mul $t1, $t0, $s1
  addi $sp, $sp, -4
  lw $t2, -20($fp)
  sub $t2, $s0, $t1
  addi $sp, $sp, -4
  lw $s2, -24($fp)
  move $s2, $t2
  addi $sp, $sp, -4
  lw $s3, -28($fp)
  move $s3, $s2
  move $v0, $s2
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

power:
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
  addi $t0, $s1, 1
  addi $sp, $sp, -4
  lw $t1, -16($fp)
  sub $t1, $t0, $s1
  addi $sp, $sp, -4
  lw $s2, -20($fp)
  move $s2, $t1
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  sw $s2, -20($fp)
  sw $t0, -12($fp)
  sw $t1, -16($fp)
label1:
  addi $sp, $sp, -4
  lw $t0, -24($fp)
  lw $s0, -20($fp)
  lw $s1, -20($fp)
  sub $t0, $s1, $s0
  addi $sp, $sp, -4
  lw $t1, -28($fp)
  addi $t1, $t0, 90
  addi $sp, $sp, -4
  lw $t2, -32($fp)
  addi $t2, $t1, -89
  addi $sp, $sp, -4
  lw $t3, -36($fp)
  addi $t3, $t2, 1
  addi $sp, $sp, -4
  lw $t4, -40($fp)
  addi $t4, $t3, -2
  sw $s0, -20($fp)
  sw $s1, -20($fp)
  sw $t0, -24($fp)
  sw $t1, -28($fp)
  sw $t2, -32($fp)
  sw $t3, -36($fp)
  sw $t4, -40($fp)
  lw $s0, -8($fp)
  lw $t0, -40($fp)
  bgt $s0, $t0, label2
  sw $s0, -8($fp)
  sw $t0, -40($fp)
  j label3
label2:
  addi $sp, $sp, -4
  lw $t0, -44($fp)
  lw $s0, -20($fp)
  lw $s1, -4($fp)
  mul $t0, $s0, $s1
  move $s0, $t0
  addi $sp, $sp, -4
  lw $t1, -48($fp)
  li $v0, 2
  li $v1, 1
  mul $t1, $v0, $v1
  addi $sp, $sp, -4
  lw $t2, -52($fp)
  lw $s2, -8($fp)
  mul $t2, $t1, $s2
  addi $sp, $sp, -4
  lw $t3, -56($fp)
  li $v0, 1
  mul $t3, $v0, $s2
  addi $sp, $sp, -4
  lw $t4, -60($fp)
  sub $t4, $t2, $t3
  addi $sp, $sp, -4
  lw $t5, -64($fp)
  addi $t5, $t4, -1
  move $s2, $t5
  sw $s0, -20($fp)
  sw $s1, -4($fp)
  sw $s2, -8($fp)
  sw $t0, -44($fp)
  sw $t1, -48($fp)
  sw $t2, -52($fp)
  sw $t3, -56($fp)
  sw $t4, -60($fp)
  sw $t5, -64($fp)
  j label1
label3:
  lw $s0, -20($fp)
  move $v0, $s0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

getNumDigits:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a0
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  li $s1, 0
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  lw $s0, -4($fp)
  li $v1, 0
  blt $s0, $v1, label4
  sw $s0, -4($fp)
  j label5
label4:
  addi $sp, $sp, -4
  lw $t0, -12($fp)
  li $v1, 0
  addi $t0, $v1, -1
  move $v0, $t0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  sw $t0, -12($fp)
label5:
label6:
  lw $s0, -4($fp)
  li $v1, 0
  bgt $s0, $v1, label7
  sw $s0, -4($fp)
  j label8
label7:
  addi $sp, $sp, -4
  lw $t0, -16($fp)
  lw $s0, -4($fp)
  li $v1, 10
  div $s0, $v1
  mflo $t0
  move $s0, $t0
  addi $sp, $sp, -4
  lw $t1, -20($fp)
  lw $s1, -8($fp)
  addi $t1, $s1, 2
  move $s1, $t1
  addi $sp, $sp, -4
  lw $t2, -24($fp)
  addi $t2, $s1, 2
  move $s1, $t2
  addi $sp, $sp, -4
  lw $t3, -28($fp)
  addi $t3, $s1, -3
  move $s1, $t3
  sw $s0, -4($fp)
  sw $s1, -8($fp)
  sw $t0, -16($fp)
  sw $t1, -20($fp)
  sw $t2, -24($fp)
  sw $t3, -28($fp)
  j label6
label8:
  lw $s0, -8($fp)
  move $v0, $s0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra

isNarcissistic:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a0
  addi $sp, $sp, -4
  lw $t0, -8($fp)
  addi $t0, $s0, 1
  addi $sp, $sp, -4
  lw $t1, -12($fp)
  addi $t1, $t0, -1
  sw $s0, -4($fp)
  sw $t0, -8($fp)
  sw $t1, -12($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t1
  jal getNumDigits
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -16($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $s0, -20($fp)
  move $s0, $t0
  addi $sp, $sp, -4
  lw $s1, -24($fp)
  li $s1, 0
  addi $sp, $sp, -4
  lw $s2, -28($fp)
  lw $s3, -4($fp)
  move $s2, $s3
  sw $s0, -20($fp)
  sw $s1, -24($fp)
  sw $s2, -28($fp)
  sw $s3, -4($fp)
  sw $t0, -16($fp)
label9:
  lw $s0, -28($fp)
  li $v1, 0
  bgt $s0, $v1, label10
  sw $s0, -28($fp)
  j label11
label10:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  li $v1, 10
  move $a0, $v1
  lw $s0, -28($fp)
  move $a1, $s0
  jal mod
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -32($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $s0, -36($fp)
  move $s0, $t0
  addi $sp, $sp, -4
  lw $t1, -40($fp)
  lw $s1, -28($fp)
  sub $t1, $s1, $s0
  addi $sp, $sp, -4
  lw $t2, -44($fp)
  li $v1, 10
  div $t1, $v1
  mflo $t2
  move $s1, $t2
  sw $s0, -36($fp)
  sw $s1, -28($fp)
  sw $t0, -32($fp)
  sw $t1, -40($fp)
  sw $t2, -44($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $s2, -20($fp)
  move $a0, $s2
  move $a1, $s0
  jal power
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -48($fp)
  move $t0, $v0
  addi $sp, $sp, -4
  lw $t1, -52($fp)
  lw $s0, -24($fp)
  add $t1, $s0, $t0
  move $s0, $t1
  sw $s0, -24($fp)
  sw $t0, -48($fp)
  sw $t1, -52($fp)
  j label9
label11:
  lw $s0, -24($fp)
  lw $s1, -4($fp)
  beq $s0, $s1, label12
  sw $s0, -24($fp)
  sw $s1, -4($fp)
  j label13
label12:
  li $v0, 1
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  j label14
label13:
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
label14:

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  li $s0, 0
  addi $sp, $sp, -4
  lw $s1, -8($fp)
  li $s1, 300
  sw $s0, -4($fp)
  sw $s1, -8($fp)
label15:
  lw $s0, -8($fp)
  li $v1, 500
  blt $s0, $v1, label16
  sw $s0, -8($fp)
  j label17
label16:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $s0, -8($fp)
  move $a0, $s0
  jal isNarcissistic
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -12($fp)
  move $t0, $v0
  sw $t0, -12($fp)
  lw $t0, -12($fp)
  li $v1, 1
  beq $t0, $v1, label18
  sw $t0, -12($fp)
  j label19
label18:
  lw $s0, -8($fp)
  sw $s0, -8($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t0, -16($fp)
  lw $s0, -4($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  sw $s0, -4($fp)
  sw $t0, -16($fp)
label19:
  addi $sp, $sp, -4
  lw $t0, -20($fp)
  lw $s0, -8($fp)
  addi $t0, $s0, 1
  move $s0, $t0
  sw $s0, -8($fp)
  sw $t0, -20($fp)
  j label15
label17:
  lw $s0, -4($fp)
  sw $s0, -4($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $s0, -4($fp)
  move $v0, $s0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
