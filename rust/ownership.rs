
fn main() {
    let base = String::from("hello");

    let s1 = base.clone();
    let s2 = base;

    // error[E0382]: use of moved value: `base`
    // println!("{}", base);

    takes_ownership(s1);

    // error[E0382]: use of moved value: `base`
    // println!("{}", s1);

    let s3 = takes_and_gives_back(s2);

    // Okay!
    // println!("{}", s3);

    let (len, s4) = calc_length(s3);

    println!("({}, \"{}\")", len, s4);
}

fn takes_ownership(some_string: String) {
    println!("takes_ownership: {}", some_string);
}

fn takes_and_gives_back(some_string: String) -> String {
    some_string
}

fn calc_length(s: String) -> (usize, String) {
    (s.len(), s)
}
