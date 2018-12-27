
fn main() {
    let x = 5;
    let y = {
        let x = 3;
        x * 10
    };

    another_function(x, y);

    println!("five returned: {}", five());
}

fn another_function(x: i32, y: i32) {
    println!("The value of x is: {}", x);
    println!("The value of y is: {}", y);
}

fn five() -> i32 {
    5
}
