fn main() {
    let x: (i32, f64, u8) = (500, 6.4, 1);

    let first: i32 = x.0;
    let second: f64 = x.1;
    let third: u8 = x.2;

    println!("{}, {}, {}", first, second, third);
}
