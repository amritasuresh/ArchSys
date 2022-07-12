import Gates._
import Word._
import Adder._

object Test_Latch
{
	def main (argv : Array[String])
	{
		val x = Latches(3)(0)
		val y = Constant(3)(3)
		val c = new Input
		val e = new Input(true)
		val z = PgAdder(x,y,c)		// z = x+3

		// lors de chaque tique d'horloge, on écrit z dans x
		x.connect(e,z.sum)
		new Outputs("x",x.toWord)

		Clock.start
		Clock.tick
		Clock.tick
		Clock.tick
		Clock.tick
		Clock.tick
		Clock.tick
		Clock.tick
		Clock.tick

		Statistics.output
	}
}
