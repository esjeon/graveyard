
trait Kind {}

struct A;
struct B;

impl Kind for A{}
impl Kind for B{}

fn main() {
    let a: Box<dyn Kind> = Box::new(A{});
    let b: Box<dyn Kind> = Box::new(B{});
    assert!(a != b);
}

