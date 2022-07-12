import Gates._

object Logic
{
	case class Nand (x:Signal, y:Signal) extends Gate
	{
		var dx = -1; 
		var bx = false;

		x.subscribe(this)
		y.subscribe(this)
		Statistics.countGate

		override def receive (id:Int, b:Boolean, d:Int) =
		{
			if (dx < 0)
				{ dx = d; bx = b }
			else
			{
				val depth = dx.max(d)+1
				dx = -1;
				set(!(bx && b),depth)
			}
		}
	}

	def Not (x:Signal) = Nand(x,x)
	def Or (x:Signal, y:Signal) = Nand(Not(x),Not(y))
	def And (x:Signal, y:Signal) = Not(Nand(x,y))
	def Xor (x:Signal, y:Signal) = { 
		val a = Nand(x,y)
		Nand(Nand(x,a),Nand(a,y))
	}
}
