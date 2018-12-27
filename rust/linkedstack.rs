
use std::option::Option;
use std::rc::Rc;

struct LinkedStack<T> {
    head: Option<Rc<Node<T>>>,
}

struct Node<T> {
    data: Rc<T>,
    next: Option<Rc<Node<T>>>,
}

impl <T> LinkedStack<T> {
    fn new() -> LinkedStack<T> {
        LinkedStack { head: None }
    }

    fn push(&self, data: Rc<T>) -> LinkedStack<T> {
        let next = self.head.as_ref().and_then(|node| Some(node.clone()));
        let head = Rc::new(Node { data, next });
        LinkedStack { head: Some(head) }
    }

    fn pop(&self) -> (Option<Rc<T>>, LinkedStack<T>) {
        let data = self.head.as_ref().and_then(|node| Some(node.data.clone()));
        let head = self.head.as_ref().and_then(|node| node.next.clone());
        (data, LinkedStack { head })
    }
}

// impl <T> Node<T> {
//     fn new(data: Rc<T>, next: Option<Rc<Node<T>>>) -> Node<T> {
//         Node { data, next }
//     }
// 
//     fn data(&self) -> Rc<T> {
//         self.data.clone()
//     }
// 
//     fn next(&self) -> Option<Rc<Node<T>>> {
//         self.next.as_ref().and_then(|node| Some(node.clone()))
//     }
// }

fn main() {
    let stack = LinkedStack::new();
    let stack = stack.push(Rc::new(String::from("Hello")));
    let stack = stack.push(Rc::new(String::from("World")));
    let (world_opt, stack) = stack.pop();
    let world = world_opt.unwrap();
    let (hello_opt, _stack) = stack.pop();
    let hello = hello_opt.unwrap();
    println!("{} {}!", hello, world);
}
