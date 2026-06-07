
m3/analysis/segments/app0_seg3_42000020.bin:     file format binary


Disassembly of section .data:

42000370 <.data+0x350>:
42000370:	5c16      	movi.n	a6, 81
42000372:	193c      	s32i.n	a1, a12, 12
42000374:	2c16      	movi.n	a6, 33
42000376:	193c      	s32i.n	a1, a12, 12
42000378:	40          	.byte	0x40
42000379:	16193c        	beqz	a9, 0x4200073e
4200037c:	0816      	l32i.n	a0, a6, 4
4200037e:	193c      	s32i.n	a1, a12, 12
42000380:	112233        	l32r	a1, 0x41fcd008
42000383:	11782a        	l32r	a1, 0x41fcad64
42000386:	ca3f      	add.n	a3, a15, a12
42000388:	a0          	.byte	0xa0
42000389:	16193c        	beqz	a9, 0x4200074e
4200038c:	030303        	lsi	f0, a3, 12
4200038f:	04          	.byte	0x4
42000390:	010404        	l32r	a0, 0x41fc13a0
42000393:	042019        	mula.dd.hl.lddec	m2, a0, m0, m2
42000396:	00          	.byte	00
42000397:	50bc16        	ee.ldf.64.xp	f11, f1, a5, a12
4200039a:	193c      	s32i.n	a1, a12, 12
4200039c:	f818      	l32i.n	a15, a8, 4
4200039e:	00          	.byte	00
4200039f:	50c816        	ee.ldf.64.xp	f12, f1, a5, a8
420003a2:	193c      	s32i.n	a1, a12, 12
420003a4:	d01800        	movsp	a13, a8
420003a7:	50ec16        	ee.ldf.64.xp	f14, f1, a5, a12
420003aa:	193c      	s32i.n	a1, a12, 12
420003ac:	981e      	l32i.n	a9, a14, 4
420003ae:	ca3f      	add.n	a3, a15, a12
420003b0:	5a1e      	add.n	a1, a14, a5
420003b2:	ca3f      	add.n	a3, a15, a12
420003b4:	0c17      	movi.n	a7, 1
420003b6:	193c      	s32i.n	a1, a12, 12
420003b8:	0e180050 	ee.vmulas.u16.qacc.ld.ip.qup	q0, a0, 128, q0, q2, q0, q0
420003bc:	0c71      	movi.n	a1, 7
420003be:	ca3f      	add.n	a3, a15, a12
420003c0:	5b1c      	addi.n	a1, a12, 5
420003c2:	00505a        	msub.s	f5, f0, f0
420003c5:	1c00      	movi.n	a0, 16
420003c7:	50          	.byte	0x50
420003c8:	591c      	s32i.n	a5, a12, 4
420003ca:	005058        	ssxp	f5, a0, a0
420003cd:	1c00      	movi.n	a0, 16
420003cf:	50          	.byte	0x50
420003d0:	571c00        	beq	a12, a5, 0x420003d4
420003d3:	50          	.byte	0x50
420003d4:	5e1c0050 	ee.vmulas.u16.qacc.ld.ip.qup	q0, a5, 192, q0, q2, q0, q0
420003d8:	40          	.byte	0x40
420003d9:	420f00        	l8ui	a4, a15, 0
420003dc:	60e709        	l32e	a6, a7, -8
420003df:	4248e7        	s8i	a4, a8, 231
420003e2:	0942      	s32i.n	a0, a2, 16
420003e4:	9c98      	beqz.n	a8, 0x42000401
420003e6:	374014        	ball	a0, a3, 0x420003fe
420003e9:	1800      	l32i.n	a1, a0, 0
420003eb:	50          	.byte	0x50
420003ec:	0000a0        	addx4	a0, a0, a0
420003ef:	402d43        	min	a2, a13, a4
420003f2:	1ceb      	movi.n	a11, 30
420003f4:	e2          	.byte	0xe2
420003f5:	365a3f        	entry	a10, 0x1fa8
420003f8:	b81e      	l32i.n	a11, a14, 4
420003fa:	d53f00        	call4	0x420007f8
420003fd:	00          	.byte	00
420003fe:	5940      	s32i.n	a5, a0, 16
42000400:	0ad7      	add.n	a13, a7, a0
42000402:	23          	.byte	0x23
42000403:	3c1c      	movi.n	a12, 49
42000405:	1eca3f9a 	ee.stf.128.xp	f5, f14, f12, f3, a1, a10
42000409:	9999      	s32i.n	a9, a9, 36
4200040b:	9999      	s32i.n	a9, a9, 36
4200040d:	9910      	s32i.n	a9, a0, 4
4200040f:	40          	.byte	0x40
42000410:	1a1e      	add.n	a1, a14, a1
42000412:	ca3f      	add.n	a3, a15, a12
42000414:	000040        	ssr	a0
42000417:	40          	.byte	0x40
42000418:	f4fdd4        	ee.vst.l.64.ip	q3, a15, -24
4200041b:	78e9      	l32i.n	a7, a9, 56
4200041d:	a64bc0        	blti	a11, 4, 0x420003e1
42000420:	e17a14        	l32r	a14, 0x41fc5608
42000423:	ae474383 	ee.ldf.128.ip	f6, f6, f4, f4, a10, 112
42000427:	40          	.byte	0x40
42000428:	000da1        	sll	a0, a13
4200042b:	40          	.byte	0x40
4200042c:	9979      	s32i.n	a9, a9, 28
4200042e:	a34003        	ssi	f10, a0, 12
42000431:	00          	.byte	00
42000432:	005004        	extui	a5, a0, 0, 1
42000435:	00          	.byte	00
42000436:	005002        	andb	b5, b0, b0
42000439:	00          	.byte	00
4200043a:	005000        	syscall
4200043d:	00          	.byte	00
4200043e:	00          	.byte	00
4200043f:	3f2c2500 	ee.vmulas.s16.accx.ld.ip.qup	q0, a3, 192, q4, q4, q2, q5
42000443:	409c26        	ee.ldf.64.xp	f9, f2, a4, a12
42000446:	1642a4        	beqz	a2, 0x41fffe8e
42000449:	220040        	l8ui	a2, a0, 64
4200044c:	742200        	ee.ld.qacc_l.l.128.ip	a7, 0x220
4200044f:	40          	.byte	0x40
42000450:	0900      	s32i.n	a0, a0, 0
42000452:	0050e4        	extui	a5, a0, 0, 15
42000455:	2bca      	addi.n	a12, a10, 2
42000457:	3fb47e23 	ee.vmulas.s8.accx.ld.ip.qup	q7, a3, 64, q6, q6, q7, q6
4200045b:	3c30      	movi.n	a0, 51
4200045d:	1900      	s32i.n	a1, a0, 0
4200045f:	50          	.byte	0x50
42000460:	421900        	l16ui	a4, a9, 0
42000463:	502017        	ee.stf.64.xp	f2, f1, a5, a0
42000466:	193c      	s32i.n	a1, a12, 12
42000468:	2817      	l32i.n	a2, a7, 4
4200046a:	193c      	s32i.n	a1, a12, 12
4200046c:	951eca        	call4	0x41fca658
4200046f:	3f891eca 	ee.vmulas.u8.accx.ld.xp.qup	q5, a3, a9, q6, q0, q1, q6
42000473:	3f50a123 	ee.vmulas.s8.accx.ld.ip.qup	q6, a3, 0, q5, q3, q2, q1
42000477:	3c32      	movi.n	a2, 51
42000479:	1900      	s32i.n	a1, a0, 0
4200047b:	503017        	ee.stf.64.xp	f3, f1, a5, a0
4200047e:	193c      	s32i.n	a1, a12, 12
42000480:	3c17      	movi.n	a7, 49
42000482:	193c      	s32i.n	a1, a12, 12
42000484:	50          	.byte	0x50
42000485:	17193c        	beq	a9, a1, 0x420004c5
42000488:	64          	.byte	0x64
42000489:	17193c        	beq	a9, a1, 0x420004c9
4200048c:	6c17      	movi.n	a7, -31
4200048e:	193c      	s32i.n	a1, a12, 12
42000490:	e8b7      	l32i.n	a14, a7, 44
42000492:	174200        	ball	a2, a1, 0x42000496
42000495:	b817      	l32i.n	a11, a7, 4
42000497:	42          	.byte	0x42
42000498:	361eca        	entry	a14, 0x6508
4200049b:	3f381eca 	ee.vmulas.u8.accx.ld.xp.qup	q5, a3, a8, q4, q6, q1, q6
4200049f:	3f351eca 	ee.vmulas.u8.accx.ld.xp.qup	q5, a3, a5, q4, q6, q1, q6
420004a3:	3f241eca 	ee.vmulas.u8.accx.ld.xp.qup	q5, a3, a4, q4, q4, q1, q6
420004a7:	3f181200 	ee.vmulas.s16.accx.ld.ip.qup	q0, a3, 128, q4, q2, q1, q2
420004ab:	407417        	ee.stf.64.xp	f7, f1, a4, a4
420004ae:	193c      	s32i.n	a1, a12, 12
420004b0:	7c1f      	movi.n	a15, -15
420004b2:	193c      	s32i.n	a1, a12, 12
420004b4:	a70900        	bnone	a9, a10, 0x420004b8
420004b7:	008517        	ee.stf.64.xp	f8, f1, a0, a5
420004ba:	193c      	s32i.n	a1, a12, 12
420004bc:	54          	.byte	0x54
420004bd:	853840        	call0	0x42040848
420004c0:	521f19        	l16ui	a5, a15, 50
420004c3:	3cc1      	movi.n	a1, 60
420004c5:	0900      	s32i.n	a0, a0, 0
420004c7:	00982b        	oeq.s	b9, f8, f0
420004ca:	ca3f      	add.n	a3, a15, a12
420004cc:	00007a        	divn.s	f0, f0, f0
420004cf:	44          	.byte	0x44
420004d0:	bc29      	beqz.n	a9, 0x42000506
420004d2:	ca3f      	add.n	a3, a15, a12
420004d4:	e01dca        	float.s	f1, a13, 14
420004d7:	3fac1719 	ee.vmulas.s16.qacc.ld.ip.qup	q2, a3, 0xfffffec0, q6, q4, q1, q7
420004db:	3c5c      	movi.n	a12, 53
420004dd:	4f004270 	ee.vmulas.u8.qacc.ld.ip.qup	q0, a4, 0, q4, q0, q4, q2
420004e1:	16193c        	beqz	a9, 0x420008a6
420004e4:	401eca        	float.s	f1, a14, 4
420004e7:	3fe41719 	ee.vmulas.s16.qacc.ld.ip.qup	q2, a3, 0xfffffe40, q7, q4, q1, q7
420004eb:	3c78      	movi.n	a8, 55
420004ed:	ae174218 	ee.vmulas.s16.qacc.ld.ip.qup	q0, a10, 0xfffffe70, q0, q2, q4, q2
420004f1:	af174238 	ee.vmulas.s8.qacc.ld.ip.qup	q0, a10, 0xfffffe70, q4, q2, q4, q2
420004f5:	ae174268 	ee.vmulas.u8.accx.ld.ip.qup	q0, a10, 0xfffffe70, q0, q2, q4, q2
420004f9:	2bca      	addi.n	a12, a10, 2
420004fb:	3f4caf17 	ee.vmulas.s16.qacc.ld.ip.qup	q7, a3, 0x1c0, q5, q1, q2, q7
420004ff:	424818        	s8i	a4, a8, 24
42000502:	193c      	s32i.n	a1, a12, 12
42000504:	5c18      	movi.n	a8, 81
42000506:	193c      	s32i.n	a1, a12, 12
42000508:	7c18      	movi.n	a8, -15
4200050a:	193c      	s32i.n	a1, a12, 12
4200050c:	80          	.byte	0x80
4200050d:	1819      	l32i.n	a1, a9, 4
4200050f:	3c18      	movi.n	a8, 49
42000511:	a3          	.byte	0xa3
42000512:	23          	.byte	0x23
42000513:	3c80      	movi.n	a0, 56
42000515:	2bca      	addi.n	a12, a10, 2
42000517:	3ff01dca 	ee.vmulas.u8.accx.ld.xp.qup	q5, a3, a0, q7, q6, q1, q5
4200051b:	3fc029ca 	ee.vmulas.u8.accx.ld.xp.qup	q5, a3, a0, q7, q0, q2, q1
4200051f:	3f001200 	ee.vmulas.s16.accx.ld.ip.qup	q0, a3, 0, q4, q0, q1, q2
42000523:	409018        	lsxp	f9, a0, a4
42000526:	193c      	s32i.n	a1, a12, 12
42000528:	94          	.byte	0x94
42000529:	1819      	l32i.n	a1, a9, 4
4200052b:	3c08      	movi.n	a8, 48
4200052d:	2bca      	addi.n	a12, a10, 2
4200052f:	3f3c1eca 	ee.vmulas.u8.accx.ld.xp.qup	q5, a3, a12, q4, q6, q1, q6
42000533:	3fa01819 	ee.vmulas.s16.qacc.ld.ip.qup	q3, a3, 0xfffffe00, q6, q4, q1, q0
42000537:	3c52      	movi.n	a2, 53
42000539:	1a00      	add.n	a0, a0, a1
4200053b:	509017        	ee.stf.64.xp	f9, f1, a5, a0
4200053e:	193c      	s32i.n	a1, a12, 12
42000540:	3e1eca3f 	ee.vmulas.s8.qacc.ld.ip.qup	q7, a3, -32, q0, q3, q4, q2
42000544:	b818      	l32i.n	a11, a8, 4
42000546:	193c      	s32i.n	a1, a12, 12
42000548:	c818      	l32i.n	a12, a8, 4
4200054a:	193c      	s32i.n	a1, a12, 12
4200054c:	3b1e      	addi.n	a1, a14, 3
4200054e:	ca3f      	add.n	a3, a15, a12
42000550:	3a1e      	add.n	a1, a14, a3
42000552:	ca3f      	add.n	a3, a15, a12
42000554:	5c1c      	movi.n	a12, 81
42000556:	005095        	extui	a5, a0, 16, 10
42000559:	94          	.byte	0x94
4200055a:	ffffe818 	ee.vmulas.s16.qacc.ld.ip.qup	q1, a15, 0xfffffef0, q7, q7, q6, q0
4200055e:	193c      	s32i.n	a1, a12, 12
42000560:	0f180050 	ee.vmulas.u16.qacc.ld.ip.qup	q0, a0, 128, q4, q2, q0, q0
42000564:	101800        	movsp	a1, a8
42000567:	50          	.byte	0x50
42000568:	041919        	mula.dd.hl.lddec	m1, a9, m0, m2
4200056b:	3c10      	movi.n	a0, 49
4200056d:	1919      	s32i.n	a1, a9, 4
4200056f:	3cfc      	movi.n	a12, 63
42000571:	f6ffff        	bgeui	a15, 0x100, 0x42000574
42000574:	74          	.byte	0x74
42000575:	f6ffff        	bgeui	a15, 0x100, 0x42000578
42000578:	fc1d      	bnez.n	a13, 0x420005ad
4200057a:	ca3f      	add.n	a3, a15, a12
4200057c:	20          	.byte	0x20
4200057d:	1919      	s32i.n	a1, a9, 4
4200057f:	3c00      	movi.n	a0, 48
42000581:	20          	.byte	0x20
42000582:	00          	.byte	00
42000583:	003818        	lsxp	f3, a8, a0
42000586:	193c      	s32i.n	a1, a12, 12
42000588:	7c16      	movi.n	a6, -15
4200058a:	193c      	s32i.n	a1, a12, 12
4200058c:	94          	.byte	0x94
4200058d:	16193c        	beqz	a9, 0x42000952
42000590:	8816      	l32i.n	a8, a6, 4
42000592:	193c      	s32i.n	a1, a12, 12
42000594:	fcf7      	bnez.n	a7, 0x420005d7
42000596:	ffff0018 	ee.vmulas.s16.qacc.ld.ip.qup	q0, a15, 0xfffffef0, q7, q6, q0, q0
4200059a:	193c      	s32i.n	a1, a12, 12
4200059c:	bcf6      	beqz.n	a6, 0x420005df
4200059e:	ffffff1f 	ee.vmulas.s16.qacc.ld.ip.qup	q7, a15, -16, q7, q7, q7, q7
420005a2:	00          	.byte	00
420005a3:	001c18        	lsxp	f1, a12, a0
420005a6:	193c      	s32i.n	a1, a12, 12
420005a8:	3819      	l32i.n	a3, a9, 4
420005aa:	193c      	s32i.n	a1, a12, 12
420005ac:	ec09      	bnez.n	a9, 0x420005d0
420005ae:	0000f0        	subx8	a0, a0, a0
420005b1:	0900      	s32i.n	a0, a0, 0
420005b3:	00          	.byte	00
420005b4:	1cac      	movi.n	a12, 26
420005b6:	17428c        	ball	a2, a1, 0x42000546
420005b9:	2aca      	add.n	a12, a10, a2
420005bb:	3f501919 	ee.vmulas.s16.qacc.ld.ip.qup	q3, a3, 0xfffffe00, q5, q2, q1, q1
420005bf:	3c64      	movi.n	a4, 54
420005c1:	1919      	s32i.n	a1, a9, 4
420005c3:	3c78      	movi.n	a8, 55
420005c5:	1919      	s32i.n	a1, a9, 4
420005c7:	3c90      	movi.n	a0, 57
420005c9:	1919      	s32i.n	a1, a9, 4
420005cb:	3cd8      	movi.n	a8, 61
420005cd:	1919      	s32i.n	a1, a9, 4
420005cf:	3cec      	movi.n	a12, 62
420005d1:	1919      	s32i.n	a1, a9, 4
420005d3:	3c10      	movi.n	a0, 49
420005d5:	1a19      	add.n	a1, a9, a1
420005d7:	3ca0      	movi.n	a0, 58
420005d9:	29ca      	s32i.n	a2, a10, 48
420005db:	3f341a19 	ee.vmulas.s16.qacc.ld.ip.qup	q3, a3, 0xfffffe40, q4, q6, q1, q2
420005df:	3c48      	movi.n	a8, 52
420005e1:	1a19      	add.n	a1, a9, a1
420005e3:	3c9c      	movi.n	a12, 57
420005e5:	af174200 	ee.vmulas.s16.accx.ld.ip.qup	q0, a10, 112, q4, q2, q4, q2
420005e9:	00          	.byte	00
420005ea:	00          	.byte	00
420005eb:	cf          	.byte	0xcf
420005ec:	ffffff4e 	ee.vmulas.u16.accx.ld.ip.qup	q5, a15, -16, q7, q7, q7, q7
420005f0:	000000        	ill
420005f3:	000000        	ill
420005f6:	e0          	.byte	0xe0
420005f7:	c10000        	l32r	a12, 0x41fc05f8
420005fa:	c0          	.byte	0xc0
420005fb:	ffffffdf 	ee.fft.cmul.s16.ld.xp	q7, a15, a15, q7, q7, q7, 7
420005ff:	41d422        	l32r	a4, 0x41fc9150
42000602:	004000        	break	0, 0
42000605:	000000        	ill
42000608:	e8af      	l32i.n	a14, a15, 40
4200060a:	174214        	ball	a2, a1, 0x42000622
4200060d:	b01742        	xorb	b1, b7, b11
42000610:	742500        	ee.ld.qacc_l.l.128.ip	a7, 0x250
42000613:	40          	.byte	0x40
42000614:	000000        	ill
42000617:	008c22        	orb	b8, b12, b0
4200061a:	00          	.byte	00
4200061b:	40ccab        	movltz.s	f12, f12, a4
4200061e:	174244        	ball	a2, a1, 0x42000666
42000621:	b01742        	xorb	b1, b7, b11
42000624:	5b1a      	addi.n	a1, a10, 5
42000626:	193c      	s32i.n	a1, a12, 12
42000628:	6e1a193c 	ee.vmulas.s8.qacc.ld.ip.qup	q1, a6, -96, q0, q2, q1, q1
4200062c:	869c25        	j	0x42009ca2
4200062f:	3c6c      	movi.n	a12, 54
42000631:	240040        	ee.ld.qacc_l.l.128.ip	a2, 0xfffff800
42000634:	dc23      	bnez.n	a3, 0x4200064a
42000636:	00          	.byte	00
42000637:	40a976        	ee.ldf.64.xp	f10, f7, a4, a9
4200063a:	273c75        	bltu	a12, a2, 0x420006b3
4200063d:	1a19      	add.n	a1, a9, a1
4200063f:	      	movi.n	a8, 63
