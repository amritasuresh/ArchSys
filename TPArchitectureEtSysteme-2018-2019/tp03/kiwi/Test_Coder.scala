import Gates._
import Logic._
import Word._
import Select._

object Test_Coder
{
	def test_coder =
	{
		val k = 4
		val x = Inputs(1<<k)

		val c = NaiveCoder(x)
		//val c = Coder(x)

		new Outputs("c",c)

		for (i <- x.indices)
		{
			x(i).set(true)
			Clock.tick
			x(i).set(false)
		}
		println("Pour les OU, diviser taille par 3, profondeur par 2.")
	}

	def test_pcoder =
	{
		val k = 1	// pour k puissance de 2
		val n = 1<<k
		val x = Inputs(n)

		// Choix du codeur de priorité
		val p = PriorityCoder(x)
		//val p = PrioSquareCoder(x)

		new Outputs("x",x)
		new Outputs("y",p.y)
		new Output("z",p.z)

		//x.set(0); Clock.tick
		for (i <- 0 to (1<<n)-1)
		{
			x.set(i)
			Clock.tick
		}
	}

	def main (argv : Array[String])
	{
		test_coder
		Statistics.output
	}
}
