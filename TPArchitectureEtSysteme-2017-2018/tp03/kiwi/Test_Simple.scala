import Gates._
import Logic._

object Test_Simple
{
	def main (argv : Array[String])
	{
		// juste pour tester une formule logique
		val p = new Input
		val q = new Input
		val a = Or(Xor(q,Not(p)),q)
		val o = new Output("a",a)

		Clock.tick
		q.set(true); 		   Clock.tick
		p.set(true); q.set(false); Clock.tick
		q.set(true); 		   Clock.tick

		Statistics.output
	}
}
