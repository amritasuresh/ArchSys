import Word._
import Logic._
import Gates._

object Select
{
	// Multiplexeur entre deux mots avec un seul bit de sélection
	def Mux (bit:Signal, x:Word, y:Word) : Word =
		Vector.tabulate(x.n)(i => Or(And(Not(bit),x(i)),And(bit,y(i))))

	// Multiplexeur général, applique sel dans l'ordre sel(0)..sel(k-1)
	def Mux (sel:Word, v:Vector[Word]) : Word =
	{
		val k = sel.n
		assert(v.length == 1<<k)

		if (sel.n == 1)
			Mux(sel(0),v(0),v(1))
		else
			Mux(sel.tail,Vector.tabulate(v.length/2)(i =>
				Mux(sel(0),v(2*i),v(2*i+1))))
	}

	// Multiplexeur qui applique sel dans l'ordre sel(k-1)..sel(0)
	def MuxRev (sel:Word, v:Vector[Word]) =
	{
		val idx = (0 until sel.n).map(1<<_).foldLeft(Vector(0)){
			(v,b) => v.map(i => Vector(i,i+b)).flatten }
		Mux(sel.reverse,Vector.tabulate(v.length)(i => v(idx(i))))
	}

	// Décodeur produisant 2^n sorties, avec n la taille du mot "sel"
	def Decoder (sel:Word) : Word =
	{
		val bits = Vector(Not(sel(0)),sel(0))
		if (sel.n == 1)
			bits
		else
			Decoder(sel.tail).map(s =>
				Vector(And(s,bits(0)),And(s,bits(1)))).flatten
	}


	// Codeur avec 2^k signaux en entrée, dont exactement un est haut.
	// Les k bits en sortie codent l'indice du signal haut.
	def Coder (x:Word) : Word =
	{
		class InternalCoder (val out:Word, val bits:Word)

		def CoderRec (x:Word) : InternalCoder =
		{
			if (x.n == 2) return new InternalCoder(x.tail,x)

			val l = CoderRec(x.lower)
			val u = CoderRec(x.upper)
			val s = u.bits.reduce(Or(_,_))
			new InternalCoder(
				l.out.zip(u.out).map{case (a,b) => Or(a,b)}:+s,
				l.bits :+ s)
		}

		assert((x.n & (x.n-1)) == 0 && x.n > 1)	// test puissance de 2
		CoderRec(x).out
	}

	def NaiveCoder (x:Word) : Word =
	{
		def GrandOu (x:Word) : Signal =
		{
			if (x.n == 2) Or(x(0),x(1))
			else Or(GrandOu(x.lower),GrandOu(x.upper))
		}

		if (x.n == 2) return x.tail
		val l = NaiveCoder(x.lower)
		val u = NaiveCoder(x.upper)
		l.zip(u).map{case (a,b) => Or(a,b)} :+ GrandOu(x.upper)
	}

	// Structure de donnée pour codeur de priorité avec 2^k signaux
	// y code l'indice maximal d'un signal haut (s'il en existe)
	// z indique s'il existe un signal haut
	class PCoder (val y:Word, val z:Signal)

	// Codeur de priorité récursif un par un
	def PriorityCoder (x:Word) : PCoder =
	{
		assert((x.n & (x.n-1)) == 0)	// test puissance de 2
		if (x.n == 2) return new PCoder(x.tail,Or(x(0),x(1)))

		val l = PriorityCoder(x.lower)
		val u = PriorityCoder(x.upper)
		new PCoder(Mux(u.z,l.y,u.y) :+ u.z,Or(l.z,u.z))
	}

	// Codeur de priorité récursif par division
	def PrioSquareCoder (x:Word) : PCoder =
	{
		// on suppose que x.n = 2^(2^k) pour un k quelconque

		// on coupe 2^(2k) signaux en 2^k blocs de 2^k signaux
		def cutInputs (x:Vector[Word]) : Vector[Word] =
		{
			if (x.length == x(0).length) return x
			cutInputs(x.map(v => Vector(v.lower,v.upper)).flatten)
		}

		if (x.n == 2) return new PCoder(Vector(x(1)),Or(x(0),x(1)))

		val pcs = cutInputs(Vector(x)).map(PrioSquareCoder(_))
		val zcp = PrioSquareCoder(pcs.map(_.z))
		val mux = MuxRev(zcp.y,pcs.map(_.y))
		new PCoder(mux ++ zcp.y,zcp.z)
	}

}
