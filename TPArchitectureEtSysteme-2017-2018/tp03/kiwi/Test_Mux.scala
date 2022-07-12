import Gates._
import Word._
import Select._

object Test_Mux
{
	def main (argv : Array[String])
	{
		val n = 10	// largeur des mots dans v
		val k = 3
		val a = Vector(4,31,53,12,543,179,458,22)

		val v = a.map(Constant(n)(_))	// vecteur de longueur 2^k 
		val sel = Inputs(k)

		new Outputs("m",MuxRev(sel,v))
		new Outputs("d",Decoder(sel))

		sel.set(0); Clock.tick
		sel.set(1); Clock.tick
		sel.set(2); Clock.tick
		sel.set(3); Clock.tick
		sel.set(4); Clock.tick
		sel.set(5); Clock.tick
		sel.set(6); Clock.tick
		sel.set(7); Clock.tick

		Statistics.output
	}
}
