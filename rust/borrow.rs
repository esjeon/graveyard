
fn main() {
    let s = String::from("hello");
    let len = get_length(&s);
    println!("{} -> {}", s, len);

    let mut t = String::from("world");
    update_string(&mut t);
    println!("{} {}", s, t);
}

fn get_length(s: &String) -> usize{
    s.len()
}

fn update_string(s: &mut String) {
    s.push_str("!!")
}
