import Gates._
import Logic._
import Word._
import Adder._
import Select._
import Console._

abstract class Program
{
	val code:Vector[Int]	// on rajoute des zéros pour remplir la RAM
	lazy val memory = code ++ Vector.fill(256)(0)
}

object Squares extends Program
{
	override val code = Vector(
			0x30, 0x01,	// 00:     mov a,1
			0x34, 0x01,	// 02:     mov b,1
			0x38, 0x03,	// 04:     mov c,3
			0x3c, 0x30,	// 06:     mov d,L1
			0x70, 0x0a,	// 08: L3: cmp a,10
			0xa0, 0x16,	// 0a:     jg L2
			0x8d,		// 0c:     mov [d],b
			0x46,		// 0d:     add b,c
			0x58, 0x02,	// 0e:     add c,2
			0x50, 0x01,	// 10:     add a,1
			0x5c, 0x01,	// 12:     add d,1
			0x90, 0x08,	// 14:     jmp L3
			0x10		// 16: L2: hlt
		)
}

object TestMov extends Program
{	// tester les instructions 2 et 8
	override val code = Vector(
			0x34, 0x01,	// 00:     mov b,1
			0x29,		// 02:     mov c,b	(B=C=1)
			0x88,		// 03:     mov [c],a	(mem(1)=0)
			0x10		// 04:     hlt
		)
}

object TestAdd extends Program
{	// tester les instructions 4 et 5
	override val code = Vector(
			0x30, 0x01,	// 00:     mov a,1
			0x34, 0x02,	// 02:     mov b,2
			0x41,		// 04:     add a,b	(A=3)
			0x54, 0x03,	// 05:     add b,3	(B=5)
			0x10		// 07:     hlt
		)
}

object TestCmp extends Program
{	// tester les instructions 6 et 7
	override val code = Vector(
			0x30, 0x01,	// 00:     mov a,1
			0x34, 0x02,	// 02:     mov b,2
			0x61,		// 04:     cmp a,b	(Cy=0)
			0x64,		// 05:     cmp b,a	(Cy=1)
			0x70, 0x01,	// 06:     cmp a,1	(Cy=0)
			0x70, 0x00,	// 08:     cmp a,0	(Cy=1)
			0x10		// 0a:     hlt
		)
}

object TestJmp extends Program
{	// tester les instructions 9 et a
	// si Cy=1 au départ, on termine avec A=2, sinon avec A=1
	override val code = Vector(
			0x90, 0x03,	// 00:     jmp L1
			0x10,		// 02:     hlt
			0x30, 0x01,	// 03: L1: mov a,1
			0xa0, 0x08,	// 05:     jg L2
			0x10,		// 07:     hlt
			0x30, 0x02,	// 08: L2: mov a,2
			0x10		// 0a:     hlt
		)
}

object Kiwi
{
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// Programme à charger dans la mémoire

	val Programme = Squares

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// Microcode dans le ROM

	def RW ( in:Int = 0, ar:Int = A_PC, out:Int = D_nul, next:Int = 0,
			op:Int = 0, pc:Int = 0, cj:Int = 0, hlt:Int = 0 ) =
		(in<<12) + (ar<<11) + (out<<8) + next +
			(op<<7) + (pc<<6) + (cj<<5) + (hlt<<4)
	
	implicit class IntToBase( val digits:String ) extends AnyVal
		{ def b = Integer.parseInt( digits,2) }

	// Sources		// Destinations
	val S_RS = "00".b;	val D_PC  = "000".b
	val S_Alu = "01".b;	val D_IR  = "001".b
	val S_Mem = "10".b;	val D_RD = "010".b
	val A_PC  = "0".b;	val D_Mem = "011".b
	val A_RD = "1".b;	val D_nul = "100".b
	
	val ROM = Vector(
	  RW (in = S_Mem, ar = A_PC, out = D_IR, pc = 1),	// 0 fetch
	  RW (hlt = 1, next = 1),				// 1 halt
	  RW (in = S_RS, out = D_RD),				// 2 mov R,R
	  RW (in = S_Mem, ar = A_PC, out = D_RD, pc = 1),	// 3 mov R,v
	  RW (in = S_RS, next = 11),				// 4 add R,R
	  RW (in = S_Mem, ar = A_PC, pc = 1, next = 11),	// 5 add R,v
	  RW (in = S_RS, op = 1),				// 6 cmp R,R
	  RW (in = S_Mem, ar = A_PC, op = 1, pc = 1),		// 7 cmp R,v
	  RW (in = S_RS, out = D_Mem, ar = A_RD),		// 8 mov [R],R
	  RW (in = S_Mem, out = D_PC, pc = 1),			// 9 jmp A
	  RW (in = S_Mem, out = D_PC, pc = 1, cj = 1),		// 10 jg A
	  RW (in = S_Alu, out = D_RD),				// 11 (add)
	  0, 0, 0, 0).map(Constant(16)(_))

	class RomSignals (
		val bi1:Signal, val bi0:Signal, val ar:Signal, val bo:Word,
		val op:Signal, val pc:Signal, val cj:Signal, val hlt:Signal,
		val next:Word )

	def Signals (w:Word) = new RomSignals(w(13),w(12),w(11),
			w.slice(8,11),w(7),w(6),w(5),w(4),w.slice(0,4))


	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// Quelques circuits mineurs

	def Increment (w:Word) : Word = PgAdder(w,Constant(w.n)(1),Zero).sum
	def NotWord(w:Word) : Word = w.map(Not(_))


	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// Tout les endroits de stockage (Latch), tous initialisés à zéro

	// CPU: Les registres

	val PC = Latches(8)(0)	// program counter
	val IC = Latches(4)(0)	// instruction code
	val RD = Latches(2)(0)	// index of destination register
	val RS = Latches(2)(0)	// index of source register

	val REG = Vector.tabulate(4)(_ => Latches(8)(0)) // registers A,B,C,D

	val Carry = new Latch(false)	// CPU flags
	val Halt  = new Latch(false)

	// Mémoire principale avec son contenu initial
	val RAM = Vector.tabulate(256)(i => Latches(8)(Programme.memory(i)))

	// ALU: Stockage du résultat
	val Z = Latches(8)(0)


	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// La ROM et obtention des signaux de contrôle

	val Ctrl = Signals(Mux(IC,ROM))


	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// sélection des données dans AR et dans le bus

	// choisir des régistres selon IR
	val Mdd = Mux(RD,REG)
	new Outputs("Mdd",Mdd)
	new Output("Ctrl",Ctrl.pc)
	val Mss = Mux(RS,REG)
	val AR = Mux(Ctrl.ar,PC,Mdd)
	new Outputs("AR",AR)
	val RAM_out = Mux(AR,RAM)
	new Outputs("RAM",RAM_out)
	val Bus = Mux(Ctrl.bi1,Mux(Ctrl.bi0,Mss,Z),RAM_out)
	new Outputs("Bus",Bus)


	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// calcul dans ALU
	// L'ALU fera un calcul dans toute itération, même inutil

	val Op1 = Mdd
	val Op2 = Mux(Ctrl.op,Bus,NotWord(Bus))
	val Alu_out = PgAdder(Op1,Op2,Zero)
	new Output("Alu",Alu_out.carry)


	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// Choisir la destination des données sur le bus
	// Dans Bdest, 0 = PC, 1 = IR, 2 = Reg(RD), 3 = Mémoire

	val Bdest = Decoder(Ctrl.bo)

	// Trois possibilités pour le PC
	//  (i) rester inchangé, ce sera le cas sauf si Ctrl.pc est vrai
	//  (ii) incrémenter le PC
	//  (iii) prendre l'adresse sur le bus si:
	//		(a) celle-ci est adressée au PC et
	//		(b) soit le saut est non-conditionnel ou Carry est vrai
	//  Les lignes ci-dessous choisissent entre (ii) et (iii).
	//  Ctrl.pc est pris en compte plus bas.
	val PCplus = Increment(PC)
	val PCchx = And(Bdest(0),Or(Not(Ctrl.cj),Carry))
	val PCnext = Mux(PCchx,PCplus,Bus)
	new Outputs("PCnext",PCnext)

	// IC/RD/RS
	// Bdest(1) indique qu'on charge le prochain opcode (Next)
	// Si c'est le cas, IC reçoit les quatre bits haut, RD/RS les autres
	// Sinon, IR devient la prochaine adresse du microcode,
	//		et RS/RD restent inchangés
	val Next = Bdest(1)

	// Registres : on écrit dans le registres choisi par RD
	//		si le bus est destiné aux registres
	val Renable = Decoder(RD).map(And(_,Bdest(2)))

	// Mémoire : on écrit dans une adresse si elle est dans AR
	// 		et que le bus est destiné à la mémoire
	val Menable = Decoder(AR).map(And(_,Bdest(3)))


	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// Stocker les résultats dans les latch

	PC.connect(Ctrl.pc,PCnext)
	IC.connect(One,Mux(Next,Ctrl.next,Bus.upper))
	RD.connect(Next,Bus.lower.upper)
	RS.connect(Next,Bus.lower.lower)
	REG.indices.foreach(i => REG(i).connect(Renable(i),Bus))
	Carry.connect(Ctrl.op,Alu_out.carry)
	Halt.connect(Ctrl.hlt,Ctrl.hlt)

	RAM.indices.foreach(i => RAM(i).connect(Menable(i),Bus))
	Z.connect(One,Alu_out.sum)



	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// Hexdump de mémoire

	val registers = ('A' to 'D')
	def byte (w:LatchWord) = w.indices.map(i =>
			if (w(i).value) 1<<i else 0).sum
	def hex (w:LatchWord) = ("%0"+(w.length+3)/4+"x").format(byte(w))
	def en (w:LatchWord,esc:String) = if (w(0).con.enable) esc else ""
	def dump (w:LatchWord) = en(w,RED) + hex(w) + en(w,RESET)
	def eq (v:Int,w:Int,esc:String) = if (v == w) esc else ""
	def memdump (pc:Int,i:Int) = eq(pc,i,BLUE) +dump(RAM(i))+ eq(pc,i,RESET)

	def disasm (pc:Int, v:Int) =
	{
		val opcode = v>>4
		val rd = registers((v & 0xc) >> 2)
		val rs = registers(v & 0x3)
		//val pcb = memdump(pc,pc)
		val pcb = BLUE + "mm" + RESET
		opcode match {
			case 0 => "fetch"
			case 1 => "halt"
			case 2 => "mov %c,%c".format(rd,rs)
			case 3 => "mov %c,%s".format(rd,pcb)
			case 4 => "Z := %c+%c".format(rd,rs)
			case 5 => "Z := %c+%s".format(rd,pcb)
			case 6 => "cmp %c,%c".format(rd,rs)
			case 7 => "cmp %c,%s".format(rd,pcb)
			case 8 => "mov [%c],%c".format(rd,rs)
			case 9 => "jmp %s".format(pcb)
			case 10 => "jg %s".format(pcb)
			case 11 => "%c := Z".format(rd)
			case _ => "?"
		}
	}

	def hexdump =
	{
		val pc = byte(PC)
		val ic = byte(IC)
		print("PC = " + dump(PC) + ", ")
		print("IR = " + dump(IC)+dump(RS++RD))
		print(" [" + disasm(pc,byte(RS++RD++IC)) + "], ")
		print("Cy = " + dump(Vector(Carry)) + ", ")
		println("Ht = " + dump(Vector(Halt)))

		registers.indices.foreach(i =>
			print(registers(i) + " = " + dump(REG(i)) + ", "))
		println("Z = " + eq(ic,11,RED) + hex(Z) + eq(ic,11,RESET))
		println

		for (i <- 0 to 3) println((0 to 15).
			map(j => memdump(pc,i*16+j)).reduce(_ + " " + _))
	}

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	// On laisse tourner jusqu'à ce que le programme s'arrête

	def main (argv:Array[String])
	{
		var i = 0
		while (!Halt.value)
		{
			println("Step "+i); hexdump
			Clock.tick;
			i += 1
		}
		println("Step "+i); hexdump
		Clock.tick;
		Statistics.output
	}

}
