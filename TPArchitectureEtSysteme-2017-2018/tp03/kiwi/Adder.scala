import Gates._
import Logic._
import Word._

object Adder
{
	class BitAdder (val sum:Signal, val carry:Signal,
				val prop:Signal = null, val gen:Signal = null)

	def DemiAdd (x:Signal, y:Signal) : BitAdder =
	{
		val n = Nand(x,y)
		new BitAdder(Nand(Nand(x,n),Nand(n,y)),Not(n))
	}

	def FullAdd (x:Signal, y:Signal, c:Signal) : BitAdder =
	{
		val da1 = DemiAdd(x,y)
		val da2 = DemiAdd(da1.sum,c)
		new BitAdder(da2.sum,Or(da1.carry,da2.carry),da1.sum,da1.carry)
	}

	class IntAdder (val sum:Word, val carry:Signal,
		val prop:Signal = null, val gen:Signal = null)

	def SerialAdder (x:Word, y:Word, c:Signal) : IntAdder =
	{
		val fa = FullAdd(x(0),y(0),c)

		if (x.n == 1)
			new IntAdder(Vector(fa.sum),fa.carry)
		else
		{
			val rest = SerialAdder(x.tail,y.tail,fa.carry)
			new IntAdder(fa.sum +: rest.sum,rest.carry)
		}
	}

	def PgAdder (x:Word, y:Word, c:Signal) : IntAdder =
	{
		if (x.n == 1)
		{
			val pg = FullAdd(x(0),y(0),c)
			new IntAdder(Vector(pg.sum),pg.carry,pg.prop,pg.gen)
		}
		else
		{
			val low = PgAdder(x.lower,y.lower,c)
			val up = PgAdder(x.upper,y.upper,low.carry)

			val prop = And(low.prop,up.prop)
			val gen = Or(up.gen,And(low.gen,up.prop))
			val carry = Or(gen,And(c,prop))
			new IntAdder(low.sum ++ up.sum,carry,prop,gen)
		}
	}
}
