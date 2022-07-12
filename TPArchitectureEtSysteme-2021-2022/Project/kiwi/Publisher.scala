import scala.collection.mutable.ArrayBuffer

/* Simplified version of Publisher, hopefully with fewer bugs. */

trait Publisher[Evt]
{
	type Sub = Subscriber[Evt]

	private val subs = new ArrayBuffer[(Sub,Int)]

	def subscribe(sub:Sub) = { subs += ((sub,0)) }
	def subscribe(sub:Sub, id:Int) = { subs += ((sub,id)) }

	protected def publish(event: Evt)
		{ subs.foreach{case (sub,id) => sub.notify(event,id)} }

	override def equals(obj: Any): Boolean =
		obj match {
			case that: Publisher[_] => subs == that.subs
			case _                  => false
		}
}

trait Subscriber[-Evt]
{
	def notify(event:Evt,id:Int): Unit
}
