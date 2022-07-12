import Gates._
import Word._
import Adder._

object Test_Adder
{
	def main (argv : Array[String])
	{
		val x = Inputs(32)		// nombre de bits = 32
		val y = Inputs(32)
		val c = new Input
		val z = PgAdder(x,y,c)		// PgAdder ou SerialAdder

		new Outputs("x",x)
		new Outputs("y",y)
		new Outputs("z",z.sum)
		new Output("c",z.carry)

		x.set(451746)
		y.set(385474)
		Clock.tick

		Statistics.output
	}
}
