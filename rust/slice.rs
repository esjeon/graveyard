use std::str::Chars;

fn main() {
    let s = String::from("Hello World! My name is Eon.");

    for i in 0..7 {
        println!("{}", nth_word(&s, i));
    }
}

fn nth_word(s: &str, nth: u32) -> &str {
    let cs: Chars = s.chars();
    let mut begin: usize = 0;
    let mut word_count: u32 = 0;

    for (i, c) in cs.enumerate() {
        if c == ' ' {
            if word_count == nth {
                return &s[begin..i];
            } else {
                word_count += 1;
                begin = i+1;
            }
        }
    }
    &s[begin..]
}
