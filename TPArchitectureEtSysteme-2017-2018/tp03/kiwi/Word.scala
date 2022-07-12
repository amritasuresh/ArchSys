import Gates._

object Word
{
	type Word = Vector[Signal]
	type InputWord = Vector[Input]
	type LatchWord = Vector[Latch]

	implicit class ImpWord (val under: Word) extends AnyVal
	{
		def n = under.length
		def lower : Word = under.slice(0,n/2)
		def upper : Word = under.slice(n/2,n)
	}

	def Inputs (size:Int) : InputWord =
		Vector.tabulate(size)(_ => new Input)
	def Constant (size:Int)(value:Int) : Word =
		Vector.tabulate(size)(i =>
			if ((value & (1<<i)) != 0) One else Zero)

	implicit class ImpInput (val under: InputWord) extends AnyVal
	{
		def set (value:Int) : Unit =
			under.indices.foreach(i =>
				under(i).set((value & (1<<i)) != 0))
	}

	class Outputs (name:String, x:Word) extends Receptor
	{
		var count = 0
		var value = 0
		var depth = 0
		var n = x.length

		x.indices.foreach(i => x(i).subscribe(this,i))

		def output = println(name + " = " + value)
		override def receive (id:Int, b:Boolean, d:Int)
		{
			Statistics.countDepth(d)
			if (b) value += 1<<id
			depth = d.max(depth)
			count += 1
			if (count < n) return

			output
			value = 0
			count = 0
			depth = 0
		}
	}

	class OutputsWithDepth (name:String, x:Word) extends Outputs(name,x)
	{
		override def output = println(name + " = " + value +
						" at depth " + depth)
	}

	def Latches (size:Int)(value:Int) : LatchWord =
		Vector.tabulate(size)(i => new Latch((value & (1<<i)) != 0))

	implicit class ImpLatch (val under: LatchWord) extends AnyVal
	{
		def toWord : Word = under.map(_.con)
		def connect (e:Signal, w:Word) : Unit =
			w.indices.foreach(i => under(i).connect(e,w(i)))
	}
}
