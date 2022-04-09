addui	r0	2
addui	r1	1
addui	r2	2
addui	r3	1
addui	r4	2
addui	r5	2
addui	r6	2
addui	r7	2	
add		r0	r0	//no-ops
add		r1	r2
bz		r3	12
addi	r2	2
addui	r4	4
bp		r1	15
addi	r6	6
addi	r5	7
put		r2
put		r1
put		r3
put		r6
halt