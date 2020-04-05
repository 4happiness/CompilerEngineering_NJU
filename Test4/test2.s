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

fact:
  addi $sp, $sp, -4
  sw $fp, ($sp)
  move $fp, $sp
  addi $sp, $sp, -4
  lw $s0, -4($fp)
  move $s0, $a0
  li $v1, 1
  beq $s0, $v1, label1
  j label2
label1:
  move $v0, $s0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
  j label3
label2:
  addi $sp, $sp, -4
  lw $t0, -8($fp)
  addi $t0, $s0, -1
  sw $s0, -4($fp)
  sw $t0, -8($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $t0
  jal fact
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t1, -12($fp)
  move $t1, $v0
  addi $sp, $sp, -4
  lw $t2, -16($fp)
  lw $s1, -4($fp)
  mul $t2, $s1, $t1
  move $v0, $t2
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
label3:

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
  lw $t3, -4($fp)
  move $t3, $v0
  addi $sp, $sp, -4
  lw $s2, -8($fp)
  move $s2, $t3
  li $v1, 1
  bgt $s2, $v1, label4
  j label5
label4:
  sw $s2, -8($fp)
  sw $t3, -4($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s2
  jal fact
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, -4
  lw $t4, -12($fp)
  move $t4, $v0
  addi $sp, $sp, -4
  lw $s3, -16($fp)
  move $s3, $t4
  j label6
label5:
  li $s3, 1
label6:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  move $a0, $s3
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $v0, 0
  move $sp $fp
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
