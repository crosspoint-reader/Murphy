
analysis/segments/app0_seg3_42000020.bin:     file format binary


Disassembly of section .data:

421009f0 <.data+0x1009d0>:
421009f0:	000610        	and	a0, a6, a0
421009f3:	00          	.byte	00
421009f4:	021900        	l16ui	a0, a9, 0
421009f7:	00          	.byte	00
421009f8:	03          	.byte	0x3
421009f9:	1900      	s32i.n	a1, a0, 0
421009fb:	00d812        	andbc	b13, b8, b0
421009fe:	03          	.byte	0x3
421009ff:	60          	.byte	0x60
42100a00:	ffffff7f 	ee.vmulas.u8.qacc.ld.ip.qup	q7, a15, -16, q7, q7, q7, q7
42100a04:	000000        	ill
42100a07:	802011        	slli	a2, a0, 8
42100a0a:	03          	.byte	0x3
42100a0b:	60d902        	andb	b13, b9, b6
42100a0e:	ffff3011 	ee.vmulas.s16.qacc.ld.ip.qup	q2, a15, 240, q7, q6, q3, q0
42100a12:	03          	.byte	0x3
42100a13:	60          	.byte	0x60
42100a14:	00          	.byte	00
42100a15:	0c0c      	movi.n	a12, 0
42100a17:	003411        	slli	a3, a4, 16
42100a1a:	03          	.byte	0x3
42100a1b:	603811        	slli	a3, a8, 10
42100a1e:	03          	.byte	0x3
42100a1f:	603c11        	slli	a3, a12, 10
42100a22:	03          	.byte	0x3
42100a23:	607011        	slli	a7, a0, 10
42100a26:	03          	.byte	0x3
42100a27:	60          	.byte	0x60
42100a28:	600c0a        	add.s	f0, f12, f6
42100a2b:	007411        	slli	a7, a4, 16
42100a2e:	03          	.byte	0x3
42100a2f:	604811        	slli	a4, a8, 10
42100a32:	03          	.byte	0x3
42100a33:	60          	.byte	0x60
42100a34:	cd          	.byte	0xcd
42100a35:	cccc      	bnez.n	a12, 0x42100a45
42100a37:	cc78      	bnez.n	a8, 0x42100a42
42100a39:	ffce3f00 	ee.vmulas.s16.accx.ld.ip.qup	q1, a15, 224, q7, q0, q3, q7
42100a3d:	80          	.byte	0x80
42100a3e:	ffffd412 	ee.vmulas.s16.qacc.ld.ip.qup	q4, a15, 240, q7, q7, q5, q4
42100a42:	03          	.byte	0x3
42100a43:	60          	.byte	0x60
42100a44:	e2          	.byte	0xe2
42100a45:	3a28      	add.n	a2, a8, a3
42100a47:	3c24      	movi.n	a4, 50
42100a49:	10          	.byte	0x10
42100a4a:	03          	.byte	0x3
42100a4b:	60          	.byte	0x60
42100a4c:	80          	.byte	0x80
42100a4d:	fcce      	bnez.n	a14, 0x42100a8d
42100a4f:	3fffff3f 	ee.vmulas.s8.qacc.ld.ip.qup	q7, a3, -16, q7, q7, q7, q7
42100a53:	007811        	slli	a7, a8, 16
42100a56:	03          	.byte	0x3
42100a57:	607c11        	slli	a7, a12, 10
42100a5a:	03          	.byte	0x3
42100a5b:	600410        	and	a0, a4, a6
42100a5e:	03          	.byte	0x3
42100a5f:	60001b        	un.s	b0, f0, f6
42100a62:	000936        	ee.ldf.64.xp	f0, f3, a0, a9
42100a65:	810020        	l32r	a8, 0x420c8a68 (0xf01d3c42)
42100a68:	207426        	ee.ldf.64.xp	f7, f2, a2, a4
42100a6b:	125026        	s16i	a1, a0, 76
42100a6e:	220286        	l8ui	a2, a2, 134
42100a71:	050141        	call0	0x42141a84
42100a74:	8ef422a0 	ee.fft.ams.s16.st.incp	q2, q0, a8, a4, q2, q3, q6, 0
42100a78:	003804        	extui	a3, a0, 8, 1
42100a7b:	a1bff5        	l32r	a10, 0x420fe178 (0xc00)
42100a7e:	32232f        	l32i	a3, a3, 188
42100a81:	e00300        	callx8	a3
42100a84:	2aaa      	add.n	a10, a10, a2
42100a86:	0c03      	movi.n	a3, 0
42100a88:	c02000        	memw
42100a8b:	325a00        	s16i	a3, a10, 0
42100a8e:	8b22      	addi.n	a2, a2, 8
42100a90:	32a060        	movi	a3, 96
42100a93:	3792e2        	bne	a2, a3, 0x42100a79
42100a96:	2188f4        	l32r	a2, 0x420fdcb8 (0x3fceff68)
42100a99:	2802      	l32i.n	a2, a2, 0
42100a9b:	22229e        	l32i	a2, a2, 0x278
42100a9e:	e00200        	callx8	a2
42100aa1:	31c8ff        	l32r	a3, 0x421009c4 (0x600310d0)
42100aa4:	21c9ff        	l32r	a2, 0x421009c8 (0xffffefff)
42100aa7:	c02000        	memw
42100aaa:	4803      	l32i.n	a4, a3, 0
42100aac:	202410        	and	a2, a4, a2
42100aaf:	c02000        	memw
42100ab2:	2903      	s32i.n	a2, a3, 0
42100ab4:	21c6ff        	l32r	a2, 0x421009cc (0x60031050)
42100ab7:	0c03      	movi.n	a3, 0
42100ab9:	c02000        	memw
42100abc:	3902      	s32i.n	a3, a2, 0
42100abe:	0c42      	movi.n	a2, 4
42100ac0:	224104        	s8i	a2, a1, 4
42100ac3:	21c3ff        	l32r	a2, 0x421009d0 (0x3fcefbf0)
42100ac6:	cd01      	mov.n	a12, a1
42100ac8:	2802      	l32i.n	a2, a2, 0
42100aca:	4bb1      	addi.n	a11, a1, 4
42100acc:	3c1a      	movi.n	a10, 49
42100ace:	e00200        	callx8	a2
42100ad1:	2176f4        	l32r	a2, 0x420fdcac (0x3fceff5c)
42100ad4:	56ca07        	bnez	a10, 0x42100b54
42100ad7:	720103        	l8ui	a7, a1, 3
42100ada:	320102        	l8ui	a3, a1, 2
42100add:	807701        	slli	a7, a7, 24
42100ae0:	520101        	l8ui	a5, a1, 1
42100ae3:	420100        	l8ui	a4, a1, 0
42100ae6:	d6e700        	bgez	a7, 0x42100af8
42100ae9:	622200        	l32i	a6, a2, 0
42100aec:	c1baff        	l32r	a12, 0x421009d4 (0xa7d)
42100aef:	622602        	l32i	a6, a6, 8
42100af2:	b1b9ff        	l32r	a11, 0x421009d8 (0x3c283adc)
42100af5:	e00600        	callx8	a6
42100af8:	006311        	slli	a6, a3, 16
42100afb:	31b4f8        	l32r	a3, 0x420fedcc (0xff80ffff)
42100afe:	37060e        	bnone	a6, a3, 0x42100b10
42100b01:	3802      	l32i.n	a3, a2, 0
42100b03:	c1b6ff        	l32r	a12, 0x421009dc (0xa7f)
42100b06:	3823      	l32i.n	a3, a3, 8
42100b08:	b1b4ff        	l32r	a11, 0x421009d8 (0x3c283adc)
42100b0b:	0c0a      	movi.n	a10, 0
42100b0d:	e00300        	callx8	a3
42100b10:	31b4ff        	l32r	a3, 0x421009e0 (0xffff80ff)
42100b13:	805511        	slli	a5, a5, 8
42100b16:	37050e        	bnone	a5, a3, 0x42100b28
42100b19:	3802      	l32i.n	a3, a2, 0
42100b1b:	c1b2ff        	l32r	a12, 0x421009e4 (0xa81)
42100b1e:	3823      	l32i.n	a3, a3, 8
42100b20:	b1aeff        	l32r	a11, 0x421009d8 (0x3c283adc)
42100b23:	0c0a      	movi.n	a10, 0
42100b25:	e00300        	callx8	a3
42100b28:	32af80        	movi	a3, -128
42100b2b:	37040e        	bnone	a4, a3, 0x42100b3d
42100b2e:	3802      	l32i.n	a3, a2, 0
42100b30:	c1aeff        	l32r	a12, 0x421009e8 (0xa83)
42100b33:	3823      	l32i.n	a3, a3, 8
42100b35:	b1a8ff        	l32r	a11, 0x421009d8 (0x3c283adc)
42100b38:	0c0a      	movi.n	a10, 0
42100b3a:	e00300        	callx8	a3
42100b3d:	31abff        	l32r	a3, 0x421009ec (0x80808080)
42100b40:	303420        	or	a3, a4, a3
42100b43:	704320        	or	a4, a3, a7
42100b46:	603420        	or	a3, a4, a6
42100b49:	41a0ff        	l32r	a4, 0x421009cc (0x60031050)
42100b4c:	503320        	or	a3, a3, a5
42100b4f:	c02000        	memw
42100b52:	3904      	s32i.n	a3, a4, 0
42100b54:	510cf6        	l32r	a5, 0x420fe384 (0x3fcef958)
42100b57:	41a6ff        	l32r	a4, 0x421009f0 (0x100600)
42100b5a:	32051a        	l8ui	a3, a5, 26
42100b5d:	1b33      	addi.n	a3, a3, 1
42100b5f:	303121        	srai	a3, a3, 1
42100b62:	403320        	or	a3, a3, a4
42100b65:	41f9f7        	l32r	a4, 0x420feb4c (0x60031000)
42100b68:	c02000        	memw
42100b6b:	3904      	s32i.n	a3, a4, 0
42100b6d:	321517        	l16ui	a3, a5, 46
42100b70:	421518        	l16ui	a4, a5, 48
42100b73:	51f8f7        	l32r	a5, 0x420feb54 (0xfc00ffff)
42100b76:	f03311        	slli	a3, a3, 1
42100b79:	f04401        	slli	a4, a4, 17
42100b7c:	3030f4        	extui	a3, a3, 0, 16
42100b7f:	57040e        	bnone	a4, a5, 0x42100b91
42100b82:	5802      	l32i.n	a5, a2, 0
42100b84:	c19cff        	l32r	a12, 0x421009f4 (0x1902)
42100b87:	5825      	l32i.n	a5, a5, 8
42100b89:	b193ff        	l32r	a11, 0x421009d8 (0x3c283adc)
42100b8c:	0c0a      	movi.n	a10, 0
42100b8e:	e00500        	callx8	a5
42100b91:	52ae00        	movi	a5, 0xfffffe00
42100b94:	570310        	bnone	a3, a5, 0x42100ba8
42100b97:	522200        	l32i	a5, a2, 0
42100b9a:	c197ff        	l32r	a12, 0x421009f8 (0x1903)
42100b9d:	5825      	l32i.n	a5, a5, 8
42100b9f:	b18eff        	l32r	a11, 0x421009d8 (0x3c283adc)
42100ba2:	a2a000        	movi	a10, 0
42100ba5:	e00500        	callx8	a5
42100ba8:	303420        	or	a3, a4, a3
42100bab:	41e9f7        	l32r	a4, 0x420feb50 (0x600310e0)
42100bae:	c02000        	memw
42100bb1:	3904      	s32i.n	a3, a4, 0
42100bb3:	31f4f5        	l32r	a3, 0x420fe384 (0x3fcef958)
42100bb6:	420345        	l8ui	a4, a3, 69
42100bb9:	3180ff        	l32r	a3, 0x421009bc (0x40166)
42100bbc:	ac94      	beqz.n	a4, 0x42100be9
42100bbe:	318fff        	l32r	a3, 0x421009fc (0x600312d8)
42100bc1:	418fff        	l32r	a4, 0x42100a00 (0x7fffffff)
42100bc4:	c02000        	memw
42100bc7:	5803      	l32i.n	a5, a3, 0
42100bc9:	405510        	and	a5, a5, a4
42100bcc:	418eff        	l32r	a4, 0x42100a04 (0x80000000)
42100bcf:	404520        	or	a4, a5, a4
42100bd2:	c02000        	memw
42100bd5:	4903      	s32i.n	a4, a3, 0
42100bd7:	c02000        	memw
42100bda:	5803      	l32i.n	a5, a3, 0
42100bdc:	1ce4      	movi.n	a4, 30
42100bde:	404520        	or	a4, a5, a4
42100be1:	c02000        	memw
42100be4:	4903      	s32i.n	a4, a3, 0
42100be6:	3176ff        	l32r	a3, 0x421009c0 (0x640166)
42100be9:	41e7f4        	l32r	a4, 0x420fdf88 (0x6003100c)
42100bec:	518bff        	l32r	a5, 0x42100a18 (0x60031134)
42100bef:	c02000        	memw
42100bf2:	3904      	s32i.n	a3, a4, 0
42100bf4:	4186ff        	l32r	a4, 0x42100a0c (0xffff02d9)
42100bf7:	3184ff        	l32r	a3, 0x42100a08 (0x60031120)
42100bfa:	c02000        	memw
42100bfd:	4903      	s32i.n	a4, a3, 0
42100bff:	4185ff        	l32r	a4, 0x42100a14 (0xc0c00)
42100c02:	3183ff        	l32r	a3, 0x42100a10 (0x60031130)
42100c05:	c02000        	memw
42100c08:	4903      	s32i.n	a4, a3, 0
42100c0a:	315bf5        	l32r	a3, 0x420fe178 (0xc00)
42100c0d:	c02000        	memw
42100c10:	3905      	s32i.n	a3, a5, 0
42100c12:	5182ff        	l32r	a5, 0x42100a1c (0x60031138)
42100c15:	c02000        	memw
42100c18:	4905      	s32i.n	a4, a5, 0
42100c1a:	4181ff        	l32r	a4, 0x42100a20 (0x6003113c)
42100c1d:	c02000        	memw
42100c20:	3904      	s32i.n	a3, a4, 0
42100c22:	4181ff        	l32r	a4, 0x42100a28 (0xa0c60)
42100c25:	317fff        	l32r	a3, 0x42100a24 (0x60031170)
42100c28:	c02000        	memw
42100c2b:	4903      	s32i.n	a4, a3, 0
42100c2d:	4151f5        	l32r	a4, 0x420fe174 (0xc60)
