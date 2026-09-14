import zmq

context = zmq.Context()

pub = context.socket(zmq.XPUB)
pub.bind("tcp://*:5556") #sub1

sub = context.socket(zmq.XSUB)
sub.bind("tcp://*:5555") #pub1

zmq.proxy(pub, sub)

pub.close()
sub.close()
context.close()
