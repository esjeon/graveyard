
use std::any::Any;
use std::boxed::Box;

trait Something {
    fn do_this(&self);
}

struct Thing {
}

impl Something for Thing {
    fn do_this(&self) {
        println!("I'm doing this!");
    }
}

fn main() {
    let sth = Thing {};
    sth.do_this();

    let any: Box<Any> = Box::new(sth);
    let sth2: Box<Thing> = any.downcast::<Thing>().unwrap();

    sth2.do_this();
}
