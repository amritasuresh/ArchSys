object Statistics
{
	var numGates = 0
	var numLatches = 0
	var depth = 0
	def countGate = { numGates += 1 }
	def countLatch = { numLatches += 1 }
	def countDepth (d:Int) = { depth = depth.max(d) }
	def output = {
		if (numLatches > 0) print("latches = " + numLatches + ", ")
		println("size = "+ numGates + ", depth = " + depth)
	}
}

object Gates
{
	case class Bit (value:Boolean, depth:Int)
	case class Tick (tock:Boolean)

	abstract class Signal extends Publisher[Bit]
	{
		def set (b:Boolean,d:Int) = { publish(Bit(b,d)) }
	}

	trait Receptor extends Subscriber[Bit]
	{
		def receive (id:Int, b:Boolean, d:Int) : Unit

		override def notify (event:Bit, id:Int) : Unit =
		{
			event match { case Bit(b,d) => { receive(id,b,d) } }
		}
	}

	object Clock extends Publisher[Tick]
	{
		def start = { publish(Tick(false)); println("----------") }
		def tick = { publish(Tick(true)); start }
	}

	trait Clocked extends Subscriber[Tick]
	{
		def send : Unit
		def accept : Unit = {}

		Clock.subscribe(this)
		override def notify (event:Tick, id:Int) : Unit =
		{
			event match { case Tick(b) => if (b) send else accept }
		}
	}

	abstract class Gate extends Signal with Receptor

	class Input (b:Boolean = false) extends Signal with Clocked
	{
		def send = { super.set(value,0) }
		var value : Boolean = b
		def set (b:Boolean) = { value = b }
	}

	object One extends Signal with Clocked
	{
		def send = super.set(true,0)
	}

	object Zero extends Signal with Clocked
	{
		def send = super.set(false,0)
	}

	class Output (name:String, s:Signal) extends Receptor
	{
		s.subscribe(this)

		override def receive (id:Int, b:Boolean, d:Int) =
		{
			println(name + " = " + b)
			Statistics.countDepth(d)
		}
	}

	class OutputWithDepth (name:String, s:Signal) extends Output(name,s)
	{
		override def receive (id:Int, b:Boolean, d:Int) =
		{
			println(name + " = " + b + " at depth " + d)
			Statistics.countDepth(d)
		}
	}

	class Connector (en:Signal, dt:Signal, var value:Boolean) extends Gate
	{
		en.subscribe(this,0)
		dt.subscribe(this,1)

		var enable:Boolean = false
		var data:Boolean = false

		override def receive (id:Int, b:Boolean, d:Int) =
		{
			Statistics.countDepth(d)
			if (id == 0) enable = b
			if (id == 1) data = b
		}

		def query : Boolean = 
		{
			if (enable) value = data
			super.set(value,0)
			value
		}
	}

	class Latch (b:Boolean = false) extends Signal with Clocked
	{
		def send = { super.set(value,0) }
		var value : Boolean = b
		var con:Connector = null
		Statistics.countLatch

		override def accept = { value = con.query }

		def connect (enable:Signal, data:Signal) =
		{
			con = new Connector(enable,data,value)
		}
	}
}
